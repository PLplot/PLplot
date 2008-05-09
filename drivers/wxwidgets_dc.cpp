

#ifdef PLD_wxwidgets

#include "wxwidgets.h"

wxPLDevDC::wxPLDevDC( void ) : wxPLDevBase()
{
  m_dc=NULL;
  m_bitmap=NULL;
}


wxPLDevDC::~wxPLDevDC()
{
  if( ownGUI ) {
    if( m_dc ) {
        ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );
        delete m_dc;
    }
    if( m_bitmap )
      delete dev->m_bitmap;
  }
}

void wxPLDevDC::Drawline( short x1a, short y1a, short x2a, short y2a )
{
	x1a=(short)(x1a/scalex); y1a=(short)(height-y1a/scaley);
	x2a=(short)(x2a/scalex);	y2a=(short)(height-y2a/scaley);

  dc->DrawLine( (wxCoord)x1a, (wxCoord)y1a, (wxCoord)x2a, (wxCoord)y2a );
}

void wxPLDevDC::DrawPolyline( short *xa, short *ya, PLINT npts )
{
	wxCoord x1a, y1a, x2a, y2a;
  
  x2a=xa[0]/scalex;
  y2a=height-ya[0]/scaley;
  for( PLINT i=1; i<npts; i++ ) {
    x1a=x2a; y1a=y2a;
    x2a=xa[i]/scalex;
    y2a=height-ya[i]/scaley;

    dc->DrawLine( x1a, y1a, x2a, y2a );
    if( !resizing && ownGUI ) 
      AddtoClipRegion( this, (int)x1a, (int)y1a, (int)x2a, (int)y2a );
  }
}

void wxPLDevDC::ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
  m_dc->SetBackground( wxBrush(wxColour(bgr, bgg, bgb)) );
  m_dc->Clear();
}

void wxPLDevDC::FillPolygon( PLStream *pls )
{
  wxPoint *points = new wxPoint[pls->dev_npts];

  for( int i=0; i < pls->dev_npts; i++ ) {
    points[i].x=(int)(pls->dev_x[i]/scalex);
    points[i].y=(int)(height-pls->dev_y[i]/scaley);
  }

  dev->dc->DrawPolygon( pls->dev_npts, points );
  delete[] points;
}

void wxPLDevDC::BlitRectangle( wxPaintDC dc, int vX, int vY, int vW, int vH )
{
  if( m_dc )
    dc.Blit( vX, vY, vW, vH, m_dc, vX, vY );
}

void wxPLDevDC::NewCanvas()
{
  ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );   /* deselect bitmap */
  if( m_bitmap )
    delete m_bitmap;
  m_bitmap = new wxBitmap( bm_width, bm_height, 32 );
  ((wxMemoryDC*)m_dc)->SelectObject( *m_bitmap );   /* select new bitmap */
}

#endif				/* PLD_wxwidgets */


