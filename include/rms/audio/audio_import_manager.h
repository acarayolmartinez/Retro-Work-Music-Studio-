#ifndef RMS_AUDIO_AUDIO_IMPORT_MANAGER_H
#define RMS_AUDIO_AUDIO_IMPORT_MANAGER_H

#include "rms/audio/audio_import_config.h"
#include "rms/audio/audio_import_history.h"
#include "rms/audio/audio_import_report.h"
#include "rms/audio/audio_project_manager.h"

#include <string>

namespace rms {
namespace audio {

class AudioImportManager {
public:
    explicit AudioImportManager(AudioProjectManager& project_manager);

    const AudioImportHistory& history() const;

    // Enables automatic persistence. Existing history is loaded when requested.
    bool configure_history_store(const std::string& path,
                                 bool load_existing = true,
                                 std::string* error = 0);
    bool save_history(std::string* error = 0);
    void set_history_auto_save(bool enabled);
    bool history_auto_save() const;
    bool load_history(std::string* error = 0);
    const std::string& history_store_path() const;
    const std::string& history_store_error() const;

    AudioImportReport import_file_with_report(const std::string& path);
    AudioImportReport import_file_with_report(
        const std::string& path, const AudioImportConfig& config);

    bool import_file(const std::string& path, std::string* error = 0);
    bool import_file(const std::string& path,
                     const AudioImportConfig& config,
                     std::string* error = 0);

    bool import_wav(const std::string& path, std::string* error = 0);
    bool import_wav(const std::string& path,
                    const AudioImportConfig& config,
                    std::string* error = 0);

private:
    void record_import(const AudioImportReport& report);

    AudioProjectManager& project_manager_;
    AudioImportHistory history_;
    std::string history_store_path_;
    std::string history_store_error_;
    bool history_auto_save_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_IMPORT_MANAGER_H
