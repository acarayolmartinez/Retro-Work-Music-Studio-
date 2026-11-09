#ifndef RMS_AUDIO_AUDIO_BUFFER_H
#define RMS_AUDIO_AUDIO_BUFFER_H

#include <cstddef>
#include <cstdint>
#include <vector>

namespace rms {
namespace audio {

// Stores interleaved floating-point PCM samples (one value per channel/frame).
class AudioBuffer {
public:
    AudioBuffer(std::uint32_t sample_rate,
                std::uint16_t channel_count,
                const std::vector<float>& samples);

    std::uint32_t sample_rate() const;
    std::uint16_t channel_count() const;
    std::size_t frame_count() const;
    const std::vector<float>& samples() const;

private:
    std::uint32_t sample_rate_;
    std::uint16_t channel_count_;
    std::vector<float> samples_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_BUFFER_H
