#include "core.h"
#include "MessagingAccessPointImpl.h"

namespace io {
    namespace openmessaging {
        namespace core {
            MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                               const boost::shared_ptr<KeyValue> &properties):
                    _url(url), _properties(properties) {

            }

            MessagingAccessPointImpl::~MessagingAccessPointImpl() {

            }

            boost::shared_ptr<KeyValue> MessagingAccessPointImpl::properties() {
                return _properties;
            }

            std::string MessagingAccessPointImpl::implVersion() {
                return "0.1-alpha";
            }

            boost::shared_ptr<producer::Producer> MessagingAccessPointImpl::createProducer(
                    boost::shared_ptr<KeyValue> properties) {

                if (!isRunning()) {
                    Initialize(NULL);
                }


            }


        }
    }
}
