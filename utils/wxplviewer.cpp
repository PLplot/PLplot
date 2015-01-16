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
		argv[2].ToLong( &fileSize );
	}
	wxPlFrame* frame = new wxPlFrame( NULL, wxID_ANY, wxT("wxPlViewer"), file, PLINT(fileSize) );
	frame->SetIcon( wxICON( amain ) );
	frame->Show();
	return true;
}
