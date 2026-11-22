#include "rms/audio/audio_output.h"
#include "rms/audio/audio_settings.h"

#include <cassert>
#include <iostream>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioOutput;

int main() {
    AudioOutput output;
    assert(!output.is_open());
    assert(!output.is_running());
    assert(output.position_frame() == 0);

    const AudioBuffer empty(44100, 1, std::vector<float>());
    assert(!output.open(empty));
    assert(!output.is_open());
    assert(!output.last_error().empty());

    const AudioBuffer audio(44100, 1, std::vector<float>(441, 0.1f));
    rms::audio::AudioSettings settings;
    settings.sample_rate = 48000;
    assert(!output.open(audio, settings));
    assert(!output.is_open());
    assert(!output.last_error().empty());

    settings.sample_rate = 44100;
    settings.channel_count = 2;
    assert(!output.open(audio, settings));
    assert(!output.is_open());

    settings.channel_count = 1;
    settings.output_device_index = -2;
    assert(!output.open(audio, settings));
    assert(!output.is_open());

    settings.output_device_index = -1;
    settings.frames_per_buffer = 256;
    assert(settings.is_valid());

#ifndef RMS_ENABLE_PORTAUDIO
    assert(!output.open(audio));
    assert(!output.open(audio, 0));
    assert(!output.start());
    assert(!output.is_open());
    assert(!output.last_error().empty());
#endif

    output.seek(12);
    assert(output.position_frame() == 0);

    output.close();
    assert(!output.is_open());
    assert(output.position_frame() == 0);

    std::cout << "AudioOutput tests passed." << std::endl;
    return 0;
}
