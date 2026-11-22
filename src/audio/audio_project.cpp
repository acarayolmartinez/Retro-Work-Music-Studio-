#include "rms/audio/audio_project.h"

#include <memory>
#include <stdexcept>

namespace rms {
namespace audio {

AudioProject::AudioProject()
    : audio_(),
      analysis_(),
      waveform_(),
      viewport_(),
      state_(AudioProjectState::Empty) {
}

void AudioProject::load_audio(const AudioBuffer& buffer,
                              std::size_t frames_per_waveform_point) {
    // Prepare derived data before replacing the current project contents.
    const std::vector<WaveformPoint> new_waveform =
        WaveformData::generate(buffer, frames_per_waveform_point);
    std::unique_ptr<AudioBuffer> new_audio(new AudioBuffer(buffer));

    audio_.swap(new_audio);
    waveform_ = new_waveform;
    analysis_ = AudioAnalysisResult();
    viewport_.set_audio_length(audio_->frame_count());
    state_ = AudioProjectState::AudioLoaded;
}

void AudioProject::clear_audio() {
    audio_.reset();
    waveform_.clear();
    analysis_ = AudioAnalysisResult();
    viewport_.set_audio_length(0);
    state_ = AudioProjectState::Empty;
}

void AudioProject::analyze(const AudioAnalysisConfig& config) {
    if (!audio_) {
        throw std::logic_error("Cannot analyze a project without audio");
    }

    AudioAnalysisResult new_analysis =
        AudioAnalysisPipeline::analyze(*audio_, config);
    analysis_ = new_analysis;
    state_ = AudioProjectState::Analyzed;
}

AudioProjectState AudioProject::state() const {
    return state_;
}

bool AudioProject::has_audio() const {
    return static_cast<bool>(audio_);
}

bool AudioProject::has_analysis() const {
    return state_ == AudioProjectState::Analyzed;
}

const AudioBuffer* AudioProject::audio() const {
    return audio_.get();
}

const AudioAnalysisResult& AudioProject::analysis() const {
    return analysis_;
}

const std::vector<WaveformPoint>& AudioProject::waveform() const {
    return waveform_;
}

WaveformViewport& AudioProject::viewport() {
    return viewport_;
}

const WaveformViewport& AudioProject::viewport() const {
    return viewport_;
}

} // namespace audio
} // namespace rms
