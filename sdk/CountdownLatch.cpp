#include "CountdownLatch.h"

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

bool CountdownLatch::predicate()  {
    return _count <= 0;
}