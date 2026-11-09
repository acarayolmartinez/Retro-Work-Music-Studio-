#include "rms/audio/audio_analyzer.h"

#include <cmath>

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

} // namespace audio
} // namespace rms
