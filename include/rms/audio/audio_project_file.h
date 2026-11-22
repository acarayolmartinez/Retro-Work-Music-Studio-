#ifndef RMS_AUDIO_AUDIO_PROJECT_FILE_H
#define RMS_AUDIO_AUDIO_PROJECT_FILE_H

#include "rms/audio/audio_project.h"

#include <cstddef>
#include <string>

namespace rms {
namespace audio {

// Versioned binary .rms container. Version 1 stores source PCM audio;
// derived analysis and waveform display data are regenerated on load.
class AudioProjectFile {
public:
    static unsigned int current_version();

    static bool save(const std::string& path,
                     const AudioProject& project,
                     std::string* error = 0);

    static bool load(const std::string& path,
                     AudioProject& project,
                     std::size_t frames_per_waveform_point,
                     std::string* error = 0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PROJECT_FILE_H
