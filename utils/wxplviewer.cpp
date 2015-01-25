#include "wxplframe.h"
#include<wx/wx.h>


class wxPlViewerApp : public wxApp
{
public:
	virtual bool OnInit();
};

IMPLEMENT_APP( wxPlViewerApp );

bool wxPlViewerApp::OnInit()
{
	wxInitAllImageHandlers();
	wxString file;
	long fileSize = 0;
	if (argc==3)
	{
		file = argv[1];
		wxString(argv[2]).ToLong( &fileSize );
	}
#ifdef __WXGTK__
	//This is to deal with the bug described at
	//https://forums.wxwidgets.org/viewtopic.php?t=37151&p=151356
	//Although this bug has been fixed acording to
	//http://trac.wxwidgets.org/ticket/16202 and
	//http://trac.wxwidgets.org/changeset/76398/svn-wx
	//I currently don't know which wxWidgets version it is
	//included in so I am applyin gthis to all wxGTK
	//builds
	wxMilliSleep( 100 );
#endif
	wxPlFrame* frame = new wxPlFrame( NULL, wxID_ANY, wxT("wxPlViewer"), file,
		PLINT(fileSize), wxDefaultPosition, wxSize( 500, 500 ) );
	frame->Show();
	return true;
}
