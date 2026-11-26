#ifndef RMS_DESKTOP_MAIN_FRAME_H
#define RMS_DESKTOP_MAIN_FRAME_H

#include "rms/audio/audio_import_manager.h"
#include "rms/audio/audio_playback_controller.h"
#include "rms/audio/audio_project_manager.h"
#include "rms/audio/audio_settings_manager.h"

#include <wx/frame.h>
#include <wx/timer.h>

#include <string>

class wxButton;
class wxStaticText;

namespace rms {
namespace desktop {

class WaveformView;

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void on_open_audio(wxCommandEvent& event);
    void on_play(wxCommandEvent& event);
    void on_stop(wxCommandEvent& event);
    void on_playback_timer(wxTimerEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void refresh_project_view();
    void refresh_transport_status();

    audio::AudioProjectManager project_manager_;
    audio::AudioImportManager import_manager_;
    audio::AudioSettingsManager settings_manager_;
    audio::AudioPlaybackController playback_controller_;
    wxTimer playback_timer_;
    WaveformView* waveform_view_;
    wxStaticText* project_status_;
    wxButton* play_button_;
    wxButton* stop_button_;
    std::string current_audio_name_;

    wxDECLARE_EVENT_TABLE();
};

} // namespace desktop
} // namespace rms

#endif // RMS_DESKTOP_MAIN_FRAME_H
