#include "rms/audio/audio_import_report.h"

namespace rms {
namespace audio {

AudioImportReport::AudioImportReport()
    : success(false),
      duration_seconds(0.0),
      sample_rate(0),
      channel_count(0) {
}

} // namespace audio
} // namespace rms
