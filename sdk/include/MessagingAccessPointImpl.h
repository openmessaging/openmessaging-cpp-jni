#ifndef OMS_MESSAGINGACCESSPOINTIMPL_H
#define OMS_MESSAGINGACCESSPOINTIMPL_H

#include "MessagingAccessPoint.h"

namespace io {
    namespace openmessaging {
        namespace core {
            class MessagingAccessPointImpl : public MessagingAccessPoint {
            public:
                MessagingAccessPointImpl(const std::string &url, const boost::shared_ptr<KeyValue> &properties);

                virtual ~MessagingAccessPointImpl();

                boost::shared_ptr<KeyValue> properties();

                std::string implVersion();

                boost::shared_ptr<producer::Producer>
                createProducer(boost::shared_ptr<KeyValue> properties = kv_nullptr);

                boost::shared_ptr<consumer::PushConsumer>
                createPushConsumer(boost::shared_ptr<KeyValue> properties = kv_nullptr);

                boost::shared_ptr<consumer::PullConsumer>
                createPullConsumer(const std::string &queueName, boost::shared_ptr<KeyValue> properties = kv_nullptr);

                boost::shared_ptr<consumer::StreamingConsumer> createStreamingConsumer(const std::string &queueName,
                                                                                       boost::shared_ptr<KeyValue> properites = kv_nullptr);

                boost::shared_ptr<ResourceManager> getResourceManager();

                void addObserver(boost::shared_ptr<observer::Observer> observer);

                void removeObserver(boost::shared_ptr<observer::Observer> observer);

                std::vector<boost::shared_ptr<producer::Producer> > producers();

                std::vector<boost::shared_ptr<consumer::PushConsumer> > pushConsumers();

                std::vector<boost::shared_ptr<consumer::PullConsumer> > pullConsumers();

                std::vector<boost::shared_ptr<consumer::StreamingConsumer> > streamingConsumers();

                void startup();

                void shutdown();

            private:
                const std::string _url;

                const boost::shared_ptr<KeyValue> _properties;
            };
        }
    }
}

#endif //OMS_MESSAGINGACCESSPOINTIMPL_H
