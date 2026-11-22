#include "rms/audio/audio_playback_controller.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioPlaybackController;
using rms::audio::AudioSettings;
using rms::audio::AudioSettingsManager;
using rms::audio::PlaybackState;

int main() {
    AudioSettingsManager settings;
    AudioSettings active = settings.settings();
    active.sample_rate = 100;
    active.channel_count = 1;
    assert(settings.set_settings(active));

    AudioPlaybackController controller(settings);
    assert(!controller.has_audio());
    assert(controller.state() == PlaybackState::Stopped);
    assert(controller.duration_frames() == 0);

    const AudioBuffer buffer(100, 1, std::vector<float>(100, 0.25f));
    controller.load(buffer, false);
    assert(controller.has_audio());
    assert(!controller.has_output());
    assert(controller.duration_frames() == 100);

    controller.play();
    assert(controller.state() == PlaybackState::Playing);
    controller.advance(25);
    assert(controller.position_frame() == 25);
    assert(std::fabs(controller.position_seconds() - 0.25) < 0.000001);

    controller.pause();
    assert(controller.state() == PlaybackState::Paused);
    controller.advance(10);
    assert(controller.position_frame() == 25);

    controller.play();
    controller.seek(50);
    assert(controller.position_frame() == 50);
    controller.stop();
    assert(controller.position_frame() == 0);
    assert(controller.state() == PlaybackState::Stopped);

    active.sample_rate = 200;
    assert(settings.set_settings(active));
    controller.load(buffer, true);
    assert(controller.has_audio());
    assert(!controller.has_output());
    assert(!controller.output_error().empty());

    controller.unload();
    assert(!controller.has_audio());
    assert(controller.position_frame() == 0);
    assert(controller.state() == PlaybackState::Stopped);

    std::cout << "AudioPlaybackController tests passed." << std::endl;
    return 0;
}
