#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "MainFrame.h"
#include <experimental/filesystem>
#include <fstream>

const int MAX_THEMES = 2, bitmap_button_width = 25, bitmap_button_height = 25;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	panel = new wxPanel(this);

	wxBitmap passer;
	add_note_button = new wxBitmapButton(panel, wxID_ANY, passer, wxPoint(0, 0), wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	search_button = new wxBitmapButton(panel, wxID_ANY, passer, wxPoint(bitmap_button_width, 0), wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	delete_button = new wxBitmapButton(panel, wxID_ANY, passer, wxPoint(2*bitmap_button_width, 0), wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	change_theme_button = new wxBitmapButton(panel, wxID_ANY, passer, wxPoint(3*bitmap_button_width, 0), wxSize(bitmap_button_width, bitmap_button_height), wxBORDER_NONE);
	
	theme = GetTheme();
	SetTheme(theme);
	BindButtons();

	//wxButton* button = new wxButton(panel, wxID_ANY, "Button", wxPoint(150, 50), wxSize(100, 35), wxBU_EXACTFIT);

	//wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", wxPoint(550, 50), wxSize(100, 35), wxCHK_3STATE | wxCHK_ALLOW_3RD_STATE_FOR_USER);

	//wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static Text", wxPoint(150, 150), wxSize(100, 50), wxALIGN_CENTRE_HORIZONTAL);
	//staticText->SetBackgroundColour(*wxLIGHT_GREY);

	//wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "Not Static Text", wxPoint(550, 150), wxSize(100, 50), wxTE_PASSWORD);

	//wxSlider* slider = new wxSlider(panel, wxID_ANY, 50, 0, 100, wxPoint(150, 250), wxSize(200, 35), wxSL_VALUE_LABEL);

	//wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(550, 250), wxSize(200, 35), wxGA_HORIZONTAL);
	//gauge->SetValue(89);

	//wxArrayString choices;
	//choices.Add("Item A");
	//choices.Add("Item B");
	//choices.Add("Item C");
	//choices.Add("Item D");

	//wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 350), wxSize(200, 50), choices, wxCB_SORT);
	//choice->Select(1);

	//wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 350), wxSize(200, 25), wxSP_WRAP);

	//wxListBox* list = new wxListBox(panel, wxID_ANY, wxPoint(150, 450), wxSize(200, -1), choices, wxLB_MULTIPLE);

	//wxRadioBox* radio = new wxRadioBox(panel, wxID_ANY, "Radio Box", wxPoint(550, 450), wxDefaultSize, choices, 2, wxRA_SPECIFY_COLS);

}

void MainFrame::BindButtons()
{
	change_theme_button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {SetTheme(theme);});
	add_note_button;
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
