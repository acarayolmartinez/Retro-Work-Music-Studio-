#include "rms/audio/beat_interval_analyzer.h"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    const std::vector<rms::audio::Transient> events = {
        rms::audio::Transient(0, 1.0f),
        rms::audio::Transient(22050, 0.8f),
        rms::audio::Transient(44100, 0.9f),
        rms::audio::Transient(66150, 0.7f)
    };

    const std::vector<double> intervals =
        rms::audio::BeatIntervalAnalyzer::intervals_seconds(events, 44100);
    if (intervals.size() != 3 ||
        std::fabs(intervals[0] - 0.5) > 0.000001 ||
        std::fabs(intervals[1] - 0.5) > 0.000001 ||
        std::fabs(intervals[2] - 0.5) > 0.000001) {
        std::cerr << "Transient interval conversion failed." << std::endl;
        return 1;
    }

    if (std::fabs(
            rms::audio::BeatIntervalAnalyzer::median_interval_seconds(
                events, 44100) - 0.5) > 0.000001) {
        std::cerr << "Median interval calculation failed." << std::endl;
        return 1;
    }

    const std::vector<rms::audio::Transient> one_event = {
        rms::audio::Transient(10, 1.0f)
    };
    if (rms::audio::BeatIntervalAnalyzer::median_interval_seconds(
            one_event, 44100) != 0.0) {
        std::cerr << "Single event should have no interval." << std::endl;
        return 1;
    }

    bool rejected_zero_rate = false;
    try {
        rms::audio::BeatIntervalAnalyzer::intervals_seconds(events, 0);
    } catch (const std::invalid_argument&) {
        rejected_zero_rate = true;
    }
    if (!rejected_zero_rate) {
        std::cerr << "Zero sample rate was not rejected." << std::endl;
        return 1;
    }

    const std::vector<rms::audio::Transient> unordered = {
        rms::audio::Transient(20, 1.0f),
        rms::audio::Transient(10, 1.0f)
    };
    bool rejected_unordered = false;
    try {
        rms::audio::BeatIntervalAnalyzer::intervals_seconds(
            unordered, 44100);
    } catch (const std::invalid_argument&) {
        rejected_unordered = true;
    }
    if (!rejected_unordered) {
        std::cerr << "Unordered transients were not rejected." << std::endl;
        return 1;
    }

    std::cout << "BeatIntervalAnalyzer tests passed." << std::endl;
    return 0;
}
