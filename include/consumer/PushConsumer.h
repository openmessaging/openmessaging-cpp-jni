#ifndef OMS_PUSH_CONSUMER_H
#define OMS_PUSH_CONSUMER_H

#include <string>

#include "smart_pointer.h"
#include "ServiceLifecycle.h"
#include "PushConsumerInterceptor.h"
#include "MessageListener.h"
#include "Namespace.h"
#include "OMS.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)
    /**
     * A {@code PushConsumer} object to receive messages from multiple queues, these messages are pushed from
     * MOM server to {@code PushConsumer} client.
     *
     * @version OMS 1.0
     * @see MessagingAccessPoint#createPushConsumer()
     * @since OMS 1.0
     */
    class PushConsumer : public virtual ServiceLifecycle {
    public:
        virtual ~PushConsumer() {

        }

        virtual KeyValuePtr properties() = 0;

        virtual void resume() = 0;

        virtual void suspend() = 0;

        virtual bool isSuspended() = 0;

        virtual PushConsumer &attachQueue(const std::string &queueName,
                                          const MessageListenerPtr &listener,
                                          const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual PushConsumer &detachQueue(const std::string &queueName) = 0;

        virtual void addInterceptor(const PushConsumerInterceptorPtr &interceptor) = 0;

        virtual void removeInterceptor(const PushConsumerInterceptorPtr &interceptor) = 0;
    };

    typedef NS::shared_ptr<PushConsumer> PushConsumerPtr;

END_NAMESPACE_3(io, openmessaging, consumer)

#endif //OMS_PUSH_CONSUMER_H
