#ifndef RMS_AUDIO_AUDIO_OUTPUT_H
#define RMS_AUDIO_AUDIO_OUTPUT_H

#include "rms/audio/audio_buffer.h"
#include "rms/audio/audio_settings.h"

#include <cstddef>
#include <memory>
#include <string>

namespace rms {
namespace audio {

// Real-time audio output backend. Requires a build with RMS_ENABLE_PORTAUDIO.
class AudioOutput {
public:
    AudioOutput();
    ~AudioOutput();

    AudioOutput(const AudioOutput&) = delete;
    AudioOutput& operator=(const AudioOutput&) = delete;

    // Uses the system default output device when device_index is -1.
    // A non-negative value selects the PortAudio device index.
    bool open(const AudioBuffer& buffer, int device_index = -1);
    bool open(const AudioBuffer& buffer, const AudioSettings& settings);
    bool start();
    void pause();
    void stop();
    void seek(std::size_t frame);
    void close();

    bool is_open() const;
    bool is_running() const;
    std::size_t position_frame() const;
    std::string last_error() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_OUTPUT_H
