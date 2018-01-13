#ifndef OMS_BYTEMESSAGE_H
#define OMS_BYTEMESSAGE_H

#include <vector>

#include <boost/move/move.hpp>

#include "Message.h"

namespace io {
    namespace openmessaging {
        class ByteMessage : public Message {
        public:
            std::vector<char>& getBody() {
                return _body;
            }

            ByteMessage& setBody(const std::vector<char>& body) {
                _body = boost::move(body);
            }

        protected:
            std::vector<char> _body;
        };
    }
}


#endif //OMS_BYTEMESSAGE_H
