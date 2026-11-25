#ifndef RMS_AUDIO_AUDIO_IMPORT_CONFIG_H
#define RMS_AUDIO_AUDIO_IMPORT_CONFIG_H

#include <cstddef>
#include <string>
#include <vector>

namespace rms {
namespace audio {

// Options that control how an audio file is accepted and prepared for import.
struct AudioImportConfig {
    std::size_t frames_per_waveform_point;
    bool allow_wav;
    bool allow_wave;
    bool allow_unknown_extensions;

    AudioImportConfig();

    bool is_valid() const;
    std::string validation_error() const;
    bool allows_path(const std::string& path) const;
    std::vector<std::string> allowed_extensions() const;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_CONFIG_H
