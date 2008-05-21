
#include "plDevs.h"

#ifdef PLD_wxwidgets

/* plplot headers */
#include "plplotP.h"

/* os specific headers */
#ifdef __WIN32__
  #include <windows.h>
#endif

/* wxwidgets headers */
#include "wx/wx.h"
#include "wx/except.h"
#include "wx/image.h"
#include "wx/filedlg.h"
#include "wx/display.h"
#include "wx/graphics.h"
    
#include "wxwidgets.h"

#if wxUSE_GRAPHICS_CONTEXT
  
wxPLDevGC::wxPLDevGC( void ) : wxPLDevBase()
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_dc=NULL;
  m_bitmap=NULL;
  m_context=NULL;
}


wxPLDevGC::~wxPLDevGC()
{
  Log_Verbose( "%s", __FUNCTION__ );

  if( ownGUI ) {
    if( m_dc ) {
        ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );
        delete m_dc;
    }
    if( m_bitmap )
      delete m_bitmap;
  }
}

void wxPLDevGC::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
  Log_Verbose( "%s", __FUNCTION__ );

	wxDouble x1=x1a/scalex;
  wxDouble y1=height-y1a/scaley;
	wxDouble x2=x2a/scalex;
  wxDouble y2=height-y2a/scaley;

  Log_Verbose( "x1=%d, y1=%d, x2=%d, y2=%d", x1, y1, x2, y2 );

  wxGraphicsPath path=m_context->CreatePath();
  path.MoveToPoint( x1, y1 );
  path.AddLineToPoint( x2, y2 );
  m_context->StrokePath( path );

  if( !resizing && ownGUI )
    AddtoClipRegion( this, (int)x1, (int)y1, (int)x2, (int)y2 );  
}

void wxPLDevGC::DrawPolyline( short *xa, short *ya, PLINT npts )
{
  Log_Verbose( "%s", __FUNCTION__ );

	wxDouble x1a, y1a, x2a, y2a;
  
  x1a=xa[0]/scalex;
  y1a=height-ya[0]/scaley;
  
  wxGraphicsPath path=m_context->CreatePath();
  path.MoveToPoint( x1a, y1a );
  for( PLINT i=1; i<npts; i++ ) {
    x2a=xa[i]/scalex;
    y2a=height-ya[i]/scaley;
    path.AddLineToPoint( x2a, y2a );
    x1a=x2a; y1a=y2a;
  }
  m_context->StrokePath( path );

  if( !resizing && ownGUI ) {
    wxDouble x, y, w, h;
    path.GetBox( &x, &y, &w, &h );
    
    AddtoClipRegion( this, (int)x, (int)y, (int)(x+w), (int)(y+h) );  
  }
}

void wxPLDevGC::ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_dc->SetBackground( wxBrush(wxColour(bgr, bgg, bgb)) );
  m_dc->Clear();
}

void wxPLDevGC::FillPolygon( PLStream *pls )
{
  Log_Verbose( "%s", __FUNCTION__ );

  wxGraphicsPath path=m_context->CreatePath();
  path.MoveToPoint( pls->dev_x[0]/scalex, height-pls->dev_y[0]/scaley );
  for( int i=1; i < pls->dev_npts; i++ )
    path.AddLineToPoint( pls->dev_x[i]/scalex, height-pls->dev_y[i]/scaley );
  path.CloseSubpath();
  
  m_context->DrawPath( path );

  if( !resizing && ownGUI ) {
    wxDouble x, y, w, h;
    path.GetBox( &x, &y, &w, &h );
    
    AddtoClipRegion( this, (int)x, (int)y, (int)(x+w), (int)(y+h) );  
  }
}

void wxPLDevGC::BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH )
{
  Log_Verbose( "%s", __FUNCTION__ );
  Log_Verbose( "vx=%d, vy=%d, vw=%d, vh=%d", vX, vY, vW, vH );

  if( m_dc )
    dc->Blit( vX, vY, vW, vH, m_dc, vX, vY );
}

void wxPLDevGC::CreateCanvas()
{
  Log_Verbose( "%s", __FUNCTION__ );

  if( ownGUI ) {
    if( !m_dc )
      m_dc = new wxMemoryDC();

    ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );   /* deselect bitmap */
    if( m_bitmap )
      delete m_bitmap;
    m_bitmap = new wxBitmap( bm_width, bm_height, 32 );
    ((wxMemoryDC*)m_dc)->SelectObject( *m_bitmap );   /* select new bitmap */
  
    m_context = wxGraphicsContext::Create( *((wxMemoryDC*)m_dc) );
    Log_Verbose( "Context created %x", m_context );
  }
}

void wxPLDevGC::SetWidth( PLStream *pls )
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
  //m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
  //                                                             pls->cmap0[pls->icol0].b),
  //                                                   pls->width>0 ? pls->width : 1, wxSOLID)) );
}

void wxPLDevGC::SetColor0( PLStream *pls )
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
                                                               pls->cmap0[pls->icol0].b),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
  //m_context->SetBrush( wxBrush(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
  //                                      pls->cmap0[pls->icol0].b)) );
}

void wxPLDevGC::SetColor1( PLStream *pls )
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->curcolor.r, pls->curcolor.g,
                                                              pls->curcolor.b),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
  m_context->SetBrush( wxBrush(wxColour(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b)) );
}

/*--------------------------------------------------------------------------*\
 *  void wx_set_dc( PLStream* pls, wxDC* dc )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wxPLDevGC::SetExternalBuffer( void* dc )
{
  Log_Verbose( "%s", __FUNCTION__ );

  m_dc=(wxDC*)dc;  /* Add the dc to the device */
  m_context = wxGraphicsContext::Create( *((wxMemoryDC*)m_dc) );
  ready = true;
  ownGUI = false;
}

void wxPLDevGC::PutPixel( short x, short y, PLINT color )
{
  Log_Verbose( "%s", __FUNCTION__ );

  const wxPen oldpen=m_dc->GetPen();
  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(GetRValue(color), GetGValue(color), GetBValue(color)),
                                                     1, wxSOLID)) );
  //m_context->DrawPoint( x, y );
  m_context->SetPen( oldpen );
}

void wxPLDevGC::PutPixel( short x, short y )
{
  Log_Verbose( "%s", __FUNCTION__ );

  //m_dc->DrawPoint( x, y );
}

PLINT wxPLDevGC::GetPixel( short x, short y )
{
  Log_Verbose( "%s", __FUNCTION__ );

  #ifdef __WXGTK__
    // The GetPixel method is incredible slow for wxGTK. Therefore we set the colour
    // always to the background color, since this is the case anyway 99% of the time.
    PLINT bgr, bgg, bgb;  /* red, green, blue */
    plgcolbg( &bgr, &bgg, &bgb );  /* get background color information */
    return RGB( bgr, bgg, bgb );
#else
    wxColour col;
    m_dc->GetPixel( x, y, &col );
    return RGB( col.Red(), col.Green(), col.Blue());
#endif
}

void wxPLDevGC::ProcessString( PLStream* pls, EscText* args )
{
}

#endif

#endif				/* PLD_wxwidgets */


