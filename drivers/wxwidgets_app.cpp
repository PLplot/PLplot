/* $Id$

   Copyright (C) 2008  Werner Smekal
   
   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

/* TODO: 
 * - Locate mode is implemented, but user functions are not called
 */


#include "plDevs.h"

#ifdef PLD_wxwidgets

/* plplot headers */
#include "plplotP.h"
#include "drivers.h"
#include "plevent.h"

/* wxwidgets headers */
#include "wx/wx.h"
    
/* std and driver headers */
#include "wxwidgets.h"

/* Application icon as XPM */
/* This free icon was taken from http://2pt3.com/news/twotone-icons-for-free/ */
static const char *graph[] = {
/* columns rows colors chars-per-pixel */
"16 16 4 2",
"   c black",
".  c #BA1825",
"X  c gray100",
"UX c None",
/* pixels */
"UX. . . . . . . . . . . . . . UX",
". . . . . . . . . . . . . . . . ",
". . . . . . . . . . . . . . . . ",
". . . . . . . . . . . X X . . . ",
". . . . . . . . . . . X X . . . ",
". . . . . . . . . . . X X . . . ",
". . . . . X X . . . . X X . . . ",
". . . . . X X . . . . X X . . . ",
". . . . . X X . X X . X X . . . ",
". . . . . X X . X X . X X . . . ",
". . . . . X X . X X . X X . . . ",
". . . . . X X . X X . X X . . . ",
". . . X X X X X X X X X X . . . ",
". . . . . . . . . . . . . . . . ",
". . . . . . . . . . . . . . . . ",
"UX. . . . . . . . . . . . . . UX"
};

struct dev_entry dev_entries[] = {
  { wxT("gif"), wxT("gif..."), wxT("Save this plot as gif!"), wxT("gif files (*.gif)|*.gif") },
  { wxT("jpeg"), wxT("jpeg..."), wxT("Save this plot as jpeg!"), wxT("jpg files (*.jpg;*.jpeg)|*.jpg;*.jpeg") },
  { wxT("png"), wxT("png..."), wxT("Save this plot as png"), wxT("png files (*.png)|*.png") },
  { wxT("pngcairo"), wxT("png (cairo)..."), wxT("Save this plot as png using cairo!"), wxT("png files (*.png)|*.png") },
  { wxT("pdfcairo"), wxT("pdf..."), wxT("Save this plot as pdf using cairo!"), wxT("pdf files (*.pdf)|*.pdf") },
  { wxT("ps"), wxT("postscript..."), wxT("Save this plot as postscript!"), wxT("ps files (*.ps)|*.ps") },
  { wxT("psc"), wxT("color postscript..."), wxT("Save this plot as color postscript!"), wxT("ps files (*.ps;*.psc)|*.ps;*.psc") },
  { wxT("pscairo"), wxT("color postscript (cairo)..."), wxT("Save this plot as color postscript using cairo!"), wxT("ps files (*.ps;*.psc)|*.ps;*.psc") },
  { wxT("svg"), wxT("svg..."), wxT("Save this plot as svg!"), wxT("svg files (*.svg)|*.svg") },
  { wxT("svgcairo"), wxT("svg (cairo)..."), wxT("Save this plot as svg using cairo!"), wxT("svg files (*.svg)|*.svg") },
  { wxT("xfig"), wxT("xfig..."), wxT("Save this plot as xfig!"), wxT("fig files (*.fig)|*.fig") }
};


IMPLEMENT_PLAPP_NO_MAIN( wxPLplotApp )

/* event table for the app */
BEGIN_EVENT_TABLE( wxPLplotApp, wxApp )
  EVT_IDLE( wxPLplotApp::OnIdle )
END_EVENT_TABLE()

/* event table for frames */
BEGIN_EVENT_TABLE( wxPLplotFrame, wxFrame )
  EVT_MENU( -1, wxPLplotFrame::OnMenu )      /* handle all menu events */
  EVT_CLOSE( wxPLplotFrame::OnClose )
