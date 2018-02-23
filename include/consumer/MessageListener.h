#ifndef OMS_MESSAGE_LISTENER_H
#define OMS_MESSAGE_LISTENER_H

#include "Context.h"
#include "Namespace.h"
#include "Message.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    class MessageListener {
    public:
        virtual ~MessageListener() {

        }

        virtual void onMessage(boost::shared_ptr<Message>& message, boost::shared_ptr<Context>& context) = 0;
    };

END_NAMESPACE_3(io, openmessaging, consumer)
#endif //OMS_MESSAGE_LISTENER_H
