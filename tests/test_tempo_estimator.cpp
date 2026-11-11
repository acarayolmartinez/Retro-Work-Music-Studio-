#include "rms/audio/tempo_estimator.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<rms::audio::Transient> events = {
        rms::audio::Transient(0, 1.0f),
        rms::audio::Transient(22050, 0.9f),
        rms::audio::Transient(44100, 0.8f),
        rms::audio::Transient(66150, 0.9f)
    };

    const double bpm =
        rms::audio::TempoEstimator::estimate_bpm(events, 44100);
    if (std::fabs(bpm - 120.0) > 0.0001) {
        std::cerr << "Expected 120 BPM, got " << bpm << std::endl;
        return 1;
    }

    const double restricted =
        rms::audio::TempoEstimator::estimate_bpm(
            events, 44100, 130.0, 200.0);
    if (restricted != 0.0) {
        std::cerr << "Tempo outside configured range should return zero."
                  << std::endl;
        return 1;
    }

    const std::vector<rms::audio::Transient> insufficient = {
        rms::audio::Transient(0, 1.0f)
    };
    if (rms::audio::TempoEstimator::estimate_bpm(
            insufficient, 44100) != 0.0) {
        std::cerr << "Insufficient events should return zero." << std::endl;
        return 1;
    }

    bool rejected_invalid_range = false;
    try {
        rms::audio::TempoEstimator::estimate_bpm(
            events, 44100, 200.0, 100.0);
    } catch (const std::invalid_argument&) {
        rejected_invalid_range = true;
    }
    if (!rejected_invalid_range) {
        std::cerr << "Invalid BPM range was not rejected." << std::endl;
        return 1;
    }

    std::cout << "TempoEstimator tests passed." << std::endl;
    return 0;
}
