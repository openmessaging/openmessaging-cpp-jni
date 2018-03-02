#include <map>

#include "producer/ProducerImpl.h"
#include "producer/LocalTransactionBranchExecutorImpl.h"
#include "KeyValueImpl.h"
#include "producer/SendResultImpl.h"
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "PromiseImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    long long sendOpaque = 0;
    boost::mutex sendAsyncMutex;
    std::map<long long, boost::shared_ptr<Promise> > sendAsyncMap;

    void sendAsyncCallback(JNIEnv *env, jobject object, jlong opaque, jobject jFuture) {

        boost::shared_ptr<Promise> promise;
        {
            boost::lock_guard<boost::mutex> lk(sendAsyncMutex);
            promise = sendAsyncMap[opaque];
            sendAsyncMap.erase(opaque);
        }

        if (!promise) {
            BOOST_LOG_TRIVIAL(warning) << "Cannot find promise of opaque: " << opaque;
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
                boost::shared_ptr<SendResult> sendResultPtr = boost::make_shared<SendResultImpl>(objectSendResult);
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

ProducerImpl::ProducerImpl(jobject proxy,
                           const boost::shared_ptr<KeyValue> &props)
        : ServiceLifecycleImpl(proxy), _properties(props) {
    CurrentEnv current;
    const char *clazzProducer = "io/openmessaging/producer/Producer";
    const char *clazzProducerAdaptor = "io/openmessaging/producer/ProducerAdaptor";

    classProducer = current.findClass(clazzProducer);
    classProducerAdaptor = current.findClass(clazzProducerAdaptor);

    if (current.env->RegisterNatives(classProducerAdaptor, methods, 1) < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to bind native methods";
        abort();
    }

    jmethodID producerAdaptorCtor = current.getMethodId(classProducerAdaptor, "<init>", "(Lio/openmessaging/producer/Producer;)V");
    objectProducerAdaptor = current.newObject(classProducerAdaptor, producerAdaptorCtor, proxy);

    const char *signature = "(Ljava/lang/String;[B)Lio/openmessaging/BytesMessage;";
    midCreateByteMessageToQueue = current.getMethodId(classProducer, "createQueueBytesMessage", signature);
    midCreateByteMessageToTopic = current.getMethodId(classProducer, "createTopicBytesMessage", signature);
    midStartup = current.getMethodId(classProducer, "startup", "()V");
    midShutdown = current.getMethodId(classProducer, "shutdown", "()V");

    const char *sendSignature = "(Lio/openmessaging/Message;)Lio/openmessaging/producer/SendResult;";
    midSend = current.getMethodId(classProducer, "send", sendSignature);

    const char *send2Signature = "(Lio/openmessaging/Message;Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/SendResult;";
    midSend2 = current.getMethodId(classProducer, "send", send2Signature);

    const char *send3Signature = "(Lio/openmessaging/Message;Lio/openmessaging/producer/LocalTransactionBranchExecutor;Ljava/lang/Object;Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/SendResult;";
    midSend3 = current.getMethodId(classProducer, "send", send3Signature);

    const char *sendAsyncSignature = "(JLio/openmessaging/Message;)V";
    midSendAsync = current.getMethodId(classProducerAdaptor, "sendAsync", sendAsyncSignature);

    const char *sendAsync2Signature = "(JLio/openmessaging/Message;Lio/openmessaging/KeyValue;)V";
    midSendAsync2 = current.getMethodId(classProducerAdaptor, "sendAsync", sendAsync2Signature);

    const char *sendOnewaySignature = "(Lio/openmessaging/Message;)V";
    midSendOneway = current.getMethodId(classProducer, "sendOneway", sendOnewaySignature);

    const char *sendOneway2Signature = "(Lio/openmessaging/Message;Lio/openmessaging/KeyValue;)V";
    midSendOneway2 = current.getMethodId(classProducer, "sendOneway", sendOneway2Signature);
}

ProducerImpl::~ProducerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classProducer);
    current.env->DeleteGlobalRef(classProducerAdaptor);
    current.env->DeleteGlobalRef(objectProducerAdaptor);
}

boost::shared_ptr<KeyValue> ProducerImpl::properties() {
    return _properties;
}

boost::shared_ptr<SendResult> ProducerImpl::send(const boost::shared_ptr<Message> &message,
                                                 const boost::shared_ptr<KeyValue> &props) {
    CurrentEnv current;

    boost::shared_ptr<ByteMessageImpl> msg = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    jobject jSendResult;
    if (props) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(props);
        jobject ret = current.callObjectMethod(_proxy, midSend2, msg->getProxy(), kv->getProxy());
        jSendResult = current.newGlobalRef(ret);
    } else {
        jobject ret = current.callObjectMethod(_proxy, midSend, msg->getProxy());
        jSendResult = current.newGlobalRef(ret);
    }

    boost::shared_ptr<SendResult> sendResult = boost::make_shared<SendResultImpl>(jSendResult);
    return sendResult;
}

