#include "rms/audio/audio_settings_manager.h"
#include "rms/audio/audio_import_manager.h"
#include "rms/audio/audio_project_manager.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

int main() {
    const std::string path = "rms_audio_settings_manager_test.cfg";
    const std::string history_path = "rms_settings_history_test.rmslog";
    std::string error;
    rms::audio::AudioSettingsManager manager;

    assert(manager.settings().output_device_index == -1);
    assert(manager.settings().sample_rate == 44100);
    assert(manager.settings().import_history_path == "rms_import_history.rmslog");
    assert(manager.settings().import_history_auto_save);

    rms::audio::AudioSettings custom;
    custom.output_device_index = 2;
    custom.sample_rate = 48000;
    custom.channel_count = 1;
    custom.frames_per_buffer = 256;
    custom.import_history_path = history_path;
    custom.import_history_auto_save = false;
    assert(manager.set_settings(custom, &error));
    assert(error.empty());
    assert(manager.settings().sample_rate == 48000);

    rms::audio::AudioSettings invalid = custom;
    invalid.sample_rate = 0;
    assert(!manager.set_settings(invalid, &error));
    assert(!error.empty());
    assert(manager.settings().sample_rate == 48000);

    assert(manager.save(path, &error));
    manager.reset_defaults();
    assert(manager.settings().sample_rate == 44100);
    assert(manager.load(path, &error));
    assert(manager.settings().output_device_index == 2);
    assert(manager.settings().sample_rate == 48000);
    assert(manager.settings().channel_count == 1);
    assert(manager.settings().frames_per_buffer == 256);
    assert(manager.settings().import_history_path == history_path);
    assert(!manager.settings().import_history_auto_save);

    rms::audio::AudioProjectManager projects(1);
    rms::audio::AudioImportManager importer(projects);
    assert(manager.configure_import_history(importer, &error));
    assert(error.empty());
    assert(importer.history_store_path() == history_path);
    assert(!importer.history_auto_save());

    // First-run configuration creates an empty history file.
    std::ifstream created(history_path.c_str(), std::ios::in | std::ios::binary);
    assert(created.good());
    created.close();

    // Disabled auto-save keeps the in-memory history but does not write it.
    assert(!importer.import_file("not-supported.mp3", &error));
    assert(importer.history().size() == 1);

    rms::audio::AudioProjectManager restored_projects(1);
    rms::audio::AudioImportManager restored(restored_projects);
    assert(restored.configure_history_store(history_path, true, &error));
    assert(restored.history().empty());

    // Enabling auto-save persists subsequent attempts.
    custom.import_history_auto_save = true;
    assert(manager.set_settings(custom, &error));
    assert(manager.configure_import_history(importer, &error));
    assert(importer.history_auto_save());
    assert(!importer.import_file("another-unsupported.mp3", &error));

    rms::audio::AudioProjectManager final_projects(1);
    rms::audio::AudioImportManager final_importer(final_projects);
    assert(final_importer.configure_history_store(history_path, true, &error));
    assert(final_importer.history().size() == 2);

    // A corrupt existing history must not be silently overwritten.
    {
        std::ofstream corrupt(history_path.c_str(), std::ios::out | std::ios::trunc);
        corrupt << "broken history";
    }
    rms::audio::AudioProjectManager corrupt_projects(1);
    rms::audio::AudioImportManager corrupt_importer(corrupt_projects);
    assert(!manager.configure_import_history(corrupt_importer, &error));
    assert(!error.empty());

    {
        std::ofstream file(path.c_str(), std::ios::trunc);
        file << "bad settings";
    }
    assert(!manager.load(path, &error));
    assert(!error.empty());
    assert(manager.settings().sample_rate == 48000);

    manager.reset_defaults();
    assert(manager.settings().is_valid());
    assert(manager.settings().output_device_index == -1);
    assert(manager.settings().sample_rate == 44100);
    assert(manager.settings().channel_count == 2);
    assert(manager.settings().frames_per_buffer == 0);

    std::remove(path.c_str());
    std::remove(history_path.c_str());
    return 0;
}
