#include "rms/desktop/main_frame.h"

#include <wx/app.h>

namespace rms {
namespace desktop {

class RmsDesktopApplication : public wxApp {
public:
    virtual bool OnInit() {
        if (!wxApp::OnInit()) {
            return false;
        }

        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }
};

} // namespace desktop
} // namespace rms

wxIMPLEMENT_APP(rms::desktop::RmsDesktopApplication);
