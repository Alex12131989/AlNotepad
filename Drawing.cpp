#include "Drawing.h"

void Drawing::Line(const wxPoint& start, const wxPoint& end)
{
    lines.push_back(std::make_pair(start, end));
    Refresh();
}

void Drawing::SetPenColor(wxColour color)
{
    pen_color = color;
}

void Drawing::SetPenWidthInPixels(int width_pixels)
{
    pen_width = width_pixels;
}

void Drawing::Draw(wxPaintEvent& event)
{
    wxBufferedPaintDC canvas(this);
    canvas.Clear();
    canvas.SetPen(wxPen(pen_color, pen_width));

    for (auto line : lines)
        canvas.DrawLine(line.first.x, line.first.y, line.second.x, line.second.y);
}