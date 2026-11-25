#include "rms/audio/audio_settings_manager.h"
#include "rms/audio/audio_settings_store.h"
#include "rms/audio/audio_import_manager.h"

#include <fstream>

namespace rms {
namespace audio {
namespace {

void set_error(std::string* error, const std::string& message) {
    if (error != 0) *error = message;
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

bool AudioSettingsManager::configure_import_history(
    AudioImportManager& import_manager, std::string* error) const {
    const std::string& path = settings_.import_history_path;
    std::ifstream existing(path.c_str(), std::ios::in | std::ios::binary);
    const bool file_exists = existing.good();
    existing.close();

    if (file_exists) {
        if (!import_manager.configure_history_store(path, true, error)) {
            return false;
        }
    } else {
        if (!import_manager.configure_history_store(path, false, error)) {
            return false;
        }
        if (!import_manager.save_history(error)) {
            return false;
        }
    }

    import_manager.set_history_auto_save(settings_.import_history_auto_save);
    set_error(error, std::string());
    return true;
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
