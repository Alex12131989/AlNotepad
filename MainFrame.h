#pragma once
#include <wx/wx.h>

//buttons styles:
//wxBU_LEFT;
//wxBU_RIGHT;
//wxBU_TOP;
//wxBU_BOTTOM;
//wxBU_EXACTFIT;
//wxBU_NOTEXT;
//wxBORDER_NONE;


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	int theme;
	//widgets
	wxPanel* panel;
	wxBitmapButton* add_note_button;
	wxBitmapButton* search_button;
	wxBitmapButton* delete_button;
	wxBitmapButton* change_theme_button;

	
	void BindButtons();
	void SetTheme(unsigned number);
	void SaveTheme(unsigned number);
	unsigned GetTheme();
	void ChangeBgOfWindow(wxWindow* object, wxColour color);
	void UpdateBitButton(wxString address, wxBitmapButton* button, unsigned image_width, unsigned image_height);
};