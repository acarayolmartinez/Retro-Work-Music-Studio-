#ifndef RMS_AUDIO_AUDIO_SETTINGS_H
#define RMS_AUDIO_AUDIO_SETTINGS_H

#include <cstddef>
#include <cstdint>
#include <string>

namespace rms {
namespace audio {

// Runtime audio and import-history preferences.
struct AudioSettings {
    int output_device_index;
    std::uint32_t sample_rate;
    std::uint16_t channel_count;
    unsigned long frames_per_buffer;
    std::string import_history_path;
    bool import_history_auto_save;

    AudioSettings();

    bool is_valid() const;
    std::string validation_error() const;
};

} // namespace audio
} // namespace rms

#endif
