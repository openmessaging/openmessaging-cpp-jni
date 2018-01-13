#ifndef OMS_PULLCONSUMER_H
#define OMS_PULLCONSUMER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "ServiceLifecycle.h"

namespace io {
    namespace openmessaging {
        namespace consumer {
            class PullConsumer : public virtual ServiceLifecycle {
            public:
                virtual ~PullConsumer() {

                }

                virtual boost::shared_ptr<KeyValue> properties() = 0;

                virtual boost::shared_ptr<Message> poll(boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;

                virtual void ack(const std::string &messageId,
                                 boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;
            };
        }
    }
}

#endif //OMS_PULLCONSUMER_H
