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

    // Batch import processes every path and reports each result.
    const std::string batch_wav_one = "rms_batch_one.wav";
    const std::string batch_wav_two = "rms_batch_two.wav";
    fixture(batch_wav_one);
    fixture(batch_wav_two);
    const std::vector<std::string> empty_paths;
    const rms::audio::AudioImportBatchReport empty_batch =
        importer.import_files(empty_paths);
    assert(empty_batch.total_count() == 0);
    assert(empty_batch.successful_count == 0);
    assert(empty_batch.failed_count == 0);
    assert(empty_batch.all_succeeded());

    std::vector<std::string> successful_paths;
    successful_paths.push_back(batch_wav_one);
    successful_paths.push_back(batch_wav_two);
    const rms::audio::AudioImportBatchReport successful_batch =
        importer.import_files(successful_paths);
    assert(successful_batch.total_count() == 2);
    assert(successful_batch.successful_count == 2);
    assert(successful_batch.failed_count == 0);
    assert(successful_batch.all_succeeded());
    assert(successful_batch.reports[0].success);
    assert(successful_batch.reports[1].success);

    std::vector<std::string> mixed_paths;
    mixed_paths.push_back(batch_wav_one);
    mixed_paths.push_back("rms_batch_missing.wav");
    mixed_paths.push_back("rms_batch_unknown.mp3");
    const rms::audio::AudioImportBatchReport mixed_batch =
        importer.import_files(mixed_paths);
    assert(mixed_batch.total_count() == 3);
    assert(mixed_batch.successful_count == 1);
    assert(mixed_batch.failed_count == 2);
    assert(!mixed_batch.all_succeeded());
    assert(mixed_batch.reports[0].success);
    assert(!mixed_batch.reports[1].success);
    assert(!mixed_batch.reports[2].success);
    assert(!mixed_batch.reports[1].error_message.empty());
    assert(!mixed_batch.reports[2].error_message.empty());

    // Preflight validates paths/configuration without importing or recording history.
    std::vector<std::string> preflight_paths;
    preflight_paths.push_back(batch_wav_one);
    preflight_paths.push_back(batch_wav_one);
    preflight_paths.push_back("rms_preflight_missing.wav");
    preflight_paths.push_back("rms_preflight_unknown.mp3");
    preflight_paths.push_back("");
    const std::size_t preflight_history_before = importer.history().size();
    const rms::audio::AudioImportPreflightReport preflight =
        importer.validate_files(preflight_paths);
    assert(preflight.total_count() == 5);
    assert(preflight.valid_count == 1);
    assert(preflight.rejected_count == 4);
    assert(!preflight.all_valid());
    assert(preflight.entries[0].valid);
    assert(!preflight.entries[1].valid);
    assert(preflight.entries[1].duplicate);
    assert(preflight.entries[2].error_message.find("cannot be opened") !=
           std::string::npos);
    assert(preflight.entries[3].error_message.find("extension") !=
           std::string::npos);
    assert(preflight.entries[4].error_message.find("empty") !=
           std::string::npos);
    assert(importer.history().size() == preflight_history_before);

    rms::audio::AudioImportConfig invalid_preflight_config;
    invalid_preflight_config.frames_per_waveform_point = 0;
    const rms::audio::AudioImportPreflightReport invalid_preflight =
        importer.validate_files(successful_paths, invalid_preflight_config);
    assert(invalid_preflight.valid_count == 0);
    assert(invalid_preflight.rejected_count == 2);
    assert(!invalid_preflight.entries[0].error_message.empty());

    const rms::audio::AudioImportPreflightReport valid_preflight =
        importer.validate_files(successful_paths);
    assert(valid_preflight.all_valid());
    assert(valid_preflight.valid_count == 2);
    assert(valid_preflight.rejected_count == 0);

    // Configurable batch limits, fail-fast, cancellation and progress.
    rms::audio::AudioImportBatchOptions limited_options;
    limited_options.max_files = 1;
    const rms::audio::AudioImportBatchReport limited_batch =
        importer.import_files(successful_paths, limited_options);
    assert(limited_batch.processed_count() == 1);
    assert(limited_batch.pending_count == 1);
    assert(limited_batch.limit_reached);
    assert(!limited_batch.cancelled);

    rms::audio::AudioImportBatchOptions fail_fast_options;
    fail_fast_options.stop_on_error = true;
    const rms::audio::AudioImportBatchReport fail_fast_batch =
        importer.import_files(mixed_paths, fail_fast_options);
    assert(fail_fast_batch.processed_count() == 2);
    assert(fail_fast_batch.successful_count == 1);
    assert(fail_fast_batch.failed_count == 1);
    assert(fail_fast_batch.pending_count == 1);
    assert(fail_fast_batch.stopped_on_error);

    bool cancel_batch = false;
    rms::audio::AudioImportBatchOptions cancel_options;
    cancel_options.cancel_requested = &cancel_batch;
    std::size_t progress_calls = 0;
    cancel_options.progress_callback =
        [&cancel_batch, &progress_calls](std::size_t processed,
                                         std::size_t pending) {
            ++progress_calls;
            assert(processed == 1);
            assert(pending == 1);
            cancel_batch = true;
        };
    const rms::audio::AudioImportBatchReport cancelled_batch =
        importer.import_files(successful_paths, cancel_options);
    assert(cancelled_batch.processed_count() == 1);
    assert(cancelled_batch.pending_count == 1);
    assert(cancelled_batch.cancelled);
    assert(progress_calls == 1);

    bool cancel_before_start = true;
    rms::audio::AudioImportBatchOptions pre_cancel_options;
    pre_cancel_options.cancel_requested = &cancel_before_start;
    const rms::audio::AudioImportBatchReport pre_cancelled_batch =
        importer.import_files(successful_paths, pre_cancel_options);
    assert(pre_cancelled_batch.processed_count() == 0);
    assert(pre_cancelled_batch.pending_count == 2);
    assert(pre_cancelled_batch.cancelled);

    std::remove(batch_wav_one.c_str());
    std::remove(batch_wav_two.c_str());

    std::remove(history_path.c_str());
    std::remove(wav_path.c_str());
    std::cout << "AudioImportManager tests passed.\n";
    return 0;
}
