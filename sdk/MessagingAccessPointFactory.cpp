#include <boost/make_shared.hpp>

#include "MessagingAccessPointFactory.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

boost::shared_ptr<MessagingAccessPoint>
MessagingAccessPointFactory::getMessagingAccessPoint(const std::string &url,
                                                     const boost::shared_ptr<KeyValue> &properties) {

    CurrentEnv current;
    const char* klassMessagingAccessPointFactory = "io/openmessaging/MessagingAccessPointFactory";
    jclass classMessagingAccessPointFactory = current.findClass(klassMessagingAccessPointFactory);
    jmethodID midGetMessagingAccessPoint;
    bool useKV = false;
    if (properties) {
        useKV = true;
        std::string sig = buildSignature(Types::MessagingAccessPoint_, 2, Types::String_, Types::KeyValue_);
        midGetMessagingAccessPoint = current.getMethodId(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", sig.c_str(), true);
    } else {
        std::string sig = buildSignature(Types::MessagingAccessPoint_, 1, Types::String_);
        midGetMessagingAccessPoint = current.getMethodId(classMessagingAccessPointFactory,
                                                                    "getMessagingAccessPoint", sig.c_str(), true);
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
        const boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> &interceptor) {

}

void MessagingAccessPointFactory::removeInterceptor(
        const boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> &interceptor) {

}