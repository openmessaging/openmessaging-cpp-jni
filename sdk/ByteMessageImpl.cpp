#include "ByteMessageImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

ByteMessageImpl::ByteMessageImpl(jobject proxy) : objectByteMessage(proxy) {
    CurrentEnv current;
    jclass classByteMessageLocal = current.env->FindClass("io/openmessaging/BytesMessage");
    classByteMessage = current.makeGlobal(classByteMessageLocal);
    midSysHeaders = getMethod(current, classByteMessage, "sysHeaders", "()Lio/openmessaging/KeyValue;");
    midUserHeaders = getMethod(current, classByteMessage, "userHeaders", "()Lio/openmessaging/KeyValue;");

    midGetBody = getMethod(current, classByteMessage, "getBody", "()[B");

    midSetBody = getMethod(current, classByteMessage, "setBody", "([B)Lio/openmessaging/BytesMessage;");

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
    jobject jSysHeader = current.callObjectMethod(objectByteMessage, midSysHeaders);
    if (current.checkAndClearException()) {
        abort();
    }
    boost::shared_ptr<KeyValue> headers = boost::make_shared<KeyValueImpl>(current.makeGlobal(jSysHeader));
    return headers;
}

boost::shared_ptr<KeyValue> ByteMessageImpl::userHeaders() {
    CurrentEnv current;
    jobject userHeaders = current.callObjectMethod(objectByteMessage, midUserHeaders);
    boost::shared_ptr<KeyValue> headers = boost::make_shared<KeyValueImpl>(current.makeGlobal(userHeaders));
    return headers;
}

scoped_array<char> ByteMessageImpl::getBody() {
    CurrentEnv current;

    jbyteArray jBody = static_cast<jbyteArray>(current.callObjectMethod(objectByteMessage,
                                                                             midGetBody));

    jbyte *pArray = current.env->GetByteArrayElements(jBody, NULL);
    jsize len = current.env->GetArrayLength(jBody);
    char *data = new char[len];
    memcpy(data, pArray, static_cast<size_t>(len));
    current.env->ReleaseByteArrayElements(jBody, pArray, 0);
    current.env->DeleteLocalRef(jBody);
    scoped_array<char> array(data, len);
    return array;
}

ByteMessage& ByteMessageImpl::setBody(scoped_array<char> &body) {
    CurrentEnv current;
    jbyteArray jBody = current.env->NewByteArray(body.getLength());
    current.env->SetByteArrayRegion(jBody, 0, body.getLength(), reinterpret_cast<const jbyte *>(body.getRawPtr()));
    jobject ret = current.callObjectMethod(objectByteMessage, midSetBody, jBody);
    current.env->DeleteLocalRef(ret);
    current.env->DeleteLocalRef(jBody);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersInt, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersLong, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersDouble, k, value);
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
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersString, k, v);
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
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersInt, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersLong, k, value);
    if (obj) {
        current.env->DeleteLocalRef(obj);
    }
    current.env->DeleteLocalRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.env->NewStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersDouble, k, value);
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
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersString, k, v);
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
