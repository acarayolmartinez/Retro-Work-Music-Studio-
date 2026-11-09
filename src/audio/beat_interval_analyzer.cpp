#include "rms/audio/beat_interval_analyzer.h"

#include <algorithm>
#include <stdexcept>

namespace rms {
namespace audio {

std::vector<double> BeatIntervalAnalyzer::intervals_seconds(
    const std::vector<Transient>& transients,
    std::uint32_t sample_rate) {
    if (sample_rate == 0) {
        throw std::invalid_argument("Sample rate must be greater than zero.");
    }

    std::vector<double> intervals;
    if (transients.size() < 2) {
        return intervals;
    }

    intervals.reserve(transients.size() - 1);
    for (std::size_t i = 1; i < transients.size(); ++i) {
        if (transients[i].frame_index <= transients[i - 1].frame_index) {
            throw std::invalid_argument(
                "Transient frame indices must be strictly increasing.");
        }

        const std::size_t frame_delta =
            transients[i].frame_index - transients[i - 1].frame_index;
        intervals.push_back(
            static_cast<double>(frame_delta) /
            static_cast<double>(sample_rate));
    }

    return intervals;
}

double BeatIntervalAnalyzer::median_interval_seconds(
    const std::vector<Transient>& transients,
    std::uint32_t sample_rate) {
    std::vector<double> intervals =
        intervals_seconds(transients, sample_rate);

    if (intervals.empty()) {
        return 0.0;
    }

    std::sort(intervals.begin(), intervals.end());
    const std::size_t middle = intervals.size() / 2;

    if (intervals.size() % 2 == 0) {
        return (intervals[middle - 1] + intervals[middle]) / 2.0;
    }

    return intervals[middle];
}

} // namespace audio
} // namespace rms