END_EVENT_TABLE()

/* event table the widget */
BEGIN_EVENT_TABLE( wxPLplotWindow, wxWindow )
  EVT_PAINT( wxPLplotWindow::OnPaint )               /* (re)draw the plot in window */
  EVT_CHAR( wxPLplotWindow::OnChar )
  EVT_IDLE( wxPLplotWindow::OnIdle )
  EVT_MOUSE_EVENTS( wxPLplotWindow::OnMouse )
	EVT_ERASE_BACKGROUND( wxPLplotWindow::OnErase )
  EVT_SIZE( wxPLplotWindow::OnSize )
  EVT_MAXIMIZE( wxPLplotWindow::OnMaximize )
END_EVENT_TABLE()


/*----------------------------------------------------------------------*\
 *  bool wxPLplotApp::OnInit()
 *
 *  This method is called before the applications gets control.
\*----------------------------------------------------------------------*/
bool wxPLplotApp::OnInit()
{
  // Log_Verbose( "wxPLplotApp::OnInit" );
  
  exit=false;
  advance=false;

  return true;
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotApp::SetRefreshFlag( bool flag )
 *
 *  XXX - missing
\*----------------------------------------------------------------------*/
void wxPLplotApp::SetRefreshFlag( bool flag )
{
  // Log_Verbose( "wxPLplotApp::SetRefreshFlag" );

	for( size_t i=0; i<FrameArray.GetCount(); i++)
		FrameArray[i]->SetRefreshFlag( flag );
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotApp::OnIdle( wxIdleEvent& WXUNUSED(event) )
 *
 *  XXX - missing
\*----------------------------------------------------------------------*/
void wxPLplotApp::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
  // Log_Verbose( "wxPLplotApp::OnIdle" );

	bool refresh=false;

  if( exit )
    ExitMainLoop();

	for( size_t i=0; i<FrameArray.GetCount(); i++)
		refresh |= FrameArray[i]->GetRefreshFlag();

  if( advance && !refresh )
    ExitMainLoop();
}


/*----------------------------------------------------------------------*\
 *  wxPLplotFrame::wxPLplotFrame( const wxString& title, PLStream *pls )
 *
 *  Constructor of wxPLplotFrame, where we create the menu and add the
 *  wxPLplotWindow. We need also to know the current PLStream.
\*----------------------------------------------------------------------*/
wxPLplotFrame::wxPLplotFrame( const wxString& title, PLStream *pls )
             : wxFrame( NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
                        wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION |
                        wxCLOSE_BOX | wxRESIZE_BORDER | wxCLIP_CHILDREN ) 
{
  // Log_Verbose( "wxPLplotFrame::wxPLplotFrame" );
  
  m_dev=(wxPLDevBase*)pls->dev;

  m_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN );
  wxBoxSizer* box = new wxBoxSizer( wxVERTICAL );
	m_window = new wxPLplotWindow( m_panel, pls );
  box->Add( m_window, 1, wxALL | wxEXPAND, 0 );
  m_panel->SetSizer( box );
	m_window->SetFocus();
	  
  wxMenu* saveMenu = new wxMenu;
  for( size_t j=0; j<sizeof(dev_entries)/sizeof(dev_entry); j++ )
    for( int i=0; i<m_dev->ndev; i++ ) {
      if( !strcmp(m_dev->devName[i], dev_entries[j].dev_name.mb_str()) )
        saveMenu->Append( wxPL_Save+j, dev_entries[j].dev_menu_short, dev_entries[j].dev_menu_long );
    }

  wxMenu* fileMenu = new wxMenu;
#if (wxMAJOR_VERSION<=2) & (wxMINOR_VERSION<=6)
  fileMenu->Append( -1, wxT("Save plot as..."), saveMenu, wxT("Save this plot as ...!") );
#else
  fileMenu->AppendSubMenu( saveMenu, wxT("Save plot as..."), wxT("Save this plot as ...!") );
#endif
  fileMenu->Append( wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Exit wxWidgets PLplot App") );

  wxMenuBar* menuBar = new wxMenuBar();
  menuBar->Append( fileMenu, wxT("&File") );
  SetMenuBar( menuBar );

  SetTitle( wxT("wxWidgets PLplot App") );
  SetIcon( wxIcon(graph) );
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotFrame::OnMenu( wxCommandEvent& event )
 *
 *  Event method, which is called if user 
\*----------------------------------------------------------------------*/
void wxPLplotFrame::OnMenu( wxCommandEvent& event )
{
  // Log_Verbose( "wxPLplotFrame::OnMenu" );

  switch( event.GetId() )
  {
  case wxID_EXIT:
    m_dev->exit=true;
    wxGetApp().ExitMainLoop();
		break;
  }
    
  size_t index=event.GetId()-wxPL_Save;
  if( (index>=0) && (index<sizeof(dev_entries)/sizeof(dev_entry)) ) {
    wxFileDialog dialog( this, wxT("Save plot as ")+dev_entries[index].dev_name, wxT(""), wxT(""),
                         dev_entries[index].dev_file_app+wxT("|All Files (*.*)|*.*"),
#if (wxMAJOR_VERSION<=2) & (wxMINOR_VERSION<=6)
                         wxSAVE | wxOVERWRITE_PROMPT );
#else
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
#endif	
    if (dialog.ShowModal() == wxID_OK) {
      const wxCharBuffer buf1=dialog.GetPath().mb_str();
      const wxCharBuffer buf2=dev_entries[index].dev_name.mb_str();
      SavePlot( (const char*)buf1, (const char*)buf2, 800, 600 );
    }
  }
}

/*----------------------------------------------------------------------*\
 *  void wxPLplotFrame::OnClose( wxCloseEvent& event )
 *
 *  Event method, which is called if user 
\*----------------------------------------------------------------------*/
void wxPLplotFrame::OnClose( wxCloseEvent& event )
{
  // Log_Verbose( "wxPLplotFrame::OnClose" );

  m_dev->exit=true;
  wxGetApp().ExitMainLoop();
}


/*----------------------------------------------------------------------*\
 *  bool wxPLplotFrame::SavePlot( const char* filename, cost char* dev, int width,
 *                                int height )
 *
 *  This function saves the current plot to a file (filename) using a
 *  device (devname) with given width and height. There is no test if
 *  the device really exists.
\*----------------------------------------------------------------------*/
bool wxPLplotFrame::SavePlot( const char* filename, const char* devname, int width,  int height )
{  
  int pls, pls_save;
  FILE *sfile;

	if( (sfile = fopen(filename, "wb+")) == NULL) {
    if( m_dev->ownGUI ) {
      wxMessageDialog dialog( 0, wxT("Couldn't open file for saving!"), wxT("plPlot error"),
														  wxOK|wxICON_ERROR );
      dialog.ShowModal();
    } 
		return false;
	}

	plgstrm( &pls );
	plmkstrm( &pls_save );  
	if( pls_save<0 ) {
    if( m_dev->ownGUI ) {
      wxMessageDialog dialog( 0, wxT("Couldn't open file for saving!"), wxT("plPlot error"),
																 wxOK|wxICON_ERROR );
      dialog.ShowModal();
    } 
		return false;
	}
	plsdev( devname );
  //plsfile( sfile );
  plsfnam(filename);       /* file name */
  
  //plspage( 0., 0., width, height, 0, 0 );
	plcpstrm( pls, 0);
	pladv( 0 );
	plreplot();
  plend1();
	plsstrm( pls );

  return true;
}


/*----------------------------------------------------------------------*\
 *  wxPLplotWindow::wxPLplotWindow( const wxString& title )
 *
 *  Constructor of wxPLplotFrame, where we create the menu.
\*----------------------------------------------------------------------*/
wxPLplotWindow::wxPLplotWindow( wxWindow* parent, PLStream *pls )
             : wxWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                         wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN )
{
  // Log_Verbose( "wxPLplotWindow::wxPLplotWindow" );

  m_pls=pls;
  m_dev=(wxPLDevBase*)pls->dev;
	refresh=false;
  mouse_x=old_mouse_x=-1;
  mouse_y=old_mouse_y=-1;
  xhair_drawn=false;

	SetBackgroundStyle( wxBG_STYLE_CUSTOM );
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotWindow::OnPaint( wxPaintEvent& WXUNUSED(event) )
 *
 *  Event method where the plots are actually drawn. Since the plots
 *  are already drawn into bitmaps, which just copy them into to client
 *  area. This method is also called, if (part) of the client area was
 *  invalidated and a refresh is necessary.
\*----------------------------------------------------------------------*/
void wxPLplotWindow::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  // Log_Verbose( "wxPLplotWindow::OnPaint" );
  
  /* copy bitmap into client area */
  wxPaintDC dc( this );

  /* only update damaged regions */
  int vX, vY, vW, vH; 
  wxRegionIterator upd( GetUpdateRegion() ); 

  // remove the xhair before updating
  if( m_dev->draw_xhair && upd && xhair_drawn ) {
    dc.SetLogicalFunction( wxINVERT );
    dc.CrossHair( old_mouse_x, old_mouse_y );
    dc.SetLogicalFunction( wxCOPY );
    xhair_drawn = false;
    old_mouse_x=old_mouse_y=-1;    
  }

  while( upd ) {
    vX = upd.GetX();
    vY = upd.GetY();
    vW = upd.GetW();
    vH = upd.GetH();

		//printf( "Clipping region: x=%d, y=%d, width=%d, height=%d, counter=%d\n", vX, vY, vW, vH, counter++ );
  
    m_dev->BlitRectangle( &dc, vX, vY, vW, vH );

    upd++ ;
  }

  if( m_dev->draw_xhair && !xhair_drawn ) {
    dc.SetLogicalFunction( wxINVERT );
    dc.CrossHair( mouse_x, mouse_y );  
    dc.SetLogicalFunction( wxCOPY );
    old_mouse_x=mouse_x;
    old_mouse_y=mouse_y;  
    xhair_drawn=true;
  }  
}
 

/*----------------------------------------------------------------------
 *  void wxPLplotWindow::OnChar( wxKeyEvent& event )
 *
 *  Handle key events.
 *----------------------------------------------------------------------*/
void wxPLplotWindow::OnChar( wxKeyEvent& event )
{
  // Log_Verbose( "wxPLplotWindow::OnChar" );

  PLGraphicsIn *gin = &(m_dev->gin);

  int width, height;
  GetClientSize( &width, &height );

  gin->pX = mouse_x;
  gin->pY = mouse_y;
  gin->dX = (PLFLT) mouse_x / (width - 1);
  gin->dY = 1.0 - (PLFLT) mouse_y / (height - 1);

  // gin->state = keyEvent->state;

  int keycode = event.GetKeyCode();
  gin->string[0] = (char)keycode;
  gin->string[1] = '\0';

  // ESCAPE, RETURN, etc. are already in ASCII equivalent
  gin->keysym = keycode;
  
  if( m_dev->locate_mode ) {   
    /* End locate mode on <Escape> */
    if( gin->keysym == PLK_Escape ) {
      if( m_dev->locate_mode == LOCATE_INVOKED_VIA_API )
        wxGetApp().SetAdvanceFlag();   
      m_dev->locate_mode = 0;
      m_dev->draw_xhair = false;
      DrawCrosshair();
      plGinInit(gin);
    }
    
    Locate();    
  } else {
    /* Call user keypress event handler.  Since this is called first, the user
     * can disable all internal event handling by setting key.keysym to 0. */
    //if (pls->KeyEH != NULL)
    //  (*pls->KeyEH) (gin, pls->KeyEH_data, &dev->exit_eventloop);
    // TODO: This must be tested and implemented
    
    switch( gin->keysym ) {
      case 'L':
        m_dev->locate_mode = LOCATE_INVOKED_VIA_DRIVER;  
        m_dev->draw_xhair=true;
        DrawCrosshair();
        break;
      case 'Q':
      case PLK_Escape:
        m_dev->exit=true;
        wxGetApp().SetExitFlag();
        break;
      case PLK_Return:
      case WXK_SPACE:
      case WXK_RIGHT:
        wxGetApp().SetAdvanceFlag();
        break;
      default:
        break;      
    }
  }
  
  event.Skip();
}

void wxPLplotWindow::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
  // Log_Verbose( "wxPLplotWindow::OnIdle" );

  if( refresh ) {
    if(!m_dev->newclipregion) {
      static wxRect rect;
			rect.x=m_dev->clipminx;
			rect.y=m_dev->clipminy;
			rect.width=m_dev->clipmaxx-m_dev->clipminx+1;
			rect.height=m_dev->clipmaxy-m_dev->clipminy+1;
#if (wxMAJOR_VERSION<=2) & (wxMINOR_VERSION<=5)
      RefreshRect( rect ); 
#else			
      RefreshRect( rect, false );  /* don't erase background */
#endif
      m_dev->newclipregion=true;
			m_dev->clipminx=m_dev->width;
			m_dev->clipmaxx=0;
			m_dev->clipminy=m_dev->height;
			m_dev->clipmaxy=0;
    } else
      Refresh( false );
    refresh=false;
  }
}


