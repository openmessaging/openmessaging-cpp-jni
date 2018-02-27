#ifndef OMS_BYTE_MESSAGE_H
#define OMS_BYTE_MESSAGE_H

#include "Message.h"
#include "Namespace.h"
#include "OMS.h"

BEGIN_NAMESPACE_2(io, openmessaging)

        class ByteMessage : public virtual Message {
        public:
            virtual ~ByteMessage() {

            }

            virtual scoped_array<char> getBody()  = 0;

            virtual ByteMessage& setBody(scoped_array<char> &body) = 0;

        };

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_BYTE_MESSAGE_H
