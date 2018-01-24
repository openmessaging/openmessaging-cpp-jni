#include "ByteMessageImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

ByteMessageImpl::ByteMessageImpl(jobject proxy) : objectByteMessage(proxy) {
    CurrentEnv current;
    jclass classByteMessageLocal = current.env->FindClass("io/openmessaging/BytesMessage");
    classByteMessage = reinterpret_cast<jclass>(current.env->NewGlobalRef(classByteMessageLocal));
    midSysHeaders = getMethod(current, classByteMessage, "sysHeaders", "()Lio/openmessaging/KeyValue;");
    midUserHeaders = getMethod(current, classByteMessage, "userHeaders", "()Lio/openmessaging/KeyValue;");
    midGetBody = getMethod(current, classByteMessage, "getBody", "()[B");

    midPutSysHeadersInt = getMethod(current, classByteMessage, "putSysHeaders",
                                    "(Ljava/lang/String;I)Lio/openmessaging/Message;");
    midPutSysHeadersLong = getMethod(current, classByteMessage, "putSysHeaders",
                                     "(Ljava/lang/String;J)Lio/openmessaging/Message;");
    midPutSysHeadersDouble = getMethod(current, classByteMessage, "putSysHeaders",
                                       "(Ljava/lang/String;D)Lio/openmessaging/Message;");
    midPutSysHeadersString = getMethod(current, classByteMessage, "putSysHeaders",
                                       "(Ljava/lang/String;Ljava/lang/String;)Lio/openmessaging/Message;");
    midPutUserHeadersInt = getMethod(current, classByteMessage, "putUserHeaders",
                                     "(Ljava/lang/String;I)Lio/openmessaging/Message;");
    midPutUserHeadersLong = getMethod(current, classByteMessage, "putUserHeaders",
                                      "(Ljava/lang/String;J)Lio/openmessaging/Message;");
    midPutUserHeadersDouble = getMethod(current, classByteMessage, "putUserHeaders",
                                        "(Ljava/lang/String;D)Lio/openmessaging/Message;");
    midPutUserHeadersString = getMethod(current, classByteMessage, "putUserHeaders",
                                        "(Ljava/lang/String;Ljava/lang/String;)Lio/openmessaging/Message;");
}

ByteMessageImpl::~ByteMessageImpl() {
    CurrentEnv current;
    current.env->DeleteGlobalRef(objectByteMessage);
}

boost::shared_ptr<KeyValue> ByteMessageImpl::sysHeaders() {
    CurrentEnv current;
    jobject jSysHeader = current.env->CallObjectMethod(objectByteMessage, midSysHeaders);
    boost::shared_ptr<KeyValue> headers = boost::make_shared<KeyValueImpl>(
            current.env->NewGlobalRef(jSysHeader));
    current.env->DeleteLocalRef(jSysHeader);
    return headers;
}

boost::shared_ptr<KeyValue> ByteMessageImpl::userHeaders() {
    CurrentEnv current;
    jobject jSysHeader = current.env->CallObjectMethod(objectByteMessage, midUserHeaders);
    boost::shared_ptr<KeyValue> headers = boost::make_shared<KeyValueImpl>(
            current.env->NewGlobalRef(jSysHeader));
    current.env->DeleteLocalRef(jSysHeader);
    return headers;
}

std::vector<char> ByteMessageImpl::getBody() {
    CurrentEnv current;
    jbyteArray jBody = static_cast<jbyteArray>(current.env->CallObjectMethod(objectByteMessage,
                                                                             midGetBody));
    jbyte *pArray = current.env->GetByteArrayElements(jBody, NULL);
    jsize len = current.env->GetArrayLength(jBody);
    std::vector<char> body(pArray, pArray + len);
    current.env->ReleaseByteArrayElements(jBody, pArray, 0);
    current.env->DeleteLocalRef(jBody);
    return body;
}

ByteMessageImpl &ByteMessageImpl::setBody(const std::vector<char> &body) {
    CurrentEnv current;
    jbyteArray jBody = current.env->NewByteArray(body.size());
    current.env->SetByteArrayRegion(jBody, 0, body.size(), reinterpret_cast<const jbyte *>(&(body.front())));
    jobject ret = current.env->CallObjectMethod(objectByteMessage, midSetBody, jBody);
    current.env->DeleteLocalRef(ret);
    current.env->DeleteLocalRef(jBody);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutSysHeadersInt, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutSysHeadersLong, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutSysHeadersDouble, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, const std::string &value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jstring v = current.env->NewStringUTF(value.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutSysHeadersDouble, k, v);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    current.env->DeleteLocalRef(v);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutUserHeadersInt, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutUserHeadersLong, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutUserHeadersDouble, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, const std::string &value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jstring v = current.env->NewStringUTF(value.c_str());
    jobject obj = current.env->CallObjectMethod(objectByteMessage, midPutUserHeadersString, k, v);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    current.env->DeleteLocalRef(v);
    return *this;
}

jobject ByteMessageImpl::getProxy() {
    return objectByteMessage;
}
