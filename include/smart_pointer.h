#ifndef OMS_SMART_POINTER_H
#define OMS_SMART_POINTER_H

#include "OMSException.h"

typedef io::openmessaging::OMSException OMSException;

// In case C++11 or later version is available.
#if __cplusplus >= 201103L
    #include <memory>
    #define NS std

    BEGIN_NAMESPACE_2(io, openmessaging)
    #define MessageBodyPtr std::unique_ptr<char[]>
    END_NAMESPACE_2(io, openmessaging)

#else
    // If boost is used, we would use Boost.SmartPointer
    #ifdef USE_BOOST
        #include <boost/smart_ptr.hpp>
        #define NS boost

        BEGIN_NAMESPACE_2(io, openmessaging)
        #define MessageBodyPtr boost::scoped_array<char>
        END_NAMESPACE_2(io, openmessaging)

    #else
        #include "shared_ptr.hpp"
        #include <cstring>
        #define NS io::openmessaging

        BEGIN_NAMESPACE_2(io, openmessaging)

        template<typename T>
        class scoped_array {
        public:
            scoped_array(const T *ptr, int length) : ptr_(NULL), length_(length) {
                ptr_ = new T[length_];
                memcpy(ptr_, ptr, length_);
            }

            /**
             * Copy constructor is assumed to be expensive and used with caution.
             */
            scoped_array(const scoped_array& other) {
                ptr_ = new T[other.length_];
                length_ = other.length_;
                std::memcpy(ptr_, other.ptr_, other.length_ * sizeof(T));
            }

            // copy assignment
            scoped_array& operator= (const scoped_array &other) {
                if (this != &other) {
                    delete[](ptr_);
                    length_ = other.length_;
                    ptr_ = new T[length_];
                    std::memcpy(ptr_, other.ptr_, length_ * sizeof(T));
                }
                return *this;
            }

            ~scoped_array() {
                delete[](ptr_);
            }

            int getLength() const {
                return length_;
            }

            const T* getRawPtr() const {
                return ptr_;
            }

            T& operator[](int i) {
                if (i >= length_ || i < 0) {
                    throw OMSException("IndexOutOfBoundary");
                }
                return *(ptr_ + i);
            }

        private:
            T *ptr_;
            int length_;
        };

        #define MessageBodyPtr scoped_array<char>

        END_NAMESPACE_2(io, openmessaging)
    #endif
#endif



#endif //OMS_SMART_POINTER_H
