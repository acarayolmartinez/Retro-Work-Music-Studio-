#ifndef RMS_AUDIO_TEMPO_ESTIMATOR_H
#define RMS_AUDIO_TEMPO_ESTIMATOR_H

#include "rms/audio/transient_detector.h"

#include <cstdint>
#include <vector>

namespace rms {
namespace audio {

class TempoEstimator {
public:
    // Estimates BPM from the median interval between consecutive transients.
    // Returns 0.0 when there are too few events or the result is outside
    // the requested BPM range.
    static double estimate_bpm(
        const std::vector<Transient>& transients,
        std::uint32_t sample_rate,
        double minimum_bpm = 40.0,
        double maximum_bpm = 240.0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_TEMPO_ESTIMATOR_H
