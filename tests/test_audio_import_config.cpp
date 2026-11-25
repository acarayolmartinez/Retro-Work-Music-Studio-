#include "rms/audio/audio_import_config.h"

#include <cassert>
#include <iostream>

int main() {
    rms::audio::AudioImportConfig config;
    assert(config.frames_per_waveform_point == 1024);
    assert(config.allow_wav);
    assert(config.allow_wave);
    assert(!config.allow_unknown_extensions);
    assert(config.allowed_extensions().size() == 2);
    assert(config.allows_path("track.wav"));
    assert(config.allows_path("track.wave"));
    assert(!config.allows_path("track.flac"));
    assert(config.is_valid());
    assert(config.validation_error().empty());

    config.frames_per_waveform_point = 0;
    assert(!config.is_valid());
    assert(!config.validation_error().empty());

    config.frames_per_waveform_point = 256;
    config.allow_wav = false;
    config.allow_wave = false;
    assert(config.is_valid());
    assert(!config.allows_path("track.wav"));
    assert(!config.allows_path("track.wave"));
    config.allow_unknown_extensions = true;
    assert(config.allows_path("track.flac"));
    assert(config.allowed_extensions().empty());

    std::cout << "AudioImportConfig tests passed.\n";
    return 0;
}
