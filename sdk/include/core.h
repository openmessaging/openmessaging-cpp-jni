#ifndef OMS_CORE_H
#define OMS_CORE_H

#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>

#include "JavaOption.h"
#include "Namespace.h"
#include "CurrentEnv.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    void Initialize();

    void Shutdown();

    bool isRunning();

    std::set<std::string> toNativeSet(CurrentEnv &env, jobject s);

    jmethodID getMethod(CurrentEnv &current, jclass clazz, const std::string &name, const std::string &signature,
                        bool isStatic = false);

    std::string expand_class_path(const std::string& wildcard);

    std::vector<std::string> list(const std::string &dir, bool (*f)(const std::string&));

    bool stringEndsWith(const std::string &s, const std::string &ext);

    bool file_name_filter(const std::string &file_name);

    std::string build_class_path_option();

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_CORE_H
