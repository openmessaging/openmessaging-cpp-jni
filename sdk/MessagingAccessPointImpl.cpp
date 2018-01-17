#include "core.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"
#include "ProducerImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                       const boost::shared_ptr<KeyValue> &properties,
                                                       jobject proxy):
            _url(url), _properties(properties), objectMessagingAccessPoint(proxy) {
        CurrentEnv current;

        jclass classMessagingAccessPointLocal = current.env->FindClass("io/openmessaging/MessagingAccessPoint");
        classMessagingAccessPoint = dynamic_cast<jclass>(current.env->NewGlobalRef(classMessagingAccessPointLocal));
        midImplVersion = current.env->GetMethodID(classMessagingAccessPoint, "implVersion", "()Ljava/lang/String;");

        midCreateProducer = current.env->GetMethodID(classMessagingAccessPoint, "createProducer", "()Lio/openmessaging/producer/Producer;");
        midCreateProducer2 = current.env->GetMethodID(classMessagingAccessPoint, "createProducer", "(Lio/openmessaging/KeyValue;)Lio/openmessaging/producer/Producer");
    }

    boost::shared_ptr<KeyValue> MessagingAccessPointImpl::properties() {
        return _properties;
    }

    std::string MessagingAccessPointImpl::implVersion() {
        CurrentEnv current;
        jstring version = dynamic_cast<jstring>(current.env->CallObjectMethod(objectMessagingAccessPoint, midImplVersion));
        const char* pVersion = current.env->GetStringUTFChars(version, NULL);
        std::string result = pVersion;
        current.env->ReleaseStringUTFChars(version, pVersion);
        return result;
    }

    boost::shared_ptr<producer::Producer> MessagingAccessPointImpl::createProducer(
            boost::shared_ptr<KeyValue> properties) {
        bool useKV = (NULL != properties);
        CurrentEnv current;
        jobject producer;
        if (useKV) {
            boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
            producer = current.env->CallObjectMethod(objectMessagingAccessPoint, midCreateProducer2, kv->getInternal());
        } else {
            producer = current.env->CallObjectMethod(objectMessagingAccessPoint, midCreateProducer);
        }
        return boost::make_shared<ProducerImpl>(producer, properties);
    }

    jobject MessagingAccessPointImpl::getProxy() {
        return objectMessagingAccessPoint;
    }

    MessagingAccessPointImpl::~MessagingAccessPointImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(objectMessagingAccessPoint);
    }

END_NAMESPACE_3(io, openmessaging, core)
