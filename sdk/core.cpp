#include <dirent.h>
#include <sys/types.h>
#include <sys/dirent.h>
#include <pthread.h>

#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)

using namespace std;

JavaVM *jvm;

JNIEnv *env;

pthread_once_t once_flag = PTHREAD_ONCE_INIT;

void init_logging() {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}

void init0() {

    init_logging();

    boost::shared_ptr<JavaOption> jOptions =
            boost::make_shared<JavaOption>(JNI_VERSION_1_8);
    std::string class_path_option = build_class_path_option();
    jOptions->addOption(class_path_option);
    jOptions->addOption("-Xms1G");
    jOptions->addOption("-Xmx1G");
//    jOptions->addOption("-verbose");
//    jOptions->addOption("-Xcheck:jni");
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
        cout << "Failed to create JVM" << endl;
        exit(EXIT_FAILURE);
    }

    jint version = env->GetVersion();
    BOOST_LOG_TRIVIAL(info) << "JVM starts OK. Version: " << ((version >> 16) & 0x0f) << "." << (version & 0x0f);
}

void Initialize() {
    pthread_once(&once_flag, init0);
}

void Shutdown() {
    if (jvm) {
        jvm->DestroyJavaVM();
        jvm = NULL;
        BOOST_LOG_TRIVIAL(info) << "JVM shutdown OK";
        return;
    }
}

bool isRunning() {
    return jvm != NULL;
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

    set<std::string> nativeSet;

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
        std::vector<std::string> files = list(dir, file_name_filter);
        for (int i = 0; i < files.size(); ++i) {
            if (result.empty()) {
                result = dir + files[i];
            } else {
                result += ":" + dir + files[i];
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

std::vector<std::string> list(const std::string &dir, bool (*f)(const std::string&)) {
    DIR* pDir;
    struct dirent *entry;
    vector<string> result;
    if ((pDir = opendir(dir.c_str())) != NULL) {
        while((entry = readdir(pDir)) != NULL) {
            if (f(entry->d_name)) {
                result.push_back(entry->d_name);
            }
        }
        closedir(pDir);
    }
    return result;
}

std::string build_class_path_option() {
    std::string option = "-Djava.class.path=";
    std::string lib_dir = "/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/*";
    std::string expanded_class_path = expand_class_path(lib_dir);
    BOOST_LOG_TRIVIAL(info) << "Class Path: " << expanded_class_path;
    return option + expanded_class_path;
}

END_NAMESPACE_2(io, openmessaging)