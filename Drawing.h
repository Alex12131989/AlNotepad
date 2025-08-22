#pragma once
#include <wx/wx.h>

class Drawing : public wxPanel
{
public:
	
	Drawing(wxWindow* parent) : wxPanel(parent)
	{
		Bind(wxEVT_PAINT, &Drawing::DrawLine, this);
	}
	
	void DrawLine(const wxPoint& start, const wxPoint& end);

private:
	std::vector<std::pair<wxPoint, wxPoint>> lines;
	void DrawLine(wxPaintEvent& event);
};

