#include "MainFrame.h"
#include "NoteEditor.h"
#include "App.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <fstream>
#include <chrono>
#include <cmath>


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	this->SetMinSize(wxSize(MainWindowWidth, MainWindowHeight));
	panel = new wxPanel(this);

	wxBoxSizer* note_field_panel_sizer = new wxBoxSizer(wxVERTICAL);
	note_field = new wxScrolledWindow(panel, wxID_ANY, wxPoint(0, 60), wxDefaultSize, wxVSCROLL);
	note_field->SetMinSize(wxSize(MainWindowWidth, MainWindowHeight - bitmap_button_height));
	note_field->SetScrollRate(0, 10);

	note_field_panel = new wxPanel(note_field, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	note_field_panel->SetMinSize(wxSize(MainWindowWidth - scroller_width, MainWindowHeight - bitmap_button_height));
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

	SetTheme(theme);
	BindObjects();


	note_field_panel_sizer->Add(note_field_panel, wxSizerFlags().Left());
	note_field->SetSizer(note_field_panel_sizer);
	this->Layout();
	LoadNotes();
	this->Show();
}

void MainFrame::BindObjects()
{
	change_theme_button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {OnChangeThemeButtonClicked(event, theme);});
	add_note_button->Bind(wxEVT_BUTTON, &MainFrame::OnAddNoteButtonClicked, this);
	this->Bind(wxEVT_SIZE, &MainFrame::OnWindowResize, this);
}

void MainFrame::OnChangeThemeButtonClicked(wxCommandEvent& event, int theme)
{
	SetTheme(theme);
}

void MainFrame::OnAddNoteButtonClicked(wxCommandEvent& event)
{
	AddNewNote();
}

void MainFrame::OnWindowResize(wxSizeEvent& event)
{
	UpdateNoteDisplay();
	event.Skip();
}

void MainFrame::SetTheme(int number)
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

void MainFrame::SaveTheme(int number)
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "theme.txt";
	std::ofstream theme_file(working_path);
	theme_file << number;
}

int MainFrame::GetTheme()
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "theme.txt";
	std::ifstream theme_file(working_path);

	int theme = 0;
	if (theme_file.is_open())
	{
		theme_file >> theme;
		if (theme < 0 || theme > MAX_THEMES - 1)
			theme = 0;
	}
	return theme;
}

wxString MainFrame::GenerateCode(int size)
{
	wxString code;
	return code;
}

void MainFrame::SaveNotes()
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "user notes.txt";

	std::ofstream user_date_file(working_path);
	std::vector<std::string> codes = { {/*NEW NOTE*/}, {/*END OF THE FIELD*/}};
	codes[0] = "kjldsfi3o4urifeklsfj8iewuri43uij52j54r8ufkcvd";
	codes[1] = "lksdjfklnmjfio43u5uweklmfkcdnkfhkaer23u3o123a";
	//for (std::string& code : codes)
	//	code = GenerateCode();
	user_date_file << "--NEWNOTE:" << codes[0] << '\n';
	user_date_file << "--ENDOFTHEFIELD:" << codes[1] << '\n';
	for (Note note : notes)
	{
		user_date_file << codes[0] << '\n';

		user_date_file << note.title << '\n';
		user_date_file << codes[1] << '\n';

		user_date_file << note.content << '\n';
		user_date_file << codes[1] << '\n';

		user_date_file << 
			note.date_of_creation[0] << "." <<
			note.date_of_creation[1] << "." << 
			note.date_of_creation[2] << "." <<
			note.date_of_creation[3] << "." <<
			note.date_of_creation[4] << "." << 
			note.date_of_creation[5] << '\n';
		user_date_file << codes[1] << '\n';

		user_date_file <<
			note.date_of_last_edition[0] << "." <<
			note.date_of_last_edition[1] << "." <<
			note.date_of_last_edition[2] << "." <<
			note.date_of_last_edition[3] << "." <<
			note.date_of_last_edition[4] << "." <<
			note.date_of_last_edition[5] << '\n';
		user_date_file << codes[1] << '\n';

		user_date_file << std::boolalpha << note.pinned << '\n';
		user_date_file << codes[1] << '\n';

		user_date_file << std::boolalpha << note.special << '\n';
		user_date_file << codes[1] << '\n';
	}
}

std::vector<int> MainFrame::ConvertStringToDateVector(std::string string)
{
	std::vector<std::string> date_strings = { "year", "month", "day", "hour", "minute", "second" };

	size_t start = 0;
	for (std::string &date_string : date_strings)
	{
		size_t end = string.find(".", start);
		date_string = string.substr(start, end - start);
		start = end + 1;
	}
	std::vector<int> date = {};
	for (std::string date_string : date_strings)
	{
		date.push_back(std::stoi(date_string));
	}
	return date;
}

bool MainFrame::ConvertStringToBool(std::string string)
{
	if (string == "true")
		return true;
	else
		return false;
}

std::vector<MainFrame::NoteInfo> MainFrame::GetNotes()
{
	using namespace std::experimental::filesystem;
	path working_path = current_path();
	working_path /= "UserData";
	create_directories(working_path);
	working_path /= "user notes.txt";
	
	std::ifstream user_data_file(working_path);
	std::vector<NoteInfo> notes;
	std::vector<std::vector<std::string>> pulled_notes;
	if (user_data_file.is_open())
	{
		std::string buffer_line;
		std::vector<std::string> codes = { {/*new file code*/}, {/*end of field code */ }};

		for (int i = 0; i < codes.size(); ++i)
		{
			std::getline(user_data_file, buffer_line);
			size_t start = buffer_line.find(":", 0);
			++start;
			size_t end = buffer_line.find('\n');
			if (start != std::string::npos)
			{
				std::string code = buffer_line.substr(start, end - start);
				codes[i] = code;
			}
			else
				//not enough codes
				return notes;
		}

		bool switched_code = false;
		std::string pending;
		while (std::getline(user_data_file, buffer_line))
		{
			if (switched_code)
			{
				pending.resize(pending.size() - 1);
				pulled_notes[pulled_notes.size() - 1].push_back(pending);
				pending = "";
				switched_code = false;
			}
			
			if (buffer_line == codes[0])
			{
				pulled_notes.push_back({});
			}
			else if (buffer_line == codes[1])
			{
				switched_code = true;
			}
			else
			{
				pending += buffer_line + '\n';
			}
		}
		pending.resize(pending.size() - 1);
		pulled_notes[pulled_notes.size() - 1].push_back(pending);

		for (std::vector<std::string> pulled_note : pulled_notes)
		{
			std::vector<int> creation, last_edition;
			creation = ConvertStringToDateVector(pulled_note[2]);
			last_edition = ConvertStringToDateVector(pulled_note[3]);
			bool pinned, special;
			pinned = ConvertStringToBool(pulled_note[4]);
			special = ConvertStringToBool(pulled_note[5]);

			NoteInfo note =
			{
				.title = pulled_note[0],
				.content = pulled_note[1],
				.date_of_creation = creation,
				.date_of_last_edition = last_edition,
				.pinned = pinned,
				.special = special
			};

			notes.push_back(note);
		}

	}

	return notes;
}

void MainFrame::AddNewNote()
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
	SaveNotes();
}

void MainFrame::UpdateNoteDisplay()
{
	wxSize total_window_client_size = this->GetClientSize();
	note_field->SetMinSize(wxSize(total_window_client_size.x, total_window_client_size.y - bitmap_button_height));

	wxSize client_window_size = note_field->GetMinClientSize();
	int note_label_width = (client_window_size.x - scroller_width - (columns - 1) * gap) / columns;
	for (int i = 0; i < note_labels.size(); ++i)
	{
		note_labels[i]->SetMinSize(wxSize(note_label_width, int(note_label_ratio * note_label_width)));
		canvases[i]->lines[0].second.x = note_label_width;
		canvases[i]->Refresh();
	}
	if (note_labels.size())
		note_field_panel->SetMinSize(	wxSize(client_window_size.x - scroller_width,
										ceil(float(note_labels.size()) / columns) * (note_labels[0]->GetMinSize().GetX() + gap) - gap));
	note_field->Layout();
	note_field->FitInside();
}

