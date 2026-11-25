#ifndef RMS_AUDIO_AUDIO_IMPORT_HISTORY_H
#define RMS_AUDIO_AUDIO_IMPORT_HISTORY_H

#include "rms/audio/audio_import_report.h"

#include <cstddef>
#include <string>
#include <vector>

namespace rms {
namespace audio {

// In-memory chronological history of audio import attempts.
class AudioImportHistory {
public:
    void add(const AudioImportReport& report);
    const std::vector<AudioImportReport>& entries() const;
    std::size_t size() const;
    bool empty() const;
    std::size_t successful_count() const;
    std::size_t failed_count() const;
    const AudioImportReport* latest() const;
    // Newest first; zero returns an empty result.
    std::vector<AudioImportReport> recent(std::size_t limit) const;
    // Case-insensitive substring search; an empty query returns no matches.
    std::vector<AudioImportReport> find_by_file_name(
        const std::string& query) const;
    void clear();

private:
    std::vector<AudioImportReport> entries_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_HISTORY_H
