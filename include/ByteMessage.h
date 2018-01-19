#ifndef OMS_BYTE_MESSAGE_H
#define OMS_BYTE_MESSAGE_H

#include <vector>

#include <boost/move/move.hpp>

#include "Message.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        class ByteMessage : public virtual Message {
        public:
            virtual ~ByteMessage() {

            }

            virtual std::vector<char> getBody()  = 0;

            virtual ByteMessage& setBody(const std::vector<char> &body) = 0;

        };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_BYTE_MESSAGE_H
