#ifndef RMS_DESKTOP_MAIN_FRAME_H
#define RMS_DESKTOP_MAIN_FRAME_H

#include <wx/frame.h>

namespace rms {
namespace desktop {

class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    void on_exit(wxCommandEvent& event);
    void on_about(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

} // namespace desktop
} // namespace rms

#endif // RMS_DESKTOP_MAIN_FRAME_H
