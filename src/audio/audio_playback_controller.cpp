#include "rms/audio/audio_playback_controller.h"

namespace rms {
namespace audio {

AudioPlaybackController::AudioPlaybackController(
    AudioSettingsManager& settings_manager)
    : settings_manager_(settings_manager) {
}

void AudioPlaybackController::load(const AudioBuffer& buffer,
                                   bool enable_output) {
    playback_.load(buffer, settings_manager_, enable_output);
}

void AudioPlaybackController::unload() {
    playback_.unload();
}

void AudioPlaybackController::play() {
    playback_.play();
}

void AudioPlaybackController::pause() {
    playback_.pause();
}

void AudioPlaybackController::stop() {
    playback_.stop();
}

void AudioPlaybackController::seek(std::size_t frame) {
    playback_.seek(frame);
}

void AudioPlaybackController::advance(std::size_t frames) {
    playback_.advance(frames);
}

bool AudioPlaybackController::has_audio() const {
    return playback_.has_audio();
}

bool AudioPlaybackController::has_output() const {
    return playback_.has_output();
}

PlaybackState AudioPlaybackController::state() const {
    return playback_.state();
}

std::size_t AudioPlaybackController::position_frame() const {
    return playback_.position_frame();
}

std::size_t AudioPlaybackController::duration_frames() const {
    return playback_.duration_frames();
}

double AudioPlaybackController::position_seconds() const {
    return playback_.position_seconds();
}

std::string AudioPlaybackController::output_error() const {
    return playback_.output_error();
}

} // namespace audio
} // namespace rms
