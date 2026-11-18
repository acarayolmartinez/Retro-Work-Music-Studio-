#ifndef RMS_AUDIO_MUSICAL_TIMELINE_H
#define RMS_AUDIO_MUSICAL_TIMELINE_H

#include "rms/audio/note_event.h"

#include <cstddef>
#include <vector>

namespace rms {
namespace audio {

class MusicalTimeline {
public:
    // Inserts an event while preserving chronological order. Overlapping
    // events are allowed for polyphonic music.
    void add_event(const NoteEvent& event);

    // Removes the event at the given sorted index. Returns false if invalid.
    bool remove_event(std::size_t index);

    const std::vector<NoteEvent>& events() const;

    // Returns events intersecting the half-open interval [start, end).
    std::vector<NoteEvent> events_between(
        std::size_t start_frame, std::size_t end_frame) const;

    // Returns events intersecting [start_frame, start_frame + duration).
    std::vector<NoteEvent> overlapping_events(
        std::size_t start_frame, std::size_t duration_frames) const;

    // End frame of the latest event, or zero when empty.
    std::size_t duration_frames() const;

    void clear();
    bool empty() const;
    std::size_t size() const;

private:
    std::vector<NoteEvent> events_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_MUSICAL_TIMELINE_H
