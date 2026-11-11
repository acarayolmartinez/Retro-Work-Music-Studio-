#include "rms/audio/melody_tracker.h"

#include "rms/audio/pitch_detector.h"

#include <stdexcept>
#include <vector>

namespace rms {
namespace audio {

MelodyNote::MelodyNote(std::size_t start,
                       std::size_t duration,
                       int midi,
                       double certainty)
    : start_frame(start),
      duration_frames(duration),
      midi_note(midi),
      confidence(certainty) {
}

std::vector<MelodyNote> MelodyTracker::track(
    const AudioBuffer& buffer,
    std::size_t window_size_frames,
    double minimum_confidence,
    std::size_t minimum_note_duration_frames) {
    if (window_size_frames == 0) {
        throw std::invalid_argument("window_size_frames must be greater than zero");
    }
    if (minimum_confidence < 0.0 || minimum_confidence > 1.0) {
        throw std::invalid_argument("minimum_confidence must be between zero and one");
    }

    std::vector<MelodyNote> notes;
    const std::size_t channels = buffer.channel_count();
    if (buffer.frame_count() == 0 || channels == 0) {
        return notes;
    }

    const std::vector<float>& samples = buffer.samples();
    for (std::size_t start = 0; start < buffer.frame_count();
         start += window_size_frames) {
        const std::size_t frames =
            (buffer.frame_count() - start < window_size_frames)
                ? buffer.frame_count() - start
                : window_size_frames;
        std::vector<float> window_samples(
            samples.begin() + start * channels,
            samples.begin() + (start + frames) * channels);
        AudioBuffer window(buffer.sample_rate(), buffer.channel_count(),
                           window_samples);
        const PitchEstimate pitch = PitchDetector::estimate(window);

        if (pitch.midi_note < 0 || pitch.confidence < minimum_confidence) {
            continue;
        }

        if (!notes.empty() &&
            notes.back().midi_note == pitch.midi_note &&
            notes.back().start_frame + notes.back().duration_frames == start) {
            const double old_weight = notes.back().confidence *
                static_cast<double>(notes.back().duration_frames);
            const double new_weight = pitch.confidence * static_cast<double>(frames);
            notes.back().duration_frames += frames;
            notes.back().confidence =
                (old_weight + new_weight) /
                static_cast<double>(notes.back().duration_frames);
        } else {
            notes.push_back(MelodyNote(start, frames, pitch.midi_note,
                                       pitch.confidence));
        }
    }

    if (minimum_note_duration_frames > 0) {
        std::vector<MelodyNote> filtered;
        for (std::size_t i = 0; i < notes.size(); ++i) {
            if (notes[i].duration_frames >= minimum_note_duration_frames) {
                filtered.push_back(notes[i]);
            }
        }
        return filtered;
    }
    return notes;
}

} // namespace audio
} // namespace rms
