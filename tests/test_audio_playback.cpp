#include "rms/audio/audio_playback.h"
#include "rms/audio/audio_settings.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioPlayback;
using rms::audio::PlaybackState;

int main() {
    AudioPlayback playback;
    assert(!playback.has_audio());
    assert(playback.state() == PlaybackState::Stopped);
    assert(playback.duration_frames() == 0);
    assert(playback.position_seconds() == 0.0);

    const AudioBuffer buffer(100, 1, std::vector<float>(100, 0.25f));
    playback.load(buffer, false);
    assert(playback.has_audio());
    assert(!playback.has_output());
    assert(playback.duration_frames() == 100);
    playback.play();
    assert(playback.state() == PlaybackState::Playing);

    playback.advance(25);
    assert(playback.position_frame() == 25);
    assert(std::fabs(playback.position_seconds() - 0.25) < 0.000001);

    playback.pause();
    assert(playback.state() == PlaybackState::Paused);
    playback.advance(10);
    assert(playback.position_frame() == 25);

    playback.play();
    playback.seek(50);
    assert(playback.position_frame() == 50);
    assert(playback.state() == PlaybackState::Playing);

    playback.advance(100);
    assert(playback.position_frame() == 100);
    assert(playback.state() == PlaybackState::Stopped);

    playback.play();
    assert(playback.position_frame() == 0);
    assert(playback.state() == PlaybackState::Playing);
    playback.stop();
    assert(playback.position_frame() == 0);
    assert(playback.state() == PlaybackState::Stopped);

    playback.seek(200);
    assert(playback.position_frame() == 100);
    assert(playback.state() == PlaybackState::Stopped);

    playback.unload();
    assert(!playback.has_audio());
    assert(playback.position_frame() == 0);
    assert(playback.state() == PlaybackState::Stopped);

    rms::audio::AudioSettings settings;
    settings.sample_rate = 200;
    playback.load(buffer, settings, true);
    assert(playback.has_audio());
    assert(!playback.has_output());
    assert(!playback.output_error().empty());
    assert(playback.state() == PlaybackState::Stopped);

    settings.sample_rate = 100;
    settings.channel_count = 2;
    playback.load(buffer, settings, true);
    assert(!playback.has_output());
    assert(!playback.output_error().empty());

    settings.channel_count = 1;
    settings.output_device_index = -2;
    playback.load(buffer, settings, true);
    assert(!playback.has_output());
    assert(!playback.output_error().empty());

    const AudioBuffer empty_buffer(44100, 1, std::vector<float>());
    playback.load(empty_buffer, false);
    playback.play();
    assert(playback.state() == PlaybackState::Stopped);
    assert(playback.position_frame() == 0);

    std::cout << "AudioPlayback tests passed." << std::endl;
    return 0;
}
