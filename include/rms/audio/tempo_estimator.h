#ifndef RMS_AUDIO_TEMPO_ESTIMATOR_H
#define RMS_AUDIO_TEMPO_ESTIMATOR_H

#include "rms/audio/transient_detector.h"

#include <cstdint>
#include <vector>

namespace rms {
namespace audio {

struct TempoEstimate {
    double bpm;
    double confidence;

    TempoEstimate(double tempo = 0.0, double certainty = 0.0)
        : bpm(tempo), confidence(certainty) {}
};

class TempoEstimator {
public:
    // Estimates tempo and interval regularity. Confidence is in [0, 1].
    // Half/double-time candidates are considered only when the base tempo
    // falls outside the requested range.
    static TempoEstimate estimate(
        const std::vector<Transient>& transients,
        std::uint32_t sample_rate,
        double minimum_bpm = 40.0,
        double maximum_bpm = 240.0);

    // Compatibility method: returns only the estimated BPM.
    static double estimate_bpm(
        const std::vector<Transient>& transients,
        std::uint32_t sample_rate,
        double minimum_bpm = 40.0,
        double maximum_bpm = 240.0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_TEMPO_ESTIMATOR_H
