#include "rms/desktop/main_frame.h"

#include "rms/audio/audio_project.h"
#include "rms/audio/waveform_data.h"

#include <wx/button.h>
#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>

#include <vector>

namespace rms {
namespace desktop {
namespace {

const int ID_MENU_OPEN_AUDIO = wxID_HIGHEST + 1;
const int ID_TIMER_PLAYBACK = wxID_HIGHEST + 2;
const int ID_MENU_EXIT = wxID_EXIT;
const int ID_MENU_ABOUT = wxID_ABOUT;

} // namespace

class WaveformView : public wxPanel {
public:
    explicit WaveformView(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                  wxBORDER_SIMPLE),
          points_() {
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &WaveformView::on_paint, this);
    }

    void set_waveform(const std::vector<audio::WaveformPoint>& points) {
        points_ = points;
        Refresh();
    }

private:
    void on_paint(wxPaintEvent&) {
        wxAutoBufferedPaintDC dc(this);
        const wxSize size = GetClientSize();
        const int width = size.GetWidth();
        const int height = size.GetHeight();

        dc.SetBackground(wxBrush(wxColour(24, 28, 35)));
        dc.Clear();

        dc.SetPen(wxPen(wxColour(48, 55, 66), 1));
        for (int x = 0; x < width; x += 40)
            dc.DrawLine(x, 0, x, height);
        for (int y = 0; y < height; y += 28)
            dc.DrawLine(0, y, width, y);

        const int center = height / 2;
        dc.SetPen(wxPen(wxColour(75, 86, 101), 1));
        dc.DrawLine(0, center, width, center);

        if (points_.empty() || width <= 0 || height <= 0) {
            const wxString message = "Waveform view - no audio loaded";
            wxCoord text_width = 0;
            wxCoord text_height = 0;
            dc.SetTextForeground(wxColour(180, 190, 204));
            dc.GetTextExtent(message, &text_width, &text_height);
            dc.DrawText(message, (width - text_width) / 2,
                        (height - text_height) / 2);
            return;
        }

        dc.SetPen(wxPen(wxColour(75, 205, 170), 1));
        const int amplitude = height / 2 - 12;
        for (int x = 0; x < width; ++x) {
            const std::size_t index =
                static_cast<std::size_t>(x) * points_.size() /
                static_cast<std::size_t>(width);
            const audio::WaveformPoint& point =
                points_[index < points_.size() ? index : points_.size() - 1];
            const int top = center -
                static_cast<int>(point.maximum * amplitude);
            const int bottom = center -
                static_cast<int>(point.minimum * amplitude);
            dc.DrawLine(x, top, x, bottom);
        }
    }

