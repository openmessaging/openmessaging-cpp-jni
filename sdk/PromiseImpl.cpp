#include "core.h"
#include "PromiseImpl.h"
#include <plog/Log.h>

using namespace io::openmessaging;

PromiseImpl::PromiseImpl() : done(false), failed(false), cancelled(false), _mtx(), _cv(_mtx) {

}

PromiseImpl::~PromiseImpl() {

}

bool PromiseImpl::isCancelled() {
    return cancelled;
}

bool PromiseImpl::isDone() {
    return done;
}

NS::shared_ptr<producer::SendResult> PromiseImpl::get(unsigned long timeout) {
    NS::shared_ptr<producer::SendResult> sr_nullptr;
    if (cancelled) {
        return sr_nullptr;
    }

    if (done) {
        return _value;
    }

    {
        LockGuard lk(_mtx);
        if (cancelled) {
            return sr_nullptr;
        }

        if (done) {
            return _value;
        }
        _cv.wait(timeout);

        if (cancelled) {
            return sr_nullptr;
        }

        if (done) {
            return _value;
        }

        return sr_nullptr;
    }
}

Future& PromiseImpl::addListener(NS::shared_ptr<FutureListener> listener) {
    LockGuard lk(_mtx);
    if (done) {
        listener->operationComplete(*this);
    } else if (cancelled) {
        return *this;
    }

    _listeners.push_back(listener);
    return *this;
}

std::exception& PromiseImpl::getThrowable() {
    return m_e;
}

bool PromiseImpl::cancel(bool interruptIfRunning) {
    if (done) {
        return false;
    }

    {
        LockGuard lk(_mtx);
        if(done) {
            return false;
        }
        cancelled = true;
    }

    _cv.notifyAll();
    return true;
}

bool PromiseImpl::set(NS::shared_ptr<producer::SendResult> &value) {
    if (cancelled) {
        return false;
    }

    {
        LockGuard lk(_mtx);
        _value = value;
        done = true;
        _cv.notifyAll();
    }

    for (std::vector<NS::shared_ptr<FutureListener> >::iterator it = _listeners.begin(); it != _listeners.end(); it++) {
        (*it)->operationComplete(*this);
    }

    return false;
}

bool PromiseImpl::setFailure(std::exception &e) {
    LockGuard lk(_mtx);
    done = true;
    failed = true;
    m_e = e;
    return false;
}

