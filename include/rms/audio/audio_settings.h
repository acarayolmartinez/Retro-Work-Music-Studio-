#ifndef RMS_AUDIO_AUDIO_SETTINGS_H
#define RMS_AUDIO_AUDIO_SETTINGS_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace rms {
namespace audio {

// Runtime audio preferences. A sample rate/channel count must match the
// AudioBuffer until RMS provides an explicit resampling/channel-conversion stage.
struct AudioSettings {
    int output_device_index;
    std::uint32_t sample_rate;
    std::uint16_t channel_count;
    unsigned long frames_per_buffer;

    AudioSettings();

    bool is_valid() const;
    std::string validation_error() const;
};

} // namespace audio
} // namespace rms

#endif
