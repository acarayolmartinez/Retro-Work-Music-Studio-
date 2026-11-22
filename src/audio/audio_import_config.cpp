#include "rms/audio/audio_import_config.h"

namespace rms {
namespace audio {

AudioImportConfig::AudioImportConfig()
    : frames_per_waveform_point(1024),
      allow_wav(true) {
}

bool AudioImportConfig::is_valid() const {
    return frames_per_waveform_point > 0;
}

std::string AudioImportConfig::validation_error() const {
    if (frames_per_waveform_point == 0) {
        return "Frames per waveform point must be greater than zero.";
    }
    return std::string();
}

} // namespace audio
} // namespace rms
