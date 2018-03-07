#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <set>
#include <string>

#include <boost/smart_ptr.hpp>
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "JavaOption.h"
#include "Namespace.h"
#include "CurrentEnv.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class Types {
    public:
        static const char *void_;
        static const char *boolean_;
        static const char *byte_;
        static const char *char_;
        static const char *short_;
        static const char *int_;
        static const char *long_;
        static const char *float_;
        static const char *double_;

        static const char *byteArray_;

        static const char *Object_;
        static const char *String_;
        static const char *KeyValue_;
        static const char *Set_;
        static const char *MessagingAccessPoint_;
        static const char *Producer_;
        static const char *PushConsumer_;
        static const char *PullConsumer_;
        static const char *Message_;
        static const char *ByteMessage_;
        static const char *SendResult_;
        static const char *LocalTransactionBranchExecutor_;
        static const char *Context_;
        static const char *MessageListener_;

    };

    std::string buildSignature(const std::string &return_type, int n, ...);

    void Initialize();

    void Shutdown();

    bool isRunning();

    std::set<std::string> toNativeSet(CurrentEnv &env, jobject s);

    std::string expand_class_path(const std::string& wildcard);

    std::vector<std::string> list(const std::string &dir, bool (*f)(const std::string&));

    bool stringEndsWith(const std::string &s, const std::string &ext);

    bool file_name_filter(const std::string &file_name);

    std::string build_class_path_option();

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_CORE_H
