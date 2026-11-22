#include "rms/audio/audio_settings.h"

namespace rms {
namespace audio {

AudioSettings::AudioSettings()
    : output_device_index(-1),
      sample_rate(44100),
      channel_count(2),
      frames_per_buffer(0) {
}

bool AudioSettings::is_valid() const {
    return output_device_index >= -1 &&
           sample_rate > 0 &&
           channel_count > 0;
}

std::string AudioSettings::validation_error() const {
    if (output_device_index < -1) {
        return "Output device index must be -1 or a non-negative index.";
    }
    if (sample_rate == 0) {
        return "Sample rate must be greater than zero.";
    }
    if (channel_count == 0) {
        return "Channel count must be greater than zero.";
    }
    return std::string();
}

} // namespace audio
} // namespace rms