void wxPLplotWindow::OnErase( wxEraseEvent &WXUNUSED(event) )
{  
  // Log_Verbose( "wxPLplotWindow::OnErase" );
}


void wxPLplotWindow::OnSize( wxSizeEvent & WXUNUSED(event) )
{
  // Log_Verbose( "wxPLplotWindow::OnSize" );

  int width, height;
  GetClientSize( &width, &height );
  
  if( m_dev->waiting ) {
    if( (width!=m_dev->width) || (height!=m_dev->height) ) {
      /* get a new bitmap if new size is bigger as bitmap size */
      if( (width>m_dev->bm_width) || (height>m_dev->bm_height) ) {
				m_dev->bm_width = m_dev->bm_width > width ? m_dev->bm_width : width;
				m_dev->bm_height = m_dev->bm_height > height ? m_dev->bm_height : height;
      }

      wx_set_size( m_pls, width, height );
      m_dev->resizing = true;
      plRemakePlot( m_pls );
      m_dev->resizing = false;
      Refresh();
    }
  }
}


void wxPLplotWindow::OnMaximize( wxMaximizeEvent & WXUNUSED(event) )
{
  // Log_Verbose( "wxPLplotWindow::OnMax" );

  wxSizeEvent event( GetClientSize() );
  AddPendingEvent( event );
}


/*----------------------------------------------------------------------
 *  void wxPLplotWindow::OnMouse( wxMouseEvent &event )
 *
 *  Handle mouse events.
 *----------------------------------------------------------------------*/
void wxPLplotWindow::OnMouse( wxMouseEvent &event )
{
  // Log_Verbose( "wxPLplotWindow::OnMouse" );

  wxPoint pos( event.GetPosition() );
  mouse_x = pos.x;
  mouse_y = pos.y;
  
  if( event.ButtonDown() ) {    
    PLGraphicsIn *gin = &(m_dev->gin);

    int width, height;
    GetClientSize( &width, &height );

    gin->pX = mouse_x;
    gin->pY = mouse_y;
    gin->dX = (PLFLT) mouse_x / (width - 1);
    gin->dY = 1.0 - (PLFLT) mouse_y / (height - 1);
    
    if( event.LeftDown() ) {
      gin->button = 1;  // X11/X.h: #define Button1	1
      gin->state = 1<<8;  // X11/X.h: #define Button1Mask	(1<<8)
    } else if ( event.MiddleDown() ) {
      gin->button = 2;  // X11/X.h: #define Button2	2
      gin->state = 1<<9;  // X11/X.h: #define Button2Mask	(1<<9)
    } else if ( event.RightDown() ) {
      gin->button = 3;  // X11/X.h: #define Button3	3
      gin->state = 1<<10;  // X11/X.h: #define Button3Mask	(1<<10)
    }
    gin->keysym = 0x20;  // keysym for button event from xwin.c

    if( m_dev->locate_mode )
      Locate();
    else {
      /* Call user event handler.  Since this is called first, the user can
       * disable all PLplot internal event handling by setting gin->button to 0. */
      //if( pls->ButtonEH != NULL)
      //  (*pls->ButtonEH)( gin, pls->ButtonEH_data, &dev->exit_eventloop );
      // TODO: This must be tested and implemented!

      /* Handle internal events */
      switch( gin->button ) {
      case 3:  // on right mouse button advance
        wxGetApp().SetAdvanceFlag();
        break;
      default:
        break;
      }
    }
  }

  DrawCrosshair();
}


