#include <iostream>
#include <gtest/gtest.h>
#include <producer/SendResultImpl.h>
#include "PromiseImpl.h"
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
            std::cout << "Execute callback OK" << std::endl;
            _countdownLatch.countdown();
        }

    private:
        CountdownLatch &_countdownLatch;
    };

    class PromiseImplTest : public ::testing::Test {
    public:
        virtual void SetUp() {
            Initialize();
            CurrentEnv ctx;
            const char *klassSendResultImpl = "io/openmessaging/rocketmq/domain/SendResultImpl";
            classSendResult = ctx.findClass(klassSendResultImpl);
            midDefaultCtor = ctx.getMethodId(classSendResult, "<init>", "(Ljava/lang/String;Lio/openmessaging/KeyValue;)V");
            jobject localObject = ctx.newObject(classSendResult, midDefaultCtor, NULL, NULL);
            objectSendResult = ctx.newGlobalRef(localObject);
        }

        virtual void TearDown() {
            CurrentEnv current;
            current.deleteRef(classSendResult);
            current.deleteRef(objectSendResult);
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

    TEST_F(PromiseImplTest, testAddListener2) {
        PromiseImpl promise;
        CountdownLatch countdownLatch(1);
        boost::shared_ptr<FutureListener> listener = boost::make_shared<FutureListenerExample>(boost::ref(countdownLatch));
        boost::shared_ptr<producer::SendResult> pSendResult =
                boost::make_shared<producer::SendResultImpl>(objectSendResult);
        promise.set(pSendResult);
        promise.addListener(listener); // Should execute listener immediately
        ASSERT_TRUE(countdownLatch.await(10000));
    }

END_NAMESPACE_2(io, openmessaging)
