#include "rms/audio/note_event.h"

#include <stdexcept>

namespace rms {
namespace audio {

NoteEvent::NoteEvent(std::size_t start,
                     std::size_t duration,
                     int midi,
                     int note_velocity,
                     double certainty)
    : start_frame(start),
      duration_frames(duration),
      midi_note(midi),
      velocity(note_velocity),
      confidence(certainty) {
    if (duration_frames == 0) {
        throw std::invalid_argument("NoteEvent duration must be greater than zero");
    }
    if (midi_note < 0 || midi_note > 127) {
        throw std::invalid_argument("NoteEvent MIDI note must be between 0 and 127");
    }
    if (velocity < 1 || velocity > 127) {
        throw std::invalid_argument("NoteEvent velocity must be between 1 and 127");
    }
    if (confidence < 0.0 || confidence > 1.0) {
        throw std::invalid_argument("NoteEvent confidence must be between zero and one");
    }
}

std::vector<NoteEvent> NoteEventConverter::convert(
    const std::vector<MelodyNote>& notes,
    int velocity) {
    if (velocity < 1 || velocity > 127) {
        throw std::invalid_argument("velocity must be between 1 and 127");
    }

    std::vector<NoteEvent> events;
    events.reserve(notes.size());
    for (std::size_t i = 0; i < notes.size(); ++i) {
        events.push_back(NoteEvent(notes[i].start_frame,
                                   notes[i].duration_frames,
                                   notes[i].midi_note,
                                   velocity,
                                   notes[i].confidence));
    }
    return events;
}

} // namespace audio
} // namespace rms
