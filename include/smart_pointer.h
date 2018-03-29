#ifndef OMS_SMART_POINTER_H
#define OMS_SMART_POINTER_H

// In case C++11 or later version is available.
#if __cplusplus >= 201103L
    #include <memory>
    #define NS std
#else
    // If boost is used, we would use Boost.SmartPointer
    #ifdef USE_BOOST
        #include <boost/smart_ptr.hpp>
        #define NS boost
    #else
        #include "shared_ptr.hpp"
        #define NS io::openmessaging
    #endif
#endif

#endif //OMS_SMART_POINTER_H
