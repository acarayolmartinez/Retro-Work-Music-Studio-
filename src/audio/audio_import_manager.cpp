#include "rms/audio/audio_import_manager.h"

#include "rms/audio/audio_file_reader.h"

#include <exception>

namespace rms {
namespace audio {

AudioImportManager::AudioImportManager(AudioProjectManager& project_manager)
    : project_manager_(project_manager) {
}

bool AudioImportManager::import_wav(const std::string& path,
                                    std::string* error) {
    try {
        // Decode first: a read failure must not modify the current project.
        const AudioBuffer buffer = AudioFileReader::read_wav(path);
        project_manager_.project().load_audio(
            buffer, project_manager_.frames_per_waveform_point());
        project_manager_.mark_modified();
        if (error != 0) {
            error->clear();
        }
        return true;
    } catch (const std::exception& exception) {
        if (error != 0) {
            *error = exception.what();
        }
        return false;
    }
}

} // namespace audio
} // namespace rms
