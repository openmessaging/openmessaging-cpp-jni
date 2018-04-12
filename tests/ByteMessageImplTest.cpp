#include <string>
#include <gtest/gtest.h>
#include "ByteMessageImpl.h"

using namespace std;
using namespace io::openmessaging;

class ByteMessageImplTest : public ::testing::Test {
public:
    ByteMessageImplTest() : byteMessage(NULL) {

    }

    void SetUp() {
        Initialize();

        if (NULL == byteMessage) {
            CurrentEnv context;
            // Create Java ByteMessage object
            jclass klass = context.findClass("io/openmessaging/rocketmq/domain/BytesMessageImpl");
            jmethodID ctor = context.getMethodId(klass, "<init>", "()V");
            jobject jByteMessage = context.newObject(klass, ctor);
            string msg = "Hello World";
            jbyteArray jba = context.env->NewByteArray(msg.size());
            context.env->SetByteArrayRegion(jba, 0, msg.size(), reinterpret_cast<const jbyte *>(msg.c_str()));
            jmethodID midSetBody = context.env->GetMethodID(klass, "setBody", "([B)Lio/openmessaging/BytesMessage;");
            context.callObjectMethod(jByteMessage, midSetBody, jba);
            context.deleteRef(jba);

            // call constructor
            byteMessage = new ByteMessageImpl(jByteMessage);
        }
    }

    void TearDown() {
//        Shutdown();
    }

protected:
    ByteMessage *byteMessage;
};

TEST_F(ByteMessageImplTest, testUserHeaders) {
    NS::shared_ptr<KeyValue> userHeaders = byteMessage->userHeaders();
    ASSERT_TRUE(userHeaders != NULL);
}


TEST_F(ByteMessageImplTest, testSysHeaders) {
    NS::shared_ptr<KeyValue> userHeaders = byteMessage->sysHeaders();
    ASSERT_TRUE(userHeaders != NULL);
}

TEST_F(ByteMessageImplTest, testPutUserHeadersInt) {
    string key = "i";
    int value = 1;
    byteMessage->putUserHeaders(key, value);
    ASSERT_EQ(byteMessage->userHeaders()->getInt(key), value);
}

TEST_F(ByteMessageImplTest, testPutUserHeadersLong) {
    string key = "l";
    long value = 1L;
    byteMessage->putUserHeaders(key, value);
    ASSERT_EQ(byteMessage->userHeaders()->getLong(key), value);
}

TEST_F(ByteMessageImplTest, testPutUserHeadersDouble) {
    string key = "double";
    double value = 1.0;
    byteMessage->putUserHeaders(key, value);
    ASSERT_EQ(byteMessage->userHeaders()->getDouble(key), value);
}

TEST_F(ByteMessageImplTest, testPutUserHeadersString) {
    string key = "s";
    string value = "string";
    byteMessage->putUserHeaders(key, value);
    ASSERT_TRUE(byteMessage->userHeaders()->getString(key) == value);
}

TEST_F(ByteMessageImplTest, testPutSysHeadersInt) {
    string key = "i";
    int value = 1;
    byteMessage->putSysHeaders(key, value);
    ASSERT_EQ(byteMessage->sysHeaders()->getInt(key), value);
}

TEST_F(ByteMessageImplTest, testPutSysHeadersLong) {
    string key = "l";
    long value = 1L;
    byteMessage->putSysHeaders(key, value);
    ASSERT_EQ(byteMessage->sysHeaders()->getLong(key), value);
}

TEST_F(ByteMessageImplTest, testPutSysHeadersDouble) {
    string key = "double";
    double value = 1.0;
    byteMessage->putSysHeaders(key, value);
    ASSERT_EQ(byteMessage->sysHeaders()->getDouble(key), value);
}

TEST_F(ByteMessageImplTest, testPutSysHeadersString) {
    string key = "sys";
    string value = "string";
    byteMessage->putSysHeaders(key, value);
    ASSERT_TRUE(byteMessage->sysHeaders()->getString(key) == value);
}