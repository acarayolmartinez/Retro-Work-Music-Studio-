#ifndef RMS_AUDIO_AUDIO_ANALYSIS_RESULT_H
#define RMS_AUDIO_AUDIO_ANALYSIS_RESULT_H

#include "rms/audio/audio_analyzer.h"
#include "rms/audio/key_detector.h"
#include "rms/audio/melody_tracker.h"
#include "rms/audio/musical_timeline.h"
#include "rms/audio/tempo_estimator.h"
#include "rms/audio/transient_detector.h"

#include <vector>

namespace rms {
namespace audio {

// Aggregated output from one complete audio analysis pass.
struct AudioAnalysisResult {
    AudioMetrics metrics;
    TempoEstimate tempo;
    KeyEstimate key;
    std::vector<Transient> transients;
    std::vector<MelodyNote> melody;
    std::vector<NoteEvent> note_events;
    MusicalTimeline timeline;

    AudioAnalysisResult();
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_ANALYSIS_RESULT_H
