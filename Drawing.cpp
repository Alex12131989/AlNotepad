#include "Drawing.h"

void Drawing::DrawLine(const wxPoint& start, const wxPoint& end)
{
    lines.push_back(std::make_pair(start, end));
    Refresh();
}

void Drawing::Draw(wxPaintEvent& event)
{
    wxPaintDC canvas(this);
    canvas.SetPen(wxPen(RGB(0, 0, 0), 2));
    for (auto line : lines)
        canvas.DrawLine(line.first.x, line.first.y, line.second.x, line.second.y);
}