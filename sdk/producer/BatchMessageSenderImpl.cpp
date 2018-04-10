#include "producer/BatchMessageSenderImpl.h"
#include "ByteMessageImpl.h"


using namespace io::openmessaging::producer;

BatchMessageSenderImpl::BatchMessageSenderImpl(const jobject &proxy) : proxy_(proxy) {
    CurrentEnv context;
    classBatchMessageSender = context.findClass("io/openmessaging/producer/BatchMessageSender");
    midSend = context.getMethodId(classBatchMessageSender, "send",
                                  buildSignature(Types::BatchMessageSender_, 1, Types::Message_));

    midCommit = context.getMethodId(classBatchMessageSender, "commit", buildSignature(Types::void_, 0));

    midRollback = context.getMethodId(classBatchMessageSender, "rollback", buildSignature(Types::void_, 0));

    midClose = context.getMethodId(classBatchMessageSender, "close", buildSignature(Types::void_, 0));
}

BatchMessageSender& BatchMessageSenderImpl::send(MessagePtr &message) {
    CurrentEnv context;
    ByteMessageImplPtr msgPtr = NS::dynamic_pointer_cast<ByteMessageImpl>(message);
    jobject ret = context.callObjectMethod(proxy_, midSend, msgPtr->getProxy());
    context.deleteRef(ret);
    return *this;
}

void BatchMessageSenderImpl::commit() {
    CurrentEnv context;
    context.callVoidMethod(proxy_, midCommit);
}

void BatchMessageSenderImpl::rollback() {
    CurrentEnv context;
    context.callVoidMethod(proxy_, midRollback);
}

void BatchMessageSenderImpl::close() {
    CurrentEnv context;
    context.callVoidMethod(proxy_, midClose);
}