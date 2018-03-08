#ifndef OMS_MESSAGING_ACCESS_POINT_FACTORY_H
#define OMS_MESSAGING_ACCESS_POINT_FACTORY_H

#include <dlfcn.h>
#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "MessagingAccessPoint.h"
#include "interceptor/MessagingAccessPointInterceptor.h"
#include "Namespace.h"
#include "OMS.h"
#include "OMSException.h"

#ifdef __cplusplus
extern "C" {
#endif

    boost::shared_ptr<io::openmessaging::MessagingAccessPoint>
    getMessagingAccessPoint(const std::string &url, const boost::shared_ptr<io::openmessaging::KeyValue> &props = io::openmessaging::kv_nullptr) {
        std::string::size_type begin = url.find(":");
        std::string::size_type end = url.find(":", begin + 1);
        std::string driver = url.substr(begin + 1, end - begin - 1);

#ifdef __APPLE__
        std::string extension = ".dylib";
#endif
#ifdef __linux__
        std::string extension = ".so";
#endif

        std::string shared_library_name = "liboms_" + driver + extension;

        void* handle = dlopen(shared_library_name);
        if (!handle) {
            std::string msg = "Failed to dlopen shared library: ";
            msg += shared_library_name;
            msg += ". Reason: ";
            msg += dlerror();
            throw io::openmessaging::OMSException(msg);
        }

        boost::shared_ptr<io::openmessaging::MessagingAccessPoint> (*fn)(const std::string&, const boost::shared_ptr<io::openmessaging::KeyValue> &);

        fn = dlsym(handle, "getMessagingAccessPointImpl");

        return fn(url, props);
    }

    boost::shared_ptr<io::openmessaging::MessagingAccessPoint>
    getMessagingAccessPointImpl(const std::string &url, const boost::shared_ptr<io::openmessaging::KeyValue> &props = io::openmessaging::kv_nullptr);

#ifdef __cplusplus
}
#endif

BEGIN_NAMESPACE_2(io, openmessaging)

    /**
     * A factory that provides some static methods to create a {@code MessagingAccessPoint}
     * from the specified OMS driver url.
     * <p>
     * The complete URL syntax is:
     * <p>
     * {@literal openmessaging:<driver_type>://<access_point>[,<access_point>,...]/<namespace>}
     * <p>
     * The first part of the URL specifies which OMS implementation is to be used, rocketmq is a
     * optional driver type.
     * <p>
     * The brackets indicate that the extra access points are optional, but a correct OMS driver url
     * needs at least one access point, which consists of hostname and port, like localhost:8081.
     * <p>
     * A namespace wraps the OMS resources in an abstraction that makes it appear to the users
     * within the namespace that they have their own isolated instance of the global OMS resources.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class MessagingAccessPointFactory {
    public:
        static boost::shared_ptr<MessagingAccessPoint>
        getMessagingAccessPoint(const std::string &url, const boost::shared_ptr<KeyValue> &properties = kv_nullptr);

        static void
        addInterceptor(const boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> &interceptor);

        static void
        removeInterceptor(const boost::shared_ptr<interceptor::MessagingAccessPointInterceptor> &interceptor);
    };

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_MESSAGING_ACCESS_POINT_FACTORY_H
