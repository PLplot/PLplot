#include "wxplframe.h"
#include <wx/menu.h>
#include<wx/msgdlg.h>

const int wxPlFrame::ID_FILE_EXIT = ::wxNewId();
const int wxPlFrame::ID_HELP_ABOUT = ::wxNewId();


BEGIN_EVENT_TABLE( wxPlFrame, wxPLplotwindow<wxFrame> )
	EVT_MENU( ID_FILE_EXIT, wxPlFrame::OnExit )
	EVT_MENU( ID_HELP_ABOUT, wxPlFrame::OnAbout )
END_EVENT_TABLE()


wxPlFrame::wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
		const wxSize &size, long style, const wxString &name )
		:wxPLplotwindow<wxFrame>( true )
{
	wxFrame::Create(parent, id, title, pos, size,  style, name);
	setupMenus();
}

wxPlFrame::wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, wxString mfi, PLINT mfiSize, const wxPoint &pos,
		const wxSize &size, long style, const wxString &name )
		:wxPLplotwindow( true, mfi, mfiSize )
{
	wxFrame::Create(parent, id, title, pos, size,  style, name);
	setupMenus();
}

void wxPlFrame::setupMenus()
{
	wxMenuBar* mbar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu( wxT("") );
	fileMenu->Append( ID_FILE_EXIT, wxT("E&xit\tAlt+F4"), wxT("Exit the application") );
	mbar->Append( fileMenu, wxT("&File") );

	wxMenu* helpMenu = new wxMenu( wxT("") );
	helpMenu->Append( ID_HELP_ABOUT, wxT("&About\tF1"), wxT("Show info about this application") );
	mbar->Append( helpMenu, wxT("&Help") );

	SetMenuBar( mbar );
}

void wxPlFrame::OnExit( wxCommandEvent& event )
{
	Close();
}

void wxPlFrame::OnAbout( wxCommandEvent& event )
{
	wxMessageBox( wxT("wxPlViewer version 1.00.0"), wxT("About wxPlViewer...") );
}

wxPlFrame::~wxPlFrame()
{
}