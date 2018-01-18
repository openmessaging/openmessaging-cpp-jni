#ifndef OMS_JAVAOPTION_H
#define OMS_JAVAOPTION_H

#include <string>
#include <vector>
#include <jni.h>

#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class JavaOption {
    public:
        JavaOption(const jint version);

        void addOption(const std::string &option);

        const jint _version;

        std::vector<std::string> options;
    };

END_NAMESPACE_2(io, openmessaging)
#endif //OMS_JAVAOPTION_H
