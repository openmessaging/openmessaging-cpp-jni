#ifndef OMS_PROMISE_IMPL_H
#define OMS_PROMISE_IMPL_H

#include <vector>
#include <boost/thread.hpp>
#include "Promise.h"
#include "producer/SendResult.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class PromiseImpl : public virtual Promise {
    public:
        PromiseImpl();

        virtual ~PromiseImpl();

        virtual bool isCancelled();

        virtual bool isDone();

        virtual boost::shared_ptr<producer::SendResult> get(unsigned long timeout = LONG_MAX);

        virtual Future &addListener(boost::shared_ptr<FutureListener> listener);

        virtual std::exception &getThrowable();

        virtual bool cancel(bool interruptIfRunning);

        virtual bool set(boost::shared_ptr<producer::SendResult> &value);

        virtual bool setFailure(std::exception &e);

    private:
        bool done;
        bool failed;
        bool cancelled;
        std::exception e;
        std::vector<boost::shared_ptr<FutureListener> > _listeners;
        boost::mutex _mtx;
        boost::condition_variable _cv;
        boost::shared_ptr<producer::SendResult> _value;
    };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_PROMISE_IMPL_H
