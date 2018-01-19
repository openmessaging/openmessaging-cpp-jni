#include "core.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"
#include "producer/ProducerImpl.h"
#include "consumer/PushConsumer.h"
#include "consumer/PullConsumer.h"
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

    jclass classMessagingAccessPointLocal = current.env->FindClass("io/openmessaging/MessagingAccessPoint");
    classMessagingAccessPoint =
            reinterpret_cast<jclass>(current.env->NewGlobalRef(classMessagingAccessPointLocal));

    midImplVersion = getMethod(current, classMessagingAccessPoint, "implVersion", "()Ljava/lang/String;");

    std::string createProducerSignature = "()Lio/openmessaging/producer/Producer;";
    midCreateProducer = getMethod(current, classMessagingAccessPoint, "createProducer", createProducerSignature);

    std::string producer2Signature = "(Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/Producer;";
    midCreateProducer2 = getMethod(current, classMessagingAccessPoint, "createProducer", producer2Signature);

    std::string pushConsumerSignature = "()Lio/openmessaging/consumer/PushConsumer;";
    midCreatePushConsumer = getMethod(current, classMessagingAccessPoint, "createPushConsumer",
                                      pushConsumerSignature);

    std::string pushConsumer2Signature = "(Lio/openmessaging/KeyValue;)Lio/openmessaging/consumer/PushConsumer;";
    midCreatePushConsumer2 = getMethod(current, classMessagingAccessPoint, "createPushConsumer",
                                       pushConsumer2Signature);

    std::string pullConsumerSignature = "(Ljava/lang/String;)Lio/openmessaging/consumer/PullConsumer;";
    midCreatePullConsumer = getMethod(current, classMessagingAccessPoint, "createPullConsumer",
                                      pullConsumerSignature);

    std::string pullConsumer2Signature =
            "(Ljava/lang/String;Lio/openmessaging/KeyValue;)Lio/openmessaging/consumer/PullConsumer;";
    midCreatePullConsumer2 = getMethod(current, classMessagingAccessPoint, "createPullConsumer",
                                       pullConsumer2Signature);


    midStartup = getMethod(current, classMessagingAccessPoint, "startup", "()V");
    midShutdown = getMethod(current, classMessagingAccessPoint, "shutdown", "()V");
}

boost::shared_ptr<KeyValue> MessagingAccessPointImpl::properties() {
    return _properties;
}

std::string MessagingAccessPointImpl::implVersion() {
    CurrentEnv current;
    jstring version =
            reinterpret_cast<jstring>(current.env->CallObjectMethod(objectMessagingAccessPoint, midImplVersion));
    const char *pVersion = current.env->GetStringUTFChars(version, NULL);
    std::string result = pVersion;
    current.env->ReleaseStringUTFChars(version, pVersion);
    return result;
}

boost::shared_ptr<Producer> MessagingAccessPointImpl::createProducer(
        boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jobject producer;
    if (properties) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        jobject producerLocal = current.env->CallObjectMethod(objectMessagingAccessPoint, midCreateProducer2, kv->getProxy());
        producer = current.env->NewGlobalRef(producerLocal);
        current.env->DeleteLocalRef(producerLocal);
    } else {
        jobject producerLocal = current.env->CallObjectMethod(objectMessagingAccessPoint, midCreateProducer);
        producer = current.env->NewGlobalRef(producerLocal);
        current.env->DeleteLocalRef(producerLocal);
    }

    boost::shared_ptr<Producer> ret = boost::make_shared<ProducerImpl>(producer, properties);
    return ret;
}

boost::shared_ptr<consumer::PushConsumer>
MessagingAccessPointImpl::createPushConsumer(boost::shared_ptr<KeyValue> properties) {

}

boost::shared_ptr<consumer::PullConsumer>
MessagingAccessPointImpl::createPullConsumer(const std::string &queueName, boost::shared_ptr<KeyValue> properties) {

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
