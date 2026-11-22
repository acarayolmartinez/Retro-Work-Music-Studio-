#include "rms/audio/audio_device_manager.h"

#ifdef RMS_ENABLE_PORTAUDIO
#include <portaudio.h>
#endif

namespace rms {
namespace audio {

AudioDeviceInfo::AudioDeviceInfo()
    : index(-1),
      maximum_output_channels(0),
      default_sample_rate(0.0),
      is_default_output(false) {
}

bool AudioDeviceManager::is_available() {
#ifdef RMS_ENABLE_PORTAUDIO
    if (Pa_Initialize() != paNoError) {
        return false;
    }
    Pa_Terminate();
    return true;
#else
    return false;
#endif
}

std::vector<AudioDeviceInfo> AudioDeviceManager::output_devices() {
    std::vector<AudioDeviceInfo> devices;
#ifdef RMS_ENABLE_PORTAUDIO
    if (Pa_Initialize() != paNoError) {
        return devices;
    }

    const PaDeviceIndex default_index = Pa_GetDefaultOutputDevice();
    const PaDeviceIndex count = Pa_GetDeviceCount();
    if (count > 0) {
        for (PaDeviceIndex i = 0; i < count; ++i) {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
            if (info == 0 || info->maxOutputChannels <= 0) {
                continue;
            }

            AudioDeviceInfo device;
            device.index = static_cast<int>(i);
            device.name = info->name != 0 ? info->name : "";
            device.maximum_output_channels = info->maxOutputChannels;
            device.default_sample_rate = info->defaultSampleRate;
            device.is_default_output = i == default_index;
            devices.push_back(device);
        }
    }
    Pa_Terminate();
#endif
    return devices;
}

int AudioDeviceManager::default_output_device_index() {
#ifdef RMS_ENABLE_PORTAUDIO
    if (Pa_Initialize() != paNoError) {
        return -1;
    }
    const PaDeviceIndex index = Pa_GetDefaultOutputDevice();
    Pa_Terminate();
    return index == paNoDevice ? -1 : static_cast<int>(index);
#else
    return -1;
#endif
}

} // namespace audio
} // namespace rms
