#include "rms/audio/key_detector.h"

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <vector>

namespace rms {
namespace audio {
namespace {

const double kPi = 3.14159265358979323846;
const std::size_t kFftSize = 4096;
const char* kNoteNames[12] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

const double kMajorProfile[12] = {
    6.35, 2.23, 3.48, 2.33, 4.38, 4.09,
    2.52, 5.19, 2.39, 3.66, 2.29, 2.88
};
const double kMinorProfile[12] = {
    6.33, 2.68, 3.52, 5.38, 2.60, 3.53,
    2.54, 4.75, 3.98, 2.69, 3.34, 3.17
};

void fft(std::vector<std::complex<double> >& values) {
    const std::size_t size = values.size();
    for (std::size_t i = 1, j = 0; i < size; ++i) {
        std::size_t bit = size >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(values[i], values[j]);
        }
    }

    for (std::size_t length = 2; length <= size; length <<= 1) {
        const double angle = -2.0 * kPi / static_cast<double>(length);
        const std::complex<double> root(std::cos(angle), std::sin(angle));
        for (std::size_t start = 0; start < size; start += length) {
            std::complex<double> factor(1.0, 0.0);
            const std::size_t half = length / 2;
            for (std::size_t offset = 0; offset < half; ++offset) {
                const std::complex<double> even = values[start + offset];
                const std::complex<double> odd =
                    values[start + offset + half] * factor;
                values[start + offset] = even + odd;
                values[start + offset + half] = even - odd;
                factor *= root;
            }
        }
    }
}

double correlation(const std::array<double, 12>& chroma,
                   const double profile[12],
                   std::size_t shift) {
    double chroma_mean = 0.0;
    double profile_mean = 0.0;
    for (std::size_t i = 0; i < 12; ++i) {
        chroma_mean += chroma[i];
        profile_mean += profile[i];
    }
    chroma_mean /= 12.0;
    profile_mean /= 12.0;

    double numerator = 0.0;
    double chroma_norm = 0.0;
    double profile_norm = 0.0;
    for (std::size_t i = 0; i < 12; ++i) {
        const double a = chroma[(i + shift) % 12] - chroma_mean;
        const double b = profile[i] - profile_mean;
        numerator += a * b;
        chroma_norm += a * a;
        profile_norm += b * b;
    }
    if (chroma_norm <= 0.0 || profile_norm <= 0.0) {
        return 0.0;
    }
    return numerator / std::sqrt(chroma_norm * profile_norm);
}

} // namespace

KeyEstimate::KeyEstimate()
    : key(), confidence(0.0), chroma() {
    chroma.fill(0.0);
}

KeyEstimate KeyDetector::estimate(const AudioBuffer& buffer) {
    KeyEstimate result;
    if (buffer.frame_count() == 0) {
        return result;
    }

    const std::size_t frames = buffer.frame_count();
    const std::uint16_t channels = buffer.channel_count();
    std::vector<std::complex<double> > spectrum(kFftSize);

    for (std::size_t start = 0; start < frames; start += kFftSize) {
        std::fill(spectrum.begin(), spectrum.end(),
                  std::complex<double>(0.0, 0.0));
        const std::size_t count =
            std::min(kFftSize, frames - start);

        for (std::size_t i = 0; i < count; ++i) {
            double mono = 0.0;
            for (std::uint16_t channel = 0; channel < channels; ++channel) {
                mono += buffer.samples()[
                    (start + i) * channels + channel];
            }
            mono /= channels;

            const double window = count > 1
                ? 0.5 - 0.5 * std::cos(
                    2.0 * kPi * static_cast<double>(i) /
                    static_cast<double>(count - 1))
                : 1.0;
            spectrum[i] = std::complex<double>(mono * window, 0.0);
        }

        fft(spectrum);

        for (std::size_t bin = 1; bin < kFftSize / 2; ++bin) {
            const double frequency =
                static_cast<double>(bin) * buffer.sample_rate() /
                static_cast<double>(kFftSize);
            if (frequency < 20.0 || frequency > 5000.0) {
                continue;
            }
            const double midi =
                69.0 + 12.0 * std::log(frequency / 440.0) /
                std::log(2.0);
            const int note = static_cast<int>(std::floor(midi + 0.5));
            const std::size_t pitch_class =
                static_cast<std::size_t>((note % 12 + 12) % 12);
            result.chroma[pitch_class] += std::abs(spectrum[bin]);
        }
    }

    double total = 0.0;
    for (std::size_t i = 0; i < 12; ++i) {
        total += result.chroma[i];
    }
    if (total <= 0.0) {
        return result;
    }
    for (std::size_t i = 0; i < 12; ++i) {
        result.chroma[i] /= total;
    }

    double best = -2.0;
    double second = -2.0;
    std::string best_key;
    for (std::size_t tonic = 0; tonic < 12; ++tonic) {
        const double major = correlation(
            result.chroma, kMajorProfile, tonic);
        const double minor = correlation(
            result.chroma, kMinorProfile, tonic);
        const double scores[2] = { major, minor };
        for (std::size_t mode = 0; mode < 2; ++mode) {
            if (scores[mode] > best) {
                second = best;
                best = scores[mode];
                best_key = std::string(kNoteNames[tonic]) +
                    (mode == 0 ? " major" : " minor");
            } else if (scores[mode] > second) {
                second = scores[mode];
            }
        }
    }

    result.key = best_key;
    result.confidence = std::max(
        0.0, std::min(1.0, (best - second) / 2.0));
    return result;
}

} // namespace audio
} // namespace rms
