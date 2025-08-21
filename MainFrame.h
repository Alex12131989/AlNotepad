#pragma once
#include <wx/wx.h>
#include <vector>
#include "Note.h"


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:

	int columns = 5;
	int gap = 20;
	int note_label_ratio = 1; // (0, 1]
	int theme;
	int height;
	std::vector <Note> notes;
	//widgets
	wxPanel* panel;
	wxBitmapButton* add_note_button;
	wxBitmapButton* search_button;
	wxBitmapButton* delete_button;
	wxBitmapButton* change_theme_button;
	wxScrolledWindow* note_field;
	wxGridSizer* note_field_sizer;
	std::vector<wxButton*> note_labels;
	
	void BindButtons();
	void OnChangeThemeButtonClicked(wxCommandEvent& event, unsigned theme);
	void OnAddNoteButtonClicked(wxCommandEvent& event);

	void SetTheme(unsigned number);
	void SaveTheme(unsigned number);
	unsigned GetTheme();

	void SaveNotes();
	std::vector<Note> GetNotes();

	void ChangeBgOfWindow(wxWindow* object, wxColour color);
	void UpdateBitButton(wxString address, wxBitmapButton* button, unsigned image_width, unsigned image_height);
	void AddNote();
	void UpdateNoteDisplay();
};