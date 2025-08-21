#include "MainFrame.h"
#include "App.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <fstream>
#include <chrono>

const int MAX_THEMES = 2, bitmap_button_width = 25, bitmap_button_height = 25;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	panel = new wxPanel(this);
	int max_width = this->GetMaxWidth(), min_width = this->GetMinWidth();

	note_field = new wxScrolledWindow(panel, wxID_ANY, wxPoint(0, 60), wxSize(MainWindowWidth, MainWindowHeight-bitmap_button_height), wxVSCROLL);
	note_field->SetScrollRate(0, 10);
	//scroller->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
	note_field_sizer = new wxGridSizer(0, columns, gap, gap);

	wxBitmap passer;
	add_note_button = new wxBitmapButton(panel, wxID_ANY, passer, wxDefaultPosition, wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	search_button = new wxBitmapButton(panel, wxID_ANY, passer, wxDefaultPosition, wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	delete_button = new wxBitmapButton(panel, wxID_ANY, passer, wxDefaultPosition, wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	change_theme_button = new wxBitmapButton(panel, wxID_ANY, passer, wxDefaultPosition, wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	
	wxBoxSizer* toolbar = new wxBoxSizer(wxHORIZONTAL);
	toolbar->Add(add_note_button);
	toolbar->AddSpacer(10);
	toolbar->Add(search_button);
	toolbar->AddSpacer(10);
	toolbar->Add(delete_button);
	toolbar->AddSpacer(10);
	toolbar->Add(change_theme_button);


	theme = GetTheme();
	SetTheme(theme);
	BindButtons();
	

	note_field->SetVirtualSize(wxSize(-1, height));

	wxBoxSizer* interactives_sizer = new wxBoxSizer(wxVERTICAL);
	interactives_sizer->Add(toolbar, wxSizerFlags().Left().Expand());
	interactives_sizer->AddSpacer(10);
	interactives_sizer->Add(note_field, wxSizerFlags());

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
	outer_sizer->Add(interactives_sizer, wxSizerFlags().Border(wxALL, 10));


	panel->Layout();
	panel->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
}

void MainFrame::BindButtons()
{
	change_theme_button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {OnChangeThemeButtonClicked(event, theme);});
	add_note_button->Bind(wxEVT_BUTTON, &MainFrame::OnAddNoteButtonClicked, this);
}

void MainFrame::OnChangeThemeButtonClicked(wxCommandEvent& event, unsigned theme)
{
	SetTheme(theme);
}

void MainFrame::OnAddNoteButtonClicked(wxCommandEvent& event)
{
	AddNote();
}

void MainFrame::SetTheme(unsigned number)
{
	unsigned image_width = bitmap_button_width, image_height = bitmap_button_height;
	switch (number)
	{
	case 0:
		UpdateBitButton("Assets/theme 1/add.png", add_note_button, image_width, image_height);
		UpdateBitButton("Assets/theme 1/magnifying glass.png", search_button, image_width, image_height);
		UpdateBitButton("Assets/theme 1/trash bin.png", delete_button, image_width, image_height);
		UpdateBitButton("Assets/theme 1/pointing circles.png", change_theme_button, image_width, image_height);
		ChangeBgOfWindow(this, RGB(13, 13, 13));
		break;

	case 1:
		UpdateBitButton("Assets/theme 2/add.png", add_note_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/magnifying glass.png", search_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/trash bin.png", delete_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/pointing circles.png", change_theme_button, image_width, image_height);
		ChangeBgOfWindow(this, RGB(255, 255, 255));
		break;
	}

	SaveTheme(theme);

	if (theme)
		theme = 0;
	else
		theme = 1;

}

void MainFrame::SaveTheme(unsigned number)
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "theme.txt";
	std::ofstream theme_file(working_path);
	theme_file << number;
}

unsigned MainFrame::GetTheme()
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "theme.txt";
	std::ifstream theme_file(working_path);

	unsigned theme = 0;
	if (theme_file.is_open())
	{
		theme_file >> theme;
		if (theme < 0 || theme > MAX_THEMES - 1)
			theme = 0;
	}
	return theme;
}

void MainFrame::SaveNotes()
{

}

std::vector<Note> MainFrame::GetNotes()
{
	std::vector<Note> notes;
	return notes;
}

void MainFrame::ChangeBgOfWindow(wxWindow* object, wxColour color)
{
	if (object)
	{
		object->SetBackgroundColour(color);
		object->Refresh();
		object->Update();

		for (wxWindow* child : object->GetChildren())
			ChangeBgOfWindow(child, color);
	}
}

void MainFrame::UpdateBitButton(wxString image_address, wxBitmapButton* button, unsigned image_width, unsigned image_height)
{
	wxBitmap bitmap_image(image_address, wxBITMAP_TYPE_PNG);
	wxImage image = bitmap_image.ConvertToImage();
	image.Rescale(image_width, image_height);
	bitmap_image = wxBitmap(image);
	button->SetBitmapLabel(bitmap_image);
}

void MainFrame::AddNote()
{
	//there's a bug i'll work on tomorrow
	wxSize client_window_size = this->GetClientSize();
	Note note = Note();
	notes.push_back(note);

	int note_label_width = (client_window_size.x - (columns - 1) * gap) / columns;
	height += (notes.size() % columns) * (note_label_ratio * note_label_width + gap);
	if (!height)
		height = note_label_width;

	note_field->SetVirtualSize(wxSize(-1, height));
	
	wxButton* note_label = new wxButton(note_field, wxID_ANY, note.title, wxDefaultPosition,
		wxSize(note_label_width, note_label_ratio * note_label_width));
	note_labels.push_back(note_label);




	note_field_sizer->Add(note_label, wxSizerFlags().Border(wxALL, 10));

}

void MainFrame::UpdateNoteDisplay()
{

}