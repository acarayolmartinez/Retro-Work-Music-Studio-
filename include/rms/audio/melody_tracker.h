#ifndef RMS_AUDIO_MELODY_TRACKER_H
#define RMS_AUDIO_MELODY_TRACKER_H

#include "rms/audio/audio_buffer.h"

#include <cstddef>
#include <vector>

namespace rms {
namespace audio {

struct MelodyNote {
    std::size_t start_frame;
    std::size_t duration_frames;
    int midi_note;
    double confidence;

    MelodyNote(std::size_t start = 0,
               std::size_t duration = 0,
               int midi = -1,
               double certainty = 0.0);
};

class MelodyTracker {
public:
    // Tracks notes in consecutive, non-overlapping windows. Adjacent windows
    // with the same MIDI note are merged. A final partial window is analyzed.
    static std::vector<MelodyNote> track(
        const AudioBuffer& buffer,
        std::size_t window_size_frames = 8192,
        double minimum_confidence = 0.2,
        std::size_t minimum_note_duration_frames = 0);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_MELODY_TRACKER_H
