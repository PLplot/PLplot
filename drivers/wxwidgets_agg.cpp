
#ifdef PLD_wxwidgets


/* antigrain headers (for antialzing) */
#ifdef HAVE_AGG
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
#endif  

wxPLDevAGG::wxPLDevAGG( void ) : wxPLDevBase()
{
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

void wxPLDevAGG::ClearBackground(  )
{
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

#endif				/* PLD_wxwidgets */
