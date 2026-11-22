#include "rms/audio/audio_playback.h"

#include <algorithm>

namespace rms {
namespace audio {

AudioPlayback::AudioPlayback()
    : buffer_(0),
      state_(PlaybackState::Stopped),
      position_frame_(0) {
}

void AudioPlayback::load(const AudioBuffer& buffer) {
    buffer_ = &buffer;
    position_frame_ = 0;
    state_ = PlaybackState::Stopped;
}

void AudioPlayback::unload() {
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

    if (position_frame_ >= buffer_->frame_count()) {
        position_frame_ = 0;
    }
    state_ = PlaybackState::Playing;
}

void AudioPlayback::pause() {
    if (state_ == PlaybackState::Playing) {
        state_ = PlaybackState::Paused;
    }
}

void AudioPlayback::stop() {
    state_ = PlaybackState::Stopped;
    position_frame_ = 0;
}

void AudioPlayback::seek(std::size_t frame) {
    if (buffer_ == 0) {
        position_frame_ = 0;
        state_ = PlaybackState::Stopped;
        return;
    }

    position_frame_ = std::min(frame, buffer_->frame_count());
    if (position_frame_ == buffer_->frame_count()) {
        state_ = PlaybackState::Stopped;
    }
}

bool AudioPlayback::has_audio() const {
    return buffer_ != 0;
}

PlaybackState AudioPlayback::state() const {
    return state_;
}

std::size_t AudioPlayback::position_frame() const {
    return position_frame_;
}

std::size_t AudioPlayback::duration_frames() const {
    return buffer_ == 0 ? 0 : buffer_->frame_count();
}

double AudioPlayback::position_seconds() const {
    if (buffer_ == 0 || buffer_->sample_rate() == 0) {
        return 0.0;
    }
    return static_cast<double>(position_frame_) /
           static_cast<double>(buffer_->sample_rate());
}

void AudioPlayback::advance(std::size_t frames) {
    if (state_ != PlaybackState::Playing || buffer_ == 0) {
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
