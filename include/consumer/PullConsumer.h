#ifndef OMS_PULL_CONSUMER_H
#define OMS_PULL_CONSUMER_H

#include <string>

#include "smart_pointer.h"
#include "KeyValue.h"
#include "ServiceLifecycle.h"
#include "Namespace.h"
#include "Message.h"
#include "OMS.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    /**
     * A {@code PullConsumer} object can poll messages from the specified queue,
     * and supports submit the consume result by acknowledgement.
     *
     * @version OMS 1.0
     * @see MessagingAccessPoint#createPullConsumer(String)
     * @since OMS 1.0
     */
    class PullConsumer : public virtual ServiceLifecycle {
    public:
        virtual ~PullConsumer() {

        }

        virtual KeyValuePtr properties() = 0;

        virtual MessagePtr poll(const KeyValuePtr &props = kv_nullptr) = 0;

        virtual void ack(const std::string &messageId,
                         const KeyValuePtr &props = kv_nullptr) = 0;
    };
    typedef NS::shared_ptr<PullConsumer> PullConsumerPtr;


END_NAMESPACE_3(io, openmessaging, consumer)

#endif //OMS_PULL_CONSUMER_H
