#include "rms/audio/audio_buffer.h"

#include <stdexcept>

namespace rms {
namespace audio {

AudioBuffer::AudioBuffer(std::uint32_t sample_rate,
                         std::uint16_t channel_count,
                         const std::vector<float>& samples)
    : sample_rate_(sample_rate),
      channel_count_(channel_count),
      samples_(samples) {
    if (sample_rate_ == 0) {
        throw std::invalid_argument("Sample rate must be greater than zero.");
    }
    if (channel_count_ == 0) {
        throw std::invalid_argument("Channel count must be greater than zero.");
    }
    if (samples_.size() % channel_count_ != 0) {
        throw std::invalid_argument(
            "Interleaved sample count must be divisible by channel count.");
    }
}

std::uint32_t AudioBuffer::sample_rate() const {
    return sample_rate_;
}

std::uint16_t AudioBuffer::channel_count() const {
    return channel_count_;
}

std::size_t AudioBuffer::frame_count() const {
    return samples_.size() / channel_count_;
}

const std::vector<float>& AudioBuffer::samples() const {
    return samples_;
}

} // namespace audio
} // namespace rms
