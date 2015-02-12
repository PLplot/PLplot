// Copyright (C) 2015  Phil Rosenberg
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

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
	EVT_KEY_DOWN( wxPlFrame::OnKey )
	EVT_MOUSE_EVENTS( wxPlFrame::OnMouse )
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

	m_locateMode = false;
	m_plottedBufferAmount = 0;
	//signal that we have opened the file
	*( (size_t*)m_memoryMap.getBuffer() + 2 ) = 1;
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
		//Check if there is anything to read
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
			m_bufferValidFlags.push_back( false );
			m_writingPage = m_pageBuffers.size() - 1;
		}
		else if( transmissionType == transmissionEndOfPage )
		{
			if( !m_bufferValidFlags[m_writingPage] )
				throw( "Received an end of page transmission after an incomplete or no draw instruction" );
			SetPageAndUpdate();
		}
		else if( transmissionType == transmissionLocate )
		{
			SetPageAndUpdate();
			m_locateMode = true;
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
			if( transmissionType == transmissionComplete )
				m_bufferValidFlags[m_writingPage] = true;
			else
				m_bufferValidFlags[m_writingPage] = false;

			//if we have a lot of buffer unplotted then plot it so we don't look like we are doing nothing
			if( m_writingPage == m_viewingPage &&
				( m_plottedBufferAmount + 1024 * 1024 ) < m_pageBuffers[ m_writingPage ].size() )
				SetPageAndUpdate();
		}
		readLocation += nRead;
		if( readLocation == m_memoryMap.getSize() )
			readLocation = plMemoryMapReservedSpace;
	}
}

void wxPlFrame::OnNextPage( wxCommandEvent& event )
{
	SetPageAndUpdate( m_viewingPage + 1 );
}

void wxPlFrame::OnPrevPage( wxCommandEvent& event )
{
	SetPageAndUpdate( m_viewingPage - 1 );
}

void wxPlFrame::OnMouse( wxMouseEvent &event )
{
	//save the mouse position for use in key presses
	m_cursorPosition = event.GetPosition();

	//If the mouse button was clicked then
    if ( m_locateMode && event.ButtonDown() )
    {
		PLNamedMutexLocker locker( &m_mutex );
		wxSize clientSize = GetClientSize();
		PLGraphicsIn &graphicsIn = *(PLGraphicsIn*)( ( ( size_t * ) m_memoryMap.getBuffer() ) + 4 ) ;
		graphicsIn.pX = m_cursorPosition.x;
		graphicsIn.pY = m_cursorPosition.y;
		graphicsIn.dX = PLFLT( m_cursorPosition.x + 0.5 ) / PLFLT( clientSize.GetWidth() );
		graphicsIn.dY = 1.0 - PLFLT( m_cursorPosition.y + 0.5 ) / PLFLT( clientSize.GetHeight() );

        if ( event.LeftDown() )
        {
            graphicsIn.button = 1;      // X11/X.h: #define Button1	1
            graphicsIn.state  = 1 << 8; // X11/X.h: #define Button1Mask	(1<<8)
        }
        else if ( event.MiddleDown() )
        {
            graphicsIn.button = 2;      // X11/X.h: #define Button2	2
            graphicsIn.state  = 1 << 9; // X11/X.h: #define Button2Mask	(1<<9)
        }
        else if ( event.RightDown() )
        {
            graphicsIn.button = 3;       // X11/X.h: #define Button3	3
            graphicsIn.state  = 1 << 10; // X11/X.h: #define Button3Mask	(1<<10)
        }

		graphicsIn.keysym = 0x20;        // keysym for button event from xwin.c

		*( ( ( size_t * ) m_memoryMap.getBuffer() ) + 3 ) = 0;
		m_locateMode = false;
    }
}

void wxPlFrame::OnKey( wxKeyEvent &event )
{
	if ( m_locateMode )
	{
		PLNamedMutexLocker locker( &m_mutex );
		PLGraphicsIn &graphicsIn = *(PLGraphicsIn*)( ( ( size_t * ) m_memoryMap.getBuffer() ) + 4 ) ;

		wxSize clientSize = GetClientSize();

		graphicsIn.pX = m_cursorPosition.x;
		graphicsIn.pY = m_cursorPosition.y;
		graphicsIn.dX = PLFLT( m_cursorPosition.x + 0.5 ) / PLFLT( clientSize.GetWidth() );
		graphicsIn.dY = 1.0 - PLFLT( m_cursorPosition.y + 0.5 ) / PLFLT( clientSize.GetHeight() );

		// gin->state = keyEvent->state;

		int keycode = event.GetKeyCode();
		graphicsIn.string[0] = (char) keycode;
		graphicsIn.string[1] = '\0';

		// ESCAPE, RETURN, etc. are already in ASCII equivalent
		graphicsIn.keysym = keycode;

		*( ( ( size_t * ) m_memoryMap.getBuffer() ) + 3 ) = 0;
		m_locateMode = false;
	}
}

void wxPlFrame::SetPageAndUpdate( size_t page )
{
	//if we get a page of -1 (which will really be max size_t) then just update the current page
	//otherwise switch to the given page
	if( page != size_t(-1) )
	{
		if( page > m_pageBuffers.size() )
			return;
		if( page != m_viewingPage )
		{
			m_viewingPage = page;
			m_plottedBufferAmount = 0;
		}
	}

	//if there is unplotted buffer and the buffer is valid (contains some instructions and ends with a
	//valid instruction) then send that buffer to the driver and replot the page.
	if( m_bufferValidFlags[m_viewingPage] && m_plottedBufferAmount < m_pageBuffers[m_viewingPage].size() )
	{
		m_plottedBufferAmount = m_pageBuffers[m_viewingPage].size();
		GetStream()->ImportBuffer( &m_pageBuffers[m_viewingPage][0], m_pageBuffers[m_viewingPage].size() );
		Refresh();
	}
}