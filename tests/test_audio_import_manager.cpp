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

    // Structured report includes file identity and decoded audio properties.
    const rms::audio::AudioImportReport success_report =
        importer.import_file_with_report(wav_path);
    assert(success_report.success);
    assert(success_report.file_path == wav_path);
    assert(success_report.file_name == wav_path);
    assert(success_report.file_extension == ".wav");
    assert(success_report.detected_format == "WAV");
    assert(success_report.error_message.empty());
    assert(success_report.sample_rate == 44100);
    assert(success_report.channel_count == 1);
    assert(success_report.duration_seconds > 0.0);

    const rms::audio::AudioImportReport unsupported_report =
        importer.import_file_with_report("folder/unknown.mp3");
    assert(!unsupported_report.success);
    assert(unsupported_report.file_name == "unknown.mp3");
    assert(unsupported_report.file_extension == ".mp3");
    assert(unsupported_report.detected_format == "Unknown");
    assert(!unsupported_report.error_message.empty());
    assert(unsupported_report.sample_rate == 0);

    // Explicit configuration controls waveform resolution.
    rms::audio::AudioImportConfig custom_config;
    custom_config.frames_per_waveform_point = 2;
    assert(importer.import_wav(wav_path, custom_config, &error));
    assert(projects.project().waveform().size() == 1);
    // Three earlier attempts plus this successful configured import.
    assert(importer.history().size() == 4);
    assert(importer.history().latest() != 0);
    assert(importer.history().latest()->success);
    assert(importer.successful_import_count() == 3);
    assert(importer.failed_import_count() == 1);
    assert(importer.recent_imports(2).size() == 2);
    assert(importer.recent_imports(1)[0].success);
    assert(importer.find_imports("FIXTURE.WAV").size() == 3);
    assert(importer.find_imports("not-present").empty());

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

    // Unknown extensions remain disabled by default.
    const std::size_t old_frames = projects.project().audio()->frame_count();
    const std::size_t history_before_failure = importer.history().size();
    assert(!importer.import_file("song.mp3", &error));
    assert(importer.history().size() == history_before_failure + 1);
    assert(!importer.history().latest()->success);
    assert(error.find("extension") != std::string::npos);
    assert(projects.project().audio()->frame_count() == old_frames);
    assert(projects.is_modified());

    // A WAV file with a .wave extension is accepted by generic import.
    const std::string wave_path = "rms_import_fixture.wave";
    fixture(wave_path);

    // Per-extension policy rejects .wave when disabled.
    custom_config.frames_per_waveform_point = 2;
    custom_config.allow_wav = true;
    custom_config.allow_wave = false;
    assert(!importer.import_file(wave_path, custom_config, &error));
    assert(!error.empty());
    assert(projects.project().audio()->frame_count() == old_frames);
    custom_config.allow_wave = true;

    custom_config.allow_unknown_extensions = true;
    assert(!importer.import_file("song.mp3", custom_config, &error));
    assert(error.find("not implemented") != std::string::npos);
    assert(projects.project().audio()->frame_count() == old_frames);
    custom_config.allow_unknown_extensions = false;

    assert(importer.import_file(wave_path, &error));
    assert(error.empty());
    assert(projects.project().audio()->frame_count() == 2);

    // Missing supported files still fail without replacing the current project.
    assert(!importer.import_file("rms_missing_import.wav", &error));
    assert(!error.empty());
    assert(projects.project().audio()->frame_count() == old_frames);
    assert(projects.is_modified());
    std::remove(wave_path.c_str());

    // Automatic persistence stores every subsequent import attempt.
    const std::string history_path = "rms_import_manager_history.rmslog";
    assert(importer.configure_history_store(history_path, false, &error));
    assert(error.empty());
    assert(importer.save_history(&error));
    assert(error.empty());

    const std::size_t before_persisted_attempt = importer.history().size();
    assert(!importer.import_file("persisted-unsupported.mp3", &error));
    assert(importer.history().size() == before_persisted_attempt + 1);
    assert(importer.history_store_error().empty());

    rms::audio::AudioProjectManager restored_projects(1);
    rms::audio::AudioImportManager restored_importer(restored_projects);
    assert(restored_importer.configure_history_store(history_path, true, &error));
    assert(error.empty());
    assert(restored_importer.history().size() == importer.history().size());
    assert(restored_importer.history().latest() != 0);
    assert(restored_importer.history().latest()->file_name ==
           "persisted-unsupported.mp3");

    // A persistence error is exposed separately and does not undo an import.
    assert(importer.configure_history_store(".", false, &error));
    const rms::audio::AudioImportReport import_with_store_error =
        importer.import_file_with_report(wav_path);
    assert(import_with_store_error.success);
    assert(projects.project().has_audio());
    assert(!importer.history_store_error().empty());

    // Clearing persists the empty history when a working store is configured.
    assert(importer.configure_history_store(history_path, true, &error));
    assert(importer.clear_history(&error));
    assert(error.empty());
    assert(importer.history().empty());
    rms::audio::AudioProjectManager cleared_projects(1);
    rms::audio::AudioImportManager cleared_importer(cleared_projects);
    assert(cleared_importer.configure_history_store(history_path, true, &error));
    assert(cleared_importer.history().empty());

    // Failed persistence restores the in-memory entries.
    importer.import_file_with_report("clear-rollback.mp3");
    const std::size_t before_failed_clear = importer.history().size();
    assert(importer.configure_history_store(".", false, &error));
    assert(!importer.clear_history(&error));
    assert(importer.history().size() == before_failed_clear);
    assert(!error.empty());

    std::remove(history_path.c_str());
    std::remove(wav_path.c_str());
    std::cout << "AudioImportManager tests passed.\n";
    return 0;
}
