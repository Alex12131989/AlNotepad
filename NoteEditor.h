#pragma once
#include <wx/wx.h>
#include "Drawing.h"
#include "Note.h"


class NoteEditor : public wxFrame
{
public:
	NoteEditor(const wxString& title, Note note_class);
	void DrawTextures();

private:
	wxPanel* panel;
	Drawing* canvas;
};

