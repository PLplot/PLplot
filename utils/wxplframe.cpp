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
#include <wx/msgdlg.h>

const int wxPlFrame::ID_FILE_EXIT      = ::wxNewId();
const int wxPlFrame::ID_HELP_ABOUT     = ::wxNewId();
const int wxPlFrame::ID_PAGE_NEXT      = ::wxNewId();
const int wxPlFrame::ID_PAGE_PREV      = ::wxNewId();
const int wxPlFrame::ID_PAGE_FIXASPECT = ::wxNewId();
const int wxPlFrame::ID_CHECK_TIMER    = ::wxNewId();


BEGIN_EVENT_TABLE( wxPlFrame, wxPLplotwindow<wxFrame> )
EVT_MENU( ID_FILE_EXIT, wxPlFrame::OnExit )
EVT_MENU( ID_HELP_ABOUT, wxPlFrame::OnAbout )
EVT_MENU( ID_PAGE_FIXASPECT, wxPlFrame::OnToggleFixAspect )
EVT_MENU( ID_PAGE_NEXT, wxPlFrame::OnNextPage )
EVT_MENU( ID_PAGE_PREV, wxPlFrame::OnPrevPage )
EVT_TIMER( ID_CHECK_TIMER, wxPlFrame::OnCheckTimer )
EVT_KEY_DOWN( wxPlFrame::OnKey )
EVT_MOUSE_EVENTS( wxPlFrame::OnMouse )
END_EVENT_TABLE()


wxPlFrame::wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, wxString file, long fileSize,
                      const wxPoint &pos, const wxSize &size, long style, const wxString &name )
    : wxPLplotwindow<wxFrame>( true ), m_checkTimer( this, ID_CHECK_TIMER )
{
    wxFrame::Create( parent, id, title, pos, size, style, name );
    setupMenus();

    m_viewingPage          = 0;
    m_writingPage          = 0;
    m_file                 = file;
    m_fileSize             = fileSize;
    m_inCheckTimerFunction = false;

    if ( file.length() > 0 )
    {
        m_memoryMap.create( file.mb_str(), m_fileSize, true, false );
        if ( m_memoryMap.isValid() )
        {
            wxString mutexName = file + wxT( "mut" );
            m_mutex.create( mutexName.mb_str() );
            if ( !m_mutex.isValid() )
                m_memoryMap.close();
        }
    }
    if ( !m_memoryMap.isValid() )
    {
        wxMessageBox( wxT( "Error initializing the shared memory and/or mutex needed for the application. The application will close" ) );
        exit( 1 );
    }

    m_locateMode          = false;
    m_transferComplete    = false;
    m_plottedBufferAmount = 0;
    //signal that we have opened the file
    MemoryMapHeader *header = (MemoryMapHeader *) ( m_memoryMap.getBuffer() );
    header->viewerOpenFlag = 1;
    m_checkTimer.Start( m_idleTimerInterval );
}

void wxPlFrame::setupMenus()
{
    wxMenuBar* mbar = new wxMenuBar();

    wxMenu   * fileMenu = new wxMenu( wxT( "" ) );
    fileMenu->Append( ID_FILE_EXIT, wxT( "E&xit\tAlt+F4" ), wxT( "Exit the application" ) );
    mbar->Append( fileMenu, wxT( "&File" ) );


    wxMenu     * pageMenu      = new wxMenu( wxT( "" ) );
    wxMenuItem *aspectMenuItem = new wxMenuItem( pageMenu, ID_PAGE_FIXASPECT, wxT( "Fix Aspect" ),
        wxT( "Fix the aspect ratio of the plot" ), wxITEM_CHECK );
    pageMenu->Append( aspectMenuItem );
    aspectMenuItem->Check( true );
    pageMenu->Append( ID_PAGE_PREV, wxT( "Previous\tPgUp" ), wxT( "Move to the previous page" ) );
    pageMenu->Append( ID_PAGE_NEXT, wxT( "Next\tEnter" ), wxT( "Move to the next page" ) );
    mbar->Append( pageMenu, wxT( "&Page" ) );

    wxMenu* helpMenu = new wxMenu( wxT( "" ) );
    helpMenu->Append( ID_HELP_ABOUT, wxT( "&About\tF1" ), wxT( "Show info about this application" ) );
    mbar->Append( helpMenu, wxT( "&Help" ) );

    SetMenuBar( mbar );

    m_stream.SetFixedAspectRatio( aspectMenuItem->IsChecked() );
}

void wxPlFrame::OnExit( wxCommandEvent& event )
{
    Close();
}

