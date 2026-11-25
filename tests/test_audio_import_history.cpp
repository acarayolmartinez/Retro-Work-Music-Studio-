#include "rms/audio/audio_import_history.h"

#include <cassert>
#include <iostream>

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

    history.clear();
    assert(history.empty());
    assert(history.latest() == 0);

    std::cout << "AudioImportHistory tests passed.\n";
    return 0;
}
