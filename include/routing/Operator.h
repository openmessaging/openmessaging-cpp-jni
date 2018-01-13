#ifndef OMS_OPERATOR_H
#define OMS_OPERATOR_H

#include <boost/shared_ptr.hpp>

#include "KeyValue.h"

namespace io {
    namespace openmessaging {
        namespace routing {
            class Operator {
            public:
                virtual ~Operator() {

                }

                virtual boost::shared_ptr<KeyValue> properties() = 0;

                virtual std::string expression() = 0;
            };
        }
    }
}

#endif //OMS_OPERATOR_H
