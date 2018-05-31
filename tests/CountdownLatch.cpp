#include "CountdownLatch.h"

using namespace io::openmessaging;

CountdownLatch::CountdownLatch(unsigned int count)  : _count(count), _mtx(), cv(_mtx) {
}

void CountdownLatch::countdown() {
    if (_count <= 0) {
        return;
    }

    {
        __sync_fetch_and_sub(&_count, 1);
        std::cout << "Count down [OK]" << std::endl;
        if (0 == _count) {
            cv.notifyAll();
            std::cout << "Notify [OK]" << std::endl;
        }
    }
}

void CountdownLatch::await() {
    if (0 == _count) {
        return;
    }

    {
        while (_count > 0) {
            std::cout << "Begin to wait" << std::endl;
            cv.wait();
            std::cout << "Wake up, due to signal or spuriously due to interrupt" << std::endl;
        }
    }
}

bool CountdownLatch::await(unsigned long timeout) {
    if (0 == _count) {
        return true;
    }

    if (timeout <= 0) {
        return true;
    }

    cv.wait(timeout);

    return _count <= 0;
}