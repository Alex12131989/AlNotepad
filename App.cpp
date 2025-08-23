#include "App.h"
#include "MainFrame.h"

//temporary, for the time of the development the note editor window
#include "NoteEditor.h"
#include "Note.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	wxInitAllImageHandlers();
	/*
	MainFrame* mainFrame = new MainFrame("AlNotepad");
	mainFrame->SetClientSize(MainWindowWidth, MainWindowHeight);
	mainFrame->Center();
	mainFrame->Show();
	*/

	//temporary
	Note test_note = Note();
	test_note.title = "Test Title";
	test_note.content = "Some text to represent the note content";
	test_note.date_of_last_edition = { 2025, 8, 22, 13, 53, 23 };
	//-------------------------------------------------------------
	//transport to MainFrame::CreateNewWindow method
	NoteEditor* mainFrame = new NoteEditor("Note Editor", test_note);
	mainFrame->SetClientSize(MainWindowWidth, MainWindowHeight);
	mainFrame->Center();
	mainFrame->Show();
	mainFrame->DrawTextures();
	//--------------------------------------------------------------
	return true;
}

wxBitmap GetScaledImage(wxString address, unsigned width, unsigned height)
{
	//TODO: consider case image address unfound
	wxBitmap bitmap_image(address, wxBITMAP_TYPE_PNG);
	wxImage image = bitmap_image.ConvertToImage();
	image.Rescale(width, height);
	bitmap_image = wxBitmap(image);

	return bitmap_image;
}

