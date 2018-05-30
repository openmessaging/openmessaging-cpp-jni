#ifndef OMS_PROMISE_IMPL_H
#define OMS_PROMISE_IMPL_H

#include <vector>

#include "smart_pointer.h"

#include "Promise.h"
#include "producer/SendResult.h"


BEGIN_NAMESPACE_2(io, openmessaging)

    class PromiseImpl : public virtual Promise {
    public:
        PromiseImpl();

        virtual ~PromiseImpl();

        virtual bool isCancelled();

        virtual bool isDone();

        virtual NS::shared_ptr<producer::SendResult> get(unsigned long timeout = LONG_MAX);

        virtual Future &addListener(NS::shared_ptr<FutureListener> listener);

        virtual std::exception &getThrowable();

        virtual bool cancel(bool interruptIfRunning);

        virtual bool set(NS::shared_ptr<producer::SendResult> &value);

        virtual bool setFailure(std::exception &e);

    private:
        bool done;
        bool failed;
        bool cancelled;
        std::exception m_e;
        std::vector<NS::shared_ptr<FutureListener> > _listeners;
        Mutex _mtx;
        Condition _cv;
        NS::shared_ptr<producer::SendResult> _value;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_PROMISE_IMPL_H
