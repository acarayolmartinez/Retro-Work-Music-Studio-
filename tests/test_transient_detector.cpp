#include "rms/audio/transient_detector.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<float> samples = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.8f, 0.8f, 0.8f, 0.8f
    };
    const rms::audio::AudioBuffer buffer(44100, 2, samples);
    const std::vector<rms::audio::Transient> events =
        rms::audio::TransientDetector::detect(buffer, 2, 0.4f, 2);

    if (events.size() != 2 ||
        events[0].frame_index != 2 ||
        events[1].frame_index != 6 ||
        std::fabs(events[0].strength - 1.0f) > 0.0001f ||
        std::fabs(events[1].strength - 0.8f) > 0.0001f) {
        std::cerr << "Transient detection test failed." << std::endl;
        return 1;
    }

    const std::vector<float> silence;
    const rms::audio::AudioBuffer silent_buffer(44100, 1, silence);
    if (!rms::audio::TransientDetector::detect(
            silent_buffer, 2, 0.1f, 0).empty()) {
        std::cerr << "Silence should not produce transients." << std::endl;
        return 1;
    }

    bool rejected_zero_window = false;
    try {
        rms::audio::TransientDetector::detect(buffer, 0, 0.4f, 0);
    } catch (const std::invalid_argument&) {
        rejected_zero_window = true;
    }

    if (!rejected_zero_window) {
        std::cerr << "Zero window was not rejected." << std::endl;
        return 1;
    }

    std::cout << "TransientDetector tests passed." << std::endl;
    return 0;
}
