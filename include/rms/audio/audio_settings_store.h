#ifndef RMS_AUDIO_AUDIO_SETTINGS_STORE_H
#define RMS_AUDIO_AUDIO_SETTINGS_STORE_H

#include "rms/audio/audio_settings.h"

#include <string>

namespace rms {
namespace audio {

// Reads and writes AudioSettings as a simple, versionable key=value file.
class AudioSettingsStore {
public:
    static bool save(const std::string& path, const AudioSettings& settings,
                     std::string* error = 0);
    static bool load(const std::string& path, AudioSettings& settings,
                     std::string* error = 0);
};

} // namespace audio
} // namespace rms

#endif
