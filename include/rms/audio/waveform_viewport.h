#ifndef RMS_AUDIO_WAVEFORM_VIEWPORT_H
#define RMS_AUDIO_WAVEFORM_VIEWPORT_H

#include <cstddef>

namespace rms {
namespace audio {

// Frame-space viewport and selection state for a waveform editor.
class WaveformViewport {
public:
    WaveformViewport();

    void set_audio_length(std::size_t total_frames);
    void set_view(std::size_t start_frame, std::size_t visible_frames);

    // factor > 1 zooms in; factor between 0 and 1 zooms out.
    // The frame under anchor_frame remains at the same relative position.
    void zoom(double factor, std::size_t anchor_frame);

    void scroll_forward(std::size_t frames);
    void scroll_backward(std::size_t frames);

    std::size_t total_frames() const;
    std::size_t start_frame() const;
    std::size_t visible_frames() const;
    std::size_t end_frame() const;

    // Maps a horizontal pixel coordinate to a source frame, clamped to view.
    std::size_t frame_at_position(double x, double width) const;
    // Maps a source frame to a horizontal pixel coordinate.
    double position_for_frame(std::size_t frame, double width) const;

    void set_selection(std::size_t start_frame, std::size_t end_frame);
    void clear_selection();
    bool has_selection() const;
    std::size_t selection_start_frame() const;
    std::size_t selection_end_frame() const;

private:
    void clamp_view();

    std::size_t total_frames_;
    std::size_t start_frame_;
    std::size_t visible_frames_;
    bool has_selection_;
    std::size_t selection_start_frame_;
    std::size_t selection_end_frame_;
};

} // namespace audio
} // namespace rms

#endif // RMS_AUDIO_WAVEFORM_VIEWPORT_H
