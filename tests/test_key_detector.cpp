#include "rms/audio/key_detector.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

void add_tone(std::vector<float>& samples,
              std::uint32_t sample_rate,
              double frequency,
              double amplitude,
              std::size_t frames) {
    const double pi = 3.14159265358979323846;
    for (std::size_t i = 0; i < frames; ++i) {
        const double time = static_cast<double>(i) / sample_rate;
        samples[i] += static_cast<float>(
            amplitude * std::sin(2.0 * pi * frequency * time));
    }
}

} // namespace

int main() {
    const std::uint32_t sample_rate = 44100;
    const std::size_t frames = 44100;
    std::vector<float> samples(frames, 0.0f);

    // C major scale: C4, D4, E4, F4, G4, A4, B4.
    const double frequencies[] = {
        261.6256, 293.6648, 329.6276, 349.2282,
        391.9954, 440.0, 493.8833
    };
    for (std::size_t i = 0; i < 7; ++i) {
        add_tone(samples, sample_rate, frequencies[i],
                 0.12, frames);
    }

    const rms::audio::AudioBuffer buffer(
        sample_rate, 1, samples);
    const rms::audio::KeyEstimate estimate =
        rms::audio::KeyDetector::estimate(buffer);

    if (estimate.key != "C major") {
        std::cerr << "Expected C major, got '" << estimate.key
                  << "'." << std::endl;
        return 1;
    }

    double chroma_total = 0.0;
    for (std::size_t i = 0; i < estimate.chroma.size(); ++i) {
        chroma_total += estimate.chroma[i];
    }
    if (std::fabs(chroma_total - 1.0) > 0.0001) {
        std::cerr << "Chroma profile should be normalized." << std::endl;
        return 1;
    }

    const rms::audio::AudioBuffer silence(
        sample_rate, 1, std::vector<float>(frames, 0.0f));
    const rms::audio::KeyEstimate silent_estimate =
        rms::audio::KeyDetector::estimate(silence);
    if (!silent_estimate.key.empty() ||
        silent_estimate.confidence != 0.0) {
        std::cerr << "Silence should not produce a key estimate."
                  << std::endl;
        return 1;
    }

    const rms::audio::AudioBuffer empty(
        sample_rate, 1, std::vector<float>());
    if (!rms::audio::KeyDetector::estimate(empty).key.empty()) {
        std::cerr << "Empty audio should not produce a key estimate."
                  << std::endl;
        return 1;
    }

    std::cout << "KeyDetector tests passed." << std::endl;
    return 0;
}
