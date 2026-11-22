#ifndef RMS_AUDIO_AUDIO_PROJECT_H
#define RMS_AUDIO_AUDIO_PROJECT_H

#include "rms/audio/audio_analysis_pipeline.h"
#include "rms/audio/waveform_data.h"
#include "rms/audio/waveform_viewport.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace rms {
namespace audio {

enum class AudioProjectState {
    Empty,
    AudioLoaded,
    Analyzed
};

// Owns the source audio and the data derived for one editing session.
class AudioProject {
public:
    AudioProject();

    void load_audio(const AudioBuffer& buffer,
                    std::size_t frames_per_waveform_point);
    void clear_audio();
    void analyze(const AudioAnalysisConfig& config = AudioAnalysisConfig());

    AudioProjectState state() const;
    bool has_audio() const;
    bool has_analysis() const;
    const AudioBuffer* audio() const;
    const AudioAnalysisResult& analysis() const;
    const std::vector<WaveformPoint>& waveform() const;
    WaveformViewport& viewport();
    const WaveformViewport& viewport() const;

private:
    std::unique_ptr<AudioBuffer> audio_;
    AudioAnalysisResult analysis_;
    std::vector<WaveformPoint> waveform_;
    WaveformViewport viewport_;
    AudioProjectState state_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_PROJECT_H
