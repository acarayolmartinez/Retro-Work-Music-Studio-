#include "rms/audio/transient_detector.h"

#include "rms/audio/audio_analyzer.h"

#include <stdexcept>

namespace rms {
namespace audio {

Transient::Transient(std::size_t frame, float value)
    : frame_index(frame),
      strength(value) {
}

std::vector<Transient> TransientDetector::detect(
    const AudioBuffer& buffer,
    std::size_t window_size_frames,
    float threshold,
    std::size_t minimum_gap_frames) {
    if (window_size_frames == 0) {
        throw std::invalid_argument("Window size must be greater than zero.");
    }
    if (threshold < 0.0f) {
        throw std::invalid_argument("Threshold cannot be negative.");
    }

    std::vector<Transient> transients;
    const std::vector<float> envelope =
        AudioAnalyzer::rms_envelope(buffer, window_size_frames);

    bool was_above_threshold = false;
    std::size_t last_detection_frame = 0;
    bool has_detection = false;

    for (std::size_t i = 0; i < envelope.size(); ++i) {
        const bool is_above_threshold = envelope[i] >= threshold;
        const std::size_t frame = i * window_size_frames;

        if (is_above_threshold && !was_above_threshold) {
            const bool outside_gap =
                !has_detection ||
                frame - last_detection_frame >= minimum_gap_frames;

            if (outside_gap) {
                transients.push_back(Transient(frame, envelope[i]));
                last_detection_frame = frame;
                has_detection = true;
            }
        }

        was_above_threshold = is_above_threshold;
    }

    return transients;
}

} // namespace audio
} // namespace rms
