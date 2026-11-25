#include "rms/audio/audio_import_history.h"

namespace rms {
namespace audio {

void AudioImportHistory::add(const AudioImportReport& report) {
    entries_.push_back(report);
}

const std::vector<AudioImportReport>& AudioImportHistory::entries() const {
    return entries_;
}

std::size_t AudioImportHistory::size() const {
    return entries_.size();
}

bool AudioImportHistory::empty() const {
    return entries_.empty();
}

const AudioImportReport* AudioImportHistory::latest() const {
    return entries_.empty() ? 0 : &entries_.back();
}

void AudioImportHistory::clear() {
    entries_.clear();
}

} // namespace audio
} // namespace rms
