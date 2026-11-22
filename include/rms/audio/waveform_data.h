#ifndef RMS_AUDIO_WAVEFORM_DATA_H
#define RMS_AUDIO_WAVEFORM_DATA_H

#include "rms/audio/audio_buffer.h"

#include <cstddef>
#include <vector>

namespace rms {
namespace audio {

// Peak range for one display segment of the source audio.
struct WaveformPoint {
    std::size_t start_frame;
    float minimum;
    float maximum;

    WaveformPoint(std::size_t start, float min_value, float max_value);
};

class WaveformData {
public:
    // Builds a compact waveform. Each point covers up to frames_per_point
    // source frames; the final point may cover fewer frames.
    // Multichannel audio is averaged to mono before min/max are measured.
    static std::vector<WaveformPoint> generate(
        const AudioBuffer& buffer,
        std::size_t frames_per_point);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_WAVEFORM_DATA_H
