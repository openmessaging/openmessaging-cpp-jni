#ifndef OMS_SMART_POINTER_H
#define OMS_SMART_POINTER_H

#if __cplusplus >= 201103L
    #include <memory>
    #define NS std
#else

    #include <boost/smart_ptr.hpp>
    #define NS boost
#ifdef USE_BOOST
#else

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)
    class atomic_int {
    public:
        atomic_int() : value_(0) {
        }

        atomic_int(int value) : value_(value) {
        }

        atomic_int(const atomic_int &other) {
            value_ = other.value_;
        }

        atomic_int &operator=(const atomic_int &other) {
            value_ = other.value_;
            return *this;
        }

        // prefix
        atomic_int &operator++() {
            __sync_fetch_and_add(&value_, 1);
            return *this;
        }

        // postfix
        atomic_int &operator++(int) {
            __sync_fetch_and_add(&value_, 1);
            return *this;
        }

        // prefix
        atomic_int &operator--() {
            __sync_fetch_and_sub(&value_, 1);
            return *this;
        }

        // postfix
        atomic_int &operator--(int) {
            __sync_fetch_and_sub(&value_, 1);
            return *this;
        }

        int get() {
            return value_;
        }

    private:
        int value_;
    };

    template<typename T>
    class shared_ptr {
    public:
        shared_ptr() : ptr_(NULL), refCount(NULL) {}

        shared_ptr(T *ptr) : ptr_(ptr), refCount(new atomic_int(1)) {

        }

        shared_ptr(T *ptr, atomic_int* refCnt) : ptr_(ptr), refCount(refCnt) {

        }

        template<typename U>
        shared_ptr(const shared_ptr<U> &other) {
            ptr_ = dynamic_cast<T*>(other.get());
            if (ptr_) {
                refCount = other.getRefCount();
                if (refCount) {
                    (*refCount)++;
                }
            }
        }

        ~shared_ptr() {
            if (refCount) {
                (*refCount)--;
                if (refCount->get() == 0) {
                    delete(ptr_);
                    delete(refCount);
                }
            }
        }

        template<typename U>
        shared_ptr<T>& operator=(const shared_ptr<U> &other) {
            if (this == &other) {
                return *this;
            }

            if (refCount) {
                (*refCount)--;
                if (refCount->get() == 0) {
                    delete (ptr_);
                    delete(refCount);
                }
            }

            ptr_ = dynamic_cast<T*>(other.get());
            if (ptr_) {
                refCount = other.getRefCount();
                if (refCount) {
                    (*refCount)++;
                }
            }

            return *this;
        }

        T& operator*() {
            return *ptr_;
        }

        T* operator->() {
            return ptr_;
        }

        T* get() const {
            return ptr_;
        }

        atomic_int* getRefCount() const {
            return refCount;
        }

    private:
        T *ptr_;
        atomic_int *refCount;
    };

    template <typename T>
    shared_ptr<T> make_shared() {
        T* p = new T();
        return shared_ptr<T>(p);
    }

    template <typename T, typename Arg>
    shared_ptr<T> make_shared(Arg arg) {
        T* p = NULL;

        try {
            p = new T(arg);
        } catch (...) {
            //TODO: allocate memory failed.
            delete p;
            return shared_ptr<T>();
        }

        return shared_ptr<T>(p);
    }

    template <typename U, typename V>
    shared_ptr<U> dynamic_pointer_cast(shared_ptr<V> &ptr) {
        U* p = dynamic_cast<U*>(ptr.get());
        if (p) {
            ptr.getRefCount()->operator++();
            return shared_ptr<U>(p, ptr.getRefCount());
        }

        return shared_ptr<U>();
    }

END_NAMESPACE_2(io, openmessaging)

// #define NS io::openmessaging

#endif

#endif


#endif //OMS_SMART_POINTER_H
