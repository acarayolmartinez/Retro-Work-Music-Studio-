#include "rms/audio/audio_import_history_store.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <vector>

namespace rms {
namespace audio {
namespace {

const char* const kHeader = "RMS_IMPORT_HISTORY_1";

void set_error(std::string* error, const std::string& message) {
    if (error != 0) {
        *error = message;
    }
}

std::string encode(const std::string& value) {
    static const char digits[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(value.size() * 2);
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        const unsigned char byte = static_cast<unsigned char>(*it);
        result += digits[(byte >> 4) & 0x0F];
        result += digits[byte & 0x0F];
    }
    return result;
}

int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool decode(const std::string& value, std::string& result) {
    if (value.size() % 2 != 0) return false;
    std::string decoded;
    decoded.reserve(value.size() / 2);
    for (std::string::size_type i = 0; i < value.size(); i += 2) {
        const int high = hex_value(value[i]);
        const int low = hex_value(value[i + 1]);
        if (high < 0 || low < 0) return false;
        decoded += static_cast<char>((high << 4) | low);
    }
    result.swap(decoded);
    return true;
}

bool split_fields(const std::string& line, std::vector<std::string>& fields) {
    fields.clear();
    std::string::size_type start = 0;
    for (;;) {
        const std::string::size_type separator = line.find('|', start);
        fields.push_back(line.substr(start, separator == std::string::npos
            ? std::string::npos : separator - start));
        if (separator == std::string::npos) break;
        start = separator + 1;
    }
    return fields.size() == 10;
}

bool parse_integer(const std::string& text, long long& value) {
    if (text.empty()) return false;
    errno = 0;
    char* end = 0;
    const long long parsed = std::strtoll(text.c_str(), &end, 10);
    if (errno == ERANGE || end == text.c_str() || *end != '\0') return false;
    value = parsed;
    return true;
}

bool parse_unsigned(const std::string& text, unsigned long long& value) {
    if (text.empty() || text[0] == '-') return false;
    errno = 0;
    char* end = 0;
    const unsigned long long parsed = std::strtoull(text.c_str(), &end, 10);
    if (errno == ERANGE || end == text.c_str() || *end != '\0') return false;
    value = parsed;
    return true;
}

bool parse_double(const std::string& text, double& value) {
    if (text.empty()) return false;
    errno = 0;
    char* end = 0;
    const double parsed = std::strtod(text.c_str(), &end);
    if (errno == ERANGE || end == text.c_str() || *end != '\0' ||
        !std::isfinite(parsed) || parsed < 0.0) return false;
    value = parsed;
    return true;
}

bool parse_report(const std::string& line, AudioImportReport& report) {
    std::vector<std::string> fields;
    if (!split_fields(line, fields)) return false;

    long long timestamp = 0;
    unsigned long long sample_rate = 0;
    unsigned long long channels = 0;
    double duration = 0.0;
    if (!parse_integer(fields[0], timestamp) ||
        (fields[1] != "0" && fields[1] != "1") ||
        !parse_double(fields[2], duration) ||
        !parse_unsigned(fields[3], sample_rate) ||
        !parse_unsigned(fields[4], channels) ||
        sample_rate > std::numeric_limits<std::uint32_t>::max() ||
        channels > std::numeric_limits<std::uint16_t>::max() ||
        timestamp < static_cast<long long>(std::numeric_limits<std::time_t>::min()) ||
        timestamp > static_cast<long long>(std::numeric_limits<std::time_t>::max())) {
        return false;
    }

    AudioImportReport parsed;
    parsed.timestamp = static_cast<std::time_t>(timestamp);
    parsed.success = fields[1] == "1";
    parsed.duration_seconds = duration;
    parsed.sample_rate = static_cast<std::uint32_t>(sample_rate);
    parsed.channel_count = static_cast<std::uint16_t>(channels);
    if (!decode(fields[5], parsed.file_path) ||
        !decode(fields[6], parsed.file_name) ||
        !decode(fields[7], parsed.file_extension) ||
        !decode(fields[8], parsed.detected_format) ||
        !decode(fields[9], parsed.error_message)) {
        return false;
    }
    if (parsed.success && !parsed.error_message.empty()) return false;
    report = parsed;
    return true;
}

} // namespace

bool AudioImportHistoryStore::save(const AudioImportHistory& history,
                                   const std::string& path,
                                   std::string* error) {
    if (path.empty()) {
        set_error(error, "History file path is empty.");
        return false;
    }

    std::ofstream output(path.c_str(), std::ios::out | std::ios::trunc);
    if (!output) {
        set_error(error, "Unable to open history file for writing.");
        return false;
    }

    output << kHeader << '\n';
    for (std::vector<AudioImportReport>::const_iterator it =
             history.entries().begin(); it != history.entries().end(); ++it) {
        output << static_cast<long long>(it->timestamp) << '|'
               << (it->success ? 1 : 0) << '|'
               << std::setprecision(17) << it->duration_seconds << '|'
               << it->sample_rate << '|' << it->channel_count << '|'
               << encode(it->file_path) << '|' << encode(it->file_name) << '|'
               << encode(it->file_extension) << '|'
               << encode(it->detected_format) << '|'
               << encode(it->error_message) << '\n';
        if (!output) {
            set_error(error, "Failed while writing import history.");
            return false;
        }
    }
    output.close();
    if (!output) {
        set_error(error, "Failed to finish writing import history.");
        return false;
    }
    if (error != 0) error->clear();
    return true;
}

bool AudioImportHistoryStore::load(const std::string& path,
                                   AudioImportHistory& destination,
                                   std::string* error) {
    if (path.empty()) {
        set_error(error, "History file path is empty.");
        return false;
    }

    std::ifstream input(path.c_str());
    if (!input) {
        set_error(error, "Unable to open history file for reading.");
        return false;
    }

    std::string line;
    if (!std::getline(input, line) || line != kHeader) {
        set_error(error, "Invalid or unsupported import history header.");
        return false;
    }

    AudioImportHistory loaded;
    std::size_t line_number = 1;
    while (std::getline(input, line)) {
        ++line_number;
        if (line.empty()) {
            set_error(error, "Invalid empty record at line " +
                      std::to_string(line_number) + ".");
            return false;
        }
        AudioImportReport report;
        if (!parse_report(line, report)) {
            set_error(error, "Invalid import history record at line " +
                      std::to_string(line_number) + ".");
            return false;
        }
        loaded.add(report);
    }
    if (input.bad()) {
        set_error(error, "Failed while reading import history.");
        return false;
    }

    destination = loaded;
    if (error != 0) error->clear();
    return true;
}

} // namespace audio
} // namespace rms
