#pragma once
#include "Drawing.h"
#include "Note.h"

#include <wx/wx.h>
#include <vector>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	//constants/variables
	const int MAX_THEMES = 2, 
		bitmap_button_width = 25, 
		bitmap_button_height = 25, 
		bitmap_width = 13,
		bitmap_height = 13,
		scroller_width = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);

	int columns = 5, 
		gap = 20, 
		note_label_ratio = 1,
		theme = GetTheme();

	//other stuff idk how to generalize
	std::vector <Note> notes;
	struct NoteInfo
	{
		std::string title;
		std::string content;
		std::vector <int> date_of_creation;
		std::vector <int> date_of_last_edition;
		bool pinned;
		bool special;
	};

	//widgets
	wxPanel* panel;
	//----------------------------------
	wxBitmapButton* add_note_button;
	wxBitmapButton* search_button;
	wxBitmapButton* delete_button;
	wxBitmapButton* change_theme_button;
	//----------------------------------
	wxScrolledWindow* note_field;
	wxGridSizer* note_field_sizer;
	wxPanel* note_field_panel;
	std::vector<wxPanel*> note_labels;
	std::vector<Drawing*> canvases;
	//----------------------------------
	
	//widget binding
	void BindObjects();
	void OnChangeThemeButtonClicked(wxCommandEvent& event, int theme);
	void OnAddNoteButtonClicked(wxCommandEvent& event);
	void OnWindowResize(wxSizeEvent& event);

	//theme changing
	void SetTheme(int number);
	void ChangeBgOfWindow(wxWindow* object, wxColour color);
	void UpdateBitButton(wxString address, wxBitmapButton* button, unsigned image_width, unsigned image_height);

	//user data managment
	void SaveTheme(int number);
	int GetTheme();
	wxString GenerateCode(int size);
	void SaveNotes();
	std::vector<int> ConvertStringToDateVector(std::string string);
	bool ConvertStringToBool(std::string string);
	std::vector<NoteInfo> GetNotes();

	//notes logic
	void AddNewNote();
	void UpdateNoteDisplay();
	void TrickOutNote(wxPanel* note, NoteInfo info);
	void DisplayNote(wxPanel* note, NoteInfo info);
	void LoadNotes();

	void CreateNewWindow(wxString title, Note note_class);
};