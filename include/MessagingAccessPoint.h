#ifndef OMS_MESSAGING_ACCESS_POINT_H
#define OMS_MESSAGING_ACCESS_POINT_H

#include <string>

#include "ServiceLifecycle.h"
#include "KeyValue.h"
#include "producer/Producer.h"
#include "consumer/PullConsumer.h"
#include "consumer/PushConsumer.h"
#include "consumer/StreamingConsumer.h"
#include "ResourceManager.h"
#include "observer/Observer.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    /**
     * The {@code MessagingAccessPoint} obtained from {@link MessagingAccessPointFactory} is capable of creating {@code
     * Producer}, {@code Consumer}, {@code ServiceEndPoint}, and so on. <p> For example:
     * <pre>
     * MessagingAccessPoint messagingAccessPoint = MessagingAccessPointFactory.getMessagingAccessPoint("openmessaging:rocketmq://localhost:10911/namespace");
     * Producer producer = messagingAccessPoint.createProducer();
     * producer.send(producer.createTopicBytesMessage("HELLO_TOPIC", "HELLO_BODY".getBytes(Charset.forName("UTF-8"))));
     * </pre>
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class MessagingAccessPoint : public virtual ServiceLifecycle {
    public:
        virtual ~MessagingAccessPoint() {

        }

        virtual KeyValuePtr properties() = 0;

        virtual std::string implVersion() = 0;

        virtual producer::ProducerPtr createProducer(const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual consumer::PushConsumerPtr createPushConsumer(const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual consumer::PullConsumerPtr createPullConsumer(const std::string &queueName, const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual consumer::StreamingConsumerPtr createStreamingConsumer(const std::string &queueName, const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual ResourceManagerPtr getResourceManager() = 0;

        virtual void addObserver(const observer::ObserverPtr &observer) = 0;

        virtual void removeObserver(const observer::ObserverPtr &observer) = 0;

        virtual std::vector<producer::ProducerPtr> producers() = 0;

        virtual std::vector<consumer::PushConsumerPtr> pushConsumers() = 0;

        virtual std::vector<consumer::PullConsumerPtr> pullConsumers() = 0;

        virtual std::vector<consumer::StreamingConsumerPtr> streamingConsumers() = 0;
    };

    typedef NS::shared_ptr<MessagingAccessPoint> MessagingAccessPointPtr;

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_MESSAGING_ACCESS_POINT_H
