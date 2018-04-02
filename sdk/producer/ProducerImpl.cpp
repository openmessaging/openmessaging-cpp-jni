#include <map>

#include "producer/ProducerImpl.h"
#include "producer/LocalTransactionBranchExecutorImpl.h"
#include "KeyValueImpl.h"
#include "producer/SendResultImpl.h"
#include "PromiseImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    long long sendOpaque = 0;
    boost::mutex sendAsyncMutex;
    std::map<long long, PromisePtr> sendAsyncMap;

    void sendAsyncCallback(JNIEnv *env, jobject object, jlong opaque, jobject jFuture) {

        NS::shared_ptr<Promise> promise;
        {
            boost::lock_guard<boost::mutex> lk(sendAsyncMutex);
            promise = sendAsyncMap[opaque];
            sendAsyncMap.erase(opaque);
        }

        if (!promise) {
            LOG_WARNING << "Cannot find promise of opaque: " << opaque;
            return;
        }

        CurrentEnv current(env);

        jclass classFuture = env->GetObjectClass(jFuture);
        jmethodID midIsCancelled = current.getMethodId(classFuture, "isCancelled", "()Z");

        if (current.callBooleanMethod(jFuture, midIsCancelled)) {
            promise->cancel();

            // clean up JNI calling contex
            env->DeleteLocalRef(jFuture);
            env->DeleteLocalRef(classFuture);
            env->DeleteLocalRef(object);
            return;
        }


        jmethodID midGetThrowable = current.getMethodId(classFuture, "getThrowable", "()Ljava/lang/Throwable;");
        jobject throwable = current.callObjectMethod(jFuture, midGetThrowable);

        if (env->IsSameObject(throwable, NULL)) {
            jmethodID midGet = current.getMethodId(classFuture, "get", "()Ljava/lang/Object;");
            jobject objectSendResultLocal = current.callObjectMethod(jFuture, midGet);
            jobject objectSendResult = current.newGlobalRef(objectSendResultLocal);
            if (objectSendResult) {
                NS::shared_ptr<SendResult> sendResultPtr = NS::make_shared<SendResultImpl>(objectSendResult);
                promise->set(sendResultPtr);
            }
        }

        // clean up JNI
        env->DeleteLocalRef(jFuture);
        env->DeleteLocalRef(classFuture);
        env->DeleteLocalRef(object);
    }

    static JNINativeMethod methods[] = {
        {
            const_cast<char*>("sendAsyncCallback"),
            const_cast<char*>("(JLio/openmessaging/Future;)V"),
            (void *)&sendAsyncCallback
        }
    };

END_NAMESPACE_3(io, openmessaging, producer)

ProducerImpl::ProducerImpl(jobject proxy, const KeyValuePtr &props)
        : ServiceLifecycleImpl(proxy), _properties(props) {
    CurrentEnv current;
    const char *clazzProducer = "io/openmessaging/producer/Producer";
    const char *clazzProducerAdaptor = "io/openmessaging/producer/ProducerAdaptor";

    classProducer = current.findClass(clazzProducer);
    classProducerAdaptor = current.findClass(clazzProducerAdaptor);

    if (current.env->RegisterNatives(classProducerAdaptor, methods, 1) < 0) {
        LOG_ERROR << "Failed to bind native methods";
        abort();
    }

    jmethodID producerAdaptorCtor = current.getMethodId(classProducerAdaptor, "<init>",
                                                        buildSignature(Types::void_, 1, Types::Producer_));
    objectProducerAdaptor = current.newObject(classProducerAdaptor, producerAdaptorCtor, proxy);

    const std::string signature = buildSignature(Types::ByteMessage_, 2, Types::String_, Types::byteArray_);
    midCreateByteMessageToQueue = current.getMethodId(classProducer, "createQueueBytesMessage", signature);
    midCreateByteMessageToTopic = current.getMethodId(classProducer, "createTopicBytesMessage", signature);
    midStartup = current.getMethodId(classProducer, "startup", buildSignature(Types::void_, 0));
    midShutdown = current.getMethodId(classProducer, "shutdown", buildSignature(Types::void_, 0));

    midSend = current.getMethodId(classProducer, "send", buildSignature(Types::SendResult_, 1, Types::Message_));

    midSend2 = current.getMethodId(classProducer, "send",
                                   buildSignature(Types::SendResult_, 2, Types::Message_, Types::KeyValue_));

    midSend3 = current.getMethodId(classProducer, "send",
                                   buildSignature(Types::SendResult_, 4, Types::Message_,
                                                  Types::LocalTransactionBranchExecutor_, Types::Object_,
                                                  Types::KeyValue_));

    midSendAsync = current.getMethodId(classProducerAdaptor, "sendAsync",
                                       buildSignature(Types::void_, 2, Types::long_, Types::Message_));

    midSendAsync2 = current.getMethodId(classProducerAdaptor, "sendAsync",
                                        buildSignature(Types::void_, 3, Types::long_, Types::Message_, Types::KeyValue_));

    midSendOneway = current.getMethodId(classProducer, "sendOneway", buildSignature(Types::void_, 1, Types::Message_));

    midSendOneway2 = current.getMethodId(classProducer, "sendOneway",
                                         buildSignature(Types::void_, 2, Types::Message_, Types::KeyValue_));
}

ProducerImpl::~ProducerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classProducer);
    current.env->DeleteGlobalRef(classProducerAdaptor);
    current.env->DeleteGlobalRef(objectProducerAdaptor);
}

KeyValuePtr ProducerImpl::attributes() {
    return _properties;
}

SendResultPtr ProducerImpl::send(const MessagePtr &message, const KeyValuePtr &props) {
    CurrentEnv current;

    ByteMessageImplPtr msg = NS::dynamic_pointer_cast<ByteMessageImpl>(message);
    jobject jSendResult;
    if (props) {
        NS::shared_ptr<KeyValueImpl> kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        jobject ret = current.callObjectMethod(_proxy, midSend2, msg->getProxy(), kv->getProxy());
        jSendResult = current.newGlobalRef(ret);
    } else {
        jobject ret = current.callObjectMethod(_proxy, midSend, msg->getProxy());
        jSendResult = current.newGlobalRef(ret);
    }

    SendResultPtr sendResult = NS::make_shared<SendResultImpl>(jSendResult);
    return sendResult;
}

ByteMessagePtr ProducerImpl::createByteMessageToQueue(const std::string &topic,
                                                                   const MessageBodyPtr &body) {
    CurrentEnv current;
    jstring pTopic = current.env->NewStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.getLength());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte *>(body.getRawPtr()));
    jobject jMessage = current.callObjectMethod(_proxy, midCreateByteMessageToQueue, pTopic, pBody);
    current.env->DeleteLocalRef(pBody);
    current.env->DeleteLocalRef(pTopic);

    NS::shared_ptr<ByteMessage> message = NS::make_shared<ByteMessageImpl>(current.newGlobalRef(jMessage));
    return message;
}

SendResultPtr ProducerImpl::send(const MessagePtr &message,
                                 const LocalTransactionBranchExecutorPtr &executor,
                                 const KeyValuePtr &props) {

    NS::shared_ptr<ByteMessageImpl> messageImpl = NS::dynamic_pointer_cast<ByteMessageImpl>(message);
    NS::shared_ptr<LocalTransactionBranchExecutorImpl> executorImpl =
            NS::dynamic_pointer_cast<LocalTransactionBranchExecutorImpl>(executor);
    NS::shared_ptr<KeyValueImpl> propertiesImpl = NS::dynamic_pointer_cast<KeyValueImpl>(props);
    CurrentEnv current;
    jobject jSendResult = current.callObjectMethod(_proxy, midSend3, messageImpl->getProxy(),
                                                        executorImpl->getProxy(), NULL, propertiesImpl->getProxy());

    NS::shared_ptr<SendResultImpl> ret = NS::make_shared<SendResultImpl>(current.newGlobalRef(jSendResult));
    return ret;
}

FuturePtr ProducerImpl::sendAsync(const MessagePtr &message, const KeyValuePtr &props) {

    NS::shared_ptr<ByteMessageImpl> messageImpl = NS::dynamic_pointer_cast<ByteMessageImpl>(message);

    if (messageImpl) {
        CurrentEnv current;
        NS::shared_ptr<Promise> ft = NS::make_shared<PromiseImpl>();
        long long opaque;
        {
            opaque = ++sendOpaque;
            boost::lock_guard<boost::mutex> lk(sendAsyncMutex);
            sendAsyncMap[opaque] = ft;
        }
        if (props) {
            NS::shared_ptr<KeyValueImpl> propertiesImpl = NS::dynamic_pointer_cast<KeyValueImpl>(props);
            if (!propertiesImpl) {
                LOG_ERROR << "Dynamic casting failed";
            }
            current.callVoidMethod(objectProducerAdaptor, midSendAsync2, opaque, messageImpl->getProxy(),
                                   propertiesImpl->getProxy());
        } else {
            current.callVoidMethod(objectProducerAdaptor, midSendAsync, opaque, messageImpl->getProxy());
        }
        return ft;
    } else {
        LOG_ERROR << "Dynamic casting failed";
        abort();
    }
}

BatchMessageSenderPtr ProducerImpl::createSequenceBatchMessageSender() {
    throw OMSException("Not Implemented");
}

void ProducerImpl::addInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) {
    throw OMSException("Not Implemented");
}

void ProducerImpl::removeInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) {
    throw OMSException("Not Implemented");
}
