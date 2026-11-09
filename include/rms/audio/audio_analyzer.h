#ifndef RMS_AUDIO_AUDIO_ANALYZER_H
#define RMS_AUDIO_AUDIO_ANALYZER_H

#include "rms/audio/audio_buffer.h"

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
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_ANALYZER_H
