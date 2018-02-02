#ifndef OMS_PRODUCER_H
#define OMS_PRODUCER_H

#include <boost/shared_ptr.hpp>

#include "MessageFactory.h"
#include "ServiceLifecycle.h"
#include "Message.h"
#include "SendResult.h"
#include "Namespace.h"
#include "OMS.h"
#include "LocalTransactionBranchExecutor.h"
#include "Future.h"
#include "BatchMessageSender.h"
#include "interceptor/ProducerInterceptor.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)
    class Producer : public virtual MessageFactory, public virtual ServiceLifecycle {
    public:
        virtual ~Producer() {

        }

        virtual boost::shared_ptr<KeyValue> properties() = 0;

        virtual boost::shared_ptr<SendResult> send(boost::shared_ptr<Message> message,
                                                   boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;

        virtual boost::shared_ptr<SendResult> send(boost::shared_ptr<Message> message,
                                                   boost::shared_ptr<LocalTransactionBranchExecutor> executor,
                                                   boost::shared_ptr<void> arg,
                                                   boost::shared_ptr<KeyValue> properties) = 0;

        /**
         * Asynchronously send a message to its destination, which is specified in system headers.
         *
         * This method returns immediately after invocation and returning future follows similar
         * semantics to that of C++ standard library. Additionally, future accepts callback hooks
         * that are executed on future completion.
         *
         * @param message The message to send.
         * @param properties Optional additional properties.
         * @return Smart pointer to Future instance.
         */
        virtual boost::shared_ptr<Future>
        sendAsync(boost::shared_ptr<Message> message,
                                 boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;

        virtual void sendOneway(boost::shared_ptr<Message> message,
                                boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;

        virtual boost::shared_ptr<BatchMessageSender> createSequenceBatchMessageSender() = 0;

        virtual void addInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor) = 0;

        virtual void removeInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor) = 0;
    };
END_NAMESPACE_3(io, openmessaging, producer)
#endif //OMS_PRODUCER_H
