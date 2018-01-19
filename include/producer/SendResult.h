#ifndef OMS_SEND_RESULT_H
#define OMS_SEND_RESULT_H

#include <string>

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"
#include "Namespace.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class SendResult {
    public:
        virtual ~SendResult() {

        }

        virtual std::string messageId() = 0;

        virtual boost::shared_ptr<KeyValue> properties() = 0;

    };

END_NAMESPACE_3(io, openmessaging, producer)

#endif // OMS_SEND_RESULT_H
