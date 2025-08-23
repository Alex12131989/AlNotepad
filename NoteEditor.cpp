#include "NoteEditor.h"
#include "App.h"

NoteEditor::NoteEditor(const wxString& title, Note note_class) : wxFrame(nullptr, wxID_ANY, title)
{
	panel = new wxPanel(this);
	panel->SetBackgroundColour(RGB(255, 255, 200));
	panel->SetMinClientSize(wxSize(MainWindowWidth, MainWindowHeight));







	panel->Layout();
	this->Show();
}

void NoteEditor::DrawTextures()
{
	wxSize size = panel->GetSize();
	canvas = new Drawing(panel);
	canvas->DrawLine(wxPoint(0, MainWindowHeight * 0.2), wxPoint(MainWindowWidth, MainWindowHeight * 0.2));
	canvas->Refresh();
}
