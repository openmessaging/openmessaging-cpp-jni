#include "CountdownLatch.h"

using namespace io::openmessaging;

CountdownLatch::CountdownLatch(unsigned int count)  : _count(count) {

}

void CountdownLatch::countdown() {
    NS::unique_lock<NS::mutex> lk(_mtx);
    if (_count <= 0) {
        return;
    }

    _count--;

    if (0 == _count) {
        cv.notify_all();
    }
}

void CountdownLatch::await() {
    if (0 == _count) {
        return;
    }

    {
        NS::unique_lock<NS::mutex> lk(_mtx);
        if(_count) {
            cv.wait(lk, NS::bind(&CountdownLatch::predicate, this));
        }
    }
}

bool CountdownLatch::await(long long timeout) {
    if (0 == _count) {
        return true;
    }

    {
        NS::unique_lock<NS::mutex> lk(_mtx);
        if (_count) {

#if __cplusplus >= 201103L
            auto timeout_point = NS::chrono::system_clock::now() + NS::chrono::milliseconds(timeout);
            cv.wait_until(lk, timeout_point, NS::bind(&CountdownLatch::predicate, this));
#else
            const NS::system_time timeout_point =
                    NS::get_system_time() + NS::posix_time::milliseconds(timeout);
            cv.timed_wait(lk, timeout_point, NS::bind(&CountdownLatch::predicate, this));
#endif
            if (_count) {
                return false;
            }
        }
    }
    return true;
}

bool CountdownLatch::predicate()  {
    return _count <= 0;
}