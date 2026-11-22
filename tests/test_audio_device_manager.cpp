#include "rms/audio/audio_device_manager.h"

#include <cassert>

int main() {
    const bool available = rms::audio::AudioDeviceManager::is_available();
    const std::vector<rms::audio::AudioDeviceInfo> devices =
        rms::audio::AudioDeviceManager::output_devices();
    const int default_index =
        rms::audio::AudioDeviceManager::default_output_device_index();

#ifndef RMS_ENABLE_PORTAUDIO
    assert(!available);
    assert(devices.empty());
    assert(default_index == -1);
#else
    if (!available) {
        assert(devices.empty());
        assert(default_index == -1);
    } else {
        for (std::size_t i = 0; i < devices.size(); ++i) {
            assert(devices[i].index >= 0);
            assert(!devices[i].name.empty());
            assert(devices[i].maximum_output_channels > 0);
            assert(devices[i].default_sample_rate > 0.0);
        }
        if (default_index >= 0) {
            bool found_default = false;
            for (std::size_t i = 0; i < devices.size(); ++i) {
                if (devices[i].index == default_index &&
                    devices[i].is_default_output) {
                    found_default = true;
                }
            }
            assert(found_default);
        }
    }
#endif
    return 0;
}
