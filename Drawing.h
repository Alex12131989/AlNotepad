#pragma once
#include <wx/wx.h>

class Drawing : public wxPanel
{
public:
	
	Drawing(wxWindow* parent) : wxPanel(parent)
	{
		Bind(wxEVT_PAINT, &Drawing::Draw, this);
	}
	
	void DrawLine(const wxPoint& start, const wxPoint& end);
	std::vector<std::pair<wxPoint, wxPoint>> lines;

private:
	void Draw(wxPaintEvent& event);
};

