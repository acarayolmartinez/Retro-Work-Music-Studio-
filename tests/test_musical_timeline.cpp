#include "rms/audio/musical_timeline.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << "\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_insertion_keeps_chronological_order() {
    rms::audio::MusicalTimeline timeline;
    timeline.add_event(rms::audio::NoteEvent(200, 50, 64));
    timeline.add_event(rms::audio::NoteEvent(0, 100, 60));
    timeline.add_event(rms::audio::NoteEvent(100, 150, 62));
    require(timeline.size() == 3, "Timeline insertion count is wrong");
    require(timeline.events()[0].start_frame == 0 &&
            timeline.events()[1].start_frame == 100 &&
            timeline.events()[2].start_frame == 200,
            "Timeline events are not chronologically sorted");
}

void test_interval_query_includes_intersections() {
    rms::audio::MusicalTimeline timeline;
    timeline.add_event(rms::audio::NoteEvent(0, 100, 60));
    timeline.add_event(rms::audio::NoteEvent(100, 100, 62));
    timeline.add_event(rms::audio::NoteEvent(250, 50, 64));
    const std::vector<rms::audio::NoteEvent> found =
        timeline.events_between(90, 180);
    require(found.size() == 2,
            "Interval query should return events intersecting its range");
}

void test_overlapping_events_and_duration() {
    rms::audio::MusicalTimeline timeline;
    timeline.add_event(rms::audio::NoteEvent(0, 200, 60));
    timeline.add_event(rms::audio::NoteEvent(50, 100, 64));
    timeline.add_event(rms::audio::NoteEvent(200, 50, 67));
    const std::vector<rms::audio::NoteEvent> overlaps =
        timeline.overlapping_events(75, 50);
    require(overlaps.size() == 2,
            "Overlap query should return both polyphonic events");
    require(timeline.duration_frames() == 250,
            "Timeline duration should reach the final event end");
}

void test_remove_clear_and_empty() {
    rms::audio::MusicalTimeline timeline;
    require(timeline.empty(), "New timeline should be empty");
    require(!timeline.remove_event(0), "Removing invalid index should fail");
    timeline.add_event(rms::audio::NoteEvent(10, 20, 60));
    require(timeline.remove_event(0), "Valid event removal failed");
    require(timeline.empty() && timeline.duration_frames() == 0,
            "Timeline should be empty after removal");
    timeline.add_event(rms::audio::NoteEvent(0, 10, 60));
    timeline.clear();
    require(timeline.empty(), "Clear should empty the timeline");
}

void test_invalid_ranges_are_rejected() {
    rms::audio::MusicalTimeline timeline;
    bool interval_threw = false;
    bool overlap_threw = false;
    try {
        timeline.events_between(10, 10);
    } catch (const std::invalid_argument&) {
        interval_threw = true;
    }
    try {
        timeline.overlapping_events(0, 0);
    } catch (const std::invalid_argument&) {
        overlap_threw = true;
    }
    require(interval_threw && overlap_threw,
            "Invalid timeline query ranges were not rejected");
}

} // namespace

int main() {
    test_insertion_keeps_chronological_order();
    test_interval_query_includes_intersections();
    test_overlapping_events_and_duration();
    test_remove_clear_and_empty();
    test_invalid_ranges_are_rejected();
    std::cout << "MusicalTimeline tests passed\n";
    return EXIT_SUCCESS;
}
