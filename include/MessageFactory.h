#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "message.h"

namespace io {
namespace openmessaging {

class MessageFactory {
public:
    virtual ~MessageFactory() {
    }

    virtual boost::shared_ptr<ByteMessage> createByteMessageToTopic(std::string &topic, std::vector<char> &body) = 0;

    virtual boost::shared_ptr<ByteMessage> createByteMessageToQueue(std::string &topic, std::vector<char> &body) = 0;
};

}

}

#endif // MESSAGEFACTORY_H
