
#include "plDevs.h"

#ifdef PLD_wxwidgets

#include "wxwidgets.h"

/* antigrain headers (for antialzing) */
#include "agg2/agg_basics.h"
#include "agg2/agg_rendering_buffer.h"
#include "agg2/agg_rasterizer_scanline_aa.h"
#include "agg2/agg_scanline_u.h"
#include "agg2/agg_conv_stroke.h"
#include "agg2/agg_pixfmt_rgb.h"
#include "agg2/agg_renderer_base.h"
#include "agg2/agg_renderer_scanline.h"
#include "agg2/agg_path_storage.h"

typedef agg::pixfmt_rgb24 pixfmt;
typedef agg::renderer_base<pixfmt> ren_base;
typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

wxPLDevAGG::wxPLDevAGG() : wxPLDevBase()
{
  m_buffer=NULL;
  m_rendering_buffer=NULL;
  m_strokewidth=1.0;
  m_StrokeOpacity=255;
  m_colredstroke=255;
  m_colgreenstroke=255;
  m_colbluestroke=255;
  m_colredfill=0;
  m_colgreenfill=0;
  m_colbluefill=0;
}

wxPLDevAGG::~wxPLDevAGG()
{
  if( ownGUI ) {
    if( m_buffer )
      delete dev->m_buffer;
    if( m_rendering_buffer )
      delete dev->m_rendering_buffer;
  }
}

void wxPLDevAGG::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
	x1a=(short)(x1a/scalex); y1a=(short)(height-y1a/scaley);
	x2a=(short)(x2a/scalex);	y2a=(short)(height-y2a/scaley);

  agg::rasterizer_scanline_aa<> ras;
  agg::scanline_u8 sl;
  pixfmt pixf( m_rendering_buffer );
  ren_base renb( pixf );
  renderer ren( renb );

  agg::path_storage path;
  path.move_to( x1a, y1a );
  path.line_to( x2a, y2a );

  agg::conv_stroke<agg::path_storage> stroke(path);
  stroke.line_join( agg::round_join );
  stroke.line_cap( agg::round_cap );
  stroke.width( m_strokewidth );
  ras.add_path( stroke );
    
  ren.color( agg::rgba8(m_colredstroke, m_colgreenstroke, m_colbluestroke, m_StrokeOpacity) );
    
  agg::render_scanlines( ras, sl, ren );
}

void wxPLDevAGG::DrawPolyline( short *xa, short *ya, PLINT npts )
{
	short x1a, y1a, x2a, y2a;
  
  agg::rasterizer_scanline_aa<> ras;
  agg::scanline_u8 sl;
  pixfmt pixf( *m_rendering_buffer );
  ren_base renb( pixf );
  renderer ren( renb );

  agg::path_storage path;
  x2a=(short)(xa[0]/scalex); y2a=(short)(height-ya[0]/scaley);
  path.move_to( x2a, y2a );
  for ( PLINT i=1; i<npts; i++ ) {
    x1a=x2a; y1a=y2a;
    x2a=(short)(xa[i]/scalex); y2a=(short)(height-ya[i]/scaley);
    path.line_to( x2a, y2a );
    if( !resizing && ownGUI ) 
      AddtoClipRegion( this, (int)x1a, (int)y1a, (int)x2a, (int)y2a );
  }

  agg::conv_stroke<agg::path_storage> stroke( path );
  stroke.line_join( agg::round_join );
  stroke.line_cap( agg::round_cap );
  stroke.width( m_strokewidth );
  ras.add_path( stroke );

  ren.color( agg::rgba8(m_colredstroke, m_colgreenstroke, m_colbluestroke, m_StrokeOpacity) );
  
  agg::render_scanlines( ras, sl, ren );
}

void wxPLDevAGG::ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
  pixfmt pixf( *m_rendering_buffer );
  ren_base renb( pixf );
  renb.clear( agg::rgba8(bgr, bgg, bgb) );
}

