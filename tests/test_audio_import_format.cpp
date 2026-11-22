#include "rms/audio/audio_import_format.h"

#include <cassert>
#include <iostream>
#include <vector>

using rms::audio::AudioImportFormat;
using rms::audio::AudioImportFormatInfo;

int main() {
    assert(AudioImportFormatInfo::detect("song.wav") == AudioImportFormat::Wav);
    assert(AudioImportFormatInfo::detect("SONG.WAV") == AudioImportFormat::Wav);
    assert(AudioImportFormatInfo::detect("folder.with.dots/song.WaVe") ==
           AudioImportFormat::Wav);
    assert(AudioImportFormatInfo::detect("song.mp3") == AudioImportFormat::Unknown);
    assert(AudioImportFormatInfo::detect("folder.wav/song") == AudioImportFormat::Unknown);
    assert(AudioImportFormatInfo::detect(".wav") == AudioImportFormat::Unknown);
    assert(AudioImportFormatInfo::detect("song.") == AudioImportFormat::Unknown);
    assert(AudioImportFormatInfo::detect("") == AudioImportFormat::Unknown);

    assert(AudioImportFormatInfo::is_supported("take.wav"));
    assert(!AudioImportFormatInfo::is_supported("take.flac"));
    assert(AudioImportFormatInfo::extension(AudioImportFormat::Wav) == ".wav");
    assert(AudioImportFormatInfo::extension(AudioImportFormat::Unknown).empty());

    const std::vector<std::string> extensions =
        AudioImportFormatInfo::supported_extensions();
    assert(extensions.size() == 2);
    assert(extensions[0] == ".wav");
    assert(extensions[1] == ".wave");

    std::cout << "AudioImportFormat tests passed.\n";
    return 0;
}
