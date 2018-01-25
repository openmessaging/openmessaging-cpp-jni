#include "PromiseImpl.h"

using namespace io::openmessaging;

PromiseImpl::PromiseImpl() : done(false), failed(false), cancelled(false) {

}

PromiseImpl::~PromiseImpl() {

}

bool PromiseImpl::isCancelled() {
    return cancelled;
}

bool PromiseImpl::isDone() {
    return done;
}

boost::shared_ptr<producer::SendResult> PromiseImpl::get(unsigned long timeout) {
    boost::shared_ptr<producer::SendResult> sr_nullptr;
    if (cancelled) {
        return sr_nullptr;
    }

    if (done) {
        return _value;
    }

    {
        boost::unique_lock<boost::mutex> lk(_mtx);
        if (cancelled) {
            return sr_nullptr;
        }

        if (done) {
            return _value;
        }

        _cv.wait_for(lk, boost::chrono::milliseconds(timeout));

        if (cancelled) {
            return sr_nullptr;
        }

        if (done) {
            return _value;
        }

        return sr_nullptr;
    }
}

Future& PromiseImpl::addListener(boost::shared_ptr<FutureListener> listener) {
    boost::lock_guard<boost::mutex> lk(_mtx);
    if (done) {
        listener->operationComplete(*this);
    } else if (cancelled) {
        return *this;
    }

    _listeners.push_back(listener);
    return *this;
}

std::exception& PromiseImpl::getThrowable() {
    return e;
}

bool PromiseImpl::cancel(bool interruptIfRunning) {
    if (done) {
        return false;
    }

    {
        boost::lock_guard<boost::mutex> lk(_mtx);
        if(done) {
            return false;
        }
        cancelled = true;
    }

    _cv.notify_all();
    return true;
}

bool PromiseImpl::set(boost::shared_ptr<producer::SendResult> &value) {
    if (cancelled) {
        return false;
    }

    {
        boost::unique_lock<boost::mutex> lk(_mtx);
        _value = value;
        done = true;
        _cv.notify_all();
    }

    for (std::vector<boost::shared_ptr<FutureListener> >::const_iterator it = _listeners.cbegin();
        it != _listeners.cend();
        it++) {
        (*it)->operationComplete(*this);
    }

    return false;
}

bool PromiseImpl::setFailure(std::exception &e) {
    boost::lock_guard<boost::mutex> lk(_mtx);
    done = true;
    failed = true;
    this->e = e;
    return false;
}

