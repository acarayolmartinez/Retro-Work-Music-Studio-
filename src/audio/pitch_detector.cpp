#include "rms/audio/pitch_detector.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace rms {
namespace audio {

namespace {

const double kPi = 3.14159265358979323846;
const double kMinimumFrequency = 50.0;
const double kMaximumFrequency = 1000.0;

const char* note_names[] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

} // namespace

PitchEstimate::PitchEstimate()
    : frequency_hz(0.0),
      midi_note(-1),
      note_name(),
      confidence(0.0) {
}

PitchEstimate PitchDetector::estimate(const AudioBuffer& buffer) {
    PitchEstimate result;
    if (buffer.frame_count() == 0 || buffer.channel_count() == 0) {
        return result;
    }

    const std::size_t frame_count =
        std::min<std::size_t>(buffer.frame_count(), 8192);
    const std::size_t channels = buffer.channel_count();
    const std::vector<float>& samples = buffer.samples();

    std::vector<double> mono(frame_count, 0.0);
    double mean = 0.0;
    for (std::size_t frame = 0; frame < frame_count; ++frame) {
        double value = 0.0;
        for (std::size_t channel = 0; channel < channels; ++channel) {
            value += samples[frame * channels + channel];
        }
        mono[frame] = value / static_cast<double>(channels);
        mean += mono[frame];
    }
    mean /= static_cast<double>(frame_count);

    double energy = 0.0;
    for (std::size_t i = 0; i < frame_count; ++i) {
        mono[i] -= mean;
        energy += mono[i] * mono[i];
    }
    if (energy < 1.0e-12) {
        return result;
    }

    const double sample_rate = static_cast<double>(buffer.sample_rate());
    const std::size_t minimum_lag = static_cast<std::size_t>(
        sample_rate / kMaximumFrequency);
    const std::size_t maximum_lag = std::min<std::size_t>(
        static_cast<std::size_t>(sample_rate / kMinimumFrequency),
        frame_count / 2);

    if (maximum_lag <= minimum_lag) {
        return result;
    }

    double best_correlation = -1.0;
    std::size_t best_lag = 0;
    for (std::size_t lag = minimum_lag; lag <= maximum_lag; ++lag) {
        double correlation = 0.0;
        double left_energy = 0.0;
        double right_energy = 0.0;
        for (std::size_t i = 0; i + lag < frame_count; ++i) {
            const double left = mono[i];
            const double right = mono[i + lag];
            correlation += left * right;
            left_energy += left * left;
            right_energy += right * right;
        }

        const double denominator = std::sqrt(left_energy * right_energy);
        if (denominator > 0.0) {
            correlation /= denominator;
            if (correlation > best_correlation) {
                best_correlation = correlation;
                best_lag = lag;
            }
        }
    }

    if (best_lag == 0 || best_correlation < 0.1) {
        return result;
    }

    result.frequency_hz = sample_rate / static_cast<double>(best_lag);
    const double midi = 69.0 + 12.0 *
        std::log(result.frequency_hz / 440.0) / std::log(2.0);
    result.midi_note = static_cast<int>(std::floor(midi + 0.5));
    const int pitch_class = ((result.midi_note % 12) + 12) % 12;
    const int octave = result.midi_note / 12 - 1;
    result.note_name = std::string(note_names[pitch_class]) +
        std::to_string(octave);
    result.confidence = std::max(0.0, std::min(1.0, best_correlation));
    return result;
}

} // namespace audio
} // namespace rms
