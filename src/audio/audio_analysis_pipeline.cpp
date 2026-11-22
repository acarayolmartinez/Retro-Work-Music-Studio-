#include "rms/audio/audio_analysis_pipeline.h"

#include "rms/audio/audio_analyzer.h"
#include "rms/audio/key_detector.h"
#include "rms/audio/melody_tracker.h"
#include "rms/audio/note_event.h"
#include "rms/audio/tempo_estimator.h"
#include "rms/audio/transient_detector.h"

namespace rms {
namespace audio {

AudioAnalysisResult::AudioAnalysisResult()
    : metrics(),
      tempo(),
      key(),
      transients(),
      melody(),
      note_events(),
      timeline() {
}

AudioAnalysisConfig::AudioAnalysisConfig()
    : transient_window_size_frames(1024),
      transient_threshold(0.1f),
      minimum_transient_gap_frames(0),
      melody_window_size_frames(8192),
      minimum_melody_confidence(0.2),
      minimum_note_duration_frames(0),
      note_velocity(100) {
}

AudioAnalysisResult AudioAnalysisPipeline::analyze(
    const AudioBuffer& buffer,
    const AudioAnalysisConfig& config) {
    AudioAnalysisResult result;

    result.metrics = AudioAnalyzer::analyze(buffer);
    result.key = KeyDetector::estimate(buffer);

    result.transients = TransientDetector::detect(
        buffer,
        config.transient_window_size_frames,
        config.transient_threshold,
        config.minimum_transient_gap_frames);

    result.tempo = TempoEstimator::estimate(
        result.transients,
        buffer.sample_rate());

    result.melody = MelodyTracker::track(
        buffer,
        config.melody_window_size_frames,
        config.minimum_melody_confidence,
        config.minimum_note_duration_frames);

    result.note_events = NoteEventConverter::convert(
        result.melody,
        config.note_velocity);

    for (std::size_t i = 0; i < result.note_events.size(); ++i) {
        result.timeline.add_event(result.note_events[i]);
    }

    return result;
}

} // namespace audio
} // namespace rms
