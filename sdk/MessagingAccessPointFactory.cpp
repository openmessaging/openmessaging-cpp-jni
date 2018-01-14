#include <boost/make_shared.hpp>

#include "MessagingAccessPointFactory.h"
#include "MessagingAccessPointImpl.h"

namespace io {
    namespace openmessaging {
        boost::shared_ptr<MessagingAccessPoint>
        MessagingAccessPointFactory::getMessagingAccessPoint(std::string &url, boost::shared_ptr<KeyValue> properties) {
            return boost::make_shared<core::MessagingAccessPointImpl>(url, properties);
        }
    }
}
