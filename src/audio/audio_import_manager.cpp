#include "rms/audio/audio_import_manager.h"

#include "rms/audio/audio_file_reader.h"
#include "rms/audio/audio_import_format.h"

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

} // namespace

AudioImportManager::AudioImportManager(AudioProjectManager& project_manager)
    : project_manager_(project_manager),
      history_() {
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

AudioImportReport AudioImportManager::import_file_with_report(
    const std::string& path) {
    AudioImportConfig config;
    config.frames_per_waveform_point =
        project_manager_.frames_per_waveform_point();
    return import_file_with_report(path, config);
}

const AudioImportHistory& AudioImportManager::history() const {
    return history_;
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
        history_.add(report);
        return report;
    }
    if (!config.allows_path(path)) {
        report.error_message =
            "The file extension is disabled or unsupported by the import configuration.";
        history_.add(report);
        return report;
    }

    try {
        std::unique_ptr<AudioBuffer> buffer;
        if (format == AudioImportFormat::Wav) {
            buffer.reset(new AudioBuffer(AudioFileReader::read_wav(path)));
        } else {
            report.error_message = "The detected audio format is not implemented.";
            history_.add(report);
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
        history_.add(report);
        return report;
    } catch (const std::exception& exception) {
        report.error_message = exception.what();
        history_.add(report);
        return report;
    }
}

bool AudioImportManager::import_file(const std::string& path,
                                     std::string* error) {
    const AudioImportReport report = import_file_with_report(path);
    if (error != 0) {
        *error = report.error_message;
    }
    return report.success;
}

bool AudioImportManager::import_file(const std::string& path,
                                     const AudioImportConfig& config,
                                     std::string* error) {
    const AudioImportReport report = import_file_with_report(path, config);
    if (error != 0) {
        *error = report.error_message;
    }
    return report.success;
}

} // namespace audio
} // namespace rms
