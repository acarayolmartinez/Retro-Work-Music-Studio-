#include "rms/audio/audio_import_config.h"

#include "rms/audio/audio_import_format.h"

namespace rms {
namespace audio {

AudioImportConfig::AudioImportConfig()
    : frames_per_waveform_point(1024),
      allow_wav(true),
      allow_wave(true),
      allow_unknown_extensions(false) {
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

bool AudioImportConfig::allows_path(const std::string& path) const {
    const AudioImportFormat format = AudioImportFormatInfo::detect(path);
    if (format == AudioImportFormat::Wav) {
        const std::string ext = AudioImportFormatInfo::extension_from_path(path);
        return ext == ".wave" ? allow_wave : allow_wav;
    }
    return allow_unknown_extensions;
}

std::vector<std::string> AudioImportConfig::allowed_extensions() const {
    std::vector<std::string> extensions;
    if (allow_wav) {
        extensions.push_back(".wav");
    }
    if (allow_wave) {
        extensions.push_back(".wave");
    }
    return extensions;
}

} // namespace audio
} // namespace rms
