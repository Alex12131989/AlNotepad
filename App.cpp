#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	wxInitAllImageHandlers();
	MainFrame* mainFrame = new MainFrame("Title");
	mainFrame->SetClientSize(MainWindowWidth, MainWindowHeight);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}