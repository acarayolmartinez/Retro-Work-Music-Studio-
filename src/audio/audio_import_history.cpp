#include "rms/audio/audio_import_history.h"

#include <algorithm>
#include <cctype>

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

std::size_t AudioImportHistory::successful_count() const {
    std::size_t count = 0;
    for (std::vector<AudioImportReport>::const_iterator it = entries_.begin();
         it != entries_.end(); ++it) {
        if (it->success) ++count;
    }
    return count;
}

std::size_t AudioImportHistory::failed_count() const {
    return size() - successful_count();
}

const AudioImportReport* AudioImportHistory::latest() const {
    return entries_.empty() ? 0 : &entries_.back();
}

std::vector<AudioImportReport> AudioImportHistory::recent(
    std::size_t limit) const {
    std::vector<AudioImportReport> result;
    const std::size_t count = std::min(limit, entries_.size());
    result.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
        result.push_back(entries_[entries_.size() - 1 - i]);
    return result;
}

std::vector<AudioImportReport> AudioImportHistory::find_by_file_name(
    const std::string& query) const {
    std::string needle(query);
    std::transform(needle.begin(), needle.end(), needle.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    std::vector<AudioImportReport> result;
    if (needle.empty()) return result;
    for (std::vector<AudioImportReport>::const_iterator it = entries_.begin();
         it != entries_.end(); ++it) {
        std::string name(it->file_name);
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (name.find(needle) != std::string::npos) result.push_back(*it);
    }
    return result;
}

void AudioImportHistory::clear() {
    entries_.clear();
}

} // namespace audio
} // namespace rms
