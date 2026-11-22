#include "rms/audio/audio_settings_manager.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

int main() {
    const std::string path = "rms_audio_settings_manager_test.cfg";
    std::string error;
    rms::audio::AudioSettingsManager manager;

    assert(manager.settings().output_device_index == -1);
    assert(manager.settings().sample_rate == 44100);

    rms::audio::AudioSettings custom;
    custom.output_device_index = 2;
    custom.sample_rate = 48000;
    custom.channel_count = 1;
    custom.frames_per_buffer = 256;
    assert(manager.set_settings(custom, &error));
    assert(error.empty());
    assert(manager.settings().sample_rate == 48000);

    rms::audio::AudioSettings invalid = custom;
    invalid.sample_rate = 0;
    assert(!manager.set_settings(invalid, &error));
    assert(!error.empty());
    assert(manager.settings().sample_rate == 48000);

    assert(manager.save(path, &error));
    manager.reset_defaults();
    assert(manager.settings().sample_rate == 44100);
    assert(manager.load(path, &error));
    assert(manager.settings().output_device_index == 2);
    assert(manager.settings().sample_rate == 48000);
    assert(manager.settings().channel_count == 1);
    assert(manager.settings().frames_per_buffer == 256);

    {
        std::ofstream file(path.c_str(), std::ios::trunc);
        file << "bad settings";
    }
    assert(!manager.load(path, &error));
    assert(!error.empty());
    assert(manager.settings().sample_rate == 48000);

    manager.reset_defaults();
    assert(manager.settings().is_valid());
    assert(manager.settings().output_device_index == -1);
    assert(manager.settings().sample_rate == 44100);
    assert(manager.settings().channel_count == 2);
    assert(manager.settings().frames_per_buffer == 0);

    std::remove(path.c_str());
    return 0;
}
