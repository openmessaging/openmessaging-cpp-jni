#include <boost/make_shared.hpp>

#include "MessagingAccessPointFactory.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    boost::shared_ptr<MessagingAccessPoint>
    MessagingAccessPointFactory::getMessagingAccessPoint(std::string &url, boost::shared_ptr<KeyValue> properties) {

        if (!core::isRunning()) {
            core::Initialize();
        }

        core::CurrentEnv current;
        jclass classMessagingAccessPointFactory = current.env->FindClass("io/openmessaging/MessagingAccessPointFactory");
        jmethodID midGetMessagingAccessPoint;
        bool useKV = false;
        if (properties) {
            useKV = true;
            const char *signature = "(Ljava/lang/String;Lio/openmessaging/KeyValue;)Lio/openmessaging/MessagingAccessPoint;";
            midGetMessagingAccessPoint = current.env->GetStaticMethodID(classMessagingAccessPointFactory,
                                                                        "getMessagingAccessPoint", signature);
        } else {
            const char *signature = "(Ljava/lang/String;)Lio/openmessaging/MessagingAccessPoint;";
            midGetMessagingAccessPoint = current.env->GetStaticMethodID(classMessagingAccessPointFactory,
                                                                        "getMessagingAccessPoint", signature);
        }

        jobject messagingAccessPoint;
        if (useKV) {
            messagingAccessPoint = current.env->CallStaticObjectMethod(classMessagingAccessPointFactory,
                                                                       midGetMessagingAccessPoint,
                                                                       boost::static_pointer_cast<core::KeyValueImpl>(properties)->getInternal());
        } else {
            messagingAccessPoint = current.env->CallStaticObjectMethod(classMessagingAccessPointFactory,
                                                                       midGetMessagingAccessPoint);
        }

        current.env->DeleteLocalRef(classMessagingAccessPointFactory);

        jobject globalRef = current.env->NewGlobalRef(messagingAccessPoint);

        return boost::make_shared<core::MessagingAccessPointImpl>(url, properties, globalRef);
    }

END_NAMESPACE_2(io, openmessaging)