/*----------------------------------------------------------------------
 *  void wxPLplotWindow::Locate( void )
 *
 *  Take care of Locate mode, called by OnChar() and OnMouse().
 *----------------------------------------------------------------------*/
void wxPLplotWindow::Locate( void )
{
  // Log_Verbose( "wxPLplotWindow::Locate" );

  PLGraphicsIn *gin = &(m_dev->gin);

  // Some event (key, button) occured, and if the locate mode
  // was initiated by the API we need to return back to the
  // user program
  if( m_dev->locate_mode == LOCATE_INVOKED_VIA_API )
    wxGetApp().SetAdvanceFlag();        
  
  /* Call user locate mode handler if provided */
  //if( pls->LocateEH != NULL )
  //  (*pls->LocateEH)( gin, pls->LocateEH_data, &dev->locate_mode );
  // TODO: This must be tested and implemented!
  // else {
    if( plTranslateCursor(gin) ) {
      /* If invoked by the API, we're done */
      /* Otherwise send report to stdout */
      if( m_dev->locate_mode == LOCATE_INVOKED_VIA_DRIVER )
        if( gin->keysym < 0xFF && isprint(gin->keysym) )
            printf("%f %f %c\n", gin->wX, gin->wY, gin->keysym);
        else
            printf("%f %f 0x%02x\n", gin->wX, gin->wY, gin->keysym);
    } else {
      /* Selected point is out of bounds, so end locate mode */
      m_dev->locate_mode = 0;
       m_dev->draw_xhair=false;
    }
  //}
}


/*----------------------------------------------------------------------
 *  void wxPLplotWindow::DrawCrosshair()
 *
 *  Draw a cross hair (inverted lines).
 *----------------------------------------------------------------------*/
void wxPLplotWindow::DrawCrosshair()
{
  /* draw cross hair */
  wxClientDC dc(this);
  if( m_dev->draw_xhair ) {
    if( (mouse_x!=old_mouse_x) || (mouse_y!=old_mouse_y) ) {
      dc.SetLogicalFunction( wxINVERT );
      if( xhair_drawn )
        dc.CrossHair( old_mouse_x, old_mouse_y );
      dc.CrossHair( mouse_x, mouse_y );
      dc.SetLogicalFunction( wxCOPY );
      old_mouse_x=mouse_x;
      old_mouse_y=mouse_y;
      xhair_drawn=true;  
    }
  } else {
    if( xhair_drawn ) {
      dc.SetLogicalFunction( wxINVERT );
      dc.CrossHair( old_mouse_x, old_mouse_y );
      dc.SetLogicalFunction( wxCOPY );
      xhair_drawn=false;
      old_mouse_x=old_mouse_y=-1;
    }
  }  
}

#endif				/* PLD_wxwidgets */
