#ifndef RMS_AUDIO_BEAT_INTERVAL_ANALYZER_H
#define RMS_AUDIO_BEAT_INTERVAL_ANALYZER_H

#include "rms/audio/transient_detector.h"

#include <cstdint>
#include <vector>

namespace rms {
namespace audio {

class BeatIntervalAnalyzer {
public:
    // Converts consecutive transient positions into seconds.
    // Transients must be ordered by strictly increasing frame index.
    static std::vector<double> intervals_seconds(
        const std::vector<Transient>& transients,
        std::uint32_t sample_rate);

    // Returns the median consecutive interval, or 0.0 when fewer than
    // two transients are supplied.
    static double median_interval_seconds(
        const std::vector<Transient>& transients,
        std::uint32_t sample_rate);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_BEAT_INTERVAL_ANALYZER_H
