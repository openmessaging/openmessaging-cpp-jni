#include "producer/ProducerImpl.h"
#include "producer/LocalTransactionBranchExecutorImpl.h"
#include "KeyValueImpl.h"
#include "producer/SendResultImpl.h"
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "PromiseImpl.h"
#include <map>

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    void sendAsyncCallback(JNIEnv *env, jobject object, jlong opaque, jobject jFuture) {

    }

    long long sendOpaque = 0;
    boost::mutex sendAsyncMutex;
    std::map<long long, boost::shared_ptr<void> > sendAsyncMap;

    static JNINativeMethod methods[] = {
        {
            const_cast<char*>("sendAsyncCallback"),
            const_cast<char*>("(JLio/openmessaging/Future;)V"),
            (void *)&sendAsyncCallback
        }
    };

END_NAMESPACE_3(io, openmessaging, producer)

ProducerImpl::ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties)
        : ServiceLifecycleImpl(proxy), _properties(properties) {
    CurrentEnv current;
    jclass classProducerLocal = current.env->FindClass("io/openmessaging/producer/Producer");
    classProducer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classProducerLocal));

    jclass classProducerProxyLocal = current.env->FindClass("io/openmessaging/producer/ProducerProxy");
    classProducerProxy = reinterpret_cast<jclass>(current.env->NewGlobalRef(classProducerProxyLocal));

    if (current.env->RegisterNatives(classProducerProxy, methods, 1) < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to bind native methods";
        abort();
    }

    jmethodID producerProxyCtor = getMethod(current, classProducerProxy, "<init>", "(Lio/openmessaging/producer/Producer;)V");
    jobject objectProducerProxyLocal = current.env->NewObject(classProducerProxy, producerProxyCtor, proxy);
    objectProducerProxy = current.env->NewGlobalRef(objectProducerProxyLocal);
    current.env->DeleteLocalRef(objectProducerProxyLocal);

    std::string signature = "(Ljava/lang/String;[B)Lio/openmessaging/Message;";
    midCreateByteMessageToQueue = getMethod(current, classProducer, "createQueueBytesMessage", signature);
    midCreateByteMessageToTopic = getMethod(current, classProducer, "createTopicBytesMessage", signature);
    midStartup = getMethod(current, classProducer, "startup", "()V");
    midShutdown = getMethod(current, classProducer, "shutdown", "()V");

    std::string sendSignature = "(Lio/openmessaging/Message;)Lio/openmessaging/producer/SendResult;";
    midSend = getMethod(current, classProducer, "send", sendSignature);

    std::string send2Signature = "(Lio/openmessaging/Message;Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/SendResult;";
    midSend2 = getMethod(current, classProducer, "send", send2Signature);

    std::string send3Signature = "(Lio/openmessaging/Message;Lio/openmessaging/producer/LocalTransactionBranchExecutor;Ljava/lang/Object;Lio/openmessaging/KeyValue;)Lio/openmessaging/SendResult;";
    midSend3 = getMethod(current, classProducer, "send", send3Signature);

    std::string sendAsyncSignature = "(JLio/openmessaging/Message;)V";
    midSendAsync = getMethod(current, classProducerProxy, "sendAsync", sendAsyncSignature);

    std::string sendAsync2Signature = "(JLio/openmessaging/Message;Lio/openmessaging/KeyValue;)V";
    midSendAsync2 = getMethod(current, classProducerProxy, "sendAsync", sendAsync2Signature);
}

ProducerImpl::~ProducerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classProducer);
    current.env->DeleteGlobalRef(classProducerProxy);
    current.env->DeleteGlobalRef(objectProducerProxy);
}

boost::shared_ptr<KeyValue> ProducerImpl::properties() {
    return _properties;
}

boost::shared_ptr<SendResult> ProducerImpl::send(boost::shared_ptr<Message> message,
                                                 boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;

    boost::shared_ptr<ByteMessageImpl> msg = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    jobject jSendResult;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        jobject ret = current.env->CallObjectMethod(_proxy, midSend2, msg->getProxy(), kv->getProxy());
        jSendResult = current.env->NewGlobalRef(ret);
        current.env->DeleteLocalRef(ret);
    } else {
        jobject ret = current.env->CallObjectMethod(_proxy, midSend, msg->getProxy());
        jSendResult = current.env->NewGlobalRef(ret);
        current.env->DeleteLocalRef(ret);
    }

    boost::shared_ptr<SendResult> sendResult = boost::make_shared<SendResultImpl>(jSendResult);
    return sendResult;
}

