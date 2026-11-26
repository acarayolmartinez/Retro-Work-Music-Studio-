#ifndef RMS_AUDIO_AUDIO_IMPORT_MANAGER_H
#define RMS_AUDIO_AUDIO_IMPORT_MANAGER_H

#include "rms/audio/audio_import_config.h"
#include "rms/audio/audio_import_history.h"
#include "rms/audio/audio_import_report.h"
#include "rms/audio/audio_project_manager.h"
#include "rms/audio/audio_import_history_csv_exporter.h"

#include <functional>
#include <string>
#include <vector>

namespace rms {
namespace audio {

struct AudioImportBatchOptions {
    // Zero means no explicit file-count limit.
    std::size_t max_files;
    bool stop_on_error;
    // The caller may set this flag from a progress callback to cancel.
    bool* cancel_requested;
    // Called after each processed file with (processed, pending).
    std::function<void(std::size_t, std::size_t)> progress_callback;

    AudioImportBatchOptions()
        : max_files(0), stop_on_error(false), cancel_requested(0),
          progress_callback() {}
};

struct AudioImportBatchReport {
    std::vector<AudioImportReport> reports;
    std::size_t successful_count;
    std::size_t failed_count;
    std::size_t pending_count;
    bool cancelled;
    bool stopped_on_error;
    bool limit_reached;

    AudioImportBatchReport()
        : reports(), successful_count(0), failed_count(0),
          pending_count(0), cancelled(false), stopped_on_error(false),
          limit_reached(false) {}

    std::size_t total_count() const { return reports.size(); }
    std::size_t processed_count() const { return reports.size(); }
    bool all_succeeded() const {
        return failed_count == 0 && pending_count == 0;
    }
};

class AudioImportManager {
public:
    explicit AudioImportManager(AudioProjectManager& project_manager);

    const AudioImportHistory& history() const;
    std::size_t successful_import_count() const;
    std::size_t failed_import_count() const;
    std::vector<AudioImportReport> recent_imports(std::size_t limit) const;
    std::vector<AudioImportReport> find_imports(
        const std::string& file_name_query) const;
    // Persists an empty history when a store is configured. On failure,
    // the previous in-memory history is restored.
    bool clear_history(std::string* error = 0);
    bool export_history_csv(const std::string& path,
                            std::string* error = 0) const;
    bool export_imports_csv(const std::vector<AudioImportReport>& reports,
                            const std::string& path,
                            std::string* error = 0) const;
    bool export_filtered_history_csv(
        const std::string& path,
        AudioImportHistoryCsvFilter filter,
        AudioImportHistoryCsvOrder order =
            AudioImportHistoryCsvOrder::OldestFirst,
        std::string* error = 0) const;
    bool export_search_results_csv(
        const std::string& query,
        const std::string& path,
        AudioImportHistoryCsvOrder order =
            AudioImportHistoryCsvOrder::OldestFirst,
        std::string* error = 0) const;

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

    AudioImportBatchReport import_files(
        const std::vector<std::string>& paths);
    AudioImportBatchReport import_files(
        const std::vector<std::string>& paths,
        const AudioImportConfig& config);
    AudioImportBatchReport import_files(
        const std::vector<std::string>& paths,
        const AudioImportBatchOptions& options);
    AudioImportBatchReport import_files(
        const std::vector<std::string>& paths,
        const AudioImportConfig& config,
        const AudioImportBatchOptions& options);

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