    std::vector<audio::WaveformPoint> points_;
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_MENU_OPEN_AUDIO, MainFrame::on_open_audio)
    EVT_MENU(ID_MENU_EXIT, MainFrame::on_exit)
    EVT_MENU(ID_MENU_ABOUT, MainFrame::on_about)
    EVT_TIMER(ID_TIMER_PLAYBACK, MainFrame::on_playback_timer)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
    : wxFrame(0, wxID_ANY, "RetroWorks Music Studio",
              wxDefaultPosition, wxSize(1100, 720)),
      project_manager_(1024),
      import_manager_(project_manager_),
      settings_manager_(),
      playback_controller_(settings_manager_),
      playback_timer_(this, ID_TIMER_PLAYBACK),
      waveform_view_(0),
      project_status_(0),
      play_button_(0),
      stop_button_(0),
      current_audio_name_() {
    wxMenu* file_menu = new wxMenu();
    file_menu->Append(ID_MENU_OPEN_AUDIO, "&Open Audio...\\tCtrl+O");
    file_menu->AppendSeparator();
    file_menu->Append(ID_MENU_EXIT, "E&xit\\tAlt-F4");

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

    waveform_view_ = new WaveformView(root);
    root_sizer->Add(waveform_view_, 1, wxEXPAND | wxLEFT | wxRIGHT, 12);

    wxBoxSizer* transport_sizer = new wxBoxSizer(wxHORIZONTAL);
    play_button_ = new wxButton(root, wxID_ANY, "Play");
    stop_button_ = new wxButton(root, wxID_ANY, "Stop");
    play_button_->Enable(false);
    stop_button_->Enable(false);
    play_button_->Bind(wxEVT_BUTTON, &MainFrame::on_play, this);
    stop_button_->Bind(wxEVT_BUTTON, &MainFrame::on_stop, this);

    transport_sizer->Add(play_button_, 0, wxRIGHT, 8);
    transport_sizer->Add(stop_button_, 0);
    transport_sizer->AddStretchSpacer(1);
    project_status_ = new wxStaticText(root, wxID_ANY, "No project loaded");
    transport_sizer->Add(project_status_, 0, wxALIGN_CENTER_VERTICAL);

    root_sizer->Add(transport_sizer, 0, wxEXPAND | wxALL, 12);
    root->SetSizer(root_sizer);

    CreateStatusBar(1);
    SetStatusText("Ready - open a WAV file to begin");
    Centre();
}

void MainFrame::on_open_audio(wxCommandEvent&) {
    wxFileDialog dialog(
        this,
        "Open audio file",
        wxEmptyString,
        wxEmptyString,
        "WAV audio (*.wav;*.wave)|*.wav;*.wave",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (dialog.ShowModal() != wxID_OK)
        return;

    // Release the previous stream before AudioProject replaces its buffer.
    playback_timer_.Stop();
    playback_controller_.unload();

    const std::string path = dialog.GetPath().ToStdString();
    const audio::AudioImportReport report =
        import_manager_.import_file_with_report(path);

    if (!report.success) {
        refresh_transport_status();
        wxMessageBox(
            wxString::FromUTF8(report.error_message.c_str()),
            "Audio import failed",
            wxOK | wxICON_ERROR,
            this);
        SetStatusText("Audio import failed");
        return;
    }

    current_audio_name_ = report.file_name;
    refresh_project_view();
    SetStatusText(wxString("Loaded ") + dialog.GetFilename());
}

void MainFrame::on_play(wxCommandEvent&) {
    if (!playback_controller_.has_output()) {
        SetStatusText("Audio output unavailable - build with PortAudio enabled");
        return;
    }

    playback_controller_.play();
    if (playback_controller_.state() != audio::PlaybackState::Playing) {
        const std::string error = playback_controller_.output_error();
        SetStatusText(error.empty()
            ? "Unable to start audio playback"
            : wxString::FromUTF8(error.c_str()));
        return;
    }

    playback_timer_.Start(100);
    refresh_transport_status();
}

void MainFrame::on_stop(wxCommandEvent&) {
    playback_timer_.Stop();
    playback_controller_.stop();
    refresh_transport_status();
    SetStatusText("Playback stopped");
}

void MainFrame::on_playback_timer(wxTimerEvent&) {
    if (playback_controller_.state() != audio::PlaybackState::Playing) {
        playback_timer_.Stop();
        refresh_transport_status();
        SetStatusText("Playback finished");
        return;
    }

    const double position = playback_controller_.position_seconds();
    const double duration =
        static_cast<double>(playback_controller_.duration_frames()) /
        static_cast<double>(project_manager_.project().audio()->sample_rate());
    SetStatusText(wxString::Format(
        "Playing  %.1f / %.1f s", position, duration));
}

void MainFrame::refresh_project_view() {
    const audio::AudioProject& project = project_manager_.project();
    waveform_view_->set_waveform(project.waveform());

    const audio::AudioBuffer* buffer = project.audio();
    if (buffer == 0) {
        playback_controller_.unload();
        project_status_->SetLabel("No project loaded");
        play_button_->Enable(false);
        stop_button_->Enable(false);
        return;
    }

    playback_controller_.load(*buffer, true);
    const bool output_ready = playback_controller_.has_output();
    play_button_->Enable(output_ready);
    stop_button_->Enable(output_ready);

    const double duration =
        static_cast<double>(buffer->frame_count()) /
        static_cast<double>(buffer->sample_rate());
    const wxString details = wxString::Format(
        "%.2f s  |  %u Hz  |  %u ch",
        duration,
        static_cast<unsigned int>(buffer->sample_rate()),
        static_cast<unsigned int>(buffer->channel_count()));
    project_status_->SetLabel(
        wxString::FromUTF8(current_audio_name_.c_str()) + "  |  " + details);
    Layout();

    if (!output_ready) {
        const std::string error = playback_controller_.output_error();
        SetStatusText(error.empty()
            ? "Audio loaded - enable PortAudio to use playback"
            : wxString::FromUTF8(error.c_str()));
    }
}

void MainFrame::refresh_transport_status() {
    const bool output_ready = playback_controller_.has_output();
    play_button_->Enable(output_ready);
    stop_button_->Enable(output_ready);
}

void MainFrame::on_exit(wxCommandEvent&) {
    playback_timer_.Stop();
    playback_controller_.unload();
    Close(true);
}

void MainFrame::on_about(wxCommandEvent&) {
    wxMessageBox(
        "RetroWorks Music Studio\\n"
        "A C++11 music analysis and retro audio production studio.",
        "About RMS",
        wxOK | wxICON_INFORMATION,
        this);
}

} // namespace desktop
} // namespace rms
