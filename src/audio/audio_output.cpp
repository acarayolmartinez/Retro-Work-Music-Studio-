#include "rms/audio/audio_output.h"

#include <algorithm>
#include <atomic>
#include <sstream>
#include <vector>

#ifdef RMS_ENABLE_PORTAUDIO
#include <portaudio.h>
#endif

namespace rms {
namespace audio {

struct AudioOutput::Impl {
    Impl()
        : open(false), running(false), position(0)
#ifdef RMS_ENABLE_PORTAUDIO
        , stream(0), initialized(false)
#endif
    {
    }

    std::vector<float> samples;
    std::uint32_t sample_rate;
    std::uint16_t channels;
    std::size_t frames;
    std::atomic<bool> open;
    std::atomic<bool> running;
    std::atomic<std::size_t> position;
    std::string error;
#ifdef RMS_ENABLE_PORTAUDIO
    PaStream* stream;
    bool initialized;

    static int callback(const void*, void* output,
                        unsigned long frame_count,
                        const PaStreamCallbackTimeInfo*,
                        PaStreamCallbackFlags,
                        void* user_data) {
        Impl* self = static_cast<Impl*>(user_data);
        float* destination = static_cast<float*>(output);
        const std::size_t channel_count = self->channels;
        const std::size_t current = self->position.load();

        if (!self->running.load() || current >= self->frames) {
            std::fill(destination,
                      destination + frame_count * channel_count, 0.0f);
            if (current >= self->frames) {
                self->running.store(false);
            }
            return current >= self->frames ? paComplete : paContinue;
        }

        const std::size_t available = self->frames - current;
        const std::size_t copied = std::min<std::size_t>(
            available, static_cast<std::size_t>(frame_count));
        const std::size_t sample_offset = current * channel_count;
        const std::size_t sample_count = copied * channel_count;

        std::copy(self->samples.begin() + sample_offset,
                  self->samples.begin() + sample_offset + sample_count,
                  destination);
        std::fill(destination + sample_count,
                  destination + frame_count * channel_count, 0.0f);

        self->position.store(current + copied);
        if (copied < static_cast<std::size_t>(frame_count)) {
            self->running.store(false);
        }
        return paContinue;
    }
#endif
};

AudioOutput::AudioOutput()
    : impl_(new Impl()) {
    impl_->sample_rate = 0;
    impl_->channels = 0;
    impl_->frames = 0;
}

AudioOutput::~AudioOutput() {
    close();
}

bool AudioOutput::open(const AudioBuffer& buffer) {
    close();
    if (buffer.frame_count() == 0) {
        impl_->error = "Cannot open an empty audio buffer.";
        return false;
    }

#ifndef RMS_ENABLE_PORTAUDIO
    (void)buffer;
    impl_->error = "RMS was built without PortAudio support. "
                   "Enable RMS_ENABLE_PORTAUDIO in CMake.";
    return false;
#else
    impl_->samples = buffer.samples();
    impl_->sample_rate = buffer.sample_rate();
    impl_->channels = buffer.channel_count();
    impl_->frames = buffer.frame_count();
    impl_->position.store(0);

    PaError result = Pa_Initialize();
    if (result != paNoError) {
        impl_->error = Pa_GetErrorText(result);
        impl_->samples.clear();
        return false;
    }
    impl_->initialized = true;

    result = Pa_OpenDefaultStream(
        &impl_->stream,
        0,
        static_cast<int>(impl_->channels),
        paFloat32,
        static_cast<double>(impl_->sample_rate),
        paFramesPerBufferUnspecified,
        &Impl::callback,
        impl_.get());

    if (result != paNoError) {
        impl_->error = Pa_GetErrorText(result);
        close();
        return false;
    }

    impl_->open.store(true);
    impl_->error.clear();
    return true;
#endif
}

bool AudioOutput::start() {
#ifdef RMS_ENABLE_PORTAUDIO
    if (!impl_->open.load() || impl_->stream == 0) {
        impl_->error = "Audio output is not open.";
        return false;
    }
    if (impl_->position.load() >= impl_->frames) {
        impl_->position.store(0);
    }
    impl_->running.store(true);
    const PaError result = Pa_StartStream(impl_->stream);
    if (result != paNoError && result != paStreamIsNotStopped) {
        impl_->running.store(false);
        impl_->error = Pa_GetErrorText(result);
        return false;
    }
    impl_->error.clear();
    return true;
#else
    impl_->error = "PortAudio support is not enabled.";
    return false;
#endif
}

void AudioOutput::pause() {
#ifdef RMS_ENABLE_PORTAUDIO
    if (impl_->stream != 0 && impl_->running.load()) {
        impl_->running.store(false);
        Pa_StopStream(impl_->stream);
    }
#endif
}

void AudioOutput::stop() {
#ifdef RMS_ENABLE_PORTAUDIO
    if (impl_->stream != 0 && Pa_IsStreamActive(impl_->stream) == 1) {
        impl_->running.store(false);
        Pa_StopStream(impl_->stream);
    }
#endif
    impl_->position.store(0);
}

void AudioOutput::seek(std::size_t frame) {
    if (!impl_->open.load()) {
        return;
    }
    const std::size_t bounded = std::min(frame, impl_->frames);
    impl_->position.store(bounded);
    if (bounded >= impl_->frames) {
        impl_->running.store(false);
#ifdef RMS_ENABLE_PORTAUDIO
        if (impl_->stream != 0 && Pa_IsStreamActive(impl_->stream) == 1) {
            Pa_StopStream(impl_->stream);
        }
#endif
    }
}

void AudioOutput::close() {
#ifdef RMS_ENABLE_PORTAUDIO
    impl_->running.store(false);
    if (impl_->stream != 0) {
        if (Pa_IsStreamActive(impl_->stream) == 1) {
            Pa_StopStream(impl_->stream);
        }
        Pa_CloseStream(impl_->stream);
        impl_->stream = 0;
    }
    if (impl_->initialized) {
        Pa_Terminate();
        impl_->initialized = false;
    }
#endif
    impl_->open.store(false);
    impl_->position.store(0);
    impl_->samples.clear();
    impl_->frames = 0;
}

bool AudioOutput::is_open() const {
    return impl_->open.load();
}

bool AudioOutput::is_running() const {
    return impl_->running.load();
}

std::size_t AudioOutput::position_frame() const {
    return impl_->position.load();
}

std::string AudioOutput::last_error() const {
    return impl_->error;
}

} // namespace audio
} // namespace rms
