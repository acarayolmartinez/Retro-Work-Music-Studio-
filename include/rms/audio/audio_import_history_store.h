#ifndef RMS_AUDIO_AUDIO_IMPORT_HISTORY_STORE_H
#define RMS_AUDIO_AUDIO_IMPORT_HISTORY_STORE_H

#include "rms/audio/audio_import_history.h"

#include <string>

namespace rms {
namespace audio {

// Persists an import history in a versioned, validated text format.
class AudioImportHistoryStore {
public:
    static bool save(const AudioImportHistory& history,
                     const std::string& path,
                     std::string* error = 0);

    // Replaces destination only after the complete file has been validated.
    static bool load(const std::string& path,
                     AudioImportHistory& destination,
                     std::string* error = 0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_HISTORY_STORE_H
