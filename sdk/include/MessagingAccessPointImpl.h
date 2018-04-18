#ifndef OMS_MESSAGING_ACCESS_POINT_IMPL_H
#define OMS_MESSAGING_ACCESS_POINT_IMPL_H

#include "MessagingAccessPoint.h"
#include "ServiceLifecycleImpl.h"
#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class MessagingAccessPointImpl : public virtual MessagingAccessPoint, public virtual ServiceLifecycleImpl {
    public:
        MessagingAccessPointImpl(const std::string &url, const KeyValuePtr &properties, jobject proxy);

        virtual ~MessagingAccessPointImpl();

        KeyValuePtr attributes();

        std::string implVersion();

        producer::ProducerPtr createProducer(const KeyValuePtr &properties);

        consumer::PushConsumerPtr createPushConsumer(const KeyValuePtr &properties);

        consumer::PullConsumerPtr createPullConsumer(const KeyValuePtr &properties);

        consumer::StreamingConsumerPtr createStreamingConsumer(const KeyValuePtr &properties);

        ResourceManagerPtr resourceManager();

        jobject getProxy();

    private:
        const std::string _url;

        const KeyValuePtr _properties;

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

    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_MESSAGING_ACCESS_POINT_IMPL_H
