#ifndef RMS_AUDIO_AUDIO_PLAYBACK_H
#define RMS_AUDIO_AUDIO_PLAYBACK_H

#include "rms/audio/audio_buffer.h"
#include "rms/audio/audio_output.h"
#include "rms/audio/audio_settings.h"
#include "rms/audio/audio_settings_manager.h"

#include <cstddef>
#include <string>

namespace rms {
namespace audio {

enum class PlaybackState {
    Stopped,
    Playing,
    Paused
};

// Transport controller. Uses AudioOutput when a PortAudio backend is available;
// retains deterministic frame advancement for backend-free unit tests.
class AudioPlayback {
public:
    AudioPlayback();

    void load(const AudioBuffer& buffer, bool enable_output = true);
    void load(const AudioBuffer& buffer, const AudioSettings& settings,
              bool enable_output = true);
    void load(const AudioBuffer& buffer,
              const AudioSettingsManager& settings_manager,
              bool enable_output = true);
    void unload();
    void play();
    void pause();
    void stop();
    void seek(std::size_t frame);

    bool has_audio() const;
    bool has_output() const;
    std::string output_error() const;
    PlaybackState state() const;
    std::size_t position_frame() const;
    std::size_t duration_frames() const;
    double position_seconds() const;

    // Advances position only when running without a physical output backend.
    void advance(std::size_t frames);

private:
    const AudioBuffer* buffer_;
    AudioOutput output_;
    PlaybackState state_;
    std::size_t position_frame_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PLAYBACK_H
