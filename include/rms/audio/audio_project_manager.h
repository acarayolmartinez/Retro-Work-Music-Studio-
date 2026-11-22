#ifndef RMS_AUDIO_AUDIO_PROJECT_MANAGER_H
#define RMS_AUDIO_AUDIO_PROJECT_MANAGER_H

#include "rms/audio/audio_project.h"

#include <cstddef>
#include <string>

namespace rms {
namespace audio {

// Coordinates project lifecycle and tracks the current file and unsaved changes.
class AudioProjectManager {
public:
    explicit AudioProjectManager(
        std::size_t frames_per_waveform_point = 1024);

    bool new_project();
    bool open_project(const std::string& path, std::string* error = 0);
    bool save_project(std::string* error = 0);
    bool save_project_as(const std::string& path, std::string* error = 0);

    void mark_modified();
    bool is_modified() const;
    bool has_file_path() const;
    const std::string& file_path() const;

    AudioProject& project();
    const AudioProject& project() const;

private:
    AudioProject project_;
    std::size_t frames_per_waveform_point_;
    std::string file_path_;
    bool modified_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PROJECT_MANAGER_H
