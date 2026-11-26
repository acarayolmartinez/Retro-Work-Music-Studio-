#ifndef RMS_DESKTOP_MAIN_FRAME_H
#define RMS_DESKTOP_MAIN_FRAME_H

#include "rms/audio/audio_import_manager.h"
#include "rms/audio/audio_project_manager.h"

#include <wx/frame.h>

class wxStaticText;

namespace rms {
namespace desktop {

class WaveformView;

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void on_open_audio(wxCommandEvent& event);
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);
    void refresh_project_view();

    audio::AudioProjectManager project_manager_;
    audio::AudioImportManager import_manager_;
    WaveformView* waveform_view_;
    wxStaticText* project_status_;

    wxDECLARE_EVENT_TABLE();
};

} // namespace desktop
} // namespace rms

#endif // RMS_DESKTOP_MAIN_FRAME_H
