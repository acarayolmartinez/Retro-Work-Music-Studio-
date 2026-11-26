#include "rms/audio/audio_import_manager.h"

#include "rms/audio/audio_file_reader.h"
#include "rms/audio/audio_import_format.h"
#include "rms/audio/audio_import_history_store.h"

#include <exception>
#include <memory>

namespace rms {
namespace audio {
namespace {

std::string file_name(const std::string& path) {
    const std::string::size_type slash = path.find_last_of("/\\");
    return slash == std::string::npos ? path : path.substr(slash + 1);
}

std::string format_name(AudioImportFormat format) {
    return format == AudioImportFormat::Wav ? "WAV" : "Unknown";
}

void set_error(std::string* error, const std::string& message) {
    if (error != 0) *error = message;
}

} // namespace

AudioImportManager::AudioImportManager(AudioProjectManager& project_manager)
    : project_manager_(project_manager),
      history_(),
      history_store_path_(),
      history_store_error_(),
      history_auto_save_(true) {
}

const AudioImportHistory& AudioImportManager::history() const {
    return history_;
}

std::size_t AudioImportManager::successful_import_count() const {
    return history_.successful_count();
}

std::size_t AudioImportManager::failed_import_count() const {
    return history_.failed_count();
}

std::vector<AudioImportReport> AudioImportManager::recent_imports(
    std::size_t limit) const {
    return history_.recent(limit);
}

std::vector<AudioImportReport> AudioImportManager::find_imports(
    const std::string& file_name_query) const {
    return history_.find_by_file_name(file_name_query);
}

bool AudioImportManager::clear_history(std::string* error) {
    AudioImportHistory previous = history_;
    history_.clear();
    if (!history_store_path_.empty() &&
        !AudioImportHistoryStore::save(history_, history_store_path_, error)) {
        history_ = previous;
        if (error != 0) history_store_error_ = *error;
        return false;
    }
    history_store_error_.clear();
    if (error != 0) error->clear();
    return true;
}

bool AudioImportManager::export_history_csv(
    const std::string& path, std::string* error) const {
    return AudioImportHistoryCsvExporter::save(history_, path, error);
}

bool AudioImportManager::export_imports_csv(
    const std::vector<AudioImportReport>& reports,
    const std::string& path, std::string* error) const {
    return AudioImportHistoryCsvExporter::save(reports, path, error);
}

bool AudioImportManager::export_filtered_history_csv(
    const std::string& path, AudioImportHistoryCsvFilter filter,
    AudioImportHistoryCsvOrder order, std::string* error) const {
    return AudioImportHistoryCsvExporter::save_filtered(
        history_.entries(), path, filter, order, error);
}

bool AudioImportManager::export_search_results_csv(
    const std::string& query, const std::string& path,
    AudioImportHistoryCsvOrder order, std::string* error) const {
    const std::vector<AudioImportReport> matches =
        history_.find_by_file_name(query);
    return AudioImportHistoryCsvExporter::save_filtered(
        matches, path, AudioImportHistoryCsvFilter::All, order, error);
}

bool AudioImportManager::configure_history_store(const std::string& path,
                                                  bool load_existing,
                                                  std::string* error) {
    if (path.empty()) {
        set_error(error, "History store path is empty.");
        return false;
    }

    if (load_existing) {
        AudioImportHistory loaded;
        std::string load_error;
        if (!AudioImportHistoryStore::load(path, loaded, &load_error)) {
            history_store_error_ = load_error;
            set_error(error, load_error);
            return false;
        }
        history_ = loaded;
    }

    history_store_path_ = path;
    history_store_error_.clear();
    if (error != 0) error->clear();
    return true;
}

bool AudioImportManager::save_history(std::string* error) {
    if (history_store_path_.empty()) {
        set_error(error, "History store has not been configured.");
        return false;
    }
    std::string store_error;
    if (!AudioImportHistoryStore::save(history_, history_store_path_,
                                       &store_error)) {
        history_store_error_ = store_error;
        set_error(error, store_error);
        return false;
    }
    history_store_error_.clear();
    if (error != 0) error->clear();
    return true;
}

bool AudioImportManager::load_history(std::string* error) {
    if (history_store_path_.empty()) {
        set_error(error, "History store has not been configured.");
        return false;
    }
    std::string store_error;
    if (!AudioImportHistoryStore::load(history_store_path_, history_,
                                       &store_error)) {
        history_store_error_ = store_error;
        set_error(error, store_error);
        return false;
    }
    history_store_error_.clear();
    if (error != 0) error->clear();
    return true;
}

const std::string& AudioImportManager::history_store_path() const {
    return history_store_path_;
}

const std::string& AudioImportManager::history_store_error() const {
    return history_store_error_;
}

void AudioImportManager::set_history_auto_save(bool enabled) {
    history_auto_save_ = enabled;
}

bool AudioImportManager::history_auto_save() const {
    return history_auto_save_;
}

void AudioImportManager::record_import(const AudioImportReport& report) {
    history_.add(report);
    if (history_auto_save_ && !history_store_path_.empty()) {
        // Persistence failures are reported separately and never change the
        // result of the audio import itself.
        save_history(&history_store_error_);
    }
}

bool AudioImportManager::import_wav(const std::string& path,
                                    std::string* error) {
    AudioImportConfig config;
    config.frames_per_waveform_point =
        project_manager_.frames_per_waveform_point();
    return import_wav(path, config, error);
}

bool AudioImportManager::import_wav(const std::string& path,
                                    const AudioImportConfig& config,
                                    std::string* error) {
    return import_file(path, config, error);
}

AudioImportBatchReport AudioImportManager::import_files(
    const std::vector<std::string>& paths) {
    AudioImportConfig config;
    config.frames_per_waveform_point =
        project_manager_.frames_per_waveform_point();
    return import_files(paths, config);
}

AudioImportBatchReport AudioImportManager::import_files(
    const std::vector<std::string>& paths,
    const AudioImportConfig& config) {
    AudioImportBatchReport batch;
    batch.reports.reserve(paths.size());
    for (std::vector<std::string>::const_iterator it = paths.begin();
         it != paths.end(); ++it) {
        const AudioImportReport report = import_file_with_report(*it, config);
        batch.reports.push_back(report);
        if (report.success) ++batch.successful_count;
        else ++batch.failed_count;
    }
    return batch;
}

AudioImportReport AudioImportManager::import_file_with_report(
    const std::string& path) {
    AudioImportConfig config;
    config.frames_per_waveform_point =
        project_manager_.frames_per_waveform_point();
    return import_file_with_report(path, config);
}

AudioImportReport AudioImportManager::import_file_with_report(
    const std::string& path, const AudioImportConfig& config) {
    AudioImportReport report;
    report.file_path = path;
    report.file_name = file_name(path);
    report.file_extension = AudioImportFormatInfo::extension_from_path(path);

    const AudioImportFormat format = AudioImportFormatInfo::detect(path);
    report.detected_format = format_name(format);

    if (!config.is_valid()) {
        report.error_message = config.validation_error();
        record_import(report);
        return report;
    }
    if (!config.allows_path(path)) {
        report.error_message =
            "The file extension is disabled or unsupported by the import configuration.";
        record_import(report);
        return report;
    }

    try {
        std::unique_ptr<AudioBuffer> buffer;
        if (format == AudioImportFormat::Wav) {
            buffer.reset(new AudioBuffer(AudioFileReader::read_wav(path)));
        } else {
            report.error_message = "The detected audio format is not implemented.";
            record_import(report);
            return report;
        }

        report.sample_rate = buffer->sample_rate();
        report.channel_count = buffer->channel_count();
        report.duration_seconds =
            static_cast<double>(buffer->frame_count()) /
            static_cast<double>(buffer->sample_rate());

        project_manager_.project().load_audio(
            *buffer, config.frames_per_waveform_point);
        project_manager_.mark_modified();
        report.success = true;
        record_import(report);
        return report;
    } catch (const std::exception& exception) {
        report.error_message = exception.what();
        record_import(report);
        return report;
    }
}

bool AudioImportManager::import_file(const std::string& path,
                                     std::string* error) {
    const AudioImportReport report = import_file_with_report(path);
    if (error != 0) *error = report.error_message;
    return report.success;
}

bool AudioImportManager::import_file(const std::string& path,
                                     const AudioImportConfig& config,
                                     std::string* error) {
    const AudioImportReport report = import_file_with_report(path, config);
    if (error != 0) *error = report.error_message;
    return report.success;
}

} // namespace audio
} // namespace rms
