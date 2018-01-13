#ifndef OMS_CONTEXT_H
#define OMS_CONTEXT_H

namespace io {
    namespace openmessaging {
        namespace consumer {
            class Context {
            public:
                ~Context() {
                }

                boost::shared_ptr<KeyValue> properties() = 0;

                void ack(boost::shared_ptr<KeyValue> properties = kv_nullptr) = 0;
            };
        }
    }
}

#endif //OMS_CONTEXT_H
