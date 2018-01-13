#ifndef OMS_JAVAOPTION_H
#define OMS_JAVAOPTION_H

#include <string>
#include <vector>

#include <jni.h>

namespace io {
    namespace openmessaging {
        namespace core {
            class JavaOption {
            public:
                JavaOption(const jint version);

                void addOption(const std::string &option);

                const jint _version;

                std::vector<std::string> options;
            };
        }
    }
}

#endif //OMS_JAVAOPTION_H
