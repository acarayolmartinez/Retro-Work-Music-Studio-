#include "rms/audio/audio_import_history.h"

#include <cassert>
#include <iostream>
#include <vector>

int main() {
    rms::audio::AudioImportHistory history;
    assert(history.empty());
    assert(history.size() == 0);
    assert(history.latest() == 0);

    rms::audio::AudioImportReport first;
    first.file_name = "first.wav";
    first.success = true;
    history.add(first);

    rms::audio::AudioImportReport second;
    second.file_name = "broken.mp3";
    second.error_message = "Unsupported format";
    history.add(second);

    assert(history.size() == 2);
    assert(!history.empty());
    assert(history.entries()[0].file_name == "first.wav");
    assert(history.entries()[0].success);
    assert(history.latest() != 0);
    assert(history.latest()->file_name == "broken.mp3");
    assert(!history.latest()->success);
    assert(history.latest()->error_message == "Unsupported format");
    assert(history.successful_count() == 1);
    assert(history.failed_count() == 1);
    const std::vector<rms::audio::AudioImportReport> latest = history.recent(1);
    assert(latest.size() == 1);
    assert(latest[0].file_name == "broken.mp3");
    assert(history.recent(0).empty());
    assert(history.recent(10).size() == 2);
    assert(history.find_by_file_name("FIRST").size() == 1);
    assert(history.find_by_file_name(".mp3").size() == 1);
    assert(history.find_by_file_name("").empty());

    history.clear();
    assert(history.empty());
    assert(history.latest() == 0);

    std::cout << "AudioImportHistory tests passed.\n";
    return 0;
}
