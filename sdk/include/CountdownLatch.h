#ifndef OMS_COUNTDOWN_LATCH_H
#define OMS_COUNTDOWN_LATCH_H

#include <boost/thread.hpp>

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class CountdownLatch {
    public:
        CountdownLatch(unsigned int count);

        void countdown();

        void await();

        bool await(long long timeout);

    private:
        boost::mutex _mtx;
        unsigned int _count;
        boost::condition_variable cv;

        bool predicate();
    };
END_NAMESPACE_2(io, openmessaging)

#endif //OMS_COUNTDOWN_LATCH_H
