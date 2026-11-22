#include "rms/audio/audio_project_file.h"

#include "rms/audio/audio_buffer.h"

#include <cstring>
#include <fstream>
#include <limits>
#include <vector>

namespace rms {
namespace audio {
namespace {

const char kMagic[8] = {'R', 'M', 'S', 'P', 'R', 'O', 'J', '\0'};
const std::uint32_t kVersion = 1;

void set_error(std::string* error, const std::string& message) {
    if (error != 0) {
        *error = message;
    }
}

bool write_u16(std::ostream& stream, std::uint16_t value) {
    const char bytes[2] = {
        static_cast<char>(value & 0xffu),
        static_cast<char>((value >> 8) & 0xffu)
    };
    stream.write(bytes, 2);
    return static_cast<bool>(stream);
}

bool write_u32(std::ostream& stream, std::uint32_t value) {
    char bytes[4];
    for (unsigned int i = 0; i < 4; ++i) {
        bytes[i] = static_cast<char>((value >> (i * 8)) & 0xffu);
    }
    stream.write(bytes, 4);
    return static_cast<bool>(stream);
}

bool write_u64(std::ostream& stream, std::uint64_t value) {
    char bytes[8];
    for (unsigned int i = 0; i < 8; ++i) {
        bytes[i] = static_cast<char>((value >> (i * 8)) & 0xffu);
    }
    stream.write(bytes, 8);
    return static_cast<bool>(stream);
}

bool read_u16(std::istream& stream, std::uint16_t& value) {
    unsigned char bytes[2];
    stream.read(reinterpret_cast<char*>(bytes), 2);
    if (!stream) return false;
    value = static_cast<std::uint16_t>(bytes[0]) |
            static_cast<std::uint16_t>(bytes[1] << 8);
    return true;
}

bool read_u32(std::istream& stream, std::uint32_t& value) {
    unsigned char bytes[4];
    stream.read(reinterpret_cast<char*>(bytes), 4);
    if (!stream) return false;
    value = 0;
    for (unsigned int i = 0; i < 4; ++i) {
        value |= static_cast<std::uint32_t>(bytes[i]) << (i * 8);
    }
    return true;
}

bool read_u64(std::istream& stream, std::uint64_t& value) {
    unsigned char bytes[8];
    stream.read(reinterpret_cast<char*>(bytes), 8);
    if (!stream) return false;
    value = 0;
    for (unsigned int i = 0; i < 8; ++i) {
        value |= static_cast<std::uint64_t>(bytes[i]) << (i * 8);
    }
    return true;
}

} // namespace

unsigned int AudioProjectFile::current_version() {
    return kVersion;
}

bool AudioProjectFile::save(const std::string& path,
                            const AudioProject& project,
                            std::string* error) {
    if (path.empty()) {
        set_error(error, "Project file path is empty.");
        return false;
    }
    if (!project.has_audio() || project.audio() == 0) {
        set_error(error, "Cannot save a project without audio.");
        return false;
    }
    if (sizeof(float) != 4 || !std::numeric_limits<float>::is_iec559) {
        set_error(error, "This platform does not support IEEE 754 32-bit floats.");
        return false;
    }

    std::ofstream stream(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!stream) {
        set_error(error, "Could not open project file for writing.");
        return false;
    }

    const AudioBuffer& buffer = *project.audio();
    stream.write(kMagic, sizeof(kMagic));
    const bool header_ok =
        write_u32(stream, kVersion) &&
        write_u32(stream, buffer.sample_rate()) &&
        write_u16(stream, buffer.channel_count()) &&
        write_u64(stream, static_cast<std::uint64_t>(buffer.samples().size()));

    if (!header_ok) {
        set_error(error, "Failed to write project header.");
        return false;
    }

    const std::vector<float>& samples = buffer.samples();
    for (std::size_t i = 0; i < samples.size(); ++i) {
        std::uint32_t bits = 0;
        std::memcpy(&bits, &samples[i], sizeof(bits));
        if (!write_u32(stream, bits)) {
            set_error(error, "Failed while writing project audio data.");
            return false;
        }
    }

    stream.flush();
    if (!stream) {
        set_error(error, "Failed to finish writing project file.");
        return false;
    }

    if (error != 0) error->clear();
    return true;
}

bool AudioProjectFile::load(const std::string& path,
                            AudioProject& project,
                            std::size_t frames_per_waveform_point,
                            std::string* error) {
    if (path.empty()) {
        set_error(error, "Project file path is empty.");
        return false;
    }
    if (frames_per_waveform_point == 0) {
        set_error(error, "Waveform point size must be greater than zero.");
        return false;
    }
    if (sizeof(float) != 4 || !std::numeric_limits<float>::is_iec559) {
        set_error(error, "This platform does not support IEEE 754 32-bit floats.");
        return false;
    }

    std::ifstream stream(path.c_str(), std::ios::binary);
    if (!stream) {
        set_error(error, "Could not open project file for reading.");
        return false;
    }

    char magic[sizeof(kMagic)];
    stream.read(magic, sizeof(magic));
    if (!stream || std::memcmp(magic, kMagic, sizeof(kMagic)) != 0) {
        set_error(error, "Invalid RMS project signature.");
        return false;
    }

    std::uint32_t version = 0;
    std::uint32_t sample_rate = 0;
    std::uint16_t channels = 0;
    std::uint64_t sample_count = 0;
    if (!read_u32(stream, version) ||
        !read_u32(stream, sample_rate) ||
        !read_u16(stream, channels) ||
        !read_u64(stream, sample_count)) {
        set_error(error, "Incomplete RMS project header.");
        return false;
    }
    if (version != kVersion) {
        set_error(error, "Unsupported RMS project version.");
        return false;
    }
    if (sample_rate == 0 || channels == 0 ||
        sample_count > static_cast<std::uint64_t>(
            std::numeric_limits<std::size_t>::max())) {
        set_error(error, "Invalid audio metadata in RMS project.");
        return false;
    }

    const std::size_t count = static_cast<std::size_t>(sample_count);
    if (count % channels != 0 ||
        count > std::vector<float>().max_size()) {
        set_error(error, "Invalid audio sample count in RMS project.");
        return false;
    }

    std::vector<float> samples;
    try {
        samples.resize(count);
    } catch (const std::exception&) {
        set_error(error, "RMS project audio data is too large.");
        return false;
    }

    for (std::size_t i = 0; i < count; ++i) {
        std::uint32_t bits = 0;
        if (!read_u32(stream, bits)) {
            set_error(error, "RMS project audio data is truncated.");
            return false;
        }
        std::memcpy(&samples[i], &bits, sizeof(bits));
    }

    char trailing = 0;
    if (stream.read(&trailing, 1)) {
        set_error(error, "Unexpected trailing data in RMS project.");
        return false;
    }
    if (!stream.eof()) {
        set_error(error, "Failed while reading RMS project file.");
        return false;
    }

    try {
        const AudioBuffer buffer(sample_rate, channels, samples);
        project.load_audio(buffer, frames_per_waveform_point);
    } catch (const std::exception& exception) {
        set_error(error, exception.what());
        return false;
    }

    if (error != 0) error->clear();
    return true;
}

} // namespace audio
} // namespace rms
