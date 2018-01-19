#include <boost/make_shared.hpp>

#include "MessagingAccessPointFactory.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

boost::shared_ptr<MessagingAccessPoint>
MessagingAccessPointFactory::getMessagingAccessPoint(std::string &url, boost::shared_ptr<KeyValue> properties) {
    CurrentEnv current;
    jclass classMessagingAccessPointFactory = current.env->FindClass("io/openmessaging/MessagingAccessPointFactory");
    jmethodID midGetMessagingAccessPoint;
    bool useKV = false;
    if (properties) {
        useKV = true;
        const char *signature = "(Ljava/lang/String;Lio/openmessaging/KeyValue;)Lio/openmessaging/MessagingAccessPoint;";
        midGetMessagingAccessPoint = current.env->GetStaticMethodID(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", signature);
    } else {
        const char *signature = "(Ljava/lang/String;)Lio/openmessaging/MessagingAccessPoint;";
        midGetMessagingAccessPoint = current.env->GetStaticMethodID(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", signature);
    }

    jobject messagingAccessPoint;
    if (useKV) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        messagingAccessPoint = current.env->CallStaticObjectMethod(classMessagingAccessPointFactory,
                                                                   midGetMessagingAccessPoint,
                                                                   kv->getInternal());
    } else {
        messagingAccessPoint = current.env->CallStaticObjectMethod(classMessagingAccessPointFactory,
                                                                   midGetMessagingAccessPoint);
    }

    current.env->DeleteLocalRef(classMessagingAccessPointFactory);
    jobject globalRef = current.env->NewGlobalRef(messagingAccessPoint);
    return boost::make_shared<MessagingAccessPointImpl>(url, properties, globalRef);
}

void MessagingAccessPointFactory::addInterceptor(
        boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor) {

}

void MessagingAccessPointFactory::removeInterceptor(
        boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor) {

}