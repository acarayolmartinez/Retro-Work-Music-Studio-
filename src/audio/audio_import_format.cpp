#include "rms/audio/audio_import_format.h"

#include <algorithm>
#include <cctype>

namespace rms {
namespace audio {
namespace {

std::string lowercase(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        });
    return value;
}

} // namespace

AudioImportFormat AudioImportFormatInfo::detect(const std::string& path) {
    const std::string::size_type slash = path.find_last_of("/\\");
    const std::string::size_type dot = path.find_last_of('.');
    const std::string::size_type filename_start =
        slash == std::string::npos ? 0 : slash + 1;
    if (dot == std::string::npos || dot < filename_start ||
        dot == filename_start || dot + 1 >= path.size()) {
        return AudioImportFormat::Unknown;
    }

    const std::string ext = lowercase(path.substr(dot));
    if (ext == ".wav" || ext == ".wave") {
        return AudioImportFormat::Wav;
    }
    return AudioImportFormat::Unknown;
}

std::string AudioImportFormatInfo::extension_from_path(const std::string& path) {
    const std::string::size_type slash = path.find_last_of("/\\");
    const std::string::size_type dot = path.find_last_of('.');
    const std::string::size_type filename_start =
        slash == std::string::npos ? 0 : slash + 1;
    if (dot == std::string::npos || dot <= filename_start || dot + 1 >= path.size()) {
        return std::string();
    }
    return lowercase(path.substr(dot));
}

bool AudioImportFormatInfo::is_supported(const std::string& path) {
    return detect(path) != AudioImportFormat::Unknown;
}

std::string AudioImportFormatInfo::extension(AudioImportFormat format) {
    if (format == AudioImportFormat::Wav) {
        return ".wav";
    }
    return std::string();
}

std::vector<std::string> AudioImportFormatInfo::supported_extensions() {
    std::vector<std::string> extensions;
    extensions.push_back(".wav");
    extensions.push_back(".wave");
    return extensions;
}

} // namespace audio
} // namespace rms
