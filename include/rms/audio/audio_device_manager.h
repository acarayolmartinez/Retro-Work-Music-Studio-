#ifndef RMS_AUDIO_AUDIO_DEVICE_MANAGER_H
#define RMS_AUDIO_AUDIO_DEVICE_MANAGER_H

#include <string>
#include <vector>

namespace rms {
namespace audio {

struct AudioDeviceInfo {
    int index;
    std::string name;
    int maximum_output_channels;
    double default_sample_rate;
    bool is_default_output;

    AudioDeviceInfo();
};

class AudioDeviceManager {
public:
    // False when RMS was built without PortAudio or initialization fails.
    static bool is_available();
    static std::vector<AudioDeviceInfo> output_devices();
    // Returns -1 when no default output device is available.
    static int default_output_device_index();
};

} // namespace audio
} // namespace rms

#endif
