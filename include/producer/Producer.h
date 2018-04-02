#ifndef OMS_PRODUCER_H
#define OMS_PRODUCER_H

#include "smart_pointer.h"
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

    /**
     * A {@code Producer} is a simple object used to send messages on behalf
     * of a {@code MessagingAccessPoint}. An instance of {@code Producer} is
     * created by calling the {@link MessagingAccessPoint#createProducer()} method.
     * It provides various {@code send} methods to send a message to a specified destination.
     * A destination can be a {@link MessageHeader#TOPIC} or a {@link MessageHeader#QUEUE}.
     * <p>
     *
     * {@link Producer#send(Message)} means send a message to destination synchronously,
     * the calling thread will block until the send request complete.
     * <p>
     * {@link Producer#sendAsync(Message)} means send a message to destination asynchronously,
     * the calling thread won't block and will return immediately. Since the send call is asynchronous
     * it returns a {@link Promise} for the send result.
     * <p>
     * {@link Producer#sendOneway(Message)} means send a message to destination in one way,
     * the calling thread won't block and will return immediately. The caller won't care about
     * the send result, while the server has no responsibility for returning the result.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class Producer : public virtual MessageFactory, public virtual ServiceLifecycle {
    public:
        virtual ~Producer() {

        }

        virtual KeyValuePtr properties() = 0;

        virtual SendResultPtr send(const MessagePtr &message,
                                                   const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual SendResultPtr send(const MessagePtr &message,
                                                   const LocalTransactionBranchExecutorPtr &executor,
                                                   const NS::shared_ptr<void> &arg,
                                                   const KeyValuePtr &properties) = 0;

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
        virtual FuturePtr
        sendAsync(const MessagePtr &message,
                  const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual void sendOneway(const MessagePtr &message,
                                const KeyValuePtr &properties = kv_nullptr) = 0;

        virtual BatchMessageSenderPtr createSequenceBatchMessageSender() = 0;

        virtual void addInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) = 0;

        virtual void removeInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) = 0;
    };

    typedef NS::shared_ptr<Producer> ProducerPtr;

END_NAMESPACE_3(io, openmessaging, producer)
#endif //OMS_PRODUCER_H
