#include "rms/core/application.h"

#include <iostream>

int main() {
    const rms::core::Application application;
    const std::string expectedName("RetroWorks Music Studio");

    if (application.name() != expectedName) {
        std::cerr << "Application name test failed. Expected: "
                  << expectedName << ", got: " << application.name()
                  << std::endl;
        return 1;
    }

    std::cout << "Application name test passed." << std::endl;
    return 0;
}
