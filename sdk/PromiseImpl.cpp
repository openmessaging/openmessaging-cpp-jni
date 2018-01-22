#include "PromiseImpl.h"

using namespace io::openmessaging;

PromiseImpl::PromiseImpl() {

}

PromiseImpl::~PromiseImpl() {

}

bool PromiseImpl::isCancelled() {
    return false;
}

bool PromiseImpl::isDone() {
    return false;
}

boost::shared_ptr<producer::SendResult> PromiseImpl::get() {
    boost::shared_ptr<producer::SendResult> sendResult;
    return sendResult;
}

boost::shared_ptr<producer::SendResult> PromiseImpl::get(long timeout) {
    boost::shared_ptr<producer::SendResult> sendResult;
    return sendResult;
}

Future& PromiseImpl::addListener(boost::shared_ptr<FutureListener> listener) {
    return *this;
}

std::exception& PromiseImpl::getThrowable() {
    return e;
}

bool PromiseImpl::cancel(bool interruptIfRunning) {
    return false;
}

bool PromiseImpl::set(boost::shared_ptr<producer::SendResult> &value) {
    return false;
}

bool PromiseImpl::setFailure(std::exception &e) {
    return false;
}

