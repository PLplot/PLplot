#ifndef WXPLFRAME_H
#define WXPLFRAME_H

#include <wx/frame.h>
#include "wxPLplotstream.h"
#include "wxPLplotwindow.h"

class wxPlFrame : public wxPLplotwindow<wxFrame>
{
public:
	static const int ID_FILE_EXIT;
	static const int ID_HELP_ABOUT;

	wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition,
		const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE, const wxString &name=wxFrameNameStr );
	wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, wxString mfi, PLINT mfiSize, const wxPoint &pos=wxDefaultPosition,
		const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE, const wxString &name=wxFrameNameStr );
	~wxPlFrame();
private:
	void setupMenus();
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	DECLARE_EVENT_TABLE();
};
#endif // APEX_MAINFRAME_H
