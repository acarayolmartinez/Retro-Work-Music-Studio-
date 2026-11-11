#include "rms/audio/pitch_detector.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

std::vector<float> make_tone(std::uint32_t sample_rate,
                             std::size_t frames,
                             double frequency) {
    const double pi = 3.14159265358979323846;
    std::vector<float> samples(frames, 0.0f);
    for (std::size_t i = 0; i < frames; ++i) {
        samples[i] = static_cast<float>(
            0.7 * std::sin(2.0 * pi * frequency * i / sample_rate));
    }
    return samples;
}

} // namespace

int main() {
    const std::uint32_t sample_rate = 44100;
    const std::size_t frames = 8192;

    const rms::audio::AudioBuffer a4(
        sample_rate, 1, make_tone(sample_rate, frames, 440.0));
    const rms::audio::PitchEstimate estimate =
        rms::audio::PitchDetector::estimate(a4);
    if (std::fabs(estimate.frequency_hz - 440.0) > 2.0) {
        std::cerr << "Expected approximately 440 Hz, got "
                  << estimate.frequency_hz << " Hz." << std::endl;
        return 1;
    }
    if (estimate.note_name != "A4" || estimate.midi_note != 69) {
        std::cerr << "Expected A4 / MIDI 69, got "
                  << estimate.note_name << " / "
                  << estimate.midi_note << "." << std::endl;
        return 1;
    }
    if (estimate.confidence <= 0.0 || estimate.confidence > 1.0) {
        std::cerr << "Confidence should be in (0, 1]." << std::endl;
        return 1;
    }

    const rms::audio::AudioBuffer c4(
        sample_rate, 1, make_tone(sample_rate, frames, 261.6256));
    const rms::audio::PitchEstimate c4_estimate =
        rms::audio::PitchDetector::estimate(c4);
    if (c4_estimate.note_name != "C4") {
        std::cerr << "Expected C4, got " << c4_estimate.note_name
                  << "." << std::endl;
        return 1;
    }

    const rms::audio::AudioBuffer silence(
        sample_rate, 1, std::vector<float>(frames, 0.0f));
    if (rms::audio::PitchDetector::estimate(silence).midi_note != -1) {
        std::cerr << "Silence should not produce a pitch." << std::endl;
        return 1;
    }

    const rms::audio::AudioBuffer empty(
        sample_rate, 1, std::vector<float>());
    if (rms::audio::PitchDetector::estimate(empty).midi_note != -1) {
        std::cerr << "Empty audio should not produce a pitch." << std::endl;
        return 1;
    }

    std::cout << "PitchDetector tests passed." << std::endl;
    return 0;
}
