#ifndef RMS_CORE_APPLICATION_H
#define RMS_CORE_APPLICATION_H

#include <string>

namespace rms {
namespace core {

class Application {
public:
    Application();

    int run() const;
    const std::string& name() const;

private:
    std::string name_;
};

} // namespace core
} // namespace rms

#endif // RMS_CORE_APPLICATION_H
