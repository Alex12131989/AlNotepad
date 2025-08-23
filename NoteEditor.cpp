#include "NoteEditor.h"
#include "App.h"
#include <chrono>

NoteEditor::NoteEditor(const wxString& title, Note note_class) : wxFrame(nullptr, wxID_ANY, title)
{
	panel = new Drawing(this);
	panel->SetBackgroundColour(RGB(255, 255, 200));
	//panel->SetMinSize(wxSize(MainWindowWidth, MainWindowHeight));
	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
	panel_sizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(panel_sizer);

	panel->SetPenColor(RGB(200, 180, 0));
	panel->SetPenWidthInPixels(3);
	panel->Line(wxPoint(0, MainWindowHeight * 0.2), wxPoint(MainWindowWidth, MainWindowHeight * 0.2));
	panel->Layout();

	this->Layout();
}

void NoteEditor::DrawTextures()
{

}


