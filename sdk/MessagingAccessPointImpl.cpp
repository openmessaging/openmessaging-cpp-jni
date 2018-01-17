#include "core.h"
#include "MessagingAccessPointImpl.h"

BEGIN_NAMESPACE_3(io, openmessaging, core)

    MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                       const boost::shared_ptr<KeyValue> &properties,
                                                       jobject proxy):
            _url(url), _properties(properties), objectMessagingAccessPoint(proxy) {
        CurrentEnv current;

        jclass classMessagingAccessPointLocal = current.env->FindClass("io/openmessaging/MessagingAccessPoint");
        classMessagingAccessPoint = dynamic_cast<jclass>(current.env->NewGlobalRef(classMessagingAccessPointLocal));
        midImplVersion = current.env->GetMethodID(classMessagingAccessPoint, "implVersion", "()Ljava/lang/String;");
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

        if (!isRunning()) {
            Initialize();
        }

    }

    jobject MessagingAccessPointImpl::getProxy() {
        return objectMessagingAccessPoint;
    }

    MessagingAccessPointImpl::~MessagingAccessPointImpl() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(objectMessagingAccessPoint);
    }

END_NAMESPACE_3(io, openmessaging, core)