#include "rms/audio/audio_playback.h"

#include <algorithm>

namespace rms {
namespace audio {

AudioPlayback::AudioPlayback()
    : buffer_(0),
      state_(PlaybackState::Stopped),
      position_frame_(0) {
}

void AudioPlayback::load(const AudioBuffer& buffer, bool enable_output) {
    AudioSettings settings;
    settings.sample_rate = buffer.sample_rate();
    settings.channel_count = buffer.channel_count();
    load(buffer, settings, enable_output);
}

void AudioPlayback::load(const AudioBuffer& buffer,
                         const AudioSettings& settings,
                         bool enable_output) {
    output_.close();
    buffer_ = &buffer;
    position_frame_ = 0;
    state_ = PlaybackState::Stopped;
    if (enable_output) {
        output_.open(buffer, settings);
    }
}

void AudioPlayback::unload() {
    output_.close();
    buffer_ = 0;
    position_frame_ = 0;
    state_ = PlaybackState::Stopped;
}

void AudioPlayback::play() {
    if (buffer_ == 0 || buffer_->frame_count() == 0) {
        state_ = PlaybackState::Stopped;
        position_frame_ = 0;
        return;
    }

    if (position_frame() >= buffer_->frame_count()) {
        seek(0);
    }
    if (output_.is_open()) {
        if (!output_.start()) {
            state_ = PlaybackState::Stopped;
            return;
        }
    }
    state_ = PlaybackState::Playing;
}

void AudioPlayback::pause() {
    if (state_ == PlaybackState::Playing) {
        output_.pause();
        position_frame_ = position_frame();
        state_ = PlaybackState::Paused;
    }
}

void AudioPlayback::stop() {
    output_.stop();
    position_frame_ = 0;
    state_ = PlaybackState::Stopped;
}

void AudioPlayback::seek(std::size_t frame) {
    if (buffer_ == 0) {
        position_frame_ = 0;
        state_ = PlaybackState::Stopped;
        return;
    }

    const std::size_t bounded = std::min(frame, buffer_->frame_count());
    output_.seek(bounded);
    position_frame_ = bounded;
    if (bounded == buffer_->frame_count()) {
        state_ = PlaybackState::Stopped;
    }
}

bool AudioPlayback::has_audio() const {
    return buffer_ != 0;
}

bool AudioPlayback::has_output() const {
    return output_.is_open();
}

std::string AudioPlayback::output_error() const {
    return output_.last_error();
}

PlaybackState AudioPlayback::state() const {
    if (state_ == PlaybackState::Playing && output_.is_open() &&
        !output_.is_running() &&
        output_.position_frame() >= duration_frames()) {
        return PlaybackState::Stopped;
    }
    return state_;
}

std::size_t AudioPlayback::position_frame() const {
    return output_.is_open() ? output_.position_frame() : position_frame_;
}

std::size_t AudioPlayback::duration_frames() const {
    return buffer_ == 0 ? 0 : buffer_->frame_count();
}

double AudioPlayback::position_seconds() const {
    if (buffer_ == 0 || buffer_->sample_rate() == 0) {
        return 0.0;
    }
    return static_cast<double>(position_frame()) /
           static_cast<double>(buffer_->sample_rate());
}

void AudioPlayback::advance(std::size_t frames) {
    if (state_ != PlaybackState::Playing || output_.is_open() || buffer_ == 0) {
        return;
    }

    const std::size_t remaining = buffer_->frame_count() - position_frame_;
    if (frames >= remaining) {
        position_frame_ = buffer_->frame_count();
        state_ = PlaybackState::Stopped;
    } else {
        position_frame_ += frames;
    }
}

} // namespace audio
} // namespace rms
