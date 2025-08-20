#pragma once
#include <wx/wx.h>

const int MainWindowHeight = 600;
const int MainWindowWidth = 800;

class App : public wxApp
{
public: 
	bool OnInit();
};