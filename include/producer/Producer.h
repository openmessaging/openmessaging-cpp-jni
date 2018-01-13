#ifndef OMS_PRODUCER_H
#define OMS_PRODUCER_H

#include <boost/shared_ptr.hpp>

#include "MessageFactory.h"
#include "ServiceLifecycle.h"
#include "Message.h"
#include "SendResult.h"

namespace io {
    namespace openmessaging {
        extern boost::shared_ptr<KeyValue> kv_nullptr;
        namespace producer {
            class Producer : public virtual MessageFactory, public virtual ServiceLifecycle {
            public:
                virtual ~Producer() {

                }

                virtual boost::shared_ptr<KeyValue> properties() = 0;

                virtual boost::shared_ptr<SendResult> send(boost::shared_ptr<Message> message,
                                                           boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;
            };
        }
    }
}

#endif //OMS_PRODUCER_H