boost::shared_ptr<ByteMessage> ProducerImpl::createByteMessageToTopic(std::string &topic,
                                                                      std::vector<unsigned char> &body) {
    CurrentEnv current;
    jstring pTopic = current.env->NewStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.size());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte *>(body.data()));
    jobject jMessage = current.env->CallObjectMethod(_proxy, midCreateByteMessageToTopic, pTopic, pBody);
    current.env->DeleteLocalRef(pBody);
    current.env->DeleteLocalRef(pTopic);

    boost::shared_ptr<ByteMessage> message = boost::make_shared<ByteMessageImpl>(current.env->NewGlobalRef(jMessage));
    return message;
}

boost::shared_ptr<ByteMessage> ProducerImpl::createByteMessageToQueue(std::string &topic,
                                                                      std::vector<unsigned char> &body) {
    CurrentEnv current;
    jstring pTopic = current.env->NewStringUTF(topic.c_str());
    jsize len = static_cast<jint>(body.size());
    jbyteArray pBody = current.env->NewByteArray(len);
    current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte *>(body.data()));
    jobject jMessage = current.env->CallObjectMethod(_proxy, midCreateByteMessageToQueue, pTopic, pBody);
    current.env->DeleteLocalRef(pBody);
    current.env->DeleteLocalRef(pTopic);

    boost::shared_ptr<ByteMessage> message = boost::make_shared<ByteMessageImpl>(current.env->NewGlobalRef(jMessage));
    return message;
}

boost::shared_ptr<SendResult> ProducerImpl::send(boost::shared_ptr<Message> message,
                                   boost::shared_ptr<LocalTransactionBranchExecutor> executor,
                                   boost::shared_ptr<void> arg,
                                   boost::shared_ptr<KeyValue> properties) {

    boost::shared_ptr<ByteMessageImpl> messageImpl = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    boost::shared_ptr<LocalTransactionBranchExecutorImpl> executorImpl =
            boost::dynamic_pointer_cast<LocalTransactionBranchExecutorImpl>(executor);
    boost::shared_ptr<KeyValueImpl> propertiesImpl = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
    CurrentEnv current;
    jobject jSendResult = current.env->CallObjectMethod(_proxy, midSend3, messageImpl->getProxy(),
                                                        executorImpl->getProxy(), NULL, propertiesImpl->getProxy());
    boost::shared_ptr<SendResultImpl> ret = boost::make_shared<SendResultImpl>(current.env->NewGlobalRef(jSendResult));
    current.env->DeleteLocalRef(jSendResult);
    return ret;
}

boost::shared_ptr<Future>
ProducerImpl::sendAsync(boost::shared_ptr<Message> message,
          boost::shared_ptr<KeyValue> properties) {

    boost::shared_ptr<ByteMessageImpl> messageImpl = boost::dynamic_pointer_cast<ByteMessageImpl>(message);
    boost::shared_ptr<KeyValueImpl> propertiesImpl = boost::dynamic_pointer_cast<KeyValueImpl>(properties);

    if (messageImpl && properties) {
        CurrentEnv current;
        boost::shared_ptr<Promise> ft = boost::make_shared<PromiseImpl>();
        long long opaque;
        {
            opaque = ++sendOpaque;
            boost::interprocess::scoped_lock<boost::mutex> lk(sendAsyncMutex);
            sendAsyncMap[opaque] = ft;
        }
        current.env->CallVoidMethod(objectProducerProxy, midSendAsync, opaque, messageImpl->getProxy());
        return ft;
    } else {
        BOOST_LOG_TRIVIAL(error) << "Dynamic casting failed";
        abort();
    }

    boost::shared_ptr<Future> f_nullptr;
    return f_nullptr;
}

void ProducerImpl::sendOneway(boost::shared_ptr<Message> Message,
                boost::shared_ptr<KeyValue> properties) {

}

boost::shared_ptr<BatchMessageSender> ProducerImpl::createSequenceBatchMessageSender() {

}

void ProducerImpl::addInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor) {

}

void ProducerImpl::removeInterceptor(boost::shared_ptr<interceptor::ProducerInterceptor> interceptor) {

}
