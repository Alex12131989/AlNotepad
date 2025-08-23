#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>

class Drawing : public wxPanel
{
public:
	
	Drawing(wxWindow* parent) : wxPanel(parent)
	{
		SetBackgroundStyle(wxBG_STYLE_PAINT);
		Bind(wxEVT_PAINT, &Drawing::Draw, this);
	}
	
	void Line(const wxPoint& start, const wxPoint& end);
	void SetPenWidthInPixels(int width_pixels);
	void SetPenColor(wxColour color);

	std::vector<std::pair<wxPoint, wxPoint>> lines;

private:
	wxColour pen_color = RGB(0, 0, 0);
	int pen_width = 1;
	void Draw(wxPaintEvent& event);
};

