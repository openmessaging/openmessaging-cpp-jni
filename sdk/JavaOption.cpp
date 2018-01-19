#include "JavaOption.h"

using namespace io::openmessaging;

JavaOption::JavaOption(const jint version) : _version(version) {

}

void JavaOption::addOption(const std::string &option) {
    options.push_back(option);
}