#include "rms/audio/audio_import_history_csv_exporter.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    rms::audio::AudioImportHistory history;
    rms::audio::AudioImportReport report;
    report.timestamp = static_cast<std::time_t>(123456);
    report.success = false;
    report.file_name = "mix, \"take\".wav";
    report.file_path = "audio/folder,one/mix.wav";
    report.file_extension = ".wav";
    report.detected_format = "WAV";
    report.duration_seconds = 1.25;
    report.sample_rate = 44100;
    report.channel_count = 2;
    report.error_message = "bad header, \"RIFF\"\ntry again";
    history.add(report);

    const std::string path = "rms_import_history_export.csv";
    std::string error;
    assert(rms::audio::AudioImportHistoryCsvExporter::save(
        history, path, &error));
    assert(error.empty());

    std::ifstream input(path.c_str(), std::ios::binary);
    const std::string csv((std::istreambuf_iterator<char>(input)),
                          std::istreambuf_iterator<char>());
    assert(csv.find("timestamp,success,file_name,file_path,file_extension,") == 0);
    assert(csv.find("\"mix, \"\"take\"\".wav\"") != std::string::npos);
    assert(csv.find("\"audio/folder,one/mix.wav\"") != std::string::npos);
    assert(csv.find("\"bad header, \"\"RIFF\"\"\ntry again\"") !=
           std::string::npos);
    assert(csv.find("123456,false,") != std::string::npos);
    std::remove(path.c_str());

    std::vector<rms::audio::AudioImportReport> filtered;
    filtered.push_back(report);
    assert(rms::audio::AudioImportHistoryCsvExporter::save(
        filtered, path, &error));
    std::remove(path.c_str());

    assert(!rms::audio::AudioImportHistoryCsvExporter::save(
        history, "", &error));
    assert(!error.empty());
    assert(!rms::audio::AudioImportHistoryCsvExporter::save(
        history, ".", &error));
    assert(!error.empty());

    std::cout << "AudioImportHistoryCsvExporter tests passed.\n";
    return 0;
}
