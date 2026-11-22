#include "rms/audio/audio_settings_store.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

int main() {
    const std::string path = "rms_audio_settings_store_test.cfg";
    std::string error;
    rms::audio::AudioSettings original;
    original.output_device_index = 3;
    original.sample_rate = 48000;
    original.channel_count = 2;
    original.frames_per_buffer = 512;

    assert(rms::audio::AudioSettingsStore::save(path, original, &error));
    assert(error.empty());

    rms::audio::AudioSettings loaded;
    assert(rms::audio::AudioSettingsStore::load(path, loaded, &error));
    assert(error.empty());
    assert(loaded.output_device_index == 3);
    assert(loaded.sample_rate == 48000);
    assert(loaded.channel_count == 2);
    assert(loaded.frames_per_buffer == 512);

    {
        std::ofstream file(path.c_str(), std::ios::trunc);
        file << "output_device_index=-1\n"
             << "sample_rate=0\n"
             << "channel_count=2\n"
             << "frames_per_buffer=0\n";
    }
    loaded.sample_rate = 22050;
    assert(!rms::audio::AudioSettingsStore::load(path, loaded, &error));
    assert(!error.empty());
    assert(loaded.sample_rate == 22050);

    {
        std::ofstream file(path.c_str(), std::ios::trunc);
        file << "output_device_index=-1\n"
             << "sample_rate=44100\n"
             << "channel_count=2\n";
    }
    assert(!rms::audio::AudioSettingsStore::load(path, loaded, &error));

    rms::audio::AudioSettings invalid;
    invalid.sample_rate = 0;
    assert(!rms::audio::AudioSettingsStore::save(path, invalid, &error));

    std::remove(path.c_str());
    return 0;
}
