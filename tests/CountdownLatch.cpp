#include "CountdownLatch.h"

using namespace io::openmessaging;

CountdownLatch::CountdownLatch(unsigned int count)  : _count(count) {
    pthread_cond_init(&cv, NULL);
}

void CountdownLatch::countdown() {
    LockGuard lk(_mtx);
    if (_count <= 0) {
        return;
    }

    _count--;

    if (0 == _count) {
        pthread_cond_broadcast(&cv);
    }
}

void CountdownLatch::await() {
    if (0 == _count) {
        return;
    }

    {
        LockGuard lk(_mtx);
        while (_count > 0) {
            pthread_cond_wait(&cv, _mtx.get());
        }
    }
}

bool CountdownLatch::await(long long timeout) {
    if (0 == _count) {
        return true;
    }

    if (timeout <= 0) {
        return true;
    }

    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct timespec ts;

        long one_second_in_nano = 1000000000;
        long one_milli_second_in_nano = 1000000;
        long one_micro_second_in_nano = 1000;
        long one_second_in_milli = 1000;

        ts.tv_sec = tv.tv_sec + timeout / one_second_in_milli;
        ts.tv_nsec = tv.tv_usec * one_micro_second_in_nano + (timeout % one_second_in_milli) * one_milli_second_in_nano;
        if (ts.tv_nsec >= one_second_in_nano) {
            ts.tv_sec = ts.tv_sec + (ts.tv_nsec / one_second_in_nano);
            ts.tv_nsec = ts.tv_nsec % one_second_in_nano;
        }

        long abs_time = ts.tv_sec * one_second_in_nano + ts.tv_nsec;

        LockGuard lk(_mtx);
        while (true) {
            if (!_count) {
                return true;
            }

            gettimeofday(&tv, NULL);
            if (tv.tv_sec * one_second_in_nano + tv.tv_usec * one_micro_second_in_nano > abs_time) {
                if (_count) {
                    return false;
                }
            }
            pthread_cond_timedwait(&cv, _mtx.get(), &ts);
        }
    }
}