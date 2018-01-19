#ifndef OMS_BATCHMESSAGESENDER_H
#define OMS_BATCHMESSAGESENDER_H

#include "Namespace.h"
#include "Message.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class BatchMessageSender {
    public:
        virtual ~BatchMessageSender() {

        }

        virtual void send(Message &message, boost::shared_ptr<KeyValue> &properties = kv_nullptr) = 0;

        virtual void commit() = 0;

        virtual void rollback() = 0;

        virtual void close() = 0;
    };

END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_BATCHMESSAGESENDER_H
