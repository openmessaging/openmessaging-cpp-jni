#include <iostream>

#include <boost/make_shared.hpp>
#include <boost/thread/pthread/once_atomic.hpp>
#include "include/core.h"


namespace io {
    namespace openmessaging {
        namespace core {

            using namespace std;

            boost::once_flag once = BOOST_ONCE_INIT;

            JavaVM *jvm;
            JNIEnv *env;

            void init0(const boost::shared_ptr<JavaOption> &javaOption) {

                boost::shared_ptr<JavaOption> jOptions;

                if (javaOption) {
                    jOptions = javaOption;
                } else {
                    const jint version = JNI_VERSION_1_8;
                    jOptions = boost::make_shared<JavaOption>(version);
                    jOptions->addOption("-Djava.class.path=/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/commons-cli-1.2.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/commons-lang3-3.4.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/fastjson-1.2.29.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/guava-19.0.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/javassist-3.20.0-GA.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/jna-4.2.2.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/logback-classic-1.0.13.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/logback-core-1.0.13.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/netty-all-4.0.42.Final.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/netty-tcnative-1.1.33.Fork22-osx-x86_64.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/openmessaging-api-0.1.0-alpha.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-broker-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-client-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-common-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-example-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-filter-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-filtersrv-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-namesrv-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-openmessaging-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-remoting-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-srvutil-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-store-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-tools-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/slf4j-api-1.7.7.jar");
                    jOptions->addOption("-Xms1G");
                    jOptions->addOption("-Xmx1G");
                }

                JavaVMInitArgs vm_args;
                int optionCount = jOptions->options.size();
                JavaVMOption *options = new JavaVMOption[optionCount];

                for (int i = 0; i < optionCount; ++i) {
                    options[i].optionString = const_cast<char *>(jOptions->options[i].c_str());
                }

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
                cout << "JVM starts OK. Version: ";
                jint version = env->GetVersion();
                cout << ((version >> 16) & 0x0f) << "." << (version & 0x0f) << endl;
            }

            void Initialize(const boost::shared_ptr<JavaOption> &javaOption) {
                boost::call_once(once, boost::bind(init0, javaOption));
            }

            void Shutdown() {
                if (jvm) {
                    jvm->DestroyJavaVM();
                    jvm = NULL;
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