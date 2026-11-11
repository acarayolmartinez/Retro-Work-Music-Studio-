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

    const rms::audio::TempoEstimate estimate =
        rms::audio::TempoEstimator::estimate(events, 44100);
    if (std::fabs(estimate.bpm - 120.0) > 0.0001 ||
        std::fabs(estimate.confidence - 1.0) > 0.0001) {
        std::cerr << "Expected 120 BPM with full confidence." << std::endl;
        return 1;
    }

    const double bpm =
        rms::audio::TempoEstimator::estimate_bpm(events, 44100);
    if (std::fabs(bpm - 120.0) > 0.0001) {
        std::cerr << "Compatibility estimate_bpm failed." << std::endl;
        return 1;
    }

    const double half_time =
        rms::audio::TempoEstimator::estimate_bpm(
            events, 44100, 50.0, 80.0);
    if (std::fabs(half_time - 60.0) > 0.0001) {
        std::cerr << "Expected half-time candidate of 60 BPM." << std::endl;
        return 1;
    }

    const double double_time =
        rms::audio::TempoEstimator::estimate_bpm(
            events, 44100, 200.0, 260.0);
    if (std::fabs(double_time - 240.0) > 0.0001) {
        std::cerr << "Expected double-time candidate of 240 BPM."
                  << std::endl;
        return 1;
    }

    const std::vector<rms::audio::Transient> irregular = {
        rms::audio::Transient(0, 1.0f),
        rms::audio::Transient(22050, 0.9f),
        rms::audio::Transient(48510, 0.8f),
        rms::audio::Transient(66150, 0.9f)
    };
    const rms::audio::TempoEstimate less_regular =
        rms::audio::TempoEstimator::estimate(irregular, 44100);
    if (less_regular.confidence < 0.0 ||
        less_regular.confidence >= estimate.confidence) {
        std::cerr << "Irregular intervals should reduce confidence."
                  << std::endl;
        return 1;
    }

    const std::vector<rms::audio::Transient> insufficient = {
        rms::audio::Transient(0, 1.0f)
    };
    const rms::audio::TempoEstimate empty_estimate =
        rms::audio::TempoEstimator::estimate(insufficient, 44100);
    if (empty_estimate.bpm != 0.0 ||
        empty_estimate.confidence != 0.0) {
        std::cerr << "Insufficient events should return an empty estimate."
                  << std::endl;
        return 1;
    }

    bool rejected_invalid_range = false;
    try {
        rms::audio::TempoEstimator::estimate(
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
