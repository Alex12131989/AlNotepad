#include "MainFrame.h"
#include "App.h"
#include "Drawing.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <fstream>
#include <chrono>
#include <cmath>


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	
	panel = new wxPanel(this);

	wxBoxSizer* note_field_panel_sizer = new wxBoxSizer(wxVERTICAL);
	note_field = new wxScrolledWindow(panel, wxID_ANY, wxPoint(0, 60), wxSize(MainWindowWidth, MainWindowHeight-bitmap_button_height), wxVSCROLL);
	note_field->SetScrollRate(0, 10);

	note_field_panel = new wxPanel(note_field, wxID_ANY, wxDefaultPosition, wxSize(MainWindowWidth - scroller_width, MainWindowHeight - bitmap_button_height));
	note_field_sizer = new wxGridSizer(0, columns, wxSize(gap, gap));
	note_field_panel->SetSizer(note_field_sizer);
	note_field_panel->Layout();


	wxBitmap passer;
	add_note_button = new wxBitmapButton(	panel, wxID_ANY, passer, wxDefaultPosition, 
											wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);

	search_button = new wxBitmapButton(	panel, wxID_ANY, passer, wxDefaultPosition, 
										wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);

	delete_button = new wxBitmapButton(	panel, wxID_ANY, passer, wxDefaultPosition, 
										wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);

	change_theme_button = new wxBitmapButton(	panel, wxID_ANY, passer, wxDefaultPosition, 
												wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	
	wxBoxSizer* toolbar = new wxBoxSizer(wxHORIZONTAL);
	toolbar->Add(add_note_button);
	toolbar->AddSpacer(10);
	toolbar->Add(search_button);
	toolbar->AddSpacer(10);
	toolbar->Add(delete_button);
	toolbar->AddSpacer(10);
	toolbar->Add(change_theme_button);



	wxBoxSizer* interactives_sizer = new wxBoxSizer(wxVERTICAL);
	interactives_sizer->Add(toolbar, wxSizerFlags().Left().Expand());
	interactives_sizer->AddSpacer(10);
	interactives_sizer->Add(note_field, wxSizerFlags(1).Expand());

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
	outer_sizer->Add(interactives_sizer, wxSizerFlags().Border(wxALL, 10));


	panel->SetSizer(outer_sizer);
	panel->Layout();
	outer_sizer->SetSizeHints(this);

	SetTheme(theme);
	BindObjects();


	note_field_panel_sizer->Add(note_field_panel, wxSizerFlags().Left());
	note_field->SetSizer(note_field_panel_sizer);
	this->Show();
}

void MainFrame::BindObjects()
{
	change_theme_button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {OnChangeThemeButtonClicked(event, theme);});
	add_note_button->Bind(wxEVT_BUTTON, &MainFrame::OnAddNoteButtonClicked, this);
	this->Bind(wxEVT_SIZE, &MainFrame::OnWindowResize, this);
}

void MainFrame::OnChangeThemeButtonClicked(wxCommandEvent& event, unsigned theme)
{
	SetTheme(theme);
}

void MainFrame::OnAddNoteButtonClicked(wxCommandEvent& event)
{
	AddNote();
}

void MainFrame::OnWindowResize(wxSizeEvent& event)
{
	UpdateNoteDisplay();
	event.Skip();
}

wxBitmap MainFrame::GetScaledImage(wxString address, unsigned width, unsigned height)
{
	//TODO: consider case image address unfound
	wxBitmap bitmap_image(address, wxBITMAP_TYPE_PNG);
	wxImage image = bitmap_image.ConvertToImage();
	image.Rescale(width, height);
	bitmap_image = wxBitmap(image);

	return bitmap_image;
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
		for (wxPanel* note_label : note_labels)
			note_label->SetBackgroundColour(RGB(255, 255, 255));
		break;

	case 1:
		UpdateBitButton("Assets/theme 2/add.png", add_note_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/magnifying glass.png", search_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/trash bin.png", delete_button, image_width, image_height);
		UpdateBitButton("Assets/theme 2/pointing circles.png", change_theme_button, image_width, image_height);
		ChangeBgOfWindow(this, RGB(255, 255, 255));
		for (wxPanel* note_label : note_labels)
			note_label->SetBackgroundColour(RGB(255, 255, 255));
		break;
	}

	SaveTheme(theme);

	if (theme)
		theme = 0;
	else
		theme = 1;

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
	wxBitmap bitmap_image = GetScaledImage(image_address, image_width, image_height);
	button->SetBitmapLabel(bitmap_image);
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

