#ifndef OMS_MESSAGE_FACTORY_H
#define OMS_MESSAGE_FACTORY_H

#include <string>
#include <vector>

#include "ByteMessage.h"
#include "Namespace.h"
#include "OMS.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    /**
     * A factory interface for creating {@code Message} objects.
     *
     */
    class MessageFactory {
    public:
        virtual ~MessageFactory() {
        }

        virtual ByteMessagePtr createByteMessageToTopic(const std::string &topic, const MessageBodyPtr &body) = 0;

        virtual ByteMessagePtr createByteMessageToQueue(const std::string &topic, const MessageBodyPtr &body) = 0;
    };

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_MESSAGE_FACTORY_H
