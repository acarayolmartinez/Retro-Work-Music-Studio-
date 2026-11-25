#ifndef RMS_AUDIO_AUDIO_IMPORT_HISTORY_H
#define RMS_AUDIO_AUDIO_IMPORT_HISTORY_H

#include "rms/audio/audio_import_report.h"

#include <cstddef>
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
    const AudioImportReport* latest() const;
    void clear();

private:
    std::vector<AudioImportReport> entries_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_HISTORY_H
