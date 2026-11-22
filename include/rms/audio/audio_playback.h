#ifndef RMS_AUDIO_AUDIO_PLAYBACK_H
#define RMS_AUDIO_AUDIO_PLAYBACK_H

#include "rms/audio/audio_buffer.h"

#include <cstddef>

namespace rms {
namespace audio {

enum class PlaybackState {
    Stopped,
    Playing,
    Paused
};

// Transport state for an audio buffer. Device output is provided by a backend.
class AudioPlayback {
public:
    AudioPlayback();

    void load(const AudioBuffer& buffer);
    void unload();
    void play();
    void pause();
    void stop();
    void seek(std::size_t frame);

    bool has_audio() const;
    PlaybackState state() const;
    std::size_t position_frame() const;
    std::size_t duration_frames() const;
    double position_seconds() const;

    // Advances transport position as if frames had been consumed by an output
    // device. Stops automatically at the end of the loaded buffer.
    void advance(std::size_t frames);

private:
    const AudioBuffer* buffer_;
    PlaybackState state_;
    std::size_t position_frame_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PLAYBACK_H
