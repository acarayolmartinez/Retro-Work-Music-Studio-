#ifndef RMS_AUDIO_AUDIO_ANALYZER_H
#define RMS_AUDIO_AUDIO_ANALYZER_H

#include "rms/audio/audio_buffer.h"

#include <vector>

namespace rms {
namespace audio {

struct AudioMetrics {
    float peak_amplitude;
    float rms_amplitude;

    AudioMetrics();
};

class AudioAnalyzer {
public:
    static AudioMetrics analyze(const AudioBuffer& buffer);

    // Returns one RMS value per window. The final window may be shorter.
    static std::vector<float> rms_envelope(
        const AudioBuffer& buffer,
        std::size_t window_size_frames);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_ANALYZER_H
