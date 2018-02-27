#include "core.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"
#include "producer/ProducerImpl.h"
#include "consumer/PushConsumerImpl.h"
#include "consumer/PullConsumerImpl.h"
#include "consumer/StreamingConsumer.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;
using namespace io::openmessaging::consumer;
using namespace io::openmessaging::observer;

MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                   const boost::shared_ptr<KeyValue> &properties,
                                                   jobject proxy) :
        _url(url), _properties(properties), objectMessagingAccessPoint(proxy) {

    CurrentEnv current;
    const char *klassMessagingAccessPoint = "io/openmessaging/MessagingAccessPoint";
    classMessagingAccessPoint = current.findClass(klassMessagingAccessPoint);

    midImplVersion = current.getMethodId(classMessagingAccessPoint, "implVersion", "()Ljava/lang/String;");

    const char *createProducerSignature = "()Lio/openmessaging/producer/Producer;";
    midCreateProducer = current.getMethodId(classMessagingAccessPoint, "createProducer", createProducerSignature);

    const char *producer2Signature = "(Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/Producer;";
    midCreateProducer2 = current.getMethodId(classMessagingAccessPoint, "createProducer", producer2Signature);

    const char *pushConsumerSignature = "()Lio/openmessaging/consumer/PushConsumer;";
    midCreatePushConsumer = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                      pushConsumerSignature);

    const char *pushConsumer2Signature = "(Lio/openmessaging/KeyValue;)Lio/openmessaging/consumer/PushConsumer;";
    midCreatePushConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                       pushConsumer2Signature);

    const char *pullConsumerSignature = "(Ljava/lang/String;)Lio/openmessaging/consumer/PullConsumer;";
    midCreatePullConsumer = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                      pullConsumerSignature);

    const char *pullConsumer2Signature =
            "(Ljava/lang/String;Lio/openmessaging/KeyValue;)Lio/openmessaging/consumer/PullConsumer;";
    midCreatePullConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                       pullConsumer2Signature);

    midStartup = current.getMethodId(classMessagingAccessPoint, "startup", "()V");
    midShutdown = current.getMethodId(classMessagingAccessPoint, "shutdown", "()V");
}

boost::shared_ptr<KeyValue> MessagingAccessPointImpl::properties() {
    return _properties;
}

std::string MessagingAccessPointImpl::implVersion() {
    CurrentEnv current;
    jstring version =
            reinterpret_cast<jstring>(current.callObjectMethod(objectMessagingAccessPoint, midImplVersion));
    const char *pVersion = current.env->GetStringUTFChars(version, NULL);
    std::string result = pVersion;
    current.env->ReleaseStringUTFChars(version, pVersion);
    return result;
}

boost::shared_ptr<Producer> MessagingAccessPointImpl::createProducer(
        boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jobject producerLocal;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        producerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreateProducer2, kv->getProxy());
    } else {
        producerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreateProducer);
    }

    jobject producer = current.newGlobalRef(producerLocal);

    boost::shared_ptr<Producer> ret = boost::make_shared<ProducerImpl>(producer, properties);
    return ret;
}

boost::shared_ptr<consumer::PushConsumer>
MessagingAccessPointImpl::createPushConsumer(boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jobject pushConsumerLocal;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        pushConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePushConsumer2, kv->getProxy());

    } else {
        pushConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePushConsumer);
    }

    jobject pushConsumer = current.newGlobalRef(pushConsumerLocal);
    boost::shared_ptr<PushConsumer> ret = boost::make_shared<PushConsumerImpl>(pushConsumer);
    return ret;
}

boost::shared_ptr<consumer::PullConsumer>
MessagingAccessPointImpl::createPullConsumer(const std::string &queueName, boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jobject pullConsumerLocal;

    const char *str = queueName.c_str();
    jstring queue_name = current.newStringUTF(str);

    if (!queue_name) {
        // Allocate memory fails
        current.checkAndClearException();
        abort();
    }

    if (properties) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        pullConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePullConsumer2,
                                                          queue_name, kv->getProxy());
    } else {
        pullConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePullConsumer,
                                                          queue_name);
    }

    current.deleteRef(queue_name);

    jobject pullConsumer = current.newGlobalRef(pullConsumerLocal);
    boost::shared_ptr<PullConsumer> ret = boost::make_shared<PullConsumerImpl>(pullConsumer);
    return ret;
}

boost::shared_ptr<consumer::StreamingConsumer>
MessagingAccessPointImpl::createStreamingConsumer(const std::string &queueName,
                                                  boost::shared_ptr<KeyValue> properties) {

}

boost::shared_ptr<ResourceManager> MessagingAccessPointImpl::getResourceManager() {

}

void MessagingAccessPointImpl::addObserver(boost::shared_ptr<observer::Observer> observer) {

}

void MessagingAccessPointImpl::removeObserver(boost::shared_ptr<observer::Observer> observer) {

}

std::vector<boost::shared_ptr<Producer> >
MessagingAccessPointImpl::producers() {

}

std::vector<boost::shared_ptr<consumer::PushConsumer> >
MessagingAccessPointImpl::pushConsumers() {

}

std::vector<boost::shared_ptr<consumer::PullConsumer> >
MessagingAccessPointImpl::pullConsumers() {

}

std::vector<boost::shared_ptr<consumer::StreamingConsumer> >
MessagingAccessPointImpl::streamingConsumers() {

}

void MessagingAccessPointImpl::startup() {
    CurrentEnv current;
    current.env->CallVoidMethod(objectMessagingAccessPoint, midStartup);
}

void MessagingAccessPointImpl::shutdown() {
    CurrentEnv current;
    current.env->CallVoidMethod(objectMessagingAccessPoint, midShutdown);
}

jobject MessagingAccessPointImpl::getProxy() {
    return objectMessagingAccessPoint;
}

MessagingAccessPointImpl::~MessagingAccessPointImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(objectMessagingAccessPoint);
}
