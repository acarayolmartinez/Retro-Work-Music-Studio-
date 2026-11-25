#include "rms/audio/audio_import_report.h"

#include <cassert>
#include <iostream>

int main() {
    const rms::audio::AudioImportReport report;
    assert(report.file_path.empty());
    assert(report.file_name.empty());
    assert(report.file_extension.empty());
    assert(report.detected_format.empty());
    assert(!report.success);
    assert(report.error_message.empty());
    assert(report.duration_seconds == 0.0);
    assert(report.sample_rate == 0);
    assert(report.channel_count == 0);
    std::cout << "AudioImportReport tests passed.\n";
    return 0;
}
