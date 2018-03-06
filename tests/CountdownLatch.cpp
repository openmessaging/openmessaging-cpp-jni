#include "CountdownLatch.h"

#include <boost/date_time.hpp>

using namespace io::openmessaging;

CountdownLatch::CountdownLatch(unsigned int count)  : _count(count) {

}

void CountdownLatch::countdown() {
    boost::unique_lock<boost::mutex> lk(_mtx);
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
        boost::unique_lock<boost::mutex> lk(_mtx);
        if(_count) {
            cv.wait(lk, boost::bind(&CountdownLatch::predicate, this));
        }
    }
}

bool CountdownLatch::await(long long timeout) {
    if (0 == _count) {
        return true;
    }

    {
        boost::unique_lock<boost::mutex> lk(_mtx);
        if (_count) {
            const boost::system_time timeout_point =
                    boost::get_system_time() + boost::posix_time::milliseconds(timeout);
            cv.timed_wait(lk, timeout_point, boost::bind(&CountdownLatch::predicate, this));
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