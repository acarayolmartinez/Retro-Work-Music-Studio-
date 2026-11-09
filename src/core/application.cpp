#include "rms/core/application.h"

#include <iostream>

namespace rms {
namespace core {

Application::Application()
    : name_("RetroWorks Music Studio") {
}

int Application::run() const {
    std::cout << name_ << std::endl;
    std::cout << "RMS core initialized (C++11)." << std::endl;
    return 0;
}

const std::string& Application::name() const {
    return name_;
}

} // namespace core
} // namespace rms
