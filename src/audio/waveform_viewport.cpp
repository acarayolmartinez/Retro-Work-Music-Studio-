#include "rms/audio/waveform_viewport.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace rms {
namespace audio {

WaveformViewport::WaveformViewport()
    : total_frames_(0),
      start_frame_(0),
      visible_frames_(0),
      has_selection_(false),
      selection_start_frame_(0),
      selection_end_frame_(0) {
}

void WaveformViewport::set_audio_length(std::size_t total_frames) {
    total_frames_ = total_frames;
    start_frame_ = 0;
    visible_frames_ = total_frames;
    clear_selection();
}

void WaveformViewport::set_view(std::size_t start_frame,
                                std::size_t visible_frames) {
    if (total_frames_ == 0) {
        if (start_frame != 0 || visible_frames != 0) {
            throw std::invalid_argument("Cannot set a view for empty audio");
        }
        start_frame_ = 0;
        visible_frames_ = 0;
        return;
    }
    if (visible_frames == 0) {
        throw std::invalid_argument("Visible frame count must be positive");
    }
    start_frame_ = start_frame;
    visible_frames_ = visible_frames;
    clamp_view();
}

void WaveformViewport::zoom(double factor, std::size_t anchor_frame) {
    if (!std::isfinite(factor) || factor <= 0.0) {
        throw std::invalid_argument("Zoom factor must be finite and positive");
    }
    if (total_frames_ == 0) {
        return;
    }

    if (anchor_frame < start_frame_) {
        anchor_frame = start_frame_;
    }
    if (anchor_frame > end_frame()) {
        anchor_frame = end_frame();
    }

    const long double relative =
        static_cast<long double>(anchor_frame - start_frame_) /
        static_cast<long double>(visible_frames_);
    long double requested =
        static_cast<long double>(visible_frames_) /
        static_cast<long double>(factor);

    if (requested < 1.0L) {
        requested = 1.0L;
    }
    if (requested > static_cast<long double>(total_frames_)) {
        requested = static_cast<long double>(total_frames_);
    }

    const std::size_t new_visible =
        static_cast<std::size_t>(requested);
    const long double new_start =
        static_cast<long double>(anchor_frame) -
        relative * static_cast<long double>(new_visible);

    if (new_start <= 0.0L) {
        start_frame_ = 0;
    } else if (new_start >= static_cast<long double>(total_frames_)) {
        start_frame_ = total_frames_ - 1;
    } else {
        start_frame_ = static_cast<std::size_t>(new_start);
    }
    visible_frames_ = new_visible;
    clamp_view();
}

void WaveformViewport::scroll_forward(std::size_t frames) {
    if (frames > total_frames_ - start_frame_) {
        start_frame_ = total_frames_ - visible_frames_;
    } else {
        start_frame_ += frames;
    }
    clamp_view();
}

void WaveformViewport::scroll_backward(std::size_t frames) {
    start_frame_ = frames > start_frame_ ? 0 : start_frame_ - frames;
    clamp_view();
}

std::size_t WaveformViewport::total_frames() const {
    return total_frames_;
}

std::size_t WaveformViewport::start_frame() const {
    return start_frame_;
}

std::size_t WaveformViewport::visible_frames() const {
    return visible_frames_;
}

std::size_t WaveformViewport::end_frame() const {
    return start_frame_ + visible_frames_;
}

std::size_t WaveformViewport::frame_at_position(double x,
                                                 double width) const {
    if (!std::isfinite(x) || !std::isfinite(width) || width <= 0.0) {
        throw std::invalid_argument("Position and width must be finite; width must be positive");
    }
    if (visible_frames_ == 0) {
        return 0;
    }
    if (x <= 0.0) {
        return start_frame_;
    }
    if (x >= width) {
        return end_frame();
    }
    const long double fraction =
        static_cast<long double>(x) / static_cast<long double>(width);
    const long double offset =
        fraction * static_cast<long double>(visible_frames_);
    return start_frame_ + static_cast<std::size_t>(offset);
}

double WaveformViewport::position_for_frame(std::size_t frame,
                                            double width) const {
    if (!std::isfinite(width) || width <= 0.0) {
        throw std::invalid_argument("Width must be finite and positive");
    }
    if (visible_frames_ == 0) {
        return 0.0;
    }
    if (frame <= start_frame_) {
        return 0.0;
    }
    if (frame >= end_frame()) {
        return width;
    }
    const long double fraction =
        static_cast<long double>(frame - start_frame_) /
        static_cast<long double>(visible_frames_);
    return static_cast<double>(fraction * width);
}

void WaveformViewport::set_selection(std::size_t start_frame,
                                     std::size_t end_frame) {
    if (start_frame >= end_frame || end_frame > total_frames_) {
        throw std::invalid_argument("Selection must be a non-empty range within the audio");
    }
    selection_start_frame_ = start_frame;
    selection_end_frame_ = end_frame;
    has_selection_ = true;
}

void WaveformViewport::clear_selection() {
    has_selection_ = false;
    selection_start_frame_ = 0;
    selection_end_frame_ = 0;
}

bool WaveformViewport::has_selection() const {
    return has_selection_;
}

std::size_t WaveformViewport::selection_start_frame() const {
    return selection_start_frame_;
}

std::size_t WaveformViewport::selection_end_frame() const {
    return selection_end_frame_;
}

void WaveformViewport::clamp_view() {
    if (total_frames_ == 0) {
        start_frame_ = 0;
        visible_frames_ = 0;
        return;
    }
    if (visible_frames_ == 0 || visible_frames_ > total_frames_) {
        visible_frames_ = total_frames_;
    }
    if (start_frame_ > total_frames_ - visible_frames_) {
        start_frame_ = total_frames_ - visible_frames_;
    }
}

} // namespace audio
} // namespace rms
