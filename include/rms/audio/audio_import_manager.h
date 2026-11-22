#ifndef RMS_AUDIO_AUDIO_IMPORT_MANAGER_H
#define RMS_AUDIO_AUDIO_IMPORT_MANAGER_H

#include "rms/audio/audio_import_config.h"
#include "rms/audio/audio_project_manager.h"

#include <string>

namespace rms {
namespace audio {

class AudioImportManager {
public:
    explicit AudioImportManager(AudioProjectManager& project_manager);

    bool import_wav(const std::string& path, std::string* error = 0);
    bool import_wav(const std::string& path,
                    const AudioImportConfig& config,
                    std::string* error = 0);

private:
    AudioProjectManager& project_manager_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_MANAGER_H
