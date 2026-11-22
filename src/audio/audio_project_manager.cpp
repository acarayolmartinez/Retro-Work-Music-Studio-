#include "rms/audio/audio_project_manager.h"

#include "rms/audio/audio_project_file.h"

#include <stdexcept>

namespace rms {
namespace audio {

AudioProjectManager::AudioProjectManager(
    std::size_t frames_per_waveform_point)
    : project_(),
      frames_per_waveform_point_(frames_per_waveform_point),
      file_path_(),
      modified_(false) {
    if (frames_per_waveform_point_ == 0) {
        throw std::invalid_argument(
            "Waveform point size must be greater than zero.");
    }
}

bool AudioProjectManager::new_project() {
    project_.clear_audio();
    file_path_.clear();
    modified_ = false;
    return true;
}

bool AudioProjectManager::open_project(const std::string& path,
                                       std::string* error) {
    if (!AudioProjectFile::load(path, project_,
                                frames_per_waveform_point_, error)) {
        return false;
    }

    file_path_ = path;
    modified_ = false;
    return true;
}

bool AudioProjectManager::save_project(std::string* error) {
    if (file_path_.empty()) {
        if (error != 0) {
            *error = "Project has no file path; use save_project_as.";
        }
        return false;
    }

    if (!AudioProjectFile::save(file_path_, project_, error)) {
        return false;
    }

    modified_ = false;
    return true;
}

bool AudioProjectManager::save_project_as(const std::string& path,
                                          std::string* error) {
    if (path.empty()) {
        if (error != 0) {
            *error = "Project file path is empty.";
        }
        return false;
    }

    if (!AudioProjectFile::save(path, project_, error)) {
        return false;
    }

    file_path_ = path;
    modified_ = false;
    return true;
}

void AudioProjectManager::mark_modified() {
    modified_ = true;
}

bool AudioProjectManager::is_modified() const {
    return modified_;
}

bool AudioProjectManager::has_file_path() const {
    return !file_path_.empty();
}

const std::string& AudioProjectManager::file_path() const {
    return file_path_;
}

AudioProject& AudioProjectManager::project() {
    return project_;
}

const AudioProject& AudioProjectManager::project() const {
    return project_;
}

} // namespace audio
} // namespace rms
