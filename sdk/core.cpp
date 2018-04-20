#include <cstdlib>
#include <cstdarg>

#include "core.h"

#include <boost/filesystem.hpp>

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace boost::filesystem;

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

    boost::once_flag once_flag = BOOST_ONCE_INIT;

    void init_logging() {
        char *home = getenv("HOME");
        std::string file_name = std::string(home) + "/oms.log";
        size_t max_file_size = 1024 * 1024 * 100;
        static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(file_name.c_str(), max_file_size, 10);
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender);
    }

    void init0() {

        init_logging();

        NS::shared_ptr<JavaOption> jOptions = NS::make_shared<JavaOption>(JNI_VERSION_1_8);
        std::string class_path_option = build_class_path_option();
        jOptions->addOption(class_path_option);
        jOptions->addOption("-Xms1G");
        jOptions->addOption("-Xmx1G");
    //    jOptions->addOption("-verbose");
    //    jOptions->addOption("-Xcheck:jni");
    //    jOptions->addOption("-Drocketmq.namesrv.addr=localhost:9876");
        jOptions->addOption("-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=5005");

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

    void Initialize() {
        boost::call_once(once_flag, init0);
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

    bool file_name_filter(const std::string &file_name) {
        const std::string file_extension = ".jar";
        return stringEndsWith(file_name, file_extension);
    }

    std::string expand_class_path(const std::string& path_with_wildcard) {
        std::string result;
        const std::string wildcard = "*";
        if (stringEndsWith(path_with_wildcard, wildcard)) {
            const std::string dir = path_with_wildcard.substr(0, path_with_wildcard.size() - wildcard.size());
            std::vector<std::string> files = list_files(dir, file_name_filter);
            for (std::vector<std::string>::size_type i = 0; i < files.size(); ++i) {
                if (result.empty()) {
                    result = files[i];
                } else {
                    result += ":" + files[i];
                }
            }
        }
        return result;
    }

    bool stringEndsWith(const std::string &s, const std::string &ext) {
        if (s.size() < ext.size()) {
            return false;
        }

        return s.substr(s.size() - ext.size()) == ext;
    }

    std::vector<std::string> list_files(const std::string &dir, bool (*f)(const std::string&)) {
        path p(dir);
        std::vector<std::string> result;
        if (is_directory(p)) {
            directory_iterator iterator(p);
            directory_iterator end;
            while (iterator != end) {
                directory_entry &entry = *iterator;
                std::string full_file_name = entry.path().string();
                if (f(full_file_name)) {
                    result.push_back(full_file_name);
                }
                iterator++;
            }
        }
        return result;
    }

    std::string build_class_path_option() {
        std::string option = "-Djava.class.path=";
        const char* ROCKETMQ_HOME_KEY = "ROCKETMQ_HOME";
        const char *rocketmqHome = getenv(ROCKETMQ_HOME_KEY);
        if (NULL == rocketmqHome) {
            std::string vendor = "/usr/local/lib/oms/vendor";
            path vendor_jar_dir(vendor);
            if (exists(vendor_jar_dir)) {
                LOG_DEBUG << "Found OMS vendor implementation: " << vendor;
                rocketmqHome = vendor.c_str();
                setenv(ROCKETMQ_HOME_KEY, vendor.c_str(), 1);
                LOG_DEBUG << "set ROCKETMQ_HOME=" << vendor;
            } else {
                std::string home = getenv("HOME");
                vendor = home + "/oms/vendor";
                path alternative(vendor);
                if (exists(alternative)) {
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
        std::string lib_dir = std::string(rocketmqHome) + "/lib/*";
        std::string expanded_class_path = expand_class_path(lib_dir);
        LOG_DEBUG << "Class Path: " << expanded_class_path;
        return option + expanded_class_path;
    }

END_NAMESPACE_2(io, openmessaging)