void MainFrame::TrickOutNote(wxPanel* note, NoteInfo info)
{
	wxSize client_window_size = note_field->GetClientSize();
	client_window_size.x -= scroller_width;
	int note_label_width = (client_window_size.x - (columns - 1) * gap) / columns;

	note->SetMinSize(wxSize(note_label_width, int(note_label_ratio * note_label_width)));
	note->SetBackgroundColour(RGB(255, 255, 255));

	//topbar
	wxBoxSizer* topbar = new wxBoxSizer(wxHORIZONTAL);
	//note->SetSizer(topbar);

	wxString short_date;
	short_date << info.date_of_last_edition[1] << "/" << info.date_of_last_edition[2] << "/" << info.date_of_last_edition[0];
	wxStaticText* date_label = new wxStaticText(note, wxID_ANY, short_date);
	note->SetBackgroundColour(RGB(130, 130, 130));
	for (wxWindow* child : note->GetChildren())
		child->SetBackgroundColour(RGB(130, 130, 130));

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
	topbar->AddStretchSpacer(13);
	topbar->Add(date_label);
	topbar->AddStretchSpacer(1);


	//line
	Drawing* canvas = new Drawing(note);
	canvas->Line(wxPoint(0, 0), wxPoint(note_label_width, 0));
	canvases.push_back(canvas);

	//title
	wxPanel* title_area = new wxPanel(note);
	wxStaticText* title = new wxStaticText(title_area, wxID_ANY, info.title, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	wxBoxSizer* title_area_sizer = new wxBoxSizer(wxVERTICAL);
	title_area->SetSizer(title_area_sizer);

	title_area_sizer->AddStretchSpacer();
	title_area_sizer->Add(title, wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL));
	title_area_sizer->AddStretchSpacer();

	title_area->SetBackgroundColour(RGB(255, 255, 255));

	//piecing everything together
	wxBoxSizer* ready_note_layout = new wxBoxSizer(wxVERTICAL);
	note->SetSizer(ready_note_layout);

	ready_note_layout->Add(topbar, wxSizerFlags(0).Expand());
	ready_note_layout->Add(canvas, wxSizerFlags(0).Expand());
	canvas->Refresh();
	ready_note_layout->Add(title_area, wxSizerFlags(1).Expand());

	note->Layout();
}

void MainFrame::DisplayNote(wxPanel* note, NoteInfo info)
{
	wxSize client_window_size = note_field->GetClientSize();
	client_window_size.x -= scroller_width;
	
	note_field_sizer->Add(note);

	
	TrickOutNote(note, info);

	//I'm never using Fit() with wxGridSizers ever
	note_field_panel->SetMinSize(	wxSize(client_window_size.x - scroller_width, 
									ceil(float(note_labels.size()) / columns) * (note->GetMinSize().GetX() + gap) - gap));

	note_field->Layout();
	note_field->FitInside();
}

void MainFrame::LoadNotes()
{
	std::vector<NoteInfo> user_notes = GetNotes();
	for (NoteInfo note_info : user_notes)
	{
		Note note = Note();
		note.title = note_info.title;
		note.content = note_info.content;
		note.date_of_creation = note_info.date_of_creation;
		note.date_of_last_edition = note_info.date_of_last_edition;
		note.pinned = note_info.pinned;
		note.special = note_info.special;

		notes.push_back(note);

		wxPanel* note_label = new wxPanel(note_field_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
		note_labels.push_back(note_label);

		DisplayNote(note_label, note_info);
	}
}

void MainFrame::CreateNewWindow(wxString title, Note note_class)
{
	wxInitAllImageHandlers();
	NoteEditor* secondFrame = new NoteEditor(title, note_class);
	secondFrame->SetClientSize(MainWindowWidth, MainWindowHeight);
	secondFrame->Center();
	secondFrame->Show();
}