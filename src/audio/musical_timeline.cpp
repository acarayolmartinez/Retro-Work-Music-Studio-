#include "rms/audio/musical_timeline.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace rms {
namespace audio {

void MusicalTimeline::add_event(const NoteEvent& event) {
    if (event.duration_frames == 0 ||
        event.duration_frames >
            std::numeric_limits<std::size_t>::max() - event.start_frame) {
        throw std::invalid_argument("Timeline event has an invalid frame range");
    }

    std::vector<NoteEvent>::iterator position = std::upper_bound(
        events_.begin(), events_.end(), event.start_frame,
        [](std::size_t frame, const NoteEvent& existing) {
            return frame < existing.start_frame;
        });
    events_.insert(position, event);
}

bool MusicalTimeline::remove_event(std::size_t index) {
    if (index >= events_.size()) {
        return false;
    }
    events_.erase(events_.begin() + index);
    return true;
}

const std::vector<NoteEvent>& MusicalTimeline::events() const {
    return events_;
}

std::vector<NoteEvent> MusicalTimeline::events_between(
    std::size_t start_frame, std::size_t end_frame) const {
    if (end_frame <= start_frame) {
        throw std::invalid_argument("Timeline query end must be after start");
    }

    std::vector<NoteEvent> result;
    for (std::size_t i = 0; i < events_.size(); ++i) {
        const NoteEvent& event = events_[i];
        const std::size_t event_end = event.start_frame + event.duration_frames;
        if (event.start_frame < end_frame && event_end > start_frame) {
            result.push_back(event);
        }
        if (event.start_frame >= end_frame) {
            break;
        }
    }
    return result;
}

std::vector<NoteEvent> MusicalTimeline::overlapping_events(
    std::size_t start_frame, std::size_t duration_frames) const {
    if (duration_frames == 0 ||
        duration_frames > std::numeric_limits<std::size_t>::max() - start_frame) {
        throw std::invalid_argument("Overlap query has an invalid frame range");
    }
    return events_between(start_frame, start_frame + duration_frames);
}

std::size_t MusicalTimeline::duration_frames() const {
    if (events_.empty()) {
        return 0;
    }
    const NoteEvent& last = events_.back();
    return last.start_frame + last.duration_frames;
}

void MusicalTimeline::clear() {
    events_.clear();
}

bool MusicalTimeline::empty() const {
    return events_.empty();
}

std::size_t MusicalTimeline::size() const {
    return events_.size();
}

} // namespace audio
} // namespace rms
