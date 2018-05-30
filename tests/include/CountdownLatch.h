#ifndef OMS_COUNTDOWN_LATCH_H
#define OMS_COUNTDOWN_LATCH_H

#include "core.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class CountdownLatch {
    public:
        CountdownLatch(unsigned int count);

        void countdown();

        void await();

        bool await(long long timeout);

    private:
        Mutex _mtx;
        unsigned int _count;

        pthread_cond_t cv;
    };
END_NAMESPACE_2(io, openmessaging)

#endif //OMS_COUNTDOWN_LATCH_H
