#ifndef OMS_OBSERVER_H
#define OMS_OBSERVER_H

#include "smart_pointer.h"
#include "OMSEvent.h"
#include "Namespace.h"
#include "Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, observer)

    /**
     * A {@code Observer} interface is used to observe the {@code OMSEvent}
     * dispatches in observable objects, like {@link MessagingAccessPoint}
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class Observer : private Uncopyable {
    public:
        virtual ~Observer() {

        }

        virtual void onEvent(const OMSEvent &event) = 0;
    };

    typedef NS::shared_ptr<Observer> ObserverPtr;

END_NAMESPACE_3(io, openmessaging, observer)

#endif //OMS_OBSERVER_H
