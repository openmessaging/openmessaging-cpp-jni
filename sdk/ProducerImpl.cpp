#include "ProducerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    ProducerImpl::ProducerImpl(jobject proxy, boost::shared_ptr<KeyValue> properties)
            : objectProducer(proxy), _properties(properties) {

        CurrentEnv current;
        jclass classProducerLocal = current.env->FindClass("io/openmessaging/producer/Producer");
        classProducer = reinterpret_cast<jclass>(current.env->NewGlobalRef(classProducerLocal));
        std::string signature = "(Ljava/lang/String;[B)Lio/openmessaging/Message;";
        midCreateByteMessageToQueue = getMethod(current, classProducer, "createQueueBytesMessage", signature);
        midCreateByteMessageToTopic = getMethod(current, classProducer, "createTopicBytesMessage", signature);
        midStartup = getMethod(current, classProducer, "startup", "()V");
        midShutdown = getMethod(current, classProducer, "shutdown", "()V");
    }

    ProducerImpl::~ProducerImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(objectProducer);
    }

    boost::shared_ptr<KeyValue> ProducerImpl::properties() {
        return _properties;
    }

    boost::shared_ptr<producer::SendResult> ProducerImpl::send(boost::shared_ptr<Message> message,
                                       boost::shared_ptr<KeyValue> properties) {


    }

    boost::shared_ptr<ByteMessage> ProducerImpl::createByteMessageToTopic(std::string &topic,
                                                                          std::vector<char> &body) {
        CurrentEnv current;
        jstring pTopic = current.env->NewStringUTF(topic.c_str());
        jsize len = static_cast<jint>(body.size());
        jbyteArray pBody = current.env->NewByteArray(len);
        current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte*>(body.data()));
        jobject jMessage = current.env->CallObjectMethod(objectProducer, midCreateByteMessageToTopic, pTopic, pBody);
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
        current.env->SetByteArrayRegion(pBody, 0, len, reinterpret_cast<const jbyte*>(body.data()));
        jobject jMessage = current.env->CallObjectMethod(objectProducer, midCreateByteMessageToQueue, pTopic, pBody);
        current.env->DeleteLocalRef(pBody);
        current.env->DeleteLocalRef(pTopic);

        boost::shared_ptr<ByteMessage> message = boost::make_shared<ByteMessageImpl>(current.env->NewGlobalRef(jMessage));
        return message;
    }

    void ProducerImpl::shutdown() {
        CurrentEnv current;
        current.env->CallVoidMethod(objectProducer, midShutdown);
        if (current.env->ExceptionCheck()) {
            current.env->ExceptionDescribe();
            current.env->ExceptionClear();
        }
    }

    void ProducerImpl::startup() {
        CurrentEnv current;
        current.env->CallVoidMethod(objectProducer, midStartup);
        if (current.env->ExceptionCheck()) {
            current.env->ExceptionDescribe();
            current.env->ExceptionClear();
        }
    }

END_NAMESPACE_3(io, openmessaging, core)
