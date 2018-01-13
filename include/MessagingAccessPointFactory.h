#ifndef MESSAGINGACCESSPOINTFACTORY_H
#define MESSAGINGACCESSPOINTFACTORY_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "MessagingAccessPoint.h"


namespace io {
    namespace openmessaging {

        extern boost::shared_ptr<KeyValue> kv_nullptr;

        class MessagingAccessPointFactory {
        public:

            static boost::shared_ptr<MessagingAccessPoint> getMessagingAccessPoint(std::string &url,
                                                                                   boost::shared_ptr<KeyValue> properties = kv_nullptr) {


                return NULL;
            }

        };

    }

}

#endif // MESSAGINGACCESSPOINTFACTORY_H
