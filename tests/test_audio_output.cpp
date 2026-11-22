#include "rms/audio/audio_output.h"

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

#ifndef RMS_ENABLE_PORTAUDIO
    const AudioBuffer audio(44100, 1, std::vector<float>(441, 0.1f));
    assert(!output.open(audio));
    assert(!output.start());
    assert(!output.is_open());
    assert(!output.last_error().empty());
#endif

    output.close();
    assert(!output.is_open());
    assert(output.position_frame() == 0);

    std::cout << "AudioOutput tests passed." << std::endl;
    return 0;
}
