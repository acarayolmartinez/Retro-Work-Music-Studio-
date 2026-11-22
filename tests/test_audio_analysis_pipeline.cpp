#include "rms/audio/audio_analysis_pipeline.h"

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

void test_silence_returns_empty_analysis() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>(16384, 0.0f));

    const rms::audio::AudioAnalysisResult result =
        rms::audio::AudioAnalysisPipeline::analyze(buffer);

    require(result.metrics.peak_amplitude == 0.0f &&
            result.metrics.rms_amplitude == 0.0f,
            "Silence metrics should be zero");
    require(result.tempo.bpm == 0.0 && result.key.key.empty(),
            "Silence should not produce tempo or key estimates");
    require(result.transients.empty() && result.melody.empty(),
            "Silence should not produce transients or melody");
    require(result.note_events.empty() && result.timeline.empty(),
            "Silence should not produce note events or timeline entries");
}

void test_pipeline_runs_metrics_and_populates_key_chroma() {
    const std::uint32_t sample_rate = 8192;
    std::vector<float> samples(8192);
    const double pi = 3.14159265358979323846;

    for (std::size_t i = 0; i < samples.size(); ++i) {
        samples[i] = static_cast<float>(
            0.5 * std::sin(2.0 * pi * 440.0 *
                           static_cast<double>(i) / sample_rate));
    }

    const rms::audio::AudioBuffer buffer(sample_rate, 1, samples);
    rms::audio::AudioAnalysisConfig config;
    config.transient_threshold = 0.0f;
    config.melody_window_size_frames = 4096;

    const rms::audio::AudioAnalysisResult result =
        rms::audio::AudioAnalysisPipeline::analyze(buffer, config);

    require(result.metrics.peak_amplitude > 0.45f &&
            result.metrics.peak_amplitude <= 0.5f,
            "Pipeline did not return expected peak metrics");
    require(result.metrics.rms_amplitude > 0.3f,
            "Pipeline did not return expected RMS metrics");
    require(result.key.chroma[9] > 0.0,
            "Pipeline did not return chroma information");
}

void test_invalid_configuration_is_rejected() {
    const rms::audio::AudioBuffer buffer(
        44100, 1, std::vector<float>(1024, 0.0f));

    rms::audio::AudioAnalysisConfig zero_transient_window;
    zero_transient_window.transient_window_size_frames = 0;
    bool transient_threw = false;
    try {
        rms::audio::AudioAnalysisPipeline::analyze(
            buffer, zero_transient_window);
    } catch (const std::invalid_argument&) {
        transient_threw = true;
    }

    rms::audio::AudioAnalysisConfig invalid_velocity;
    invalid_velocity.note_velocity = 0;
    bool velocity_threw = false;
    try {
        rms::audio::AudioAnalysisPipeline::analyze(buffer, invalid_velocity);
    } catch (const std::invalid_argument&) {
        velocity_threw = true;
    }

    require(transient_threw && velocity_threw,
            "Pipeline should reject invalid configuration values");
}

} // namespace

int main() {
    test_silence_returns_empty_analysis();
    test_pipeline_runs_metrics_and_populates_key_chroma();
    test_invalid_configuration_is_rejected();
    std::cout << "AudioAnalysisPipeline tests passed\n";
    return EXIT_SUCCESS;
}
