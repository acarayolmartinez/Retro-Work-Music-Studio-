#include "rms/audio/tempo_estimator.h"

#include "rms/audio/beat_interval_analyzer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace rms {
namespace audio {
namespace {

void validate_range(double minimum_bpm, double maximum_bpm) {
    if (!std::isfinite(minimum_bpm) ||
        !std::isfinite(maximum_bpm) ||
        minimum_bpm <= 0.0 ||
        maximum_bpm < minimum_bpm) {
        throw std::invalid_argument("Invalid BPM range.");
    }
}

double median(std::vector<double> values) {
    if (values.empty()) {
        return 0.0;
    }

    std::sort(values.begin(), values.end());
    const std::size_t middle = values.size() / 2;
    if (values.size() % 2 == 0) {
        return (values[middle - 1] + values[middle]) / 2.0;
    }
    return values[middle];
}

double choose_tempo(double base_bpm,
                    double minimum_bpm,
                    double maximum_bpm) {
    if (base_bpm >= minimum_bpm && base_bpm <= maximum_bpm) {
        return base_bpm;
    }

    const double half_time = base_bpm / 2.0;
    if (half_time >= minimum_bpm && half_time <= maximum_bpm) {
        return half_time;
    }

    const double double_time = base_bpm * 2.0;
    if (double_time >= minimum_bpm && double_time <= maximum_bpm) {
        return double_time;
    }

    return 0.0;
}

} // namespace

TempoEstimate TempoEstimator::estimate(
    const std::vector<Transient>& transients,
    std::uint32_t sample_rate,
    double minimum_bpm,
    double maximum_bpm) {
    validate_range(minimum_bpm, maximum_bpm);

    const std::vector<double> intervals =
        BeatIntervalAnalyzer::intervals_seconds(transients, sample_rate);

    if (intervals.empty()) {
        return TempoEstimate();
    }

    const double typical_interval = median(intervals);
    if (typical_interval <= 0.0) {
        return TempoEstimate();
    }

    const double base_bpm = 60.0 / typical_interval;
    const double bpm = choose_tempo(
        base_bpm, minimum_bpm, maximum_bpm);

    if (bpm == 0.0) {
        return TempoEstimate();
    }

    std::vector<double> deviations;
    deviations.reserve(intervals.size());
    for (std::size_t i = 0; i < intervals.size(); ++i) {
        deviations.push_back(std::fabs(intervals[i] - typical_interval));
    }

    const double relative_deviation =
        median(deviations) / typical_interval;
    double confidence = 1.0 - (1.4826 * relative_deviation);
    confidence = std::max(0.0, std::min(1.0, confidence));

    return TempoEstimate(bpm, confidence);
}

double TempoEstimator::estimate_bpm(
    const std::vector<Transient>& transients,
    std::uint32_t sample_rate,
    double minimum_bpm,
    double maximum_bpm) {
    return estimate(
        transients, sample_rate, minimum_bpm, maximum_bpm).bpm;
}

} // namespace audio
} // namespace rms
