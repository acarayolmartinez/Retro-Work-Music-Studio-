#include "rms/audio/audio_file_reader.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

namespace rms {
namespace audio {
namespace {

std::uint16_t read_u16(const std::vector<unsigned char>& data, std::size_t offset) {
    return static_cast<std::uint16_t>(data[offset]) |
           static_cast<std::uint16_t>(data[offset + 1] << 8);
}

std::uint32_t read_u32(const std::vector<unsigned char>& data, std::size_t offset) {
    return static_cast<std::uint32_t>(data[offset]) |
           (static_cast<std::uint32_t>(data[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(data[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(data[offset + 3]) << 24);
}

bool has_id(const std::vector<unsigned char>& data, std::size_t offset,
            const char* id) {
    return data[offset] == static_cast<unsigned char>(id[0]) &&
           data[offset + 1] == static_cast<unsigned char>(id[1]) &&
           data[offset + 2] == static_cast<unsigned char>(id[2]) &&
           data[offset + 3] == static_cast<unsigned char>(id[3]);
}

float decode_sample(const std::vector<unsigned char>& data,
                    std::size_t offset, std::uint16_t bits) {
    if (bits == 8) {
        return (static_cast<int>(data[offset]) - 128) / 128.0f;
    }

    const std::size_t bytes = bits / 8;
    std::uint32_t raw = 0;
    for (std::size_t i = 0; i < bytes; ++i) {
        raw |= static_cast<std::uint32_t>(data[offset + i]) << (8 * i);
    }

    std::int64_t signed_value = raw;
    const std::int64_t sign_bit = static_cast<std::int64_t>(1) << (bits - 1);
    const std::int64_t full_scale = static_cast<std::int64_t>(1) << bits;
    if (signed_value >= sign_bit) {
        signed_value -= full_scale;
    }
    return static_cast<float>(static_cast<double>(signed_value) /
                              static_cast<double>(sign_bit));
}

} // namespace

AudioBuffer AudioFileReader::read_wav(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open WAV file: " + path);
    }

    const std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (data.size() < 12 || !has_id(data, 0, "RIFF") ||
        !has_id(data, 8, "WAVE")) {
        throw std::runtime_error("Invalid WAV: missing RIFF/WAVE header.");
    }

    const std::uint32_t riff_size = read_u32(data, 4);
    if (riff_size < 4 || static_cast<std::uint64_t>(riff_size) + 8 > data.size()) {
        throw std::runtime_error("Invalid WAV: truncated RIFF data.");
    }
    const std::size_t riff_end = static_cast<std::size_t>(riff_size) + 8;

    bool have_format = false;
    bool have_samples = false;
    std::uint16_t format_tag = 0;
    std::uint16_t channels = 0;
    std::uint16_t bits = 0;
    std::uint16_t block_align = 0;
    std::uint32_t sample_rate = 0;
    std::size_t sample_offset = 0;
    std::size_t sample_size = 0;

    std::size_t offset = 12;
    while (offset + 8 <= riff_end) {
        const std::uint32_t chunk_size = read_u32(data, offset + 4);
        const std::size_t chunk_data = offset + 8;
        if (static_cast<std::uint64_t>(chunk_data) + chunk_size > riff_end) {
            throw std::runtime_error("Invalid WAV: truncated chunk.");
        }

        if (has_id(data, offset, "fmt ")) {
            if (chunk_size < 16) {
                throw std::runtime_error("Invalid WAV: fmt chunk is too small.");
            }
            format_tag = read_u16(data, chunk_data);
            channels = read_u16(data, chunk_data + 2);
            sample_rate = read_u32(data, chunk_data + 4);
            block_align = read_u16(data, chunk_data + 12);
            bits = read_u16(data, chunk_data + 14);
            have_format = true;
        } else if (has_id(data, offset, "data")) {
            sample_offset = chunk_data;
            sample_size = chunk_size;
            have_samples = true;
        }

        const std::uint64_t next = static_cast<std::uint64_t>(chunk_data) +
                                   chunk_size + (chunk_size & 1u);
        if (next > riff_end) {
            if (next != riff_end + 1) {
                throw std::runtime_error("Invalid WAV: missing chunk padding.");
            }
            offset = riff_end;
        } else {
            offset = static_cast<std::size_t>(next);
        }
    }

    if (!have_format || !have_samples) {
        throw std::runtime_error("Invalid WAV: missing fmt or data chunk.");
    }
    if (format_tag != 1) {
        throw std::runtime_error("Unsupported WAV encoding: only integer PCM is supported.");
    }
    if (channels == 0 || sample_rate == 0) {
        throw std::runtime_error("Invalid WAV: sample rate and channel count must be positive.");
    }
    if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {
        throw std::runtime_error("Unsupported WAV PCM bit depth.");
    }

    const std::uint32_t expected_align =
        static_cast<std::uint32_t>(channels) * (bits / 8);
    if (expected_align > std::numeric_limits<std::uint16_t>::max() ||
        block_align != expected_align || sample_size % block_align != 0) {
        throw std::runtime_error("Invalid WAV: inconsistent block alignment or data size.");
    }

    const std::size_t sample_count = sample_size / (bits / 8);
    std::vector<float> samples;
    samples.reserve(sample_count);
    for (std::size_t i = 0; i < sample_count; ++i) {
        samples.push_back(decode_sample(data, sample_offset + i * (bits / 8), bits));
    }

    return AudioBuffer(sample_rate, channels, samples);
}

} // namespace audio
} // namespace rms
