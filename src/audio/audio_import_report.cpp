#include "rms/audio/audio_import_report.h"

#include <ctime>

namespace rms {
namespace audio {

AudioImportReport::AudioImportReport()
    : timestamp(std::time(0)),
      success(false),
      duration_seconds(0.0),
      sample_rate(0),
      channel_count(0) {
}

} // namespace audio
} // namespace rms
