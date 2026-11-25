#include "rms/audio/audio_import_history_store.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    const std::string path = "rms_audio_import_history_test.rmslog";
    std::string error;

    rms::audio::AudioImportHistory original;
    rms::audio::AudioImportReport first;
    first.timestamp = static_cast<std::time_t>(1700000000);
    first.file_path = "/music/Live set|take 1.wav";
    first.file_name = "Live set|take 1.wav";
    first.file_extension = ".wav";
    first.detected_format = "WAV";
    first.success = true;
    first.duration_seconds = 12.345;
    first.sample_rate = 48000;
    first.channel_count = 2;
    original.add(first);

    rms::audio::AudioImportReport second;
    second.timestamp = static_cast<std::time_t>(1700000001);
    second.file_path = "/music/broken.mp3";
    second.file_name = "broken.mp3";
    second.file_extension = ".mp3";
    second.detected_format = "Unknown";
    second.error_message = "Unsupported format: café";
    original.add(second);

    assert(rms::audio::AudioImportHistoryStore::save(original, path, &error));
    assert(error.empty());

    rms::audio::AudioImportHistory restored;
    rms::audio::AudioImportReport sentinel;
    sentinel.file_name = "keep-me.wav";
    restored.add(sentinel);
    assert(rms::audio::AudioImportHistoryStore::load(path, restored, &error));
    assert(error.empty());
    assert(restored.size() == 2);
    assert(restored.entries()[0].timestamp == first.timestamp);
    assert(restored.entries()[0].file_path == first.file_path);
    assert(restored.entries()[0].duration_seconds == first.duration_seconds);
    assert(restored.entries()[0].sample_rate == 48000);
    assert(restored.entries()[0].channel_count == 2);
    assert(restored.entries()[1].error_message == second.error_message);
    assert(!restored.entries()[1].success);

    {
        std::ofstream damaged(path.c_str(), std::ios::out | std::ios::trunc);
        damaged << "RMS_IMPORT_HISTORY_1\nnot-a-valid-record\n";
    }
    const std::size_t previous_size = restored.size();
    assert(!rms::audio::AudioImportHistoryStore::load(path, restored, &error));
    assert(!error.empty());
    assert(restored.size() == previous_size);
    assert(restored.entries()[0].file_name == "Live set|take 1.wav");

    assert(!rms::audio::AudioImportHistoryStore::save(
        original, std::string(), &error));
    assert(!error.empty());

    std::remove(path.c_str());
    std::cout << "AudioImportHistoryStore tests passed.\n";
    return 0;
}
