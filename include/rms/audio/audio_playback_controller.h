#ifndef RMS_AUDIO_AUDIO_PLAYBACK_CONTROLLER_H
#define RMS_AUDIO_AUDIO_PLAYBACK_CONTROLLER_H

#include "rms/audio/audio_playback.h"
#include "rms/audio/audio_settings_manager.h"

#include <cstddef>
#include <string>

namespace rms {
namespace audio {

// Application-facing transport facade. The settings manager must outlive this
// controller; its current settings are used whenever audio is loaded.
class AudioPlaybackController {
public:
    explicit AudioPlaybackController(AudioSettingsManager& settings_manager);

    void load(const AudioBuffer& buffer, bool enable_output = true);
    void unload();
    void play();
    void pause();
    void stop();
    void seek(std::size_t frame);
    void advance(std::size_t frames);

    bool has_audio() const;
    bool has_output() const;
    PlaybackState state() const;
    std::size_t position_frame() const;
    std::size_t duration_frames() const;
    double position_seconds() const;
    std::string output_error() const;

private:
    AudioSettingsManager& settings_manager_;
    AudioPlayback playback_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PLAYBACK_CONTROLLER_H
