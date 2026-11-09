#ifndef RMS_AUDIO_TRANSIENT_DETECTOR_H
#define RMS_AUDIO_TRANSIENT_DETECTOR_H

#include "rms/audio/audio_buffer.h"

#include <cstddef>
#include <vector>

namespace rms {
namespace audio {

struct Transient {
    std::size_t frame_index;
    float strength;

    Transient(std::size_t frame, float value);
};

class TransientDetector {
public:
    // Detects rising crossings of the windowed RMS threshold.
    // minimum_gap_frames suppresses detections that are too close together.
    static std::vector<Transient> detect(
        const AudioBuffer& buffer,
        std::size_t window_size_frames,
        float threshold,
        std::size_t minimum_gap_frames);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_TRANSIENT_DETECTOR_H
