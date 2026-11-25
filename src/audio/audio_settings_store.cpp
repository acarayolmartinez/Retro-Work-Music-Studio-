#include "rms/audio/audio_settings_store.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <string>

namespace rms {
namespace audio {
namespace {

bool parse_integer(const std::string& value, long long& result) {
    std::istringstream stream(value);
    stream >> result;
    return !stream.fail() && stream.peek() == std::char_traits<char>::eof();
}

void set_error(std::string* error, const std::string& message) {
    if (error != 0) *error = message;
}

} // namespace

bool AudioSettingsStore::save(const std::string& path,
                              const AudioSettings& settings,
                              std::string* error) {
    if (!settings.is_valid()) {
        set_error(error, settings.validation_error());
        return false;
    }

    std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
    if (!file) {
        set_error(error, "Unable to open audio settings file for writing.");
        return false;
    }

    file << "output_device_index=" << settings.output_device_index << '\n'
         << "sample_rate=" << settings.sample_rate << '\n'
         << "channel_count=" << settings.channel_count << '\n'
         << "frames_per_buffer=" << settings.frames_per_buffer << '\n'
         << "import_history_path=" << settings.import_history_path << '\n'
         << "import_history_auto_save="
         << (settings.import_history_auto_save ? 1 : 0) << '\n';
    if (!file.good()) {
        set_error(error, "Failed while writing audio settings file.");
        return false;
    }
    set_error(error, std::string());
    return true;
}

bool AudioSettingsStore::load(const std::string& path,
                              AudioSettings& settings,
                              std::string* error) {
    std::ifstream file(path.c_str());
    if (!file) {
        set_error(error, "Unable to open audio settings file for reading.");
        return false;
    }

    AudioSettings parsed;
    bool seen_device = false;
    bool seen_rate = false;
    bool seen_channels = false;
    bool seen_frames = false;
    bool seen_history_path = false;
    bool seen_history_auto_save = false;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        const std::string::size_type separator = line.find('=');
        if (separator == std::string::npos) {
            set_error(error, "Malformed audio settings line.");
            return false;
        }

        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);

        if (key == "import_history_path" && !seen_history_path) {
            if (value.empty()) {
                set_error(error, "Import history path must not be empty.");
                return false;
            }
            parsed.import_history_path = value;
            seen_history_path = true;
            continue;
        }
        if (key == "import_history_auto_save" && !seen_history_auto_save) {
            if (value != "0" && value != "1") {
                set_error(error, "Invalid import history auto-save setting.");
                return false;
            }
            parsed.import_history_auto_save = value == "1";
            seen_history_auto_save = true;
            continue;
        }

        long long number = 0;
        if (!parse_integer(value, number)) {
            set_error(error, "Invalid integer in audio settings file.");
            return false;
        }

        if (key == "output_device_index" && !seen_device) {
            if (number < -1 || number > std::numeric_limits<int>::max()) {
                set_error(error, "Output device index is out of range.");
                return false;
            }
            parsed.output_device_index = static_cast<int>(number);
            seen_device = true;
        } else if (key == "sample_rate" && !seen_rate) {
            if (number <= 0 ||
                number > std::numeric_limits<std::uint32_t>::max()) {
                set_error(error, "Sample rate is out of range.");
                return false;
            }
            parsed.sample_rate = static_cast<std::uint32_t>(number);
            seen_rate = true;
        } else if (key == "channel_count" && !seen_channels) {
            if (number <= 0 ||
                number > std::numeric_limits<std::uint16_t>::max()) {
                set_error(error, "Channel count is out of range.");
                return false;
            }
            parsed.channel_count = static_cast<std::uint16_t>(number);
            seen_channels = true;
        } else if (key == "frames_per_buffer" && !seen_frames) {
            if (number < 0 ||
                static_cast<unsigned long long>(number) >
                    std::numeric_limits<unsigned long>::max()) {
                set_error(error, "Frames per buffer is out of range.");
                return false;
            }
            parsed.frames_per_buffer = static_cast<unsigned long>(number);
            seen_frames = true;
        } else {
            set_error(error, "Unknown or duplicate audio settings key.");
            return false;
        }
    }

    if (!file.eof() || !seen_device || !seen_rate ||
        !seen_channels || !seen_frames) {
        set_error(error, "Audio settings file is incomplete or unreadable.");
        return false;
    }
    if (!parsed.is_valid()) {
        set_error(error, parsed.validation_error());
        return false;
    }

    settings = parsed;
    set_error(error, std::string());
    return true;
}

} // namespace audio
} // namespace rms
