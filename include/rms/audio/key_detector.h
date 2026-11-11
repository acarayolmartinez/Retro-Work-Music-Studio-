#ifndef RMS_AUDIO_KEY_DETECTOR_H
#define RMS_AUDIO_KEY_DETECTOR_H

#include "rms/audio/audio_buffer.h"

#include <array>
#include <string>

namespace rms {
namespace audio {

struct KeyEstimate {
    std::string key;
    double confidence;
    std::array<double, 12> chroma;

    KeyEstimate();
};

class KeyDetector {
public:
    // Estimates the most likely major/minor key from the audio spectrum.
    // Confidence expresses separation from the next-best key candidate.
    static KeyEstimate estimate(const AudioBuffer& buffer);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_KEY_DETECTOR_H
