#include "rms/audio/audio_import_manager.h"

#include "rms/audio/audio_file_reader.h"
#include "rms/audio/audio_import_format.h"

#include <exception>
#include <memory>

namespace rms {
namespace audio {

AudioImportManager::AudioImportManager(AudioProjectManager& project_manager)
    : project_manager_(project_manager) {
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

bool AudioImportManager::import_file(const std::string& path,
                                     std::string* error) {
    AudioImportConfig config;
    config.frames_per_waveform_point =
        project_manager_.frames_per_waveform_point();
    return import_file(path, config, error);
}

bool AudioImportManager::import_file(const std::string& path,
                                     const AudioImportConfig& config,
                                     std::string* error) {
    if (!config.is_valid()) {
        if (error != 0) {
            *error = config.validation_error();
        }
        return false;
    }

    const AudioImportFormat format = AudioImportFormatInfo::detect(path);
    if (format == AudioImportFormat::Unknown) {
        if (error != 0) {
            *error = "Unsupported audio file format. Supported extensions: .wav, .wave.";
        }
        return false;
    }
    if (format == AudioImportFormat::Wav && !config.allow_wav) {
        if (error != 0) {
            *error = "WAV import is disabled by the import configuration.";
        }
        return false;
    }

    try {
        std::unique_ptr<AudioBuffer> buffer;
        if (format == AudioImportFormat::Wav) {
            buffer.reset(new AudioBuffer(AudioFileReader::read_wav(path)));
        } else {
            if (error != 0) {
                *error = "The detected audio format is not implemented.";
            }
            return false;
        }

        project_manager_.project().load_audio(
            *buffer, config.frames_per_waveform_point);
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
