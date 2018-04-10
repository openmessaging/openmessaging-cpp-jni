#ifndef OMS_BATCH_MESSAGE_SENDER_IMPL_H
#define OMS_BATCH_MESSAGE_SENDER_IMPL_H

#include "producer/BatchMessageSender.h"
#include "Namespace.h"
#include "core.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)
    class BatchMessageSenderImpl : public BatchMessageSender {
    public:
        BatchMessageSenderImpl(const jobject &proxy);

        virtual ~BatchMessageSenderImpl() {
        }

        virtual BatchMessageSender& send(MessagePtr &message);

        virtual void commit();

        virtual void rollback();

        virtual void close();

        jobject getProxy() {
            return proxy_;
        }

    private:
        jclass classBatchMessageSender;

        jobject proxy_;

        jmethodID midSend;
        jmethodID midCommit;
        jmethodID midRollback;
        jmethodID midClose;
    };

    typedef NS::shared_ptr<BatchMessageSenderImpl> BatchMessageSenderImplPtr;

END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_BATCH_MESSAGE_SENDER_IMPL_H
