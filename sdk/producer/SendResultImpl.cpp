#include "producer/SendResultImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;

SendResultImpl::SendResultImpl(jobject proxy) : _proxy(proxy) {
    CurrentEnv current;
    jclass classSendResultLocal = current.env->FindClass("io/openmessaging/producer/SendResult");
    classSendResult = reinterpret_cast<jclass>(current.env->NewGlobalRef(classSendResultLocal));
    current.env->DeleteLocalRef(classSendResultLocal);

    midMessageId = getMethod(current, classSendResult, "messageId", "()Ljava/lang/String;");

    midProperties = getMethod(current, classSendResult, "properties", "()Lio/openmessaging/KeyValue;");
}


SendResultImpl::~SendResultImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(_proxy);
    current.env->DeleteGlobalRef(classSendResult);
}

std::string SendResultImpl::messageId() {
    CurrentEnv current;
    jstring msgId = reinterpret_cast<jstring>(current.env->CallObjectMethod(_proxy, midMessageId));
    const char *data = current.env->GetStringUTFChars(msgId, NULL);
    std::string result = data;
    current.env->ReleaseStringUTFChars(msgId, data);
    current.env->DeleteLocalRef(msgId);
    return result;
}

boost::shared_ptr<KeyValue> SendResultImpl::properties() {
    CurrentEnv current;
    jobject kvLocal = current.env->CallObjectMethod(_proxy, midProperties);
    jobject kv = current.env->NewGlobalRef(kvLocal);
    current.env->DeleteLocalRef(kvLocal);
    boost::shared_ptr<KeyValue> result = boost::make_shared<KeyValueImpl>(kv);
    return result;
}








