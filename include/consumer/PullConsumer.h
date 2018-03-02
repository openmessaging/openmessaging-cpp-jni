#ifndef OMS_PULL_CONSUMER_H
#define OMS_PULL_CONSUMER_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "ServiceLifecycle.h"
#include "Namespace.h"
#include "Message.h"
#include "OMS.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class PullConsumer : public virtual ServiceLifecycle {
    public:
        virtual ~PullConsumer() {

        }

        virtual boost::shared_ptr<KeyValue> properties() = 0;

        virtual boost::shared_ptr<Message> poll(const boost::shared_ptr<KeyValue> &props = kv_nullptr) = 0;

        virtual void ack(const std::string &messageId,
                         const boost::shared_ptr<KeyValue> &props = kv_nullptr) = 0;
    };

END_NAMESPACE_3(io, openmessaging, consumer)

#endif //OMS_PULL_CONSUMER_H
