#include "rms/audio/audio_project_file.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioProject;
using rms::audio::AudioProjectFile;
using rms::audio::AudioProjectState;

namespace {

const char* kPath = "rms_audio_project_test.rms";

void write_bytes(const std::string& path, const std::vector<unsigned char>& bytes) {
    std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
    file.write(reinterpret_cast<const char*>(&bytes[0]),
               static_cast<std::streamsize>(bytes.size()));
}

void append_u32(std::vector<unsigned char>& bytes, unsigned int value) {
    for (unsigned int i = 0; i < 4; ++i) {
        bytes.push_back(static_cast<unsigned char>((value >> (i * 8)) & 0xffu));
    }
}

void append_u16(std::vector<unsigned char>& bytes, unsigned int value) {
    bytes.push_back(static_cast<unsigned char>(value & 0xffu));
    bytes.push_back(static_cast<unsigned char>((value >> 8) & 0xffu));
}

void append_u64(std::vector<unsigned char>& bytes, unsigned long long value) {
    for (unsigned int i = 0; i < 8; ++i) {
        bytes.push_back(static_cast<unsigned char>((value >> (i * 8)) & 0xffu));
    }
}

std::vector<unsigned char> make_header(unsigned int version,
                                       unsigned int sample_rate,
                                       unsigned int channels,
                                       unsigned long long sample_count) {
    const char magic[8] = {'R', 'M', 'S', 'P', 'R', 'O', 'J', '\0'};
    std::vector<unsigned char> bytes(magic, magic + 8);
    append_u32(bytes, version);
    append_u32(bytes, sample_rate);
    append_u16(bytes, channels);
    append_u64(bytes, sample_count);
    return bytes;
}

} // namespace

int main() {
    assert(AudioProjectFile::current_version() == 1);

    AudioProject empty;
    std::string error;
    assert(!AudioProjectFile::save(kPath, empty, &error));
    assert(!error.empty());

    const std::vector<float> source_samples = {
        -1.0f, -0.5f, 0.0f, 0.25f, 0.5f, 1.0f
    };
    const AudioBuffer source_buffer(48000, 2, source_samples);
    AudioProject source;
    source.load_audio(source_buffer, 2);
    source.analyze();

    assert(AudioProjectFile::save(kPath, source, &error));
    assert(error.empty());

    AudioProject loaded;
    assert(AudioProjectFile::load(kPath, loaded, 2, &error));
    assert(error.empty());
    assert(loaded.state() == AudioProjectState::AudioLoaded);
    assert(loaded.has_audio());
    assert(!loaded.has_analysis());
    assert(loaded.audio()->sample_rate() == 48000);
    assert(loaded.audio()->channel_count() == 2);
    assert(loaded.audio()->samples() == source_samples);
    assert(loaded.waveform().size() == 2);
    assert(loaded.viewport().total_frames() == 3);

    loaded.analyze();
    assert(loaded.has_analysis());

    // Bad signature must not replace the existing project.
    std::vector<unsigned char> invalid(16, 0);
    write_bytes(kPath, invalid);
    assert(!AudioProjectFile::load(kPath, loaded, 2, &error));
    assert(!error.empty());
    assert(loaded.audio()->sample_rate() == 48000);

    // Unsupported version.
    write_bytes(kPath, make_header(99, 48000, 2, 0));
    assert(!AudioProjectFile::load(kPath, loaded, 2, &error));
    assert(error.find("version") != std::string::npos);

    // Truncated sample data.
    std::vector<unsigned char> truncated = make_header(1, 48000, 1, 2);
    append_u32(truncated, 0);
    write_bytes(kPath, truncated);
    assert(!AudioProjectFile::load(kPath, loaded, 1, &error));
    assert(error.find("truncated") != std::string::npos);
    assert(loaded.audio()->sample_rate() == 48000);

    // Reject trailing bytes.
    std::vector<unsigned char> trailing = make_header(1, 48000, 1, 0);
    trailing.push_back(0);
    write_bytes(kPath, trailing);
    assert(!AudioProjectFile::load(kPath, loaded, 1, &error));
    assert(error.find("trailing") != std::string::npos);

    // A bad waveform setting is rejected without changing the project.
    assert(!AudioProjectFile::load(kPath, loaded, 0, &error));
    assert(loaded.audio()->sample_rate() == 48000);

    std::remove(kPath);
    std::cout << "AudioProjectFile tests passed." << std::endl;
    return 0;
}
