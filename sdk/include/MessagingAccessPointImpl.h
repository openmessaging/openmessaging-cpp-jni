#ifndef OMS_MESSAGING_ACCESS_POINT_IMPL_H
#define OMS_MESSAGING_ACCESS_POINT_IMPL_H

#include "MessagingAccessPoint.h"
#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class MessagingAccessPointImpl : public MessagingAccessPoint {
    public:
        MessagingAccessPointImpl(const std::string &url, const KeyValuePtr &properties, jobject proxy);

        virtual ~MessagingAccessPointImpl();

        KeyValuePtr attributes();

        std::string implVersion();

        producer::ProducerPtr createProducer(const KeyValuePtr &properties);

        consumer::PushConsumerPtr createPushConsumer(const KeyValuePtr &properties);

        consumer::PullConsumerPtr createPullConsumer(const std::string &queueName, const KeyValuePtr &properties);

        consumer::StreamingConsumerPtr createStreamingConsumer(const std::string &queueName, const KeyValuePtr &properties);

        ResourceManagerPtr resourceManager();

        void startup();

        void shutdown();

        jobject getProxy();

    private:
        const std::string _url;

        const KeyValuePtr _properties;

        jobject objectMessagingAccessPoint;

        jclass classMessagingAccessPoint;

        jmethodID  midImplVersion;

        jmethodID  midCreateProducer;
        jmethodID  midCreateProducer2;

        jmethodID  midCreatePushConsumer;
        jmethodID  midCreatePushConsumer2;

        jmethodID  midCreatePullConsumer;
        jmethodID  midCreatePullConsumer2;

        jmethodID  midCreateStreamingConsumer;
        jmethodID  midGetResourceManager;
        jmethodID  midAddObserver;
        jmethodID  midRemoveObserver;
        jmethodID  midProducers;
        jmethodID  midPushConsumers;
        jmethodID  midPullConsumers;
        jmethodID  midStreamingConsumers;
        jmethodID  midStartup;
        jmethodID  midShutdown;

    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_MESSAGING_ACCESS_POINT_IMPL_H
