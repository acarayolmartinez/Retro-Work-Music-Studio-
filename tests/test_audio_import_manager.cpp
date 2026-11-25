#include "rms/audio/audio_import_manager.h"
#include "rms/audio/audio_project_manager.h"
#include "rms/audio/audio_import_config.h"
#include "rms/audio/audio_import_format.h"

#include <cassert>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

void u16(std::vector<unsigned char>& out, std::uint16_t value) {
    out.push_back(static_cast<unsigned char>(value & 255));
    out.push_back(static_cast<unsigned char>((value >> 8) & 255));
}
void u32(std::vector<unsigned char>& out, std::uint32_t value) {
    for (int i = 0; i < 4; ++i)
        out.push_back(static_cast<unsigned char>((value >> (8 * i)) & 255));
}
void id(std::vector<unsigned char>& out, const char* value) {
    for (int i = 0; i < 4; ++i)
        out.push_back(static_cast<unsigned char>(value[i]));
}
void fixture(const std::string& path) {
    std::vector<unsigned char> body;
    id(body, "WAVE");
    id(body, "fmt "); u32(body, 16);
    u16(body, 1); u16(body, 1); u32(body, 44100);
    u32(body, 88200); u16(body, 2); u16(body, 16);
    id(body, "data"); u32(body, 4);
    u16(body, 0); u16(body, 16384);
    std::ofstream out(path.c_str(), std::ios::binary);
    out.write("RIFF", 4);
    const std::uint32_t size = static_cast<std::uint32_t>(body.size());
    out.put(static_cast<char>(size & 255));
    out.put(static_cast<char>((size >> 8) & 255));
    out.put(static_cast<char>((size >> 16) & 255));
    out.put(static_cast<char>((size >> 24) & 255));
    out.write(reinterpret_cast<const char*>(&body[0]),
              static_cast<std::streamsize>(body.size()));
}

} // namespace

int main() {
    const std::string wav_path = "rms_import_fixture.wav";
    fixture(wav_path);

    rms::audio::AudioProjectManager projects(1);
    rms::audio::AudioImportManager importer(projects);
    std::string error;

    assert(importer.import_wav(wav_path, &error));
    assert(error.empty());
    assert(projects.project().has_audio());
    assert(projects.project().audio()->sample_rate() == 44100);
    assert(projects.project().audio()->channel_count() == 1);
    assert(projects.project().audio()->frame_count() == 2);
    assert(projects.project().waveform().size() == 2);
    assert(projects.project().state() == rms::audio::AudioProjectState::AudioLoaded);
    assert(projects.is_modified());

    // Explicit configuration controls waveform resolution.
    rms::audio::AudioImportConfig custom_config;
    custom_config.frames_per_waveform_point = 2;
    assert(importer.import_wav(wav_path, custom_config, &error));
    assert(projects.project().waveform().size() == 1);

    // Disabled WAV support and invalid settings are rejected transactionally.
    const std::size_t configured_frames = projects.project().audio()->frame_count();
    custom_config.allow_wav = false;
    assert(!importer.import_wav(wav_path, custom_config, &error));
    assert(!error.empty());
    assert(projects.project().audio()->frame_count() == configured_frames);

    custom_config.allow_wav = true;
    custom_config.frames_per_waveform_point = 0;
    assert(!importer.import_wav(wav_path, custom_config, &error));
    assert(!error.empty());
    assert(projects.project().audio()->frame_count() == configured_frames);

    // Per-extension policy rejects .wave when disabled.\n    custom_config.frames_per_waveform_point = 2;\n    custom_config.allow_wav = true;\n    custom_config.allow_wave = false;\n    assert(!importer.import_file(wave_path, custom_config, &error));\n    assert(!error.empty());\n    assert(projects.project().audio()->frame_count() == old_frames);\n    custom_config.allow_wave = true;\n\n    // Unknown extensions remain disabled by default.
    const std::size_t old_frames = projects.project().audio()->frame_count();
    assert(!importer.import_file("song.mp3", &error));
    assert(error.find("extension") != std::string::npos);
    assert(projects.project().audio()->frame_count() == old_frames);
    assert(projects.is_modified());

    // A WAV file with a .wave extension is accepted by generic import.
    const std::string wave_path = "rms_import_fixture.wave";
    fixture(wave_path);
    assert(importer.import_file(wave_path, &error));
    assert(error.empty());
    assert(projects.project().audio()->frame_count() == 2);

    // Missing supported files still fail without replacing the current project.
    assert(!importer.import_file("rms_missing_import.wav", &error));
    assert(!error.empty());
    assert(projects.project().audio()->frame_count() == old_frames);
    assert(projects.is_modified());
    std::remove(wave_path.c_str());

    std::remove(wav_path.c_str());
    std::cout << "AudioImportManager tests passed.\n";
    return 0;
}
