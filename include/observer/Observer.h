#ifndef OMS_OBSERVER_H
#define OMS_OBSERVER_H

#include <boost/smart_ptr.hpp>

#include "OMSEvent.h"
#include "Namespace.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, observer)
    class Observer : private Uncopyable {
    public:
        virtual ~Observer() {

        }

        virtual void onEvent(boost::shared_ptr<OMSEvent> &event) = 0;
    };

END_NAMESPACE_3(io, openmessaging, observer)

#endif //OMS_OBSERVER_H
