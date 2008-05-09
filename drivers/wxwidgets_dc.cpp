

#ifdef PLD_wxwidgets

include "wxwidgets.h"

wxPLDevDC::wxPLDevDC( void ) : wxPLDevBase()
{
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

void wxPLDevDC::ClearBackground(  )
{
}

void wxPLDevAGG::FillPolygon( PLStream *pls )
{
  wxPoint *points = new wxPoint[pls->dev_npts];

  for( int i=0; i < pls->dev_npts; i++ ) {
    points[i].x=(int)(pls->dev_x[i]/scalex);
    points[i].y=(int)(height-pls->dev_y[i]/scaley);
  }

  dev->dc->DrawPolygon( pls->dev_npts, points );
  delete[] points;
}

#endif				/* PLD_wxwidgets */


