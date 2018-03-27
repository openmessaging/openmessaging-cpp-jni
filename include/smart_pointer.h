#ifndef OMS_SMART_POINTER_H
#define OMS_SMART_POINTER_H

#if __cplusplus >= 201103L

#include <memory>

#define NS std

#else

#include <boost/smart_ptr.hpp>

#define NS boost

#endif


#endif //OMS_SMART_POINTER_H