void wxPlFrame::OnAbout( wxCommandEvent& event )
{
    wxMessageBox( wxT( "wxPlViewer version 1.00.0" ), wxT( "About wxPlViewer..." ) );
}

wxPlFrame::~wxPlFrame()
{
}

void wxPlFrame::OnCheckTimer( wxTimerEvent &event )
{
    //avoid reentrant behaviour if some function yields allowing the
    //timer to call this function again
    if ( m_inCheckTimerFunction )
        return;
    m_inCheckTimerFunction = true;
    //basically we check to see if we have any more data in the buffer
    //if so we add it to the apropriate plotBuffer
    if ( m_memoryMap.isValid() )
    {
        //lock the mutex for the duration of this scope
        PLNamedMutexLocker locker( &m_mutex );
        MemoryMapHeader    *header = (MemoryMapHeader *) ( m_memoryMap.getBuffer() );
        //Check if there is anything to read
        if ( header->readLocation == header->writeLocation )
        {
            if ( header->completeFlag != 0 )
            {
                //if there is nothing to read and the complete flag is set then
                // stop looking
                m_transferComplete = true;
                m_checkTimer.Stop();
            }
            else if ( m_currentTimerInterval != m_idleTimerInterval )
            {
                //if there is nothing to read but we might have more to come
                //then check less frequently
                m_checkTimer.Stop();
                m_checkTimer.Start( m_idleTimerInterval );
                m_currentTimerInterval = m_idleTimerInterval;
            }

            //nothing to do so return
            m_inCheckTimerFunction = false;
            return;
        }

        if ( m_currentTimerInterval != m_busyTimerInterval )
        {
            //If we have something to read then make sure
            //we keep checking regularly for more
            m_checkTimer.Stop();
            m_checkTimer.Start( m_busyTimerInterval );
            m_currentTimerInterval = m_busyTimerInterval;
        }

        unsigned char transmissionType;
        transmissionType = *( (unsigned char *) ( m_memoryMap.getBuffer() + header->readLocation ) );
        size_t        nRead = sizeof ( unsigned char );

        if ( transmissionType == transmissionSkipFileEnd )
        {
            header->readLocation   = plMemoryMapReservedSpace;
            m_inCheckTimerFunction = false;
            return;
        }
        else if ( transmissionType == transmissionBeginPage )
        {
            m_pageBuffers.resize( m_pageBuffers.size() + 1 );
            m_bufferValidFlags.push_back( false );
            m_writingPage = m_pageBuffers.size() - 1;
        }
        else if ( transmissionType == transmissionEndOfPage )
        {
            if ( !m_bufferValidFlags[m_writingPage] )
                throw( "Received an end of page transmission after an incomplete or no draw instruction" );
            SetPageAndUpdate();
        }
        else if ( transmissionType == transmissionLocate )
        {
            SetPageAndUpdate();
            m_locateMode = true;
        }
		else if (transmissionType == transmissionRequestTextSize )
		{
			wxClientDC dc( this );
			wxCoord width;
			wxCoord height;
			wxFont font( wxString( header->textSizeInfo.font ) );
			dc.GetTextExtent( wxString( header->textSizeInfo.text ), &width, &height, 0, 0, &font );
			header->textSizeInfo.width = long(width);
			header->textSizeInfo.height = long(height);
			header->textSizeInfo.written = true;
		}
        else if ( transmissionType == transmissionPartial || transmissionType == transmissionComplete )
        {
            size_t dataSize;
            dataSize = *(size_t *) ( (unsigned char *) ( m_memoryMap.getBuffer() + header->readLocation +
                                                         sizeof ( transmissionType ) ) );
            m_pageBuffers[m_writingPage].insert( m_pageBuffers[m_writingPage].end(),
                m_memoryMap.getBuffer() + header->readLocation + sizeof ( transmissionType ) + sizeof ( dataSize ),
                m_memoryMap.getBuffer() + header->readLocation + sizeof ( transmissionType ) + sizeof ( dataSize ) + dataSize );
            nRead += sizeof ( dataSize ) + dataSize;
            if ( transmissionType == transmissionComplete )
                m_bufferValidFlags[m_writingPage] = true;
            else
                m_bufferValidFlags[m_writingPage] = false;

            //if we have new buffer unplotted then it is due to either a flush, or we have so much
            // data we have wrapped the buffer or we have had a flush so plot the buffer
            if ( m_writingPage == m_viewingPage &&
                 ( m_plottedBufferAmount + 1024 ) < m_pageBuffers[ m_writingPage ].size() )
                SetPageAndUpdate();
        }
        header->readLocation += nRead;
        if ( header->readLocation == m_memoryMap.getSize() )
            header->readLocation = plMemoryMapReservedSpace;
    }
    m_inCheckTimerFunction = false;
}
void wxPlFrame::OnToggleFixAspect( wxCommandEvent &event )
{
    fixAspect( event.IsChecked() );
}

void wxPlFrame::fixAspect( bool fix )
{
    m_stream.SetFixedAspectRatio( fix );
    if ( !fix )
        this->Refresh();
}
void wxPlFrame::SetClientSize( int width, int height )
{
    wxFrame::SetClientSize( width, height );
    m_stream.SetSize( width, height );
}

void wxPlFrame::SetClientSize( const wxSize &size )
{
    SetClientSize( size.GetWidth(), size.GetHeight() );
}

void wxPlFrame::SetClientSize( const wxRect &rect )
{
    SetClientSize( rect.GetWidth(), rect.GetHeight() );
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
        MemoryMapHeader    *header    = (MemoryMapHeader *) m_memoryMap.getBuffer();
        wxSize             clientSize = GetClientSize();
        //PLGraphicsIn &     graphicsIn = header->graphicsIn;
        header->graphicsIn.pX = m_cursorPosition.x;
        header->graphicsIn.pY = m_cursorPosition.y;
        header->graphicsIn.dX = PLFLT( m_cursorPosition.x + 0.5 ) / PLFLT( clientSize.GetWidth() );
        header->graphicsIn.dY = 1.0 - PLFLT( m_cursorPosition.y + 0.5 ) / PLFLT( clientSize.GetHeight() );

        if ( event.LeftDown() )
        {
            header->graphicsIn.button = 1;      // X11/X.h: #define Button1	1
            header->graphicsIn.state  = 1 << 8; // X11/X.h: #define Button1Mask	(1<<8)
        }
        else if ( event.MiddleDown() )
        {
            header->graphicsIn.button = 2;      // X11/X.h: #define Button2	2
            header->graphicsIn.state  = 1 << 9; // X11/X.h: #define Button2Mask	(1<<9)
        }
        else if ( event.RightDown() )
        {
            header->graphicsIn.button = 3;       // X11/X.h: #define Button3	3
            header->graphicsIn.state  = 1 << 10; // X11/X.h: #define Button3Mask	(1<<10)
        }

        header->graphicsIn.keysym = 0x20;                // keysym for button event from xwin.c

        header->locateModeFlag = 0;
        m_locateMode           = false;
    }
}

void wxPlFrame::OnKey( wxKeyEvent &event )
{
    //This only works on Windows, unfortunately on wxGTK, wxFrames cannot catch key presses
    if ( m_locateMode )
    {
        PLNamedMutexLocker locker( &m_mutex );
        MemoryMapHeader    *header = (MemoryMapHeader *) m_memoryMap.getBuffer();

        wxSize             clientSize = GetClientSize();

        header->graphicsIn.pX = m_cursorPosition.x;
        header->graphicsIn.pY = m_cursorPosition.y;
        header->graphicsIn.dX = PLFLT( m_cursorPosition.x + 0.5 ) / PLFLT( clientSize.GetWidth() );
        header->graphicsIn.dY = 1.0 - PLFLT( m_cursorPosition.y + 0.5 ) / PLFLT( clientSize.GetHeight() );
        // gin->state = keyEvent->state;

        int keycode = event.GetKeyCode();
        header->graphicsIn.string[0] = (char) keycode;
        header->graphicsIn.string[1] = '\0';

        // ESCAPE, RETURN, etc. are already in ASCII equivalent
        header->graphicsIn.keysym = keycode;

        header->locateModeFlag = 0;
        m_locateMode           = false;
    }
}

void wxPlFrame::SetPageAndUpdate( size_t page )
{
    //if we get a page of -1 (which will really be max size_t) then just update the current page
    //otherwise switch to the given page
    if ( page != size_t( -1 ) )
    {
        if ( page >= m_pageBuffers.size() )
        {
            if ( m_transferComplete )
                Close();
            return;
        }
        if ( page != m_viewingPage )
        {
            m_viewingPage         = page;
            m_plottedBufferAmount = 0;
        }
    }

    //if there is unplotted buffer and the buffer is valid (contains some instructions and ends with a
    //valid instruction) then send that buffer to the driver and replot the page.
    if ( m_bufferValidFlags[m_viewingPage] && m_plottedBufferAmount < m_pageBuffers[m_viewingPage].size() )
    {
        m_plottedBufferAmount = m_pageBuffers[m_viewingPage].size();
        GetStream()->ImportBuffer( &m_pageBuffers[m_viewingPage][0], m_pageBuffers[m_viewingPage].size() );
        Refresh();
    }
}
