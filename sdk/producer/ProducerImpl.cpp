#include "producer/ProducerImpl.h"
#include "KeyValueImpl.h"
#include "producer/SendResultImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

ProducerImpl::ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties)
        : ServiceLifecycleImpl(proxy), _properties(properties) {
    CurrentEnv current;
    jclass classProducerLocal = current.env->FindClass("io/openmessaging/producer/Producer");
    classProducer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classProducerLocal));
    std::string signature = "(Ljava/lang/String;[B)Lio/openmessaging/Message;";
    midCreateByteMessageToQueue = getMethod(current, classProducer, "createQueueBytesMessage", signature);
    midCreateByteMessageToTopic = getMethod(current, classProducer, "createTopicBytesMessage", signature);
    midStartup = getMethod(current, classProducer, "startup", "()V");
    midShutdown = getMethod(current, classProducer, "shutdown", "()V");

    std::string sendSignature = "(Lio/openmessaging/Message;)Lio/openmessaging/producer/SendResult;";
    midSend = getMethod(current, classProducer, "send", sendSignature);

    std::string send2Signature = "(Lio/openmessaging/Message;Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/SendResult;";
    midSend2 = getMethod(current, classProducer, "send", send2Signature);

}

ProducerImpl::~ProducerImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(classProducer);
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
                                                                      std::vector<char> &body) {
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
                                                                      std::vector<char> &body) {
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

}

boost::shared_ptr<Future>
ProducerImpl::sendAsync(boost::shared_ptr<Message> message,
          boost::shared_ptr<KeyValue> properties) {

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
