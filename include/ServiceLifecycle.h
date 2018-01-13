#ifndef SERVICELIFECYCLE_H
#define SERVICELIFECYCLE_H

namespace io {
namespace openmessaging {

class ServiceLifecycle {
public:
    virtual void startup() = 0;

    virtual void shutdown() = 0;

    virtual ~ServiceLifecycle() {
    }
};

}

}

#endif // SERVICELIFECYCLE_H
