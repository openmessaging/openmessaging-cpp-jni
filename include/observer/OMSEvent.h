//
// Created by Li Zhanhui on 2018/1/13.
//

#ifndef OMS_OMSEVENT_H
#define OMS_OMSEVENT_H

namespace io {
    namespace openmessaging {
        namespace observer {
            class OMSEvent {
            public:
                virtual ~OMSEvent() {

                }

                virtual std::string type() = 0;

                virtual std::string details() = 0;
            };
        }
    }
}

#endif //OMS_OMSEVENT_H
