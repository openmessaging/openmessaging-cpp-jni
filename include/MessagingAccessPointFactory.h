#ifndef MESSAGINGACCESSPOINTFACTORY_H
#define MESSAGINGACCESSPOINTFACTORY_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "MessagingAccessPoint.h"
#include "interceptor/MessagingAccessPointInterceptor.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        extern boost::shared_ptr<KeyValue> kv_nullptr;

        class MessagingAccessPointFactory {
        public:

            static boost::shared_ptr<MessagingAccessPoint> getMessagingAccessPoint(std::string &url,
                                                                                   boost::shared_ptr<KeyValue> properties = kv_nullptr);

            static void addInterceptor(boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor);

            static void removeInterceptor(boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> interceptor);


        };

END_NAMESPACE_2(io, openmessaging)

#endif // MESSAGINGACCESSPOINTFACTORY_H
