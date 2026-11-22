#ifndef RMS_AUDIO_AUDIO_ANALYSIS_PIPELINE_H
#define RMS_AUDIO_AUDIO_ANALYSIS_PIPELINE_H

#include "rms/audio/audio_analysis_result.h"
#include "rms/audio/audio_buffer.h"

#include <cstddef>

namespace rms {
namespace audio {

// Parameters shared by the individual analysis modules.
struct AudioAnalysisConfig {
    std::size_t transient_window_size_frames;
    float transient_threshold;
    std::size_t minimum_transient_gap_frames;
    std::size_t melody_window_size_frames;
    double minimum_melody_confidence;
    std::size_t minimum_note_duration_frames;
    int note_velocity;

    AudioAnalysisConfig();
};

class AudioAnalysisPipeline {
public:
    // Runs metrics, transient, tempo, key, and melody analysis, then builds
    // export-ready note events and their musical timeline.
    static AudioAnalysisResult analyze(
        const AudioBuffer& buffer,
        const AudioAnalysisConfig& config = AudioAnalysisConfig());
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_ANALYSIS_PIPELINE_H
