#include "rms/audio/tempo_estimator.h"

#include "rms/audio/beat_interval_analyzer.h"

#include <cmath>
#include <stdexcept>

namespace rms {
namespace audio {

double TempoEstimator::estimate_bpm(
    const std::vector<Transient>& transients,
    std::uint32_t sample_rate,
    double minimum_bpm,
    double maximum_bpm) {
    if (!std::isfinite(minimum_bpm) ||
        !std::isfinite(maximum_bpm) ||
        minimum_bpm <= 0.0 ||
        maximum_bpm < minimum_bpm) {
        throw std::invalid_argument("Invalid BPM range.");
    }

    const double interval =
        BeatIntervalAnalyzer::median_interval_seconds(
            transients, sample_rate);

    if (interval <= 0.0) {
        return 0.0;
    }

    const double bpm = 60.0 / interval;
    if (bpm < minimum_bpm || bpm > maximum_bpm) {
        return 0.0;
    }

    return bpm;
}

} // namespace audio
} // namespace rms
