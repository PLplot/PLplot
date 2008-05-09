
#ifdef PLD_wxwidgets

#include "wxwidgets.h"

/*----------------------------------------------------------------------*\
 *  bool wxPLplotApp::OnInit()
 *
 *  This method is called before the applications gets control.
\*----------------------------------------------------------------------*/
bool wxPLplotApp::OnInit()
{
  Log_Verbose( "wxPLplotApp::OnInit" );
  
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
  Log_Verbose( "wxPLplotApp::SetRefreshFlag" );

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
  Log_Verbose( "wxPLplotApp::OnIdle" );

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
  Log_Verbose( "wxPLplotFrame::wxPLplotFrame" );
  m_dev=(wxPLdev*)pls->dev;

  m_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN );
  wxBoxSizer* box = new wxBoxSizer( wxVERTICAL );
	m_window = new wxPLplotWindow( m_panel, pls );
  box->Add( m_window, 1, wxALL | wxEXPAND, 0 );
  m_panel->SetSizer( box );
	m_window->SetFocus();
	  
  wxMenu* saveMenu = new wxMenu;
  for( size_t j=0; j<sizeof(dev_entries)/sizeof(dev_entry); j++ )
    for( size_t i=0; i<m_dev->ndev; i++ ) {
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
  Log_Verbose( "wxPLplotFrame::OnMenu" );

  switch( event.GetId() )
  {
  case wxID_EXIT:
		{
			wxMessageDialog dialog( this, wxT("Do you really want to quit?"), wxT("Close wxWidgets PLplot App?"), wxYES_NO |wxNO_DEFAULT| wxICON_EXCLAMATION );
			if( dialog.ShowModal() == wxID_YES ) {
				m_dev->exit=true;
				wxGetApp().ExitMainLoop();
			}
		}
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
  Log_Verbose( "wxPLplotFrame::OnClose" );

  // Are you old enough to know that you want really to quit?
  wxMessageDialog dialog( this, wxT("Do you really want to quit?"), wxT("Close wxWidgets PLplot App?"), wxYES_NO |wxNO_DEFAULT| wxICON_EXCLAMATION );
  if( dialog.ShowModal() == wxID_YES ) {
		m_dev->exit=true;
		wxGetApp().ExitMainLoop();
	}
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
  Log_Verbose( "wxPLplotWindow::wxPLplotWindow" );

  m_pls=pls;
  m_dev=(wxPLdev*)pls->dev;
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
  Log_Verbose( "wxPLplotWindow::OnPaint" );
	//static int counter=0;  
  /* copy bitmap into client area */
  wxPaintDC dc( this );

  /* only update damaged regions */
  int vX, vY, vW, vH; 
  wxRegionIterator upd( GetUpdateRegion() ); 

  if( m_dev->draw_xhair && upd && xhair_drawn ) {
    dc.SetLogicalFunction( wxINVERT );
    dc.CrossHair( mouse_x, mouse_y );
    dc.SetLogicalFunction( wxCOPY );
    xhair_drawn = false;
  }

  while( upd ) {
    vX = upd.GetX();
    vY = upd.GetY();
    vW = upd.GetW();
    vH = upd.GetH();

		//printf( "Clipping region: x=%d, y=%d, width=%d, height=%d, counter=%d\n", vX, vY, vW, vH, counter++ );
  
    BlitRectangle( &dc, vX, vY, vW, vH );

    upd ++ ;
  }

  if( m_dev->draw_xhair && !xhair_drawn ) {
    dc.SetLogicalFunction( wxINVERT );
    dc.CrossHair( mouse_x, mouse_y );
    dc.SetLogicalFunction( wxCOPY );
    xhair_drawn=true;
  }  
}
 

void wxPLplotWindow::OnChar( wxKeyEvent& event )
{
  Log_Verbose( "wxPLplotWindow::OnChar" );

  int keycode = event.GetKeyCode();
  switch( keycode ) {
    case 'L':
      m_dev->locate_mode = LOCATE_INVOKED_VIA_DRIVER;  
      m_dev->draw_xhair=true;
      DrawCrosshair();
      break;
    case 'Q':
    case WXK_ESCAPE:
      m_dev->exit=true;
      wxGetApp().SetExitFlag();
      break;
    case WXK_RETURN:
    case WXK_SPACE:
    case WXK_RIGHT:
      wxGetApp().SetAdvanceFlag();
      break;
    default:
      break;      
  }
  
  event.Skip();
}

void wxPLplotWindow::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotWindow::OnIdle" );

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
  Log_Verbose( "wxPLplotWindow::OnErase" );
}


void wxPLplotWindow::OnSize( wxSizeEvent & WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotWindow::OnSize" );

  int width, height;
  GetClientSize( &width, &height );
  
  if( m_dev->waiting ) {
    if( (width!=m_dev->width) || (height!=m_dev->height) ) {
      /* get a new bitmap if new size is bigger as bitmap size */
      if( (width>m_dev->bm_width) || (height>m_dev->bm_height) ) {
				m_dev->bm_width = m_dev->bm_width > width ? m_dev->bm_width : width;
				m_dev->bm_height = m_dev->bm_height > height ? m_dev->bm_height : height;
        
        m_dev->NewCanvas();
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
  Log_Verbose( "wxPLplotWindow::OnMax" );

  wxSizeEvent event( GetClientSize() );
  AddPendingEvent( event );
}

void wxPLplotWindow::OnMouse( wxMouseEvent &event )
{
  Log_Verbose( "wxPLplotWindow::OnMouse" );

  PLGraphicsIn *gin = &(m_dev->gin);
  wxPoint pos( event.GetPosition() );

  mouse_x = pos.x;
  mouse_y = pos.y;
  
  if( event.LeftDown() ) {    
    int width, height;
    GetClientSize( &width, &height );

    gin->pX = pos.x;
    gin->pY = pos.y;
    gin->dX = (PLFLT) pos.x / (width - 1);
    gin->dY = 1.0 - (PLFLT) pos.y / (height - 1);

    gin->button = 1;  // X11/X.h: #define Button1	1
    gin->state = 1<<8;  // X11/X.h: #define Button1Mask	(1<<8)
    gin->keysym = 0x20;  // TODO: ????? from xwin.c
  }

  if( event.LeftDown() ) {    
  	if( plTranslateCursor(gin) ) {
      /* If invoked by the API, we're done */
      /* Otherwise send report to stdout */
	    if( m_dev->locate_mode == LOCATE_INVOKED_VIA_DRIVER )
		    printf( "%f %f\n", gin->wX, gin->wY );
    } else {
      /* Selected point is out of bounds, so end locate mode */
        m_dev->locate_mode = 0;
        m_dev->draw_xhair=false;
    }
  }

  DrawCrosshair();
}

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