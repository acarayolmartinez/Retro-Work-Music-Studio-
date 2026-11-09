#include "rms/audio/audio_buffer.h"

#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<float> stereo_samples = {
        0.1f, -0.1f,
        0.2f, -0.2f,
        0.3f, -0.3f
    };

    const rms::audio::AudioBuffer buffer(44100, 2, stereo_samples);

    if (buffer.sample_rate() != 44100 ||
        buffer.channel_count() != 2 ||
        buffer.frame_count() != 3 ||
        buffer.samples().size() != stereo_samples.size()) {
        std::cerr << "AudioBuffer metadata test failed." << std::endl;
        return 1;
    }

    bool rejected_invalid_layout = false;
    try {
        const std::vector<float> invalid_samples = {0.0f, 0.1f, 0.2f};
        const rms::audio::AudioBuffer invalid_buffer(
            44100, 2, invalid_samples);
        (void)invalid_buffer;
    } catch (const std::invalid_argument&) {
        rejected_invalid_layout = true;
    }

    if (!rejected_invalid_layout) {
        std::cerr << "AudioBuffer accepted an incomplete frame." << std::endl;
        return 1;
    }

    std::cout << "AudioBuffer tests passed." << std::endl;
    return 0;
}
