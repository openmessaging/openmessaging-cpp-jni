#include <cstdlib>
#include <sstream>
#include <cstdarg>

#include <pthread.h>

#include <sys/stat.h>
#include <dirent.h>

#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    const char* Types::void_ = "V";
    const char* Types::boolean_ = "Z";
    const char* Types::byte_ = "B";
    const char* Types::char_ = "C";
    const char* Types::short_ = "S";
    const char* Types::int_ = "I";
    const char* Types::long_ = "J";
    const char* Types::float_ = "F";
    const char* Types::double_ = "D";

    const char* Types::byteArray_ = "[B";

    const char* Types::Object_ = "Ljava/lang/Object;";
    const char* Types::String_ = "Ljava/lang/String;";
    const char* Types::KeyValue_ = "Lio/openmessaging/KeyValue;";
    const char* Types::Set_ = "Ljava/util/Set;";
    const char* Types::MessagingAccessPoint_ = "Lio/openmessaging/MessagingAccessPoint;";
    const char* Types::Producer_ = "Lio/openmessaging/producer/Producer;";
    const char* Types::PushConsumer_ = "Lio/openmessaging/consumer/PushConsumer;";
    const char* Types::PullConsumer_ = "Lio/openmessaging/consumer/PullConsumer;";
    const char* Types::Message_ = "Lio/openmessaging/Message;";
    const char* Types::ByteMessage_ = "Lio/openmessaging/BytesMessage;";
    const char* Types::SendResult_ = "Lio/openmessaging/producer/SendResult;";
    const char* Types::LocalTransactionExecutor_ = "Lio/openmessaging/producer/LocalTransactionExecutor;";
    const char* Types::Context_ = "Lio/openmessaging/consumer/MessageListener$Context;";
    const char* Types::MessageListener_ = "Lio/openmessaging/consumer/MessageListener;";
    const char* Types::ConsumerInterceptor_ = "Lio/openmessaging/interceptor/ConsumerInterceptor;";
    const char* Types::ProducerInterceptor_ = "Lio/openmessaging/interceptor/ProducerInterceptor;";
    const char* Types::BatchMessageSender_ = "Lio/openmessaging/producer/BatchMessageSender;";
    const char* Types::OMSException_ = "Lio/openmessaging/exception/OMSException;";
    const char* Types::Future_ = "Lio/openmessaging/Future;";
    const char* Types::Class_ = "Ljava/lang/Class;";

    std::string buildSignature(const std::string &return_type, int n, ...) {
        std::string signature = "(";

        va_list args;
        va_start(args, n);
        for (int i = 0; i < n; ++i) {
            signature += va_arg(args, char*);
        }
        va_end(args);

        return signature + ")" + return_type;
    }

    JavaVM *jvm;

    JNIEnv *env;

    void VM::init_logging() {
        char *home = getenv("HOME");
        std::string file_name = std::string(home) + "/oms.log";
        size_t max_file_size = 1024 * 1024 * 100;
        static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(file_name.c_str(), max_file_size, 10);
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        const char* level = getenv("OMS_LOG_LEVEL");
        if (NULL == level || std::string("debug") == level) {
            plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);
        } else if (std::string("info") == level) {
            plog::init(plog::info, &fileAppender).addAppender(&consoleAppender);
        } else if (std::string("warn") == level) {
            plog::init(plog::warning, &fileAppender).addAppender(&consoleAppender);
        } else {
            plog::init(plog::fatal, &fileAppender).addAppender(&consoleAppender);
        }
    }

    void VM::init() {
        NS::shared_ptr<JavaOption> jOptions = NS::make_shared<JavaOption>(JNI_VERSION_1_8);
        std::string class_path_option = build_class_path_option();
        jOptions->addOption(class_path_option);

        const char* maxHeapSize = getenv("OMS_VM_MAX_HEAP");
        if (NULL == maxHeapSize) {
            jOptions->addOption("-Xmx1G");
            LOG_INFO << "Default maximum heap size: 1G";
        } else {
            std::string max("-Xmx");
            max += maxHeapSize;
            jOptions->addOption(max);
            LOG_INFO << "Maximum heap size: " << maxHeapSize;
        }

        const char* minHeapSize = getenv("OMS_VM_MIN_HEAP");
        if (NULL == minHeapSize) {
            jOptions->addOption("-Xms1G");
            LOG_INFO << "Default minimum heap size: 1G";
        } else {
            std::string minHeap("-Xms");
            minHeap += minHeapSize;
            jOptions->addOption(minHeap);
            LOG_INFO << "Min heap size: " << minHeapSize;
        }

        const char* verbose = getenv("OMS_VM_VERBOSE");
        if (NULL != verbose && std::string("true") == verbose) {
            jOptions->addOption("-verbose");
            jOptions->addOption("-Xcheck:jni");
            LOG_INFO << "Verbose output enabled";
        }

        const char* debug = getenv("OMS_VM_DEBUG");
        if (NULL != debug && std::string("true") == debug) {
            jOptions->addOption("-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005");
            LOG_INFO << "Debug enabled";
        }

        int optionCount = jOptions->options.size();
        JavaVMOption *options = new JavaVMOption[optionCount];

        for (int i = 0; i < optionCount; ++i) {
            options[i].optionString = const_cast<char *>(jOptions->options[i].c_str());
        }

        JavaVMInitArgs vm_args;
        memset(&vm_args, 0, sizeof(vm_args));
        vm_args.version = jOptions->_version;
        vm_args.nOptions = optionCount;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;
        jint rc = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
        delete[] options;

        if (rc != JNI_OK) {
            LOG_ERROR << "Failed to create JVM";
            exit(EXIT_FAILURE);
        }

        jint version = env->GetVersion();
        LOG_INFO << "JVM starts OK. Version: " << ((version >> 16) & 0x0f) << "." << (version & 0x0f);
    }

    std::set<std::string> toNativeSet(CurrentEnv &current, jobject s) {
        const char *klassIterable = "java/lang/Iterable";
        const char *klassIterator = "java/util/Iterator";
        jclass classIterable = current.findClass(klassIterable);
        jmethodID midIterator = current.getMethodId(classIterable, "iterator", "()Ljava/util/Iterator;");
        jobject objIterator = current.callObjectMethod(s, midIterator);

        jclass classIterator = current.findClass(klassIterator);
        jmethodID midHasNext = current.getMethodId(classIterator, "hasNext", "()Z");
        jmethodID midNext = current.getMethodId(classIterator, "next", "()Ljava/lang/Object;");

        std::set<std::string> nativeSet;

        while (current.callBooleanMethod(objIterator, midHasNext)) {
            jobject item = current.callObjectMethod(objIterator, midNext);
            jstring str = reinterpret_cast<jstring>(item);
            const char *buffer = current.env->GetStringUTFChars(str, NULL);
            nativeSet.insert(buffer);
            current.env->ReleaseStringUTFChars(str, buffer);
        }

        current.deleteRef(objIterator);
        current.deleteRef(classIterator);
        current.deleteRef(classIterable);
        return nativeSet;
    }

    std::string list_jars(const char* path) {
        DIR *dir;
        struct dirent *ent;
        std::string result;
        if (NULL != (dir = opendir(path))) {

            size_t len = strlen(path);
            bool hasTrailingPathSeparator = (*(path + len - 1) == '/');

            while (NULL != (ent = readdir(dir))) {
                if ((ent->d_type & DT_DIR) == DT_DIR) {
                    continue;
                }

                if ((ent->d_name)[0] == '.') {
                    continue;
                }

                result += path;
                if (!hasTrailingPathSeparator) {
                    result += "/";
                }
                result += ent->d_name;
                result += ":";
            }
            closedir(dir);
        }

        return result;
    }


    bool exists(const char* path) {
        struct stat st;
        int ret = stat(path, &st);
        return !ret && (st.st_mode & S_IFDIR) == S_IFDIR;
    }

    std::string build_class_path_option() {
        std::string option = "-Djava.class.path=";
        const char* ROCKETMQ_HOME_KEY = "ROCKETMQ_HOME";
        const char *rocketmqHome = getenv(ROCKETMQ_HOME_KEY);
        if (NULL == rocketmqHome) {

            // Construct default library path
            int bits = 8 * sizeof(void*);
            std::string vendor = "/usr/local/";
            if (bits == 32) {
                vendor += "lib";
            } else {
                std::ostringstream oss;
                oss << bits;
                vendor += "lib" + oss.str();
            }
            vendor += "/oms/vendor";

            if (exists(vendor.c_str())) {
                LOG_DEBUG << "Found OMS vendor implementation: " << vendor;
                rocketmqHome = vendor.c_str();
                setenv(ROCKETMQ_HOME_KEY, vendor.c_str(), 1);
                LOG_DEBUG << "set ROCKETMQ_HOME=" << vendor;
            } else {
                std::string home = getenv("HOME");
                vendor = home + "/oms/vendor";
                if (exists(vendor.c_str())) {
                    LOG_DEBUG << "Found OMS vendor implementation: " << vendor;
                    rocketmqHome = vendor.c_str();
                    setenv(ROCKETMQ_HOME_KEY, vendor.c_str(), 1);
                    LOG_DEBUG << "set ROCKETMQ_HOME=" << vendor;
                } else {
                    const char *msg = "Environment variable: ROCKETMQ_HOME is not set";
                    LOG_ERROR << msg;
                    throw OMSException(msg);
                }
            }
        }
        std::string lib_dir = std::string(rocketmqHome) + "/lib/";
        std::string expanded_class_path = list_jars(lib_dir.c_str());
        LOG_DEBUG << "Class Path: " << expanded_class_path;
        return option + expanded_class_path;
    }

        const long Condition::one_second_in_nano = 1000000000;
        const long Condition::one_milli_second_in_nano = 1000000;
        const long Condition::one_micro_second_in_nano = 1000;
        const long Condition::one_second_in_milli = 1000;

END_NAMESPACE_2(io, openmessaging)