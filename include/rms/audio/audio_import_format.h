#ifndef RMS_AUDIO_AUDIO_IMPORT_FORMAT_H
#define RMS_AUDIO_AUDIO_IMPORT_FORMAT_H

#include <string>
#include <vector>

namespace rms {
namespace audio {

enum class AudioImportFormat {
    Unknown,
    Wav
};

// Detects supported formats from filename extensions only; it does not inspect file contents.
class AudioImportFormatInfo {
public:
    static AudioImportFormat detect(const std::string& path);
    static bool is_supported(const std::string& path);
    static std::string extension(AudioImportFormat format);
    static std::string extension_from_path(const std::string& path);
    static std::vector<std::string> supported_extensions();
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_FORMAT_H
