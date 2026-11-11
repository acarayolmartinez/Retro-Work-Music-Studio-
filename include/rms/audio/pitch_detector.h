#ifndef RMS_AUDIO_PITCH_DETECTOR_H
#define RMS_AUDIO_PITCH_DETECTOR_H

#include "rms/audio/audio_buffer.h"

#include <string>

namespace rms {
namespace audio {

struct PitchEstimate {
    double frequency_hz;
    int midi_note;
    std::string note_name;
    double confidence;

    PitchEstimate();
};

class PitchDetector {
public:
    // Estimates the dominant fundamental frequency using autocorrelation.
    // The search range is limited to 50-1000 Hz.
    static PitchEstimate estimate(const AudioBuffer& buffer);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_PITCH_DETECTOR_H
