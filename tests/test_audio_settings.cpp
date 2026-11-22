#include "rms/audio/audio_settings.h"

#include <cassert>

int main() {
    rms::audio::AudioSettings settings;
    assert(settings.output_device_index == -1);
    assert(settings.sample_rate == 44100);
    assert(settings.channel_count == 2);
    assert(settings.frames_per_buffer == 0);
    assert(settings.is_valid());
    assert(settings.validation_error().empty());

    settings.output_device_index = 2;
    assert(settings.is_valid());

    settings.output_device_index = -2;
    assert(!settings.is_valid());
    assert(!settings.validation_error().empty());

    settings.output_device_index = -1;
    settings.sample_rate = 0;
    assert(!settings.is_valid());

    settings.sample_rate = 48000;
    settings.channel_count = 0;
    assert(!settings.is_valid());

    settings.channel_count = 1;
    settings.frames_per_buffer = 256;
    assert(settings.is_valid());
    return 0;
}