std::vector<MainFrame::NoteInfo> MainFrame::GetNotes()
{
	std::vector<NoteInfo> notes;
	return notes;
}

void MainFrame::AddNote()
{
	Note note = Note();
	NoteInfo note_info = 
	{
		.title = note.title,
		.content = note.content,
		.date_of_creation = note.date_of_creation,
		.date_of_last_edition = note.date_of_last_edition,
		.pinned = note.pinned, 
		.special = note.special
	};
	notes.push_back(note);

	wxPanel* note_label = new wxPanel(note_field_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	note_labels.push_back(note_label);

	DisplayNote(note_label, note_info);
}

void MainFrame::UpdateNoteDisplay()
{
	note_field_sizer->Clear(true);

	wxSize client_window_size = this->GetClientSize();
	client_window_size.y -= bitmap_button_height;
	note_field->SetMinSize(client_window_size);

	this->Layout();
	note_field->FitInside();
	for (Note note : notes)
	{
		NoteInfo note_info =
		{
			.title = note.title,
			.content = note.content,
			.date_of_creation = note.date_of_creation,
			.date_of_last_edition = note.date_of_last_edition,
			.pinned = note.pinned,
			.special = note.special
		};
		wxPanel* note_label = new wxPanel(note_field_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		note_labels.push_back(note_label);
		DisplayNote(note_label, note_info);
	}
}

void MainFrame::TrickOutNote(wxPanel* note, NoteInfo info)
{
	wxSize client_window_size = this->GetClientSize();
	client_window_size.x -= scroller_width;
	int note_label_width = (client_window_size.x - (columns - 1) * gap) / columns;

	note->SetMinSize(wxSize(note_label_width, int(note_label_ratio * note_label_width)));
	note->SetBackgroundColour(RGB(255, 255, 255));

	//topbar
	wxBoxSizer* topbar = new wxBoxSizer(wxHORIZONTAL);
	note->SetSizer(topbar);

	wxString short_date;
	short_date << info.date_of_last_edition[1] << "/" << info.date_of_last_edition[2] << "/" << info.date_of_last_edition[0];
	wxStaticText* date_label = new wxStaticText(note, wxID_ANY, short_date);
	date_label->SetBackgroundColour(RGB(255, 255, 255));

	wxBitmap pin, special;
	if (info.pinned)
		pin = GetScaledImage("Assets/General/pinned.png", bitmap_width, bitmap_height);
	else
		pin = GetScaledImage("Assets/General/unpinned.png", bitmap_width, bitmap_height);
	wxStaticBitmap* pin_image = new wxStaticBitmap(note, wxID_ANY, pin);


	if (info.special)
		special = GetScaledImage("Assets/General/golden star.png", bitmap_width, bitmap_height);
	else
		special = GetScaledImage("Assets/General/bleak star.png", bitmap_width, bitmap_height);
	wxStaticBitmap* special_image = new wxStaticBitmap(note, wxID_ANY, special);

	topbar->Add(pin_image);
	topbar->Add(special_image);
	topbar->AddStretchSpacer(1);
	topbar->Add(date_label);


	//line
	Drawing* canvas = new Drawing(note);
	canvas->DrawLine(wxPoint(0, bitmap_height * 1.5), wxPoint(note_label_width, bitmap_height * 1.5));

	//title


	//


	note->Layout();
}

void MainFrame::DisplayNote(wxPanel* note, NoteInfo info)
{
	wxSize client_window_size = this->GetClientSize();
	client_window_size.x -= scroller_width;
	
	note_field_sizer->Add(note);

	TrickOutNote(note, info);

	//I'm never using Fit() with wxGridSizers ever
	note_field_panel->SetMinSize(	wxSize(client_window_size.x - scroller_width, 
									ceil(float(notes.size()) / columns) * note->GetMinSize().GetX() + gap));

	note_field->Layout();
	note_field->FitInside();
}
