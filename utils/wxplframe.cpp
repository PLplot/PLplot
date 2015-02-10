#include "wxplframe.h"
#include <wx/menu.h>
#include<wx/msgdlg.h>

const int wxPlFrame::ID_FILE_EXIT = ::wxNewId();
const int wxPlFrame::ID_HELP_ABOUT = ::wxNewId();
const int wxPlFrame::ID_PAGE_NEXT = ::wxNewId();
const int wxPlFrame::ID_PAGE_PREV = ::wxNewId();
const int wxPlFrame::ID_CHECK_TIMER = ::wxNewId();


BEGIN_EVENT_TABLE( wxPlFrame, wxPLplotwindow<wxFrame> )
	EVT_MENU( ID_FILE_EXIT, wxPlFrame::OnExit )
	EVT_MENU( ID_HELP_ABOUT, wxPlFrame::OnAbout )
	EVT_MENU( ID_PAGE_NEXT, wxPlFrame::OnNextPage )
	EVT_MENU( ID_PAGE_PREV, wxPlFrame::OnPrevPage )
	EVT_TIMER( ID_CHECK_TIMER, wxPlFrame::OnCheckTimer )
END_EVENT_TABLE()


wxPlFrame::wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, wxString file, long fileSize,
	const wxPoint &pos, const wxSize &size, long style, const wxString &name )
	:wxPLplotwindow<wxFrame>( true ), m_checkTimer( this, ID_CHECK_TIMER )
{
	wxFrame::Create(parent, id, title, pos, size,  style, name);
	setupMenus();

	m_viewingPage = 0;
	m_writingPage = 0;
	m_file = file;
	m_fileSize = fileSize;

	if( file.length() > 0 )
	{
		m_memoryMap.create( file.c_str(), m_fileSize, true, false );
		if( m_memoryMap.isValid())
		{
			wxString mutexName = file + wxT("mut");
			m_mutex.create( mutexName.c_str() );
			if( !m_mutex.isValid() )
				m_memoryMap.close();
		}
	}
	if( !m_memoryMap.isValid() )
	{
		wxMessageBox( wxT("Error initializing the shared memory and/or mutex needed for the application. The application will close"));
		exit( 1 );
	}

	m_checkTimer.Start( 100 );
}

void wxPlFrame::setupMenus()
{
	wxMenuBar* mbar = new wxMenuBar();

	wxMenu* fileMenu = new wxMenu( wxT("") );
	fileMenu->Append( ID_FILE_EXIT, wxT("E&xit\tAlt+F4"), wxT("Exit the application") );
	mbar->Append( fileMenu, wxT("&File") );

	wxMenu* pageMenu = new wxMenu( wxT("") );
	pageMenu->Append( ID_PAGE_NEXT, wxT( "Next\tRIGHT" ), wxT( "Move to the next page" ) );
	pageMenu->Append( ID_PAGE_PREV, wxT( "Previous\tLEFT" ), wxT( "Move to the previous page" ) );
	mbar->Append( pageMenu, wxT("&Page") );

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

void wxPlFrame::OnCheckTimer( wxTimerEvent &event )
{
	//basically we check to see if we have any more data in the buffer
	//if so we add it to the apropriate plotBuffer
	if( m_memoryMap.isValid())
	{
		//lock the mutex for the duration of this scope
		PLNamedMutexLocker locker( &m_mutex );
		size_t &readLocation = *((size_t*)(m_memoryMap.getBuffer()));
		size_t &writeLocation = *((size_t*)(m_memoryMap.getBuffer())+1);
		//Check if there is anyhting to read
		if( readLocation == writeLocation )
			return;

		unsigned char transmissionType;
		transmissionType = *( ( unsigned char* )( m_memoryMap.getBuffer() + readLocation ) );
		size_t nRead = sizeof( unsigned char );

		if( transmissionType == transmissionSkipFileEnd )
		{
			readLocation = plMemoryMapReservedSpace;
			return;
		}
		else if( transmissionType == transmissionBeginPage )
		{
			m_pageBuffers.resize(m_pageBuffers.size() + 1 );
			m_pageCompleteFlags.push_back( false );
			m_writingPage = m_pageBuffers.size() - 1;
		}
		else if( transmissionType == transmissionEndOfPage )
		{
			m_pageCompleteFlags[m_writingPage] = true;
			if( m_writingPage == m_viewingPage )
			{
				GetStream()->ImportBuffer( &m_pageBuffers[m_viewingPage][0], m_pageBuffers[m_viewingPage].size() );
				Refresh();
			}
		}
		else if( transmissionType == transmissionPartial || transmissionType == transmissionComplete )
		{
			size_t dataSize;
			dataSize = *(size_t*)( ( unsigned char* )( m_memoryMap.getBuffer() + readLocation +
				sizeof( transmissionType ) ) );
			m_pageBuffers[m_writingPage].insert( m_pageBuffers[m_writingPage].end(),
				m_memoryMap.getBuffer() + readLocation + sizeof( transmissionType ) + sizeof (dataSize),
				m_memoryMap.getBuffer() + readLocation + sizeof( transmissionType ) + sizeof (dataSize) + dataSize );
			nRead += sizeof (dataSize) + dataSize;
		}
		readLocation += nRead;
		if( readLocation == m_memoryMap.getSize() )
			readLocation = plMemoryMapReservedSpace;
	}
}

void wxPlFrame::OnNextPage( wxCommandEvent& event )
{
	if( m_viewingPage == m_pageBuffers.size() - 1 )
		return;
	++m_viewingPage;
	GetStream()->ImportBuffer( &m_pageBuffers[m_viewingPage][0], m_pageBuffers[m_viewingPage].size() );
	Refresh();
}

void wxPlFrame::OnPrevPage( wxCommandEvent& event )
{
	if( m_viewingPage == 0 )
		return;
	--m_viewingPage;
	GetStream()->ImportBuffer( &m_pageBuffers[m_viewingPage][0], m_pageBuffers[m_viewingPage].size() );
	Refresh();
}