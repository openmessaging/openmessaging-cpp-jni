#include <boost/smart_ptr.hpp>
#include "KeyValue.h"
#include "consumer/ContextImpl.h"


using namespace io::openmessaging;
using namespace io::openmessaging::consumer;

boost::shared_ptr<KeyValue> ContextImpl::properties() {
    return kv_nullptr;
}

void ContextImpl::ack(boost::shared_ptr<KeyValue> properties) {

}
