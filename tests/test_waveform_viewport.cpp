#include "rms/audio/waveform_viewport.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << "\n";
        std::exit(EXIT_FAILURE);
    }
}

void test_initial_view_and_audio_length() {
    rms::audio::WaveformViewport viewport;
    require(viewport.total_frames() == 0 &&
            viewport.visible_frames() == 0,
            "New viewport should be empty");
    viewport.set_audio_length(1000);
    require(viewport.start_frame() == 0 &&
            viewport.visible_frames() == 1000 &&
            viewport.end_frame() == 1000,
            "Setting audio length should show the complete audio");
}

void test_view_and_scrolling_are_clamped() {
    rms::audio::WaveformViewport viewport;
    viewport.set_audio_length(1000);
    viewport.set_view(200, 300);
    require(viewport.start_frame() == 200 &&
            viewport.end_frame() == 500,
            "Explicit view range is incorrect");

    viewport.scroll_forward(900);
    require(viewport.start_frame() == 700 &&
            viewport.end_frame() == 1000,
            "Forward scrolling should clamp at audio end");
    viewport.scroll_backward(900);
    require(viewport.start_frame() == 0,
            "Backward scrolling should clamp at audio start");
}

void test_zoom_in_and_out() {
    rms::audio::WaveformViewport viewport;
    viewport.set_audio_length(1000);
    viewport.set_view(200, 400);
    viewport.zoom(2.0, 400);
    require(viewport.visible_frames() == 200 &&
            viewport.start_frame() == 300,
            "Zoom-in should preserve the anchor position");

    viewport.zoom(0.5, 400);
    require(viewport.visible_frames() == 400 &&
            viewport.start_frame() == 200,
            "Zoom-out should preserve the anchor position");
}

void test_pixel_frame_conversion() {
    rms::audio::WaveformViewport viewport;
    viewport.set_audio_length(1000);
    viewport.set_view(200, 400);
    require(viewport.frame_at_position(50.0, 200.0) == 300,
            "Pixel-to-frame conversion is incorrect");
    require(std::fabs(viewport.position_for_frame(300, 200.0) - 50.0) < 0.0001,
            "Frame-to-pixel conversion is incorrect");
    require(viewport.frame_at_position(-10.0, 200.0) == 200 &&
            viewport.frame_at_position(250.0, 200.0) == 600,
            "Pixel conversion should clamp to viewport edges");
}

void test_selection_lifecycle() {
    rms::audio::WaveformViewport viewport;
    viewport.set_audio_length(1000);
    viewport.set_selection(120, 360);
    require(viewport.has_selection() &&
            viewport.selection_start_frame() == 120 &&
            viewport.selection_end_frame() == 360,
            "Selection range was not stored");
    viewport.clear_selection();
    require(!viewport.has_selection(),
            "Clearing selection failed");
}

void test_invalid_arguments_are_rejected() {
    rms::audio::WaveformViewport viewport;
    viewport.set_audio_length(1000);
    bool view_threw = false;
    bool zoom_threw = false;
    bool selection_threw = false;
    bool conversion_threw = false;

    try { viewport.set_view(0, 0); }
    catch (const std::invalid_argument&) { view_threw = true; }
    try { viewport.zoom(0.0, 100); }
    catch (const std::invalid_argument&) { zoom_threw = true; }
    try { viewport.set_selection(500, 400); }
    catch (const std::invalid_argument&) { selection_threw = true; }
    try { viewport.frame_at_position(10.0, 0.0); }
    catch (const std::invalid_argument&) { conversion_threw = true; }

    require(view_threw && zoom_threw && selection_threw && conversion_threw,
            "Invalid viewport arguments were not rejected");
}

} // namespace

int main() {
    test_initial_view_and_audio_length();
    test_view_and_scrolling_are_clamped();
    test_zoom_in_and_out();
    test_pixel_frame_conversion();
    test_selection_lifecycle();
    test_invalid_arguments_are_rejected();
    std::cout << "WaveformViewport tests passed\n";
    return EXIT_SUCCESS;
}
