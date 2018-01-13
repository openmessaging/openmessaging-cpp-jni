#ifndef OMS_PUSHCONSUMERINTERCEPTOR_H
#define OMS_PUSHCONSUMERINTERCEPTOR_H

namespace io {
    namespace openmessaging {
        namespace consumer {
            class PushConsumerInterceptor {
            public:
                ~PushConsumerInterceptor() {
                }

                void preHandle(boost::shared_ptr<void> context) = 0;

                void postHandle(boost::shared_ptr<void> context) = 0;
            };
        }
    }
}

#endif //OMS_PUSHCONSUMERINTERCEPTOR_H
