#include "rms/audio/waveform_data.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << "\n";
        std::exit(EXIT_FAILURE);
    }
}

bool near(float left, float right) {
    return std::fabs(left - right) < 0.0001f;
}

void test_min_max_and_block_positions() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>{
            0.1f, -0.8f, 0.4f, 0.2f,
            0.9f, -0.3f, 0.1f, -0.1f
        });

    const std::vector<rms::audio::WaveformPoint> points =
        rms::audio::WaveformData::generate(buffer, 4);

    require(points.size() == 2, "Unexpected waveform point count");
    require(points[0].start_frame == 0 &&
            near(points[0].minimum, -0.8f) &&
            near(points[0].maximum, 0.4f),
            "First waveform range is incorrect");
    require(points[1].start_frame == 4 &&
            near(points[1].minimum, -0.3f) &&
            near(points[1].maximum, 0.9f),
            "Second waveform range is incorrect");
}

void test_partial_final_block_is_kept() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>{-0.5f, 0.25f, 0.75f, -1.0f, 0.1f});

    const std::vector<rms::audio::WaveformPoint> points =
        rms::audio::WaveformData::generate(buffer, 2);

    require(points.size() == 3, "Partial final block was not included");
    require(points[2].start_frame == 4 &&
            near(points[2].minimum, 0.1f) &&
            near(points[2].maximum, 0.1f),
            "Partial final block range is incorrect");
}

void test_stereo_is_averaged_to_mono() {
    const rms::audio::AudioBuffer buffer(
        44100, 2, std::vector<float>{
            1.0f, -1.0f,
            0.5f, 0.5f,
            -0.5f, 0.5f
        });

    const std::vector<rms::audio::WaveformPoint> points =
        rms::audio::WaveformData::generate(buffer, 3);

    require(points.size() == 1 &&
            near(points[0].minimum, 0.0f) &&
            near(points[0].maximum, 0.5f),
            "Stereo channel averaging is incorrect");
}

void test_empty_audio_returns_no_points() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>());
    const std::vector<rms::audio::WaveformPoint> points =
        rms::audio::WaveformData::generate(buffer, 64);
    require(points.empty(), "Empty audio should produce no waveform points");
}

void test_zero_resolution_is_rejected() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>{0.0f});
    bool threw = false;
    try {
        rms::audio::WaveformData::generate(buffer, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    require(threw, "Zero frames_per_point should be rejected");
}

} // namespace

int main() {
    test_min_max_and_block_positions();
    test_partial_final_block_is_kept();
    test_stereo_is_averaged_to_mono();
    test_empty_audio_returns_no_points();
    test_zero_resolution_is_rejected();
    std::cout << "WaveformData tests passed\n";
    return EXIT_SUCCESS;
}