boost::shared_ptr<ByteMessage> ProducerImpl::createByteMessageToTopic(const std::string &topic,
                                                                      const scoped_array<char> &body) {
    CurrentEnv current;
    jstring pTopic = current.env->NewStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.getLength());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte*>(body.getRawPtr()));
    jobject jMessage = current.callObjectMethod(_proxy, midCreateByteMessageToTopic, pTopic, pBody);
    current.deleteRef(pBody);
    current.deleteRef(pTopic);

    boost::shared_ptr<ByteMessage> message = boost::make_shared<ByteMessageImpl>(current.newGlobalRef(jMessage));
    return message;
}

boost::shared_ptr<ByteMessage> ProducerImpl::createByteMessageToQueue(const std::string &topic,
                                                                      const scoped_array<char> &body) {
    CurrentEnv current;
    jstring pTopic = current.env->NewStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.getLength());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte *>(body.getRawPtr()));
    jobject jMessage = current.callObjectMethod(_proxy, midCreateByteMessageToQueue, pTopic, pBody);
    current.env->DeleteLocalRef(pBody);
    current.env->DeleteLocalRef(pTopic);

    boost::shared_ptr<ByteMessage> message = boost::make_shared<ByteMessageImpl>(current.newGlobalRef(jMessage));
    return message;
}

boost::shared_ptr<SendResult> ProducerImpl::send(const boost::shared_ptr<Message> &message,
                                                 const boost::shared_ptr<LocalTransactionBranchExecutor> &executor,
                                                 const boost::shared_ptr<void> &arg,
                                                 const boost::shared_ptr<KeyValue> &props) {

    boost::shared_ptr<ByteMessageImpl> messageImpl = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    boost::shared_ptr<LocalTransactionBranchExecutorImpl> executorImpl =
            boost::dynamic_pointer_cast<LocalTransactionBranchExecutorImpl>(executor);
    boost::shared_ptr<KeyValueImpl> propertiesImpl = boost::dynamic_pointer_cast<KeyValueImpl>(props);
    CurrentEnv current;
    jobject jSendResult = current.callObjectMethod(_proxy, midSend3, messageImpl->getProxy(),
                                                        executorImpl->getProxy(), NULL, propertiesImpl->getProxy());

    boost::shared_ptr<SendResultImpl> ret = boost::make_shared<SendResultImpl>(current.newGlobalRef(jSendResult));
    return ret;
}

boost::shared_ptr<Future>
ProducerImpl::sendAsync(const boost::shared_ptr<Message> &message,
                        const boost::shared_ptr<KeyValue> &props) {

    boost::shared_ptr<ByteMessageImpl> messageImpl = boost::dynamic_pointer_cast<ByteMessageImpl>(message);

    if (messageImpl) {
        CurrentEnv current;
        boost::shared_ptr<Promise> ft = boost::make_shared<PromiseImpl>();
        long long opaque;
        {
            opaque = ++sendOpaque;
            boost::interprocess::scoped_lock<boost::mutex> lk(sendAsyncMutex);
            sendAsyncMap[opaque] = ft;
        }
        if (props) {
            boost::shared_ptr<KeyValueImpl> propertiesImpl = boost::dynamic_pointer_cast<KeyValueImpl>(props);
            if (!propertiesImpl) {
                BOOST_LOG_TRIVIAL(error) << "Dynamic casting failed";
            }
            current.callVoidMethod(objectProducerAdaptor, midSendAsync2, opaque, messageImpl->getProxy(),
                                   propertiesImpl->getProxy());
        } else {
            current.callVoidMethod(objectProducerAdaptor, midSendAsync, opaque, messageImpl->getProxy());
        }
        return ft;
    } else {
        BOOST_LOG_TRIVIAL(error) << "Dynamic casting failed";
        abort();
    }

    boost::shared_ptr<Future> f_nullptr;
    return f_nullptr;
}

void ProducerImpl::sendOneway(const boost::shared_ptr<Message> &message,
                              const boost::shared_ptr<KeyValue> &props) {
    CurrentEnv current;
    boost::shared_ptr<ByteMessageImpl> messageImpl = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    if (props) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(props);
        current.callVoidMethod(_proxy, midSendOneway2, messageImpl->getProxy(), kv->getProxy());
    } else {
        current.callVoidMethod(_proxy, midSendOneway, messageImpl->getProxy());
    }

    current.checkAndClearException();
}

boost::shared_ptr<BatchMessageSender> ProducerImpl::createSequenceBatchMessageSender() {
    throw OMSException("Not Implemented");
}

void ProducerImpl::addInterceptor(const boost::shared_ptr<interceptor::ProducerInterceptor> &interceptor) {
    throw OMSException("Not Implemented");
}

void ProducerImpl::removeInterceptor(const boost::shared_ptr<interceptor::ProducerInterceptor> &interceptor) {
    throw OMSException("Not Implemented");
}
