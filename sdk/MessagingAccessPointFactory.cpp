#include <boost/make_shared.hpp>

#include "MessagingAccessPointFactory.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

boost::shared_ptr<MessagingAccessPoint>
MessagingAccessPointFactory::getMessagingAccessPoint(std::string &url, boost::shared_ptr<KeyValue> properties) {

    CurrentEnv current;
    const char* klassMessagingAccessPointFactory = "io/openmessaging/MessagingAccessPointFactory";
    jclass classMessagingAccessPointFactory = current.findClass(klassMessagingAccessPointFactory);
    jmethodID midGetMessagingAccessPoint;
    bool useKV = false;
    if (properties) {
        useKV = true;
        const char *signature = "(Ljava/lang/String;Lio/openmessaging/KeyValue;)Lio/openmessaging/MessagingAccessPoint;";
        midGetMessagingAccessPoint = current.getMethodId(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", signature, true);
    } else {
        const char *signature = "(Ljava/lang/String;)Lio/openmessaging/MessagingAccessPoint;";
        midGetMessagingAccessPoint = current.getMethodId(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", signature, true);
    }

    jstring accessUrl = current.newStringUTF(url.c_str());

    jobject messagingAccessPoint;
    if (useKV) {
        boost::shared_ptr<KeyValueImpl> kv = boost::dynamic_pointer_cast<KeyValueImpl>(properties);
        jobject props = kv->getProxy();
        messagingAccessPoint = current.callStaticObjectMethod(classMessagingAccessPointFactory,
                                                                   midGetMessagingAccessPoint,
                                                                   accessUrl,
                                                                   props);
    } else {
        messagingAccessPoint = current.callStaticObjectMethod(classMessagingAccessPointFactory,
                                                                   midGetMessagingAccessPoint,
                                                                   accessUrl);
    }

    current.deleteRef(accessUrl);
    current.deleteRef(classMessagingAccessPointFactory);
    jobject globalRef = current.newGlobalRef(messagingAccessPoint);
    return boost::make_shared<MessagingAccessPointImpl>(url, properties, globalRef);
}

void MessagingAccessPointFactory::addInterceptor(
        boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor) {

}

void MessagingAccessPointFactory::removeInterceptor(
        boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor) {

}