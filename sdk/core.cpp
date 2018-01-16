#include "include/core.h"

namespace io {
    namespace openmessaging {
        namespace core {

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

                boost::shared_ptr<io::openmessaging::core::JavaOption> jOptions =
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

            JavaOption::JavaOption(const jint version) : _version(version) {

            }

            void JavaOption::addOption(const string &option) {
                options.push_back(option);
            }
        }
    }

}
