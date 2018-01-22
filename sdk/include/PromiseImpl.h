#ifndef OMS_PROMISE_IMPL_H
#define OMS_PROMISE_IMPL_H

#include "Promise.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class PromiseImpl : public virtual Promise {
    public:
        PromiseImpl();

        virtual ~PromiseImpl();

        virtual bool isCancelled();

        virtual bool isDone();

        virtual boost::shared_ptr<producer::SendResult> get();

        virtual boost::shared_ptr<producer::SendResult> get(long timeout);

        virtual Future &addListener(boost::shared_ptr<FutureListener> listener);

        virtual std::exception &getThrowable();

        virtual bool cancel(bool interruptIfRunning);

        virtual bool set(boost::shared_ptr<producer::SendResult> &value);

        virtual bool setFailure(std::exception &e);

    private:
        std::exception e;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_PROMISE_IMPL_H
