#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"
#include "MessagingAccessPointImpl.h"

using namespace io::openmessaging;

MessagingAccessPoint*
getMessagingAccessPointImpl(const std::string &url, const NS::shared_ptr<KeyValue> &props) {

    Initialize();

    CurrentEnv current;
    const char* klassOMS = "io/openmessaging/OMS";
    jclass classOMS = current.findClass(klassOMS);
    jmethodID midGetMessagingAccessPoint;
    bool useKV = false;
    if (props) {
        useKV = true;
        std::string sig = buildSignature(Types::MessagingAccessPoint_, 2, Types::String_, Types::KeyValue_);
        midGetMessagingAccessPoint = current.getMethodId(classOMS, "getMessagingAccessPoint", sig.c_str(), true);
    } else {
        std::string sig = buildSignature(Types::MessagingAccessPoint_, 1, Types::String_);
        midGetMessagingAccessPoint = current.getMethodId(classOMS, "getMessagingAccessPoint", sig.c_str(), true);
    }

    jstring accessUrl = current.newStringUTF(url.c_str());

    jobject messagingAccessPoint;
    if (useKV) {
        KeyValue* ptr = props.get();
        jobject prop = (dynamic_cast<KeyValueImpl*>(ptr))->getProxy();
        messagingAccessPoint = current.callStaticObjectMethod(classOMS,
                                                              midGetMessagingAccessPoint,
                                                              accessUrl,
                                                              prop);
    } else {
        messagingAccessPoint = current.callStaticObjectMethod(classOMS,
                                                              midGetMessagingAccessPoint,
                                                              accessUrl);
    }

    current.deleteRef(accessUrl);
    current.deleteRef(classOMS);
    jobject globalRef = current.newGlobalRef(messagingAccessPoint);
    return new MessagingAccessPointImpl(url, props, globalRef);
}

KeyValue* newKeyValueImpl() {
    Initialize();
    CurrentEnv current;
    const char *klassDefaultKeyValue = "io/openmessaging/internal/DefaultKeyValue";
    jclass classDefaultKeyValue = current.findClass(klassDefaultKeyValue);
    jmethodID ctor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.newObject(classDefaultKeyValue, ctor);
    return new KeyValueImpl(objectDefaultKeyValue);
}