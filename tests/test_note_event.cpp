#include "rms/audio/note_event.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void test_conversion_preserves_timing_and_confidence() {
    std::vector<rms::audio::MelodyNote> notes;
    notes.push_back(rms::audio::MelodyNote(0, 4096, 69, 0.95));
    notes.push_back(rms::audio::MelodyNote(4096, 8192, 72, 0.87));

    const std::vector<rms::audio::NoteEvent> events =
        rms::audio::NoteEventConverter::convert(notes, 96);
    if (events.size() != 2 ||
        events[0].start_frame != 0 ||
        events[0].duration_frames != 4096 ||
        events[0].midi_note != 69 ||
        events[0].velocity != 96 ||
        events[0].confidence != 0.95 ||
        events[1].start_frame != 4096 ||
        events[1].duration_frames != 8192 ||
        events[1].midi_note != 72) {
        std::cerr << "NoteEvent conversion did not preserve note data\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_empty_conversion() {
    const std::vector<rms::audio::MelodyNote> notes;
    if (!rms::audio::NoteEventConverter::convert(notes).empty()) {
        std::cerr << "Empty melody should convert to no events\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_invalid_values_are_rejected() {
    bool bad_velocity = false;
    bool bad_duration = false;
    bool bad_midi = false;
    try {
        std::vector<rms::audio::MelodyNote> notes;
        notes.push_back(rms::audio::MelodyNote(0, 100, 60, 0.8));
        rms::audio::NoteEventConverter::convert(notes, 128);
    } catch (const std::invalid_argument&) {
        bad_velocity = true;
    }
    try {
        rms::audio::NoteEvent(0, 0, 60, 100, 0.8);
    } catch (const std::invalid_argument&) {
        bad_duration = true;
    }
    try {
        rms::audio::NoteEvent(0, 100, 128, 100, 0.8);
    } catch (const std::invalid_argument&) {
        bad_midi = true;
    }
    if (!bad_velocity || !bad_duration || !bad_midi) {
        std::cerr << "Invalid NoteEvent data was not rejected\n";
        std::exit(EXIT_FAILURE);
    }
}

} // namespace

int main() {
    test_conversion_preserves_timing_and_confidence();
    test_empty_conversion();
    test_invalid_values_are_rejected();
    std::cout << "NoteEvent tests passed\n";
    return EXIT_SUCCESS;
}
