#include <iostream>

#include <boost/thread/pthread/once_atomic.hpp>
#include "include/core.h"


namespace io {
    namespace openmessaging {
        namespace core {

            using namespace std;

            boost::once_flag once = BOOST_ONCE_INIT;

            JavaVM *jvm;
            JNIEnv *env;

            void init0(const JavaOption &javaOption) {
                JavaVMInitArgs vm_args;
                int optionCount = javaOption.options.size();
                JavaVMOption *options = new JavaVMOption[optionCount];

                for (int i = 0; i < optionCount; ++i) {
                    options[i].optionString = const_cast<char *>(javaOption.options[i].c_str());
                }

                vm_args.version = javaOption._version;
                vm_args.nOptions = optionCount;
                vm_args.options = options;
                vm_args.ignoreUnrecognized = false;
                jint rc = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);
                delete[] options;

                if (rc != JNI_OK) {
                    cout << "Failed to create JVM" << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "JVM starts OK. Version: ";
                jint version = env->GetVersion();
                cout << ((version >> 16) & 0x0f) << "." << (version & 0x0f) << endl;
            }

            void Initialize(const JavaOption &javaOption) {
                boost::call_once(once, boost::bind(init0, javaOption));
            }

            void Shutdown() {
                if (jvm) {
                    jvm->DestroyJavaVM();
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