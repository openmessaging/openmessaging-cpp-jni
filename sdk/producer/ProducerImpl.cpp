#include <map>

#include "producer/ProducerImpl.h"
#include "producer/LocalTransactionExecutorImpl.h"
#include "producer/BatchMessageSenderImpl.h"
#include "KeyValueImpl.h"
#include "producer/SendResultImpl.h"
#include "PromiseImpl.h"
#include "ByteMessageImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    long long sendOpaque = 0;
    boost::mutex sendAsyncMutex;
    std::map<long long, PromisePtr> sendAsyncMap;

    int interceptorIndex = 0;
    boost::mutex interceptorMutex;
    std::map<int, interceptor::ProducerInterceptorPtr> interceptorMap;

    void operationComplete(JNIEnv *env, jobject object, jlong opaque, jobject jFuture) {

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
            current.deleteRef(jFuture);
            current.deleteRef(classFuture);
            current.deleteRef(object);
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
        current.deleteRef(jFuture);
        current.deleteRef(classFuture);
        current.deleteRef(object);
    }

    void doPreSend(JNIEnv *env, jobject object, int index, jobject message, jobject attributes) {
        ByteMessageImplPtr msg(new ByteMessageImpl(message));
        KeyValueImplPtr attr(new KeyValueImpl(attributes));
        {
            boost::lock_guard<boost::mutex> lk(interceptorMutex);
            std::map<int, interceptor::ProducerInterceptorPtr>::iterator it = interceptorMap.find(index);
            if (it != interceptorMap.end()) {
                it->second->preSend(msg, attr);
            }
        }
    }

    void doPostSend(JNIEnv* env, jobject object, int index, jobject message, jobject attributes, jobject exception) {
        ByteMessageImplPtr msg(new ByteMessageImpl(message));
        KeyValueImplPtr attr(new KeyValueImpl(attributes));
        {
            boost::lock_guard<boost::mutex> lk(interceptorMutex);
            std::map<int, interceptor::ProducerInterceptorPtr>::iterator it = interceptorMap.find(index);
            if (it != interceptorMap.end()) {
                it->second->postSend(msg, attr);
            }
        }
    }

    static JNINativeMethod sendAsyncMethods[] = {
        {
            const_cast<char*>("operationComplete"),
            const_cast<char*>("(JLio/openmessaging/Future;)V"),
            (void *)&operationComplete
        }
    };

    static JNINativeMethod producerInterceptorMethods[] = {
        {
                const_cast<char*>("doPreSend"),
                const_cast<char*>(buildSignature(Types::void_, 3, Types::int_, Types::Message_, Types::KeyValue_).c_str()),
                (void*)&doPreSend
        },
        {
                const_cast<char*>("doPostSend"),
                const_cast<char*>(buildSignature(Types::void_, 4, Types::int_, Types::Message_, Types::KeyValue_,
                                                 Types::OMSException_).c_str()),
                (void*)&doPostSend
        }
    };

END_NAMESPACE_3(io, openmessaging, producer)

ProducerImpl::ProducerImpl(jobject proxy, const KeyValuePtr &props)
        : ServiceLifecycleImpl(proxy), _properties(props) {
    CurrentEnv current;
    const char *clazzProducer = "io/openmessaging/producer/Producer";
    const char *clazzProducerAdaptor = "io/openmessaging/producer/ProducerAdaptor";
    const char *clazzProducerInterceptor = "io/openmessaging/interceptor/ProducerInterceptor";

    classProducer = current.findClass(clazzProducer);
    classProducerAdaptor = current.findClass(clazzProducerAdaptor);
    classProducerInterceptor = current.findClass(clazzProducerInterceptor);

    if (current.env->RegisterNatives(classProducerAdaptor, sendAsyncMethods, 1) < 0) {
        LOG_ERROR << "Failed to bind native methods to async send";
        abort();
    }

    if (current.env->RegisterNatives(classProducerInterceptor, producerInterceptorMethods, 2) < 0) {
        LOG_ERROR << "Failed to bind native methods to preSend and postSend";
        abort();
    }

    jmethodID producerAdaptorCtor = current.getMethodId(classProducerAdaptor, "<init>",
                                                        buildSignature(Types::void_, 1, Types::Producer_));
    objectProducerAdaptor = current.newObject(classProducerAdaptor, producerAdaptorCtor, proxy);

    const std::string signature = buildSignature(Types::ByteMessage_, 2, Types::String_, Types::byteArray_);
    midCreateBytesMessage = current.getMethodId(classProducer, "createBytesMessage", signature);

    midAttributes = current.getMethodId(classProducer, "attributes", buildSignature(Types::KeyValue_, 0));
    midSend = current.getMethodId(classProducer, "send", buildSignature(Types::SendResult_, 1, Types::Message_));
    midSend2 = current.getMethodId(classProducer, "send",
                                   buildSignature(Types::SendResult_, 2, Types::Message_, Types::KeyValue_));

    midSend3 = current.getMethodId(classProducer, "send",
                                   buildSignature(Types::SendResult_, 3, Types::Message_,
                                                  Types::LocalTransactionExecutor_,
                                                  Types::KeyValue_));

    midSendAsync = current.getMethodId(classProducerAdaptor, "sendAsync",
                                       buildSignature(Types::void_, 2, Types::long_, Types::Message_));

    midSendAsync2 = current.getMethodId(classProducerAdaptor, "sendAsync",
                                        buildSignature(Types::void_, 3, Types::long_, Types::Message_, Types::KeyValue_));

    midSendOneway = current.getMethodId(classProducer, "sendOneway",
                                        buildSignature(Types::void_, 1, Types::Message_));

    midSendOneway2 = current.getMethodId(classProducer, "sendOneway",
                                        buildSignature(Types::void_, 2, Types::Message_, Types::KeyValue_));

    midAddInterceptor = current.getMethodId(classProducerAdaptor, "addInterceptor",
                                            buildSignature(Types::void_, 2, Types::int_, Types::ProducerInterceptor_));
    midRemoveInterceptor = current.getMethodId(classProducerAdaptor, "removeInterceptor",
                                               buildSignature(Types::void_, 1, Types::int_));

    midProducerInterceptor = current.getMethodId(classProducerInterceptor, "<init>",
                                                 buildSignature(Types::void_, 0));

}

