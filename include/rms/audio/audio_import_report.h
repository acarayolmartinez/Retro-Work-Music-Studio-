#ifndef RMS_AUDIO_AUDIO_IMPORT_REPORT_H
#define RMS_AUDIO_AUDIO_IMPORT_REPORT_H

#include <cstdint>
#include <string>

namespace rms {
namespace audio {

// Summary of one import attempt. Audio properties are zero when no audio was imported.
struct AudioImportReport {
    std::string file_path;
    std::string file_name;
    std::string file_extension;
    std::string detected_format;
    bool success;
    std::string error_message;
    double duration_seconds;
    std::uint32_t sample_rate;
    std::uint16_t channel_count;

    AudioImportReport();
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_REPORT_H
