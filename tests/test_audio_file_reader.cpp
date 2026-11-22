#include "rms/audio/audio_file_reader.h"

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void write_u16(std::ofstream& out, std::uint16_t value) {
    out.put(static_cast<char>(value & 0xff));
    out.put(static_cast<char>((value >> 8) & 0xff));
}
void write_u32(std::ofstream& out, std::uint32_t value) {
    out.put(static_cast<char>(value & 0xff));
    out.put(static_cast<char>((value >> 8) & 0xff));
    out.put(static_cast<char>((value >> 16) & 0xff));
    out.put(static_cast<char>((value >> 24) & 0xff));
}
void append_u16(std::vector<unsigned char>& out, std::uint16_t value) {
    out.push_back(static_cast<unsigned char>(value & 0xff));
    out.push_back(static_cast<unsigned char>((value >> 8) & 0xff));
}
void append_u32(std::vector<unsigned char>& out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<unsigned char>((value >> (8 * i)) & 0xff));
}
void append_id(std::vector<unsigned char>& out, const char* id) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<unsigned char>(id[i]));
}

void write_fixture(const std::string& path, std::uint16_t bits,
                   std::uint16_t channels, std::uint16_t format = 1,
                   bool extra_chunk = false) {
    std::vector<unsigned char> body;
    append_id(body, "WAVE");
    if (extra_chunk) {
        append_id(body, "JUNK"); append_u32(body, 1); body.push_back(0x42);
        body.push_back(0);
    }
    append_id(body, "fmt "); append_u32(body, 16);
    append_u16(body, format); append_u16(body, channels);
    append_u32(body, 44100); append_u32(body, 44100 * channels * (bits / 8));
    append_u16(body, static_cast<std::uint16_t>(channels * (bits / 8)));
    append_u16(body, bits);
    std::vector<unsigned char> pcm;
    if (bits == 8) {
        pcm.push_back(0); pcm.push_back(128); pcm.push_back(255);
    } else if (bits == 16) {
        append_u16(pcm, 0x8000); append_u16(pcm, 0); append_u16(pcm, 0x7fff);
    } else if (bits == 24) {
        pcm.push_back(0); pcm.push_back(0); pcm.push_back(0x80);
        pcm.push_back(0); pcm.push_back(0); pcm.push_back(0);
        pcm.push_back(0xff); pcm.push_back(0xff); pcm.push_back(0x7f);
    } else {
        append_u32(pcm, 0x80000000u); append_u32(pcm, 0);
        append_u32(pcm, 0x7fffffffu);
    }
    if (channels == 2) {
        if (bits == 8) pcm.push_back(128);
        else if (bits == 16) append_u16(pcm, 0);
        else if (bits == 24) { pcm.push_back(0); pcm.push_back(0); pcm.push_back(0); }
        else append_u32(pcm, 0);
    }
    append_id(body, "data"); append_u32(body, static_cast<std::uint32_t>(pcm.size()));
    body.insert(body.end(), pcm.begin(), pcm.end());
    if (pcm.size() & 1u) body.push_back(0);

    std::ofstream out(path.c_str(), std::ios::binary);
    out.write("RIFF", 4);
    write_u32(out, static_cast<std::uint32_t>(body.size()));
    out.write(reinterpret_cast<const char*>(&body[0]),
              static_cast<std::streamsize>(body.size()));
}

bool near(float a, float b) { return (a - b < 0.0001f) && (b - a < 0.0001f); }

void test_bit_depth(std::uint16_t bits, const std::string& path) {
    write_fixture(path, bits, 1, 1, bits == 8);
    const rms::audio::AudioBuffer buffer = rms::audio::AudioFileReader::read_wav(path);
    if (buffer.sample_rate() != 44100 || buffer.channel_count() != 1 ||
        buffer.frame_count() != 3 || !near(buffer.samples()[0], -1.0f) ||
        !near(buffer.samples()[1], 0.0f) || buffer.samples()[2] < 0.99f) {
        throw std::runtime_error("PCM decoding failed for " + std::to_string(bits) + " bits");
    }
    std::remove(path.c_str());
}

void test_rejects(const std::string& path) {
    write_fixture(path, 16, 1, 3);
    bool rejected = false;
    try { rms::audio::AudioFileReader::read_wav(path); }
    catch (const std::runtime_error&) { rejected = true; }
    std::remove(path.c_str());
    if (!rejected) throw std::runtime_error("Non-PCM WAV was not rejected.");
}

} // namespace

int main() {
    try {
        test_bit_depth(8, "rms_test_8bit.wav");
        test_bit_depth(16, "rms_test_16bit.wav");
        test_bit_depth(24, "rms_test_24bit.wav");
        test_bit_depth(32, "rms_test_32bit.wav");
        const std::string stereo_path = "rms_test_stereo.wav";
        write_fixture(stereo_path, 16, 2);
        const rms::audio::AudioBuffer stereo =
            rms::audio::AudioFileReader::read_wav(stereo_path);
        std::remove(stereo_path.c_str());
        if (stereo.channel_count() != 2 || stereo.frame_count() != 1) {
            throw std::runtime_error("Stereo WAV metadata was not read correctly.");
        }
        test_rejects("rms_test_non_pcm.wav");
        std::cout << "All AudioFileReader tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