ProducerImpl::~ProducerImpl() {
    CurrentEnv current;
    current.deleteRef(classProducer);
    current.deleteRef(classProducerAdaptor);
    current.deleteRef(objectProducerAdaptor);
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

ByteMessagePtr ProducerImpl::createBytesMessage(const std::string &topic, const MessageBodyPtr &body) {
    CurrentEnv current;
    jstring pTopic = current.newStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.getLength());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte *>(body.getRawPtr()));
    jobject jMessage = current.callObjectMethod(_proxy, midCreateBytesMessage, pTopic, pBody);
    current.deleteRef(pBody);
    current.deleteRef(pTopic);

    NS::shared_ptr<ByteMessage> message = NS::make_shared<ByteMessageImpl>(current.newGlobalRef(jMessage));
    return message;
}

SendResultPtr ProducerImpl::send(const MessagePtr &message,
                                 const LocalTransactionExecutorPtr &executor,
                                 const KeyValuePtr &props) {

    ByteMessageImplPtr messageImpl = NS::dynamic_pointer_cast<ByteMessageImpl>(message);
    NS::shared_ptr<LocalTransactionExecutorImpl> executorImpl =
            NS::dynamic_pointer_cast<LocalTransactionExecutorImpl>(executor);
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

void ProducerImpl::sendOneway(const MessagePtr &message, const KeyValuePtr &properties) {
    CurrentEnv context;

    ByteMessageImplPtr msgPtr = NS::dynamic_pointer_cast<ByteMessageImpl>(message);

    if (properties) {
        KeyValueImplPtr ptr = NS::dynamic_pointer_cast<KeyValueImpl>(properties);
        context.callVoidMethod(_proxy, midSendOneway2, msgPtr->getProxy(), ptr->getProxy());
        return;
    }

    context.callVoidMethod(_proxy, midSendOneway, msgPtr->getProxy());
}

BatchMessageSenderPtr ProducerImpl::createSequenceBatchMessageSender() {
    CurrentEnv context;
    jobject jBatchMessageSender = context.callObjectMethod(_proxy, midCreateBatchMessageSender);
    BatchMessageSenderImplPtr result(new BatchMessageSenderImpl(jBatchMessageSender));
    return result;
}

void ProducerImpl::addInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) {
    CurrentEnv context;
    int index;
    {
        boost::lock_guard<boost::mutex> lk(interceptorMutex);
        index = ++interceptorIndex;
        interceptorMap[index] = interceptor;
    }

    jobject jInterceptor = context.newObject(classProducerInterceptor, midProducerInterceptor);
    context.callVoidMethod(objectProducerAdaptor, midAddInterceptor, index, jInterceptor);
    context.deleteRef(jInterceptor);
}

void ProducerImpl::removeInterceptor(const interceptor::ProducerInterceptorPtr &interceptor) {
    {
        boost::lock_guard<boost::mutex> lk(interceptorMutex);
        for (std::map<int, interceptor::ProducerInterceptorPtr>::iterator it = interceptorMap.begin();
                it != interceptorMap.end(); it++) {
            if (it->second == interceptor) {
                interceptorMap.erase(it->first);
                break;
            }
        }
    }
}
