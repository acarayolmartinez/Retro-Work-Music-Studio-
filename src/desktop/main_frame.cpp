#include "rms/desktop/main_frame.h"

#include <wx/button.h>
#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>

namespace rms {
namespace desktop {
namespace {

const int ID_MENU_EXIT = wxID_EXIT;
const int ID_MENU_ABOUT = wxID_ABOUT;

class WaveformPlaceholder : public wxPanel {
public:
    explicit WaveformPlaceholder(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                  wxBORDER_SIMPLE) {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &WaveformPlaceholder::on_paint, this);
    }

private:
    void on_paint(wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        const wxSize size = GetClientSize();

        dc.SetBackground(wxBrush(wxColour(28, 31, 38)));
        dc.Clear();

        dc.SetPen(wxPen(wxColour(55, 61, 72), 1));
        const int horizontal_step = 32;
        const int vertical_step = 28;
        for (int x = 0; x < size.GetWidth(); x += horizontal_step) {
            dc.DrawLine(x, 0, x, size.GetHeight());
        }
        for (int y = 0; y < size.GetHeight(); y += vertical_step) {
            dc.DrawLine(0, y, size.GetWidth(), y);
        }

        dc.SetTextForeground(wxColour(190, 198, 210));
        dc.SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                          wxFONTWEIGHT_NORMAL));
        const wxString message = "Waveform view - no audio loaded";
        wxCoord text_width = 0;
        wxCoord text_height = 0;
        dc.GetTextExtent(message, &text_width, &text_height);
        dc.DrawText(message,
                    (size.GetWidth() - text_width) / 2,
                    (size.GetHeight() - text_height) / 2);
    }
};

} // namespace

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_MENU_EXIT, MainFrame::on_exit)
    EVT_MENU(ID_MENU_ABOUT, MainFrame::on_about)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(0, wxID_ANY, "RetroWorks Music Studio",
              wxDefaultPosition, wxSize(1100, 720)) {
    wxMenu* file_menu = new wxMenu();
    file_menu->Append(ID_MENU_EXIT, "E&xit\tAlt-F4");

    wxMenu* help_menu = new wxMenu();
    help_menu->Append(ID_MENU_ABOUT, "&About RMS");

    wxMenuBar* menu_bar = new wxMenuBar();
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(help_menu, "&Help");
    SetMenuBar(menu_bar);

    wxPanel* root = new wxPanel(this, wxID_ANY);
    wxBoxSizer* root_sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* heading = new wxStaticText(
        root, wxID_ANY, "Music Analyzer");
    wxFont heading_font = heading->GetFont();
    heading_font.SetPointSize(16);
    heading_font.SetWeight(wxFONTWEIGHT_BOLD);
    heading->SetFont(heading_font);
    root_sizer->Add(heading, 0, wxALL, 12);

    WaveformPlaceholder* waveform = new WaveformPlaceholder(root);
    root_sizer->Add(waveform, 1, wxEXPAND | wxLEFT | wxRIGHT, 12);

    wxBoxSizer* transport_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* play_button = new wxButton(root, wxID_ANY, "Play");
    wxButton* stop_button = new wxButton(root, wxID_ANY, "Stop");
    play_button->Enable(false);
    stop_button->Enable(false);

    transport_sizer->Add(play_button, 0, wxRIGHT, 8);
    transport_sizer->Add(stop_button, 0);
    transport_sizer->AddStretchSpacer(1);
    transport_sizer->Add(
        new wxStaticText(root, wxID_ANY, "No project loaded"),
        0, wxALIGN_CENTER_VERTICAL);

    root_sizer->Add(transport_sizer, 0,
                    wxEXPAND | wxALL, 12);
    root->SetSizer(root_sizer);

    CreateStatusBar(1);
    SetStatusText("Ready");
    Centre();
}

void MainFrame::on_exit(wxCommandEvent&) {
    Close(true);
}

void MainFrame::on_about(wxCommandEvent&) {
    wxMessageBox(
        "RetroWorks Music Studio\n"
        "A C++11 music analysis and retro audio production studio.",
        "About RMS",
        wxOK | wxICON_INFORMATION,
        this);
}

} // namespace desktop
} // namespace rms
