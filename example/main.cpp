#include <iostream>
#include "core.h"
#include <jni.h>

int main() {

    using namespace io::openmessaging::core;

    std::cout << "Hello, World!" << std::endl;

    const jint version = JNI_VERSION_1_8;
    boost::shared_ptr<JavaOption> javaOption = boost::make_shared<JavaOption>(version);
    javaOption->addOption("-Djava.class.path=/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/commons-cli-1.2.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/commons-lang3-3.4.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/fastjson-1.2.29.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/guava-19.0.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/javassist-3.20.0-GA.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/jna-4.2.2.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/logback-classic-1.0.13.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/logback-core-1.0.13.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/netty-all-4.0.42.Final.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/netty-tcnative-1.1.33.Fork22-osx-x86_64.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/openmessaging-api-0.1.0-alpha.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-broker-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-client-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-common-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-example-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-filter-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-filtersrv-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-namesrv-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-openmessaging-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-remoting-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-srvutil-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-store-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/rocketmq-tools-4.3.0-SNAPSHOT.jar:/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq/lib/slf4j-api-1.7.7.jar");
    javaOption->addOption("-Xms1G");
    javaOption->addOption("-Xmx1G");

    Initialize(javaOption);

    Shutdown();

    return 0;
}