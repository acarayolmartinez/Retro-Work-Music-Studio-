#include "rms/audio/waveform_data.h"

#include <algorithm>
#include <stdexcept>

namespace rms {
namespace audio {

WaveformPoint::WaveformPoint(std::size_t start,
                             float min_value,
                             float max_value)
    : start_frame(start),
      minimum(min_value),
      maximum(max_value) {
}

std::vector<WaveformPoint> WaveformData::generate(
    const AudioBuffer& buffer,
    std::size_t frames_per_point) {
    if (frames_per_point == 0) {
        throw std::invalid_argument(
            "frames_per_point must be greater than zero");
    }

    std::vector<WaveformPoint> points;
    const std::size_t total_frames = buffer.frame_count();
    if (total_frames == 0) {
        return points;
    }

    const std::size_t point_count =
        total_frames / frames_per_point +
        (total_frames % frames_per_point == 0 ? 0 : 1);
    points.reserve(point_count);

    const std::vector<float>& samples = buffer.samples();
    const std::size_t channels = buffer.channel_count();

    for (std::size_t start = 0;
         start < total_frames;) {
        const std::size_t frame_count =
            std::min(frames_per_point, total_frames - start);

        float minimum = 0.0f;
        float maximum = 0.0f;

        for (std::size_t offset = 0; offset < frame_count; ++offset) {
            const std::size_t frame = start + offset;
            float mono_sample = 0.0f;

            for (std::size_t channel = 0; channel < channels; ++channel) {
                mono_sample += samples[frame * channels + channel];
            }
            mono_sample /= static_cast<float>(channels);

            if (offset == 0) {
                minimum = mono_sample;
                maximum = mono_sample;
            } else {
                minimum = std::min(minimum, mono_sample);
                maximum = std::max(maximum, mono_sample);
            }
        }

        points.push_back(WaveformPoint(start, minimum, maximum));
        start += frame_count;
    }

    return points;
}

} // namespace audio
} // namespace rms
