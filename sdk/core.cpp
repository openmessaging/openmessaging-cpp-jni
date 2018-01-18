#include "include/core.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    using namespace std;

    boost::once_flag once = BOOST_ONCE_INIT;

    JavaVM *jvm;
    JNIEnv *env;

    void init_logging()
    {
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    }

    void init0() {

        init_logging();

        boost::shared_ptr<JavaOption> jOptions =
                boost::make_shared<JavaOption>(JNI_VERSION_1_8);
        jOptions->addOption("-Djava.class.path=/Users/lizhanhui/work/openmessaging-java/openmessaging-api/target/openmessaging-api-0.2.0-alpha.1-SNAPSHOT.jar");
        jOptions->addOption("-Xms1G");
        jOptions->addOption("-Xmx1G");

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
        boost::call_once(once, init0);
    }

    void Shutdown() {
        if (jvm) {
            jvm->DestroyJavaVM();
            jvm = NULL;
        }
    }

    bool isRunning() {
        return jvm != NULL;
    }

    std::set<std::string> toNativeSet(CurrentEnv &current, jobject s) {
        jclass classIterable = current.env->FindClass("java/lang/Iterable");
        jmethodID midIterator = current.env->GetMethodID(classIterable, "iterator", "()Ljava/util/Iterator;");
        jobject objIterator = current.env->CallObjectMethod(s, midIterator);

        jclass classIterator = current.env->FindClass("java/util/Iterator");
        jmethodID midHasNext = current.env->GetMethodID(classIterator, "hasNext", "()Z");
        jmethodID midNext = current.env->GetMethodID(classIterator, "next", "()Ljava/lang/Object;");

        set<std::string> nativeSet;

        while (current.env->CallBooleanMethod(objIterator, midHasNext)) {
            jobject item = current.env->CallObjectMethod(objIterator, midNext);
            jstring str = static_cast<jstring>(item);
            const char* buffer = current.env->GetStringUTFChars(str, NULL);
            nativeSet.insert(buffer);
            current.env->ReleaseStringUTFChars(str, buffer);
        }

        current.env->DeleteLocalRef(objIterator);
        current.env->DeleteLocalRef(classIterator);
        current.env->DeleteLocalRef(classIterable);
        return nativeSet;
    }

    CurrentEnv::CurrentEnv() : attached(false) {
        if (jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8) != JNI_OK) {
            if (jvm->AttachCurrentThread(reinterpret_cast<void **>(&env), NULL) == JNI_OK) {
                attached = true;
            }
        }
    }

    CurrentEnv::~CurrentEnv() {
        if (attached) {
            jvm->DetachCurrentThread();
            env = NULL;
        }
    }

    jmethodID getMethod(CurrentEnv &current, jclass clazz, const std::string &name,
                                             const std::string &signature, bool isStatic) {
        jmethodID  methodId;
        if (isStatic) {
            methodId = current.env->GetStaticMethodID(clazz, name.c_str(), signature.c_str());
        } else {
            methodId = current.env->GetMethodID(clazz, name.c_str(), signature.c_str());
        }

        if (!methodId) {
            BOOST_LOG_TRIVIAL(warning) << "Failed to GetMethodID. Method: " << name << ", Signature: " << signature;
            abort();
        }

        return methodId;
    }

    JavaOption::JavaOption(const jint version) : _version(version) {

    }

    void JavaOption::addOption(const string &option) {
        options.push_back(option);
    }

END_NAMESPACE_2(io, openmessaging)
