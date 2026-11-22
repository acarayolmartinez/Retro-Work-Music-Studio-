#ifndef RMS_AUDIO_AUDIO_FILE_READER_H
#define RMS_AUDIO_AUDIO_FILE_READER_H

#include <string>

#include "rms/audio/audio_buffer.h"

namespace rms {
namespace audio {

class AudioFileReader {
public:
    // Reads uncompressed integer PCM WAV files (8, 16, 24 or 32 bits).
    static AudioBuffer read_wav(const std::string& path);
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_AUDIO_FILE_READER_H