void wxPLDevAGG::FillPolygon( PLStream *pls )
{
  agg::rasterizer_scanline_aa<> ras;
  agg::scanline_u8 sl;
  pixfmt pixf( *m_rendering_buffer );
  ren_base renb( pixf );
  renderer ren( renb );

  agg::path_storage path;
  x2a=(short)(pls->dev_x[0]/scalex); y2a=(short)(height-pls->dev_y[0]/scaley);
  path.move_to( x2a, y2a );
  for ( PLINT i=1; i<pls->dev_npts; i++ ) {
    x1a=x2a; y1a=y2a;
    x2a=(short)(pls->dev_x[i]/scalex); y2a=(short)(height-pls->dev_y[i]/scaley);
    path.line_to( x2a, y2a );
    if( !resizing && ownGUI ) 
      AddtoClipRegion( dev, (int)x1a, (int)y1a, (int)x2a, (int)y2a );
  }
  path.line_to( pls->dev_x[0]/scalex, height-pls->dev_y[0]/scaley );
  path.close_polygon();

  ren.color( agg::rgba8(m_colredstroke, m_colgreenstroke, m_colbluestroke, m_StrokeOpacity) );
  ras.add_path( path );
  agg::render_scanlines( ras, sl, ren );

  agg::conv_stroke<agg::path_storage> stroke( path );
  stroke.line_join( agg::round_join );
  stroke.line_cap( agg::round_cap );
  stroke.width( m_strokewidth );
  ras.add_path( stroke );    
  ren.color( agg::rgba8(m_colredstroke, m_colgreenstroke, m_colbluestroke, m_StrokeOpacity) );
  agg::render_scanlines( ras, sl, ren );
}

void wxPLDevAGG::BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH )
{
  if( m_buffer ) {
    wxMemoryDC MemoryDC;
    wxBitmap bitmap( m_buffer->GetSubImage(wxRect(vX, vY, vW, vH)), -1 );
    MemoryDC.SelectObject( bitmap );
    dc->Blit( vX, vY, vW, vH, &MemoryDC, 0, 0 );
    MemoryDC.SelectObject( wxNullBitmap );
  }
}

void wxPLDevAGG::CreateCanvas()
{
  if( m_rendering_buffer )
    delete m_rendering_buffer;
  m_rendering_buffer = new agg::rendering_buffer;

  if( ownGUI ) {
    /* get a new wxImage (image buffer) */
    if( m_buffer )
      delete m_buffer;
    m_buffer = new wxImage( bm_width, bm_height );
    m_rendering_buffer->attach( m_buffer->GetData(), bm_width, bm_height, bm_width*3 );
  }
  else
    m_rendering_buffer->attach( m_buffer->GetData(), width, height, width*3 );
}

void wxPLDevAGG::SetWidth( PLStream *pls )
{
  m_strokewidth = pls->width>0 ? pls->width : 1;  // TODO: why and when ist width 0???
}

void wxPLDevAGG::SetColor0( PLStream *pls )
{
  m_colredstroke = pls->cmap0[pls->icol0].r;
  m_colgreenstroke = pls->cmap0[pls->icol0].g;
  m_colbluestroke = pls->cmap0[pls->icol0].b;
  m_StrokeOpacity = (wxUint8)(pls->cmap0[pls->icol0].a*255);
}

void wxPLDevAGG::SetColor1( PLStream *pls )
{
  m_colredstroke = pls->curcolor.r;
  m_colgreenstroke = pls->curcolor.g;
  m_colbluestroke = pls->curcolor.b;      
  m_StrokeOpacity = (wxUint8)(pls->curcolor.a*255);
}


/*--------------------------------------------------------------------------*\
 *  void wx_set_buffer( PLStream* pls, wxImage* dc )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wxPLDevAGG::SetExternalBuffer( void* buffer )
{
  m_buffer = (wxImage*)buffer;
  if( m_rendering_buffer )
    delete m_rendering_buffer;
  m_rendering_buffer = new agg::rendering_buffer;
  m_rendering_buffer->attach( m_buffer->GetData(), width, height, width*3 );

  ready = true;
  ownGUI = false;
}

void wxPLDevAGG::PutPixel( short x, short y, PLINT color )
{
  m_buffer->SetRGB( x, y, GetRValue(colour), GetGValue(colour), GetBValue(colour) );   
}

void wxPLDevAGG::PutPixel( short x, short y )
{
  m_buffer->SetRGB( x, y, m_colredstroke, m_colgreenstroke, m_colbluestroke );
}

PLINT wxPLDevAGG::PutPixel( short x, short y )
{
  return RGB( m_buffer->GetRed( x, y ), m_buffer->GetGreen( x, y ), m_buffer->GetBlue( x, y ) );    
}

#endif				/* PLD_wxwidgets */
