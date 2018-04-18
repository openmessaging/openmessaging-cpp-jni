#include "core.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"
#include "producer/ProducerImpl.h"
#include "consumer/PushConsumerImpl.h"
#include "consumer/PullConsumerImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;
using namespace io::openmessaging::consumer;
using namespace io::openmessaging::observer;

MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                   const KeyValuePtr &props,
                                                   jobject proxy) :
        _url(url), _properties(props), ServiceLifecycleImpl(proxy) {

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
                                      buildSignature(Types::PullConsumer_, 0));

    midCreatePullConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                       buildSignature(Types::PullConsumer_, 1, Types::KeyValue_));
}

KeyValuePtr MessagingAccessPointImpl::attributes() {
    return _properties;
}

std::string MessagingAccessPointImpl::implVersion() {
    CurrentEnv current;
    jstring version =
            reinterpret_cast<jstring>(current.callObjectMethod(_proxy, midImplVersion));
    const char *pVersion = current.env->GetStringUTFChars(version, NULL);
    std::string result = pVersion;
    current.env->ReleaseStringUTFChars(version, pVersion);
    return result;
}

ProducerPtr MessagingAccessPointImpl::createProducer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject producerLocal;
    if (props) {
        KeyValueImplPtr kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        producerLocal = current.callObjectMethod(_proxy, midCreateProducer2, kv->getProxy());
    } else {
        producerLocal = current.callObjectMethod(_proxy, midCreateProducer);
    }

    jobject producer = current.newGlobalRef(producerLocal);

    NS::shared_ptr<Producer> ret = NS::make_shared<ProducerImpl>(producer, props);
    return ret;
}

consumer::PushConsumerPtr MessagingAccessPointImpl::createPushConsumer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject pushConsumerLocal;
    if (props) {
        KeyValueImplPtr kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        pushConsumerLocal = current.callObjectMethod(_proxy, midCreatePushConsumer2, kv->getProxy());

    } else {
        pushConsumerLocal = current.callObjectMethod(_proxy, midCreatePushConsumer);
    }

    jobject pushConsumer = current.newGlobalRef(pushConsumerLocal);
    NS::shared_ptr<PushConsumer> ret = NS::make_shared<PushConsumerImpl>(pushConsumer);
    return ret;
}

consumer::PullConsumerPtr MessagingAccessPointImpl::createPullConsumer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject pullConsumerLocal;

    if (props) {
        NS::shared_ptr<KeyValueImpl> kv = NS::dynamic_pointer_cast<KeyValueImpl>(props);
        pullConsumerLocal = current.callObjectMethod(_proxy, midCreatePullConsumer2, kv->getProxy());
    } else {
        pullConsumerLocal = current.callObjectMethod(_proxy, midCreatePullConsumer);
    }

    jobject pullConsumer = current.newGlobalRef(pullConsumerLocal);
    NS::shared_ptr<PullConsumer> ret = NS::make_shared<PullConsumerImpl>(pullConsumer);
    return ret;
}

consumer::StreamingConsumerPtr MessagingAccessPointImpl::createStreamingConsumer(const KeyValuePtr &props) {
    throw OMSException("Not Implemented");
}

ResourceManagerPtr MessagingAccessPointImpl::resourceManager() {
    throw OMSException("Not Implemented");
}

jobject MessagingAccessPointImpl::getProxy() {
    return _proxy;
}

MessagingAccessPointImpl::~MessagingAccessPointImpl() {
    CurrentEnv context;
    context.deleteRef(classMessagingAccessPoint);
}
