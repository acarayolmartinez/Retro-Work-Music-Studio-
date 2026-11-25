#ifndef RMS_AUDIO_AUDIO_SETTINGS_MANAGER_H
#define RMS_AUDIO_AUDIO_SETTINGS_MANAGER_H

#include "rms/audio/audio_settings.h"

#include <string>

namespace rms {
namespace audio {

class AudioImportManager;

// Owns the active audio preferences and coordinates persistence.
class AudioSettingsManager {
public:
    AudioSettingsManager();

    const AudioSettings& settings() const;
    bool set_settings(const AudioSettings& settings,
                      std::string* error = 0);
    void reset_defaults();

    bool configure_import_history(AudioImportManager& import_manager,
                                  std::string* error = 0) const;

    bool save(const std::string& path, std::string* error = 0) const;
    bool load(const std::string& path, std::string* error = 0);

private:
    AudioSettings settings_;
};

} // namespace audio
} // namespace rms

#endif
