#ifndef OMS_MESSAGINGACCESSPOINTIMPL_H
#define OMS_MESSAGINGACCESSPOINTIMPL_H

#include "MessagingAccessPoint.h"
#include "core.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)
    class MessagingAccessPointImpl : public MessagingAccessPoint {
    public:
        MessagingAccessPointImpl(const std::string &url, const boost::shared_ptr<KeyValue> &properties, jobject proxy);

        virtual ~MessagingAccessPointImpl();

        boost::shared_ptr<KeyValue> properties();

        std::string implVersion();

        boost::shared_ptr<producer::Producer>
        createProducer(boost::shared_ptr<KeyValue> properties);

        boost::shared_ptr<consumer::PushConsumer>
        createPushConsumer(boost::shared_ptr<KeyValue> properties);

        boost::shared_ptr<consumer::PullConsumer>
        createPullConsumer(const std::string &queueName, boost::shared_ptr<KeyValue> properties);

        boost::shared_ptr<consumer::StreamingConsumer> createStreamingConsumer(const std::string &queueName,
                                                                               boost::shared_ptr<KeyValue> properties);

        boost::shared_ptr<ResourceManager> getResourceManager();

        void addObserver(boost::shared_ptr<observer::Observer> observer);

        void removeObserver(boost::shared_ptr<observer::Observer> observer);

        std::vector<boost::shared_ptr<producer::Producer> > producers();

        std::vector<boost::shared_ptr<consumer::PushConsumer> > pushConsumers();

        std::vector<boost::shared_ptr<consumer::PullConsumer> > pullConsumers();

        std::vector<boost::shared_ptr<consumer::StreamingConsumer> > streamingConsumers();

        void startup();

        void shutdown();

        jobject getProxy();

    private:
        const std::string _url;

        const boost::shared_ptr<KeyValue> _properties;

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

END_NAMESPACE_3(io, openmessaging, core)

#endif //OMS_MESSAGINGACCESSPOINTIMPL_H
