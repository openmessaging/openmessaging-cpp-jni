#ifndef OMS_SMART_POINTER_H
#define OMS_SMART_POINTER_H

#include "OMSException.h"
#include <cstring>
#include <string>
#include "shared_ptr.hpp"

#define NS io::openmessaging

BEGIN_NAMESPACE_2(io, openmessaging)

    template <typename T>
    class ManagedArray {
    public:
        ManagedArray(T* payload, unsigned int len, bool copy = true) : len_(len), copy_(copy) {
            payload_ = new T[len_];
            if (copy_) {
                std::memcpy(payload_, payload, len * sizeof(T));
            } else {
                payload_ = payload;
            }
        }

        ManagedArray(const ManagedArray &other) {
            len_ = other.len_;
            copy_ = true;
            payload_ = new T[len_];
            std::memcpy(payload_, other.payload_, len_ * sizeof (T));
        }

        ~ManagedArray() {
            if (copy_) {
                delete[](payload_);
            }
        }

        T* get() const {
            return payload_;
        }

        unsigned int length() const {
            return len_;
        }

        T& operator[](int index) {
            if (index < len_) {
                return *(payload_ + index);
            }
            throw io::openmessaging::OMSException("IndexOutOfBoundary");
        }

        std::string toString() {
            return std::string(reinterpret_cast<char*>(payload_), len_);
        }

    private:
        T* payload_;
        unsigned int len_;
        bool copy_;
    };

typedef ManagedArray<signed char> MessageBody;

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_SMART_POINTER_H
