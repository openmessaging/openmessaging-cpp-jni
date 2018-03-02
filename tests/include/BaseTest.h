#ifndef OMS_BASE_TEST_H
#define OMS_BASE_TEST_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <gtest/gtest.h>
#include "Namespace.h"
#include "CurrentEnv.h"
#include "core.h"
#include <cstdlib>

BEGIN_NAMESPACE_2(io, openmessaging)

    class BaseTest : public ::testing::Test {
    public:

        void SetUp() {
            Initialize();
            CurrentEnv context;
            const char* klassNameSrvStartup = "org/apache/rocketmq/namesrv/NamesrvStartup";
            classNamesrvStartup = context.findClass(klassNameSrvStartup);
            const char* sigMain = "([Ljava/lang/String;)V";
            midNameSrvStartupMain = context.getMethodId(classNamesrvStartup, "main", sigMain, true);
            jclass classString = context.findClass("java/lang/String");
            jobjectArray args = context.env->NewObjectArray(1,
                                                            classString,
                                                            context.newStringUTF(""));

            char *rocketmq_home = "ROCKETMQ_HOME=/Users/lizhanhui/work/apache_rocketmq/distribution/target/apache-rocketmq";
            putenv(rocketmq_home);
            context.callStaticVoidMethod(classNamesrvStartup, midNameSrvStartupMain, args);

            const char *klassBrokerStartup = "org/apache/rocketmq/broker/BrokerStartup";
            classBrokerStartup = context.findClass(klassBrokerStartup);
            midBrokerStartupMain = context.getMethodId(classBrokerStartup, "main", sigMain, true);

            char *namesrv_addr = "NAMESRV_ADDR=localhost:9876";
            putenv(namesrv_addr);

            context.callStaticVoidMethod(classBrokerStartup, midBrokerStartupMain, args);

            std::cout << "Wait for 30 seconds till MQ cluster is properly setup" << std::endl;

            boost::this_thread::sleep(boost::posix_time::seconds(300));

            std::cout << "Sleep completes" << std::endl;
        }

        void TearDown() {

        }

    protected:
        jclass classNamesrvStartup;
        jmethodID midNameSrvStartupMain;

        jclass classBrokerStartup;
        jmethodID  midBrokerStartupMain;

    };

END_NAMESPACE_2(io, openmessaging)
#endif //OMS_BASE_TEST_H
