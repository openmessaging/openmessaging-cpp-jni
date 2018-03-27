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
                                                   const NS::shared_ptr<KeyValue> &props,
                                                   jobject proxy) :
        _url(url), _properties(props), objectMessagingAccessPoint(proxy) {

    CurrentEnv current;
    const char *klassMessagingAccessPoint = "io/openmessaging/MessagingAccessPoint";
    classMessagingAccessPoint = current.findClass(klassMessagingAccessPoint);

    midImplVersion = current.getMethodId(classMessagingAccessPoint, "implVersion",
                                         buildSignature(Types::String_, 0));

    midCreateProducer = current.getMethodId(classMessagingAccessPoint, "createProducer",
                                            buildSignature(Types::Producer_, 0));

    midCreateProducer2 = current.getMethodId(classMessagingAccessPoint, "createProducer",
                                             buildSignature(Types::Producer_, 1, Types::KeyValue_));

    midCreatePushConsumer = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                      buildSignature(Types::PushConsumer_, 0));

    midCreatePushConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                       buildSignature(Types::PushConsumer_, 1, Types::KeyValue_));

    midCreatePullConsumer = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                      buildSignature(Types::PullConsumer_, 1, Types::String_));

    midCreatePullConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                       buildSignature(Types::PullConsumer_, 2, Types::String_, Types::KeyValue_));

    midStartup = current.getMethodId(classMessagingAccessPoint, "startup", buildSignature(Types::void_, 0));
    midShutdown = current.getMethodId(classMessagingAccessPoint, "shutdown", buildSignature(Types::void_, 0));
}

NS::shared_ptr<KeyValue> MessagingAccessPointImpl::properties() {
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

NS::shared_ptr<Producer>
MessagingAccessPointImpl::createProducer(const NS::shared_ptr<KeyValue> &props) {
    CurrentEnv current;
    jobject producerLocal;
    if (props) {
        NS::shared_ptr<KeyValueImpl> kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        producerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreateProducer2, kv->getProxy());
    } else {
        producerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreateProducer);
    }

    jobject producer = current.newGlobalRef(producerLocal);

    NS::shared_ptr<Producer> ret = NS::make_shared<ProducerImpl>(producer, props);
    return ret;
}

NS::shared_ptr<consumer::PushConsumer>
MessagingAccessPointImpl::createPushConsumer(const NS::shared_ptr<KeyValue> &props) {
    CurrentEnv current;
    jobject pushConsumerLocal;
    if (props) {
        NS::shared_ptr<KeyValueImpl> kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        pushConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePushConsumer2, kv->getProxy());

    } else {
        pushConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePushConsumer);
    }

    jobject pushConsumer = current.newGlobalRef(pushConsumerLocal);
    NS::shared_ptr<PushConsumer> ret = NS::make_shared<PushConsumerImpl>(pushConsumer);
    return ret;
}

NS::shared_ptr<consumer::PullConsumer>
MessagingAccessPointImpl::createPullConsumer(const std::string &queueName,
                                             const NS::shared_ptr<KeyValue> &props) {
    CurrentEnv current;
    jobject pullConsumerLocal;

    const char *str = queueName.c_str();
    jstring queue_name = current.newStringUTF(str);

    if (!queue_name) {
        // Allocate memory fails
        current.checkAndClearException();
        abort();
    }

    if (props) {
        NS::shared_ptr<KeyValueImpl> kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        pullConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePullConsumer2,
                                                          queue_name, kv->getProxy());
    } else {
        pullConsumerLocal = current.callObjectMethod(objectMessagingAccessPoint, midCreatePullConsumer,
                                                          queue_name);
    }

    current.deleteRef(queue_name);

    jobject pullConsumer = current.newGlobalRef(pullConsumerLocal);
    NS::shared_ptr<PullConsumer> ret = NS::make_shared<PullConsumerImpl>(pullConsumer);
    return ret;
}

NS::shared_ptr<consumer::StreamingConsumer>
MessagingAccessPointImpl::createStreamingConsumer(const std::string &queueName,
                                                  const NS::shared_ptr<KeyValue> &props) {
    throw OMSException("Not Implemented");

}

NS::shared_ptr<ResourceManager> MessagingAccessPointImpl::getResourceManager() {
    throw OMSException("Not Implemented");
}

void MessagingAccessPointImpl::addObserver(const NS::shared_ptr<observer::Observer> &observer) {
    throw OMSException("Not Implemented");
}

void MessagingAccessPointImpl::removeObserver(const NS::shared_ptr<observer::Observer> &observer) {
    throw OMSException("Not Implemented");
}

std::vector<NS::shared_ptr<Producer> >
MessagingAccessPointImpl::producers() {
    throw OMSException("Not Implemented");
}

std::vector<NS::shared_ptr<consumer::PushConsumer> >
MessagingAccessPointImpl::pushConsumers() {
    throw OMSException("Not Implemented");
}

std::vector<NS::shared_ptr<consumer::PullConsumer> >
MessagingAccessPointImpl::pullConsumers() {
    throw OMSException("Not Implemented");
}

std::vector<NS::shared_ptr<consumer::StreamingConsumer> >
MessagingAccessPointImpl::streamingConsumers() {
    throw OMSException("Not Implemented");
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
