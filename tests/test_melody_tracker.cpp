#include "rms/audio/melody_tracker.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

const std::uint32_t kSampleRate = 44100;
const std::size_t kWindow = 8192;

void append_tone(std::vector<float>& samples, double frequency,
                 std::size_t frames) {
    const double pi = 3.14159265358979323846;
    for (std::size_t i = 0; i < frames; ++i) {
        samples.push_back(static_cast<float>(
            0.7 * std::sin(2.0 * pi * frequency *
                           static_cast<double>(i) / kSampleRate)));
    }
}

void test_merges_repeated_notes_and_tracks_changes() {
    std::vector<float> samples;
    append_tone(samples, 440.0, kWindow);
    append_tone(samples, 440.0, kWindow);
    append_tone(samples, 523.251, kWindow);

    const rms::audio::AudioBuffer buffer(kSampleRate, 1, samples);
    const std::vector<rms::audio::MelodyNote> notes =
        rms::audio::MelodyTracker::track(buffer, kWindow, 0.5);

    if (notes.size() != 2 ||
        notes[0].midi_note != 69 ||
        notes[0].start_frame != 0 ||
        notes[0].duration_frames != 2 * kWindow ||
        notes[1].midi_note != 72 ||
        notes[1].start_frame != 2 * kWindow ||
        notes[1].duration_frames != kWindow) {
        std::cerr << "Melody tracking or note merging failed\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_silence_returns_no_notes() {
    const std::vector<float> samples(kWindow, 0.0f);
    const rms::audio::AudioBuffer buffer(kSampleRate, 1, samples);
    if (!rms::audio::MelodyTracker::track(buffer, kWindow).empty()) {
        std::cerr << "Silence should not produce melody notes\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_invalid_arguments() {
    const std::vector<float> samples(kWindow, 0.0f);
    const rms::audio::AudioBuffer buffer(kSampleRate, 1, samples);
    bool zero_window_threw = false;
    bool confidence_threw = false;
    try {
        rms::audio::MelodyTracker::track(buffer, 0);
    } catch (const std::invalid_argument&) {
        zero_window_threw = true;
    }
    try {
        rms::audio::MelodyTracker::track(buffer, kWindow, 1.1);
    } catch (const std::invalid_argument&) {
        confidence_threw = true;
    }
    if (!zero_window_threw || !confidence_threw) {
        std::cerr << "Invalid tracker arguments were not rejected\n";
        std::exit(EXIT_FAILURE);
    }
}

} // namespace

int main() {
    test_merges_repeated_notes_and_tracks_changes();
    test_silence_returns_no_notes();
    test_invalid_arguments();
    std::cout << "MelodyTracker tests passed\n";
    return EXIT_SUCCESS;
}
