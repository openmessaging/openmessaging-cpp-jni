#ifndef OMS_MESSAGELISTENER_H
#define OMS_MESSAGELISTENER_H

namespace io {
    namespace openmessaging {
        namespace consumer {
            class MessageListener {
            public:
                virtual ~MessageListener() {

                }

                virtual void onMessage(boost::shared_ptr<Message> message,
                               boost::shared_ptr<Context> context) = 0;
            };
        }
    }
}
#endif //OMS_MESSAGELISTENER_H
