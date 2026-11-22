#include "rms/audio/audio_project.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

using rms::audio::AudioBuffer;
using rms::audio::AudioProject;
using rms::audio::AudioProjectState;

int main() {
    AudioProject project;
    assert(project.state() == AudioProjectState::Empty);
    assert(!project.has_audio());
    assert(!project.has_analysis());
    assert(project.audio() == 0);
    assert(project.waveform().empty());
    assert(project.viewport().total_frames() == 0);

    bool analyze_empty_threw = false;
    try {
        project.analyze();
    } catch (const std::logic_error&) {
        analyze_empty_threw = true;
    }
    assert(analyze_empty_threw);

    const AudioBuffer buffer(8192, 1, std::vector<float>(8192, 0.0f));
    project.load_audio(buffer, 1024);
    assert(project.state() == AudioProjectState::AudioLoaded);
    assert(project.has_audio());
    assert(!project.has_analysis());
    assert(project.audio() != 0);
    assert(project.audio()->frame_count() == 8192);
    assert(project.waveform().size() == 8);
    assert(project.viewport().total_frames() == 8192);
    assert(project.viewport().visible_frames() == 8192);

    project.viewport().set_selection(100, 500);
    project.analyze();
    assert(project.state() == AudioProjectState::Analyzed);
    assert(project.has_analysis());
    assert(project.analysis().metrics.peak_amplitude == 0.0f);
    assert(project.analysis().timeline.empty());

    const AudioBuffer replacement(44100, 1, std::vector<float>(4410, 0.25f));
    project.load_audio(replacement, 441);
    assert(project.state() == AudioProjectState::AudioLoaded);
    assert(!project.has_analysis());
    assert(project.audio()->sample_rate() == 44100);
    assert(project.waveform().size() == 10);
    assert(!project.viewport().has_selection());
    assert(project.viewport().total_frames() == 4410);

    bool invalid_waveform_threw = false;
    try {
        project.load_audio(buffer, 0);
    } catch (const std::invalid_argument&) {
        invalid_waveform_threw = true;
    }
    assert(invalid_waveform_threw);
    assert(project.audio()->sample_rate() == 44100);

    project.clear_audio();
    assert(project.state() == AudioProjectState::Empty);
    assert(!project.has_audio());
    assert(!project.has_analysis());
    assert(project.waveform().empty());
    assert(project.viewport().total_frames() == 0);

    std::cout << "AudioProject tests passed." << std::endl;
    return 0;
}
