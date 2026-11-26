#include "rms/audio/audio_import_history_csv_exporter.h"

#include <fstream>
#include <iomanip>

namespace rms {
namespace audio {
namespace {

void set_error(std::string* error, const std::string& message) {
    if (error != 0) *error = message;
}

std::string csv_field(const std::string& value) {
    if (value.find_first_of(",\"\r\n") == std::string::npos)
        return value;
    std::string escaped("\"");
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (*it == '\"') escaped += "\"\\"";
        else escaped += *it;
    }
    escaped += '\"';
    return escaped;
}

bool write_reports(const std::vector<AudioImportReport>& reports,
                   const std::string& path,
                   std::string* error) {
    if (path.empty()) {
        set_error(error, "CSV export path is empty.");
        return false;
    }
    std::ofstream output(path.c_str(), std::ios::out | std::ios::trunc);
    if (!output) {
        set_error(error, "Unable to open CSV export file for writing.");
        return false;
    }

    output << "timestamp,success,file_name,file_path,file_extension,"
              "detected_format,duration_seconds,sample_rate,channel_count,error_message\r\n";
    for (std::vector<AudioImportReport>::const_iterator it = reports.begin();
         it != reports.end(); ++it) {
        output << static_cast<long long>(it->timestamp) << ','
               << (it->success ? "true" : "false") << ','
               << csv_field(it->file_name) << ','
               << csv_field(it->file_path) << ','
               << csv_field(it->file_extension) << ','
               << csv_field(it->detected_format) << ','
               << std::setprecision(17) << it->duration_seconds << ','
               << it->sample_rate << ',' << it->channel_count << ','
               << csv_field(it->error_message) << "\r\n";
        if (!output) {
            set_error(error, "Failed while writing CSV export.");
            return false;
        }
    }
    output.close();
    if (!output) {
        set_error(error, "Failed to finish CSV export.");
        return false;
    }
    if (error != 0) error->clear();
    return true;
}

} // namespace

bool AudioImportHistoryCsvExporter::save(const AudioImportHistory& history,
                                         const std::string& path,
                                         std::string* error) {
    return write_reports(history.entries(), path, error);
}

bool AudioImportHistoryCsvExporter::save(
    const std::vector<AudioImportReport>& reports,
    const std::string& path,
    std::string* error) {
    return write_reports(reports, path, error);
}

} // namespace audio
} // namespace rms
