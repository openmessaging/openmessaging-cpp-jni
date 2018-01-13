#ifndef OMS_EXCEPTION_H
#define OMS_EXCEPTION_H

#include <exception>
#include <string>

class OMSException : public std::exception {
public:
    OMSException(const std::string &reason = "Unknown") : _reason(reason) {

    }

    const char *what() {
        return _reason.c_str();
    }

private:
    string _reason;
};

#endif //OMS_EXCEPTION_H
