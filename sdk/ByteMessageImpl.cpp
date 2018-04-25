#include "ByteMessageImpl.h"
#include "KeyValueImpl.h"

using namespace io::openmessaging;

ByteMessageImpl::ByteMessageImpl(jobject proxy) : objectByteMessage(proxy) {
    CurrentEnv current;

    const char *klassBytesMessage = "io/openmessaging/BytesMessage";
    classByteMessage = current.findClass(klassBytesMessage);

    midSysHeaders = current.getMethodId(classByteMessage, "sysHeaders", buildSignature(Types::KeyValue_, 0));
    midUserHeaders = current.getMethodId(classByteMessage, "userHeaders", buildSignature(Types::KeyValue_, 0));

    midGetBody = current.getMethodId(classByteMessage, "getBody", buildSignature(Types::byteArray_, 0));

    midSetBody = current.getMethodId(classByteMessage, "setBody",
                                     buildSignature(Types::ByteMessage_, 1, Types::byteArray_));

    midPutSysHeadersInt = current.getMethodId(classByteMessage, "putSysHeaders",
                                              buildSignature(Types::Message_, 2, Types::String_, Types::int_));
    midPutSysHeadersLong = current.getMethodId(classByteMessage, "putSysHeaders",
                                               buildSignature(Types::Message_, 2, Types::String_, Types::long_));
    midPutSysHeadersDouble = current.getMethodId(classByteMessage, "putSysHeaders",
                                                 buildSignature(Types::Message_, 2, Types::String_, Types::double_));
    midPutSysHeadersString = current.getMethodId(classByteMessage, "putSysHeaders",
                                                 buildSignature(Types::Message_, 2, Types::String_, Types::String_));
    midPutUserHeadersInt = current.getMethodId(classByteMessage, "putUserHeaders",
                                               buildSignature(Types::Message_, 2, Types::String_, Types::int_));
    midPutUserHeadersLong = current.getMethodId(classByteMessage, "putUserHeaders",
                                                buildSignature(Types::Message_, 2, Types::String_, Types::long_));
    midPutUserHeadersDouble = current.getMethodId(classByteMessage, "putUserHeaders",
                                                  buildSignature(Types::Message_, 2, Types::String_, Types::double_));
    midPutUserHeadersString = current.getMethodId(classByteMessage, "putUserHeaders",
                                                  buildSignature(Types::Message_, 2, Types::String_, Types::String_));
}

ByteMessageImpl::~ByteMessageImpl() {
    CurrentEnv current;
    current.deleteRef(objectByteMessage);
}

NS::shared_ptr<KeyValue> ByteMessageImpl::sysHeaders() {
    CurrentEnv current;
    jobject jSysHeader = current.callObjectMethod(objectByteMessage, midSysHeaders);
    NS::shared_ptr<KeyValue> headers = NS::make_shared<KeyValueImpl>(current.newGlobalRef(jSysHeader));
    return headers;
}

NS::shared_ptr<KeyValue> ByteMessageImpl::userHeaders() {
    CurrentEnv current;
    jobject jUserHeaders = current.callObjectMethod(objectByteMessage, midUserHeaders);
    NS::shared_ptr<KeyValue> headers = NS::make_shared<KeyValueImpl>(current.newGlobalRef(jUserHeaders));
    return headers;
}

MessageBody ByteMessageImpl::getBody() {
    CurrentEnv current;
    jbyteArray jBody = static_cast<jbyteArray>(current.callObjectMethod(objectByteMessage, midGetBody));
    jbyte *pArray = current.env->GetByteArrayElements(jBody, NULL);
    jsize len = current.env->GetArrayLength(jBody);
    MessageBody result(pArray, len);
    current.env->ReleaseByteArrayElements(jBody, pArray, 0);
    current.env->DeleteLocalRef(jBody);
    return result;
}

ByteMessage& ByteMessageImpl::setBody(const MessageBody &body) {
    CurrentEnv current;
    jbyteArray jBody = current.env->NewByteArray(body.length());
    current.env->SetByteArrayRegion(jBody, 0, body.length(), reinterpret_cast<const jbyte *>(body.get()));
    jobject ret = current.callObjectMethod(objectByteMessage, midSetBody, jBody);
    current.env->DeleteLocalRef(ret);
    current.env->DeleteLocalRef(jBody);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersInt, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersLong, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersDouble, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putSysHeaders(const std::string &key, const std::string &value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jstring v = current.newStringUTF(value.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutSysHeadersString, k, v);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    current.deleteRef(v);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, int value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersInt, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, long value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersLong, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}


ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, double value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersDouble, k, value);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    return *this;
}

ByteMessageImpl &ByteMessageImpl::putUserHeaders(const std::string &key, const std::string &value) {
    CurrentEnv current;
    jstring k = current.newStringUTF(key.c_str());
    jstring v = current.newStringUTF(value.c_str());
    jobject obj = current.callObjectMethod(objectByteMessage, midPutUserHeadersString, k, v);
    if (obj) {
        current.deleteRef(obj);
    }
    current.deleteRef(k);
    current.deleteRef(v);
    return *this;
}

jobject ByteMessageImpl::getProxy() {
    return objectByteMessage;
}
