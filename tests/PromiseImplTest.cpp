#include <iostream>
#include <gtest/gtest.h>
#include <producer/SendResultImpl.h>
#include "PromiseImpl.h"
#include "core.h"
#include "CountdownLatch.h"

BEGIN_NAMESPACE_2(io, openmessaging)
class FutureListenerExample : public io::openmessaging::FutureListener {
public:
    explicit FutureListenerExample(CountdownLatch &countdownLatch) : _countdownLatch(countdownLatch) {
        std::cout << "Ctor()" << std::endl;
    }

    virtual ~FutureListenerExample() {
        std::cout << "Dtor()" << std::endl;
    }

    virtual void operationComplete(const io::openmessaging::Future& future) {
        std::cout << "OK" << std::endl;
        _countdownLatch.countdown();
    }

private:
    CountdownLatch &_countdownLatch;

};
END_NAMESPACE_2(io, openmessaging)

using namespace io::openmessaging;

class PromiseImplTest : public ::testing::Test {
public:
    virtual void SetUp() {
        Initialize();
        CurrentEnv ctx;
        jclass classSendResultLocal = ctx.env->FindClass("io/openmessaging/rocketmq/domain/SendResultImpl");
        if (classSendResultLocal) {
            classSendResult = reinterpret_cast<jclass>(ctx.env->NewGlobalRef(classSendResultLocal));
            ctx.env->DeleteLocalRef(classSendResultLocal);

            midDefaultCtor = getMethod(ctx, classSendResult, "<init>", "(Ljava/lang/String;Lio/openmessaging/KeyValue;)V");

            jobject localObject = ctx.env->NewObject(classSendResult, midDefaultCtor, NULL, NULL);
            if (localObject) {
                objectSendResult = ctx.env->NewGlobalRef(localObject);
            } else {
                checkAndClearException(ctx);
            }
        }
    }

    virtual void TearDown() {
        CurrentEnv current;
        current.env->DeleteGlobalRef(classSendResult);
        current.env->DeleteGlobalRef(objectSendResult);
    }

protected:
    jclass classSendResult;
    jobject objectSendResult;
    jmethodID midDefaultCtor;
};

TEST_F(PromiseImplTest, testCtor) {
    PromiseImpl promise;
    ASSERT_FALSE(promise.isCancelled());
    ASSERT_FALSE(promise.isDone());
}

TEST_F(PromiseImplTest, testAddListener) {
    PromiseImpl promise;
    CountdownLatch countdownLatch(1);
    boost::shared_ptr<FutureListener> listener = boost::make_shared<FutureListenerExample>(boost::ref(countdownLatch));
    promise.addListener(listener);
    boost::shared_ptr<producer::SendResult> pSendResult =
            boost::make_shared<producer::SendResultImpl>(objectSendResult);
    promise.set(pSendResult);
    ASSERT_TRUE(countdownLatch.await(10000));
}
