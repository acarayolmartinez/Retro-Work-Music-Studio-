#include "rms/audio/audio_analyzer.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<float> samples = {
        1.0f, -1.0f, 0.5f, -0.5f
    };
    const rms::audio::AudioBuffer buffer(44100, 2, samples);
    const rms::audio::AudioMetrics metrics =
        rms::audio::AudioAnalyzer::analyze(buffer);

    const float expected_rms = std::sqrt(0.625f);

    if (std::fabs(metrics.peak_amplitude - 1.0f) > 0.0001f) {
        std::cerr << "Peak amplitude test failed." << std::endl;
        return 1;
    }

    if (std::fabs(metrics.rms_amplitude - expected_rms) > 0.0001f) {
        std::cerr << "RMS amplitude test failed." << std::endl;
        return 1;
    }

    const std::vector<float> silence;
    const rms::audio::AudioBuffer silent_buffer(44100, 1, silence);
    const rms::audio::AudioMetrics silent_metrics =
        rms::audio::AudioAnalyzer::analyze(silent_buffer);

    if (silent_metrics.peak_amplitude != 0.0f ||
        silent_metrics.rms_amplitude != 0.0f) {
        std::cerr << "Silence analysis test failed." << std::endl;
        return 1;
    }

    const std::vector<float> envelope_samples = {
        1.0f, -1.0f, 1.0f, -1.0f,
        0.5f, -0.5f
    };
    const rms::audio::AudioBuffer envelope_buffer(
        44100, 2, envelope_samples);
    const std::vector<float> envelope =
        rms::audio::AudioAnalyzer::rms_envelope(envelope_buffer, 2);

    if (envelope.size() != 2 ||
        std::fabs(envelope[0] - 1.0f) > 0.0001f ||
        std::fabs(envelope[1] - 0.5f) > 0.0001f) {
        std::cerr << "RMS envelope test failed." << std::endl;
        return 1;
    }

    bool rejected_zero_window = false;
    try {
        rms::audio::AudioAnalyzer::rms_envelope(buffer, 0);
    } catch (const std::invalid_argument&) {
        rejected_zero_window = true;
    }

    if (!rejected_zero_window) {
        std::cerr << "Zero-sized RMS window was not rejected." << std::endl;
        return 1;
    }

    std::cout << "AudioAnalyzer tests passed." << std::endl;
    return 0;
}
