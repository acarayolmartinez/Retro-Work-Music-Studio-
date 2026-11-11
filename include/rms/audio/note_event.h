#ifndef RMS_AUDIO_NOTE_EVENT_H
#define RMS_AUDIO_NOTE_EVENT_H

#include <cstddef>
#include <vector>

#include "rms/audio/melody_tracker.h"

namespace rms {
namespace audio {

struct NoteEvent {
    std::size_t start_frame;
    std::size_t duration_frames;
    int midi_note;
    int velocity;
    double confidence;

    NoteEvent(std::size_t start,
              std::size_t duration,
              int midi,
              int note_velocity = 100,
              double certainty = 0.0);
};

class NoteEventConverter {
public:
    // Converts tracked melody notes to export-ready events. Velocity is
    // supplied separately because pitch confidence is not note loudness.
    static std::vector<NoteEvent> convert(
        const std::vector<MelodyNote>& notes,
        int velocity = 100);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_NOTE_EVENT_H
