#include "rms/audio/audio_settings_manager.h"
#include "rms/audio/audio_settings_store.h"

namespace rms {
namespace audio {
namespace {

void set_error(std::string* error, const std::string& message) {
    if (error != 0) {
        *error = message;
    }
}

} // namespace

AudioSettingsManager::AudioSettingsManager()
    : settings_() {
}

const AudioSettings& AudioSettingsManager::settings() const {
    return settings_;
}

bool AudioSettingsManager::set_settings(const AudioSettings& settings,
                                        std::string* error) {
    if (!settings.is_valid()) {
        set_error(error, settings.validation_error());
        return false;
    }
    settings_ = settings;
    set_error(error, std::string());
    return true;
}

void AudioSettingsManager::reset_defaults() {
    settings_ = AudioSettings();
}

bool AudioSettingsManager::save(const std::string& path,
                                std::string* error) const {
    return AudioSettingsStore::save(path, settings_, error);
}

bool AudioSettingsManager::load(const std::string& path,
                                std::string* error) {
    AudioSettings loaded;
    if (!AudioSettingsStore::load(path, loaded, error)) {
        return false;
    }
    return set_settings(loaded, error);
}

} // namespace audio
} // namespace rms
