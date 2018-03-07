#include "producer/SendResultImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

SendResultImpl::SendResultImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;

    const char *clazzSendResult = "io/openmessaging/producer/SendResult";
    classSendResult = current.findClass(clazzSendResult);

    midMessageId = current.getMethodId(classSendResult, "messageId", "()Ljava/lang/String;");
    midProperties = current.getMethodId(classSendResult, "properties", "()Lio/openmessaging/KeyValue;");
}


SendResultImpl::~SendResultImpl() {
    CurrentEnv current;
    current.deleteRef(_proxy);
    current.deleteRef(classSendResult);
}

std::string SendResultImpl::messageId() {
    CurrentEnv current;
    jstring msgId = reinterpret_cast<jstring>(current.env->CallObjectMethod(_proxy, midMessageId));
    if (current.checkAndClearException()) {
        LOG_WARNING << "Java SDK throws an exception";
        return "";
    }
    const char *data = current.env->GetStringUTFChars(msgId, NULL);
    std::string result = data;
    current.env->ReleaseStringUTFChars(msgId, data);
    current.env->DeleteLocalRef(msgId);
    return result;
}

boost::shared_ptr<KeyValue> SendResultImpl::properties() {
    CurrentEnv current;
    jobject kvLocal = current.env->CallObjectMethod(_proxy, midProperties);
    if (current.checkAndClearException()) {
        LOG_WARNING << "Java SDK throws an exception";
        boost::shared_ptr<KeyValue> kv_nullptr;
        return kv_nullptr;
    }
    jobject kv = current.env->NewGlobalRef(kvLocal);
    current.env->DeleteLocalRef(kvLocal);
    boost::shared_ptr<KeyValue> result = boost::make_shared<KeyValueImpl>(kv);
    return result;
}








