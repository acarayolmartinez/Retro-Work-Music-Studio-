#ifndef RMS_AUDIO_AUDIO_IMPORT_HISTORY_CSV_EXPORTER_H
#define RMS_AUDIO_AUDIO_IMPORT_HISTORY_CSV_EXPORTER_H

#include "rms/audio/audio_import_history.h"

#include <string>
#include <vector>

namespace rms {
namespace audio {

// Exports import reports as UTF-8 CSV with RFC 4180-style field escaping.
enum class AudioImportHistoryCsvOrder { OldestFirst, NewestFirst };
enum class AudioImportHistoryCsvFilter { All, Successful, Failed };

class AudioImportHistoryCsvExporter {
public:
    static bool save(const AudioImportHistory& history,
                     const std::string& path,
                     std::string* error = 0);
    static bool save(const std::vector<AudioImportReport>& reports,
                     const std::string& path,
                     std::string* error = 0);
    static bool save_filtered(
        const std::vector<AudioImportReport>& reports,
        const std::string& path,
        AudioImportHistoryCsvFilter filter,
        AudioImportHistoryCsvOrder order = AudioImportHistoryCsvOrder::OldestFirst,
        std::string* error = 0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_HISTORY_CSV_EXPORTER_H
