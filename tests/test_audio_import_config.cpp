#include "rms/audio/audio_import_config.h"

#include <cassert>
#include <iostream>

int main() {
    rms::audio::AudioImportConfig config;
    assert(config.frames_per_waveform_point == 1024);
    assert(config.allow_wav);
    assert(config.is_valid());
    assert(config.validation_error().empty());

    config.frames_per_waveform_point = 0;
    assert(!config.is_valid());
    assert(!config.validation_error().empty());

    config.frames_per_waveform_point = 256;
    config.allow_wav = false;
    assert(config.is_valid());

    std::cout << "AudioImportConfig tests passed.\n";
    return 0;
}
