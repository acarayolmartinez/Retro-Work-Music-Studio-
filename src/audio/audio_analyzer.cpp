#include "rms/audio/audio_analyzer.h"

#include <cmath>
#include <stdexcept>

namespace rms {
namespace audio {

AudioMetrics::AudioMetrics()
    : peak_amplitude(0.0f),
      rms_amplitude(0.0f) {
}

AudioMetrics AudioAnalyzer::analyze(const AudioBuffer& buffer) {
    AudioMetrics metrics;
    const std::vector<float>& samples = buffer.samples();

    if (samples.empty()) {
        return metrics;
    }

    double sum_of_squares = 0.0;
    float peak = 0.0f;

    for (std::size_t i = 0; i < samples.size(); ++i) {
        const float magnitude = std::fabs(samples[i]);

        if (magnitude > peak) {
            peak = magnitude;
        }

        sum_of_squares += static_cast<double>(samples[i]) *
                          static_cast<double>(samples[i]);
    }

    metrics.peak_amplitude = peak;
    metrics.rms_amplitude = static_cast<float>(
        std::sqrt(sum_of_squares / static_cast<double>(samples.size())));

    return metrics;
}

std::vector<float> AudioAnalyzer::rms_envelope(
    const AudioBuffer& buffer,
    std::size_t window_size_frames) {
    if (window_size_frames == 0) {
        throw std::invalid_argument("Window size must be greater than zero.");
    }

    std::vector<float> envelope;
    const std::vector<float>& samples = buffer.samples();
    const std::size_t channels = buffer.channel_count();
    const std::size_t window_size_samples = window_size_frames * channels;

    for (std::size_t begin = 0; begin < samples.size();
         begin += window_size_samples) {
        const std::size_t end =
            begin + window_size_samples < samples.size()
                ? begin + window_size_samples
                : samples.size();

        double sum_of_squares = 0.0;
        for (std::size_t i = begin; i < end; ++i) {
            const double sample = static_cast<double>(samples[i]);
            sum_of_squares += sample * sample;
        }

        const std::size_t count = end - begin;
        envelope.push_back(static_cast<float>(
            std::sqrt(sum_of_squares / static_cast<double>(count))));
    }

    return envelope;
}

} // namespace audio
} // namespace rms
