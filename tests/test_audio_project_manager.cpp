#include "rms/audio/audio_project_manager.h"
#include "rms/audio/audio_buffer.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioProjectManager;
using rms::audio::AudioProjectState;

int main() {
    const char* first_path = "rms_manager_first.rms";
    const char* second_path = "rms_manager_second.rms";
    std::string error;

    AudioProjectManager manager(2);
    assert(!manager.is_modified());
    assert(!manager.has_file_path());
    assert(manager.project().state() == AudioProjectState::Empty);

    // Saving a new, empty project is rejected and does not assign a path.
    assert(!manager.save_project(&error));
    assert(!manager.has_file_path());
    assert(!error.empty());

    const std::vector<float> samples = {
        -0.75f, 0.75f, -0.25f, 0.25f
    };
    const AudioBuffer buffer(44100, 2, samples);
    manager.project().load_audio(buffer, 2);
    manager.mark_modified();
    assert(manager.is_modified());

    assert(manager.save_project(&error) == false);
    assert(manager.save_project_as(first_path, &error));
    assert(!manager.is_modified());
    assert(manager.has_file_path());
    assert(manager.file_path() == first_path);

    manager.mark_modified();
    assert(manager.is_modified());
    assert(manager.save_project(&error));
    assert(!manager.is_modified());

    // Failed open must preserve the loaded project, path, and dirty state.
    manager.mark_modified();
    const std::string old_path = manager.file_path();
    assert(!manager.open_project("rms_file_that_does_not_exist.rms", &error));
    assert(manager.is_modified());
    assert(manager.file_path() == old_path);
    assert(manager.project().audio()->sample_rate() == 44100);

    // Open a valid project in a separate manager and verify clean state.
    AudioProjectManager opened(2);
    assert(opened.open_project(first_path, &error));
    assert(!opened.is_modified());
    assert(opened.file_path() == first_path);
    assert(opened.project().state() == AudioProjectState::AudioLoaded);

    // Save As changes the active path only after a successful write.
    assert(opened.save_project_as(second_path, &error));
    assert(opened.file_path() == second_path);
    assert(!opened.is_modified());

    // New project clears audio, path, and dirty state.
    opened.mark_modified();
    assert(opened.new_project());
    assert(opened.project().state() == AudioProjectState::Empty);
    assert(!opened.has_file_path());
    assert(!opened.is_modified());

    // Empty path and empty project cannot be saved.
    assert(!opened.save_project_as("", &error));
    assert(!opened.has_file_path());

    std::remove(first_path);
    std::remove(second_path);
    std::cout << "AudioProjectManager tests passed." << std::endl;
    return 0;
}
