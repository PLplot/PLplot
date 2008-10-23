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

#include "plDevs.h"

#ifdef PLD_wxwidgets

/* plplot headers */
#include "plplotP.h"
#include "plfci-truetype.h"

/* wxwidgets headers */
#include "wx/wx.h"
#include <wx/strconv.h>
    
/* std and driver headers */
#include "wxwidgets.h"
#include <wchar.h>

/* helper functions */
#if !defined(WIN32) || defined(__GNUC__)
  #include <unistd.h>
#else
  #define F_OK 1
  #include <stdio.h>
  int access( char *filename, int flag )
	{
		FILE *infile ;
		infile = fopen( filename, "r" ) ;
		if( infile != NULL ) {
			fclose( infile ) ;
			return 0 ;
		} else
			return 1 ;
  }
#endif

#define makeunixslash( b ) do { char *I; for( I=b;*I!=0;*I++ ) if( *I=='\\' ) *I='/';} while(0)
	
/* Constructor initializes all variables and objects */
wxPLDevAGG::wxPLDevAGG() :
		wxPLDevBase(),
    mRenderingBuffer(),
    mPixFormat( mRenderingBuffer ),
    mRendererBase( mPixFormat ),
    mRendererSolid( mRendererBase ),
    
    mPath(),
    mTransform(),
    mConvCurve( mPath ),
    mConvStroke( mConvCurve ),
    mPathTransform( mConvCurve, mTransform ),
    mStrokeTransform( mConvStroke, mTransform ),
    
		mFontEngine(),
    mFontManager( mFontEngine ),
		mCurves( mFontManager.path_adaptor() ),
    mContour( mCurves ),
    
    mBuffer( NULL ),
    mStrokeWidth( 1.0 ),
    mStrokeOpacity( 255 ),
    mColorRedStroke( 255 ),
    mColorGreenStroke( 255 ),
    mColorBlueStroke( 255 ),
    mColorRedFill( 0 ),
    mColorGreenFill( 0 ),
    mColorBlueFill( 0 )
{
  mCurves.approximation_scale( 2.0 );
  mContour.auto_detect_orientation( false );
  mConvStroke.line_join( agg::round_join );
  mConvStroke.line_cap( agg::round_cap );
	
	/* determine font directory */
#if defined(WIN32)
    static char *default_font_names[]={"arial.ttf","times.ttf","timesi.ttf","arial.ttf",
				       "symbol.ttf"};
    char WINDIR_PATH[255];
    char *b;
    b=getenv("WINDIR");
    strcpy(WINDIR_PATH,b);

/*
 * Work out if we have Win95+ or Win3.?... sort of.
 * Actually, this just tries to find the place where the fonts live by looking
 * for arial, which should be on all windows machines.
 * At present, it only looks in two places, on one drive. I might change this
 * soon.
 */
    if (WINDIR_PATH==NULL)
    {
        if (access("c:\\windows\\fonts\\arial.ttf", F_OK)==0) {
            strcpy(font_dir,"c:/windows/fonts/");
        }
        else if ( access("c:\\windows\\system\\arial.ttf", F_OK)==0) {
            strcpy(font_dir,"c:/windows/system/");
        }
        else
        plwarn("Could not find font path; I sure hope you have defined fonts manually !");
    }
    else
    {
      strcat(WINDIR_PATH,"\\fonts\\arial.ttf");
      if (access(WINDIR_PATH, F_OK)==0)
        {
          b=strrchr(WINDIR_PATH,'\\');
          b++;
          *b=0;
          makeunixslash(WINDIR_PATH);
          strcpy(font_dir,WINDIR_PATH);
        }
      else
        plwarn("Could not find font path; I sure hope you have defined fonts manually !");
    }

    if (pls->debug) fprintf( stderr, "%s\n", font_dir ) ;
#else
	/*  For Unix systems, we will set the font path up a little differently in
	 *  that the configured PL_FREETYPE_FONT_DIR has been set as the default path,
	 *  but the user can override this by setting the environmental variable
	 *  "PLPLOT_FREETYPE_FONT_DIR" to something else.
	 *  NOTE WELL - the trailing slash must be added for now !
	 */
	const char *str;

	fontdir.Clear();
	if( (str=getenv("PLPLOT_FREETYPE_FONT_DIR"))!=NULL )
		fontdir.Append( wxString(str, wxConvFile) );
	else
		fontdir.Append( wxT(PL_FREETYPE_FONT_DIR) );
	
	printf("fontdir=%ws, len=%d\n", fontdir.c_str(), fontdir.Length() );
#endif	
}


/* Deconstructor frees allocated buffer */
wxPLDevAGG::~wxPLDevAGG()
{
  if( ownGUI )
    if( mBuffer )
      delete mBuffer;
}


void wxPLDevAGG::drawPath( drawPathFlag flag )
{
  mRasterizer.reset();
  
  switch( flag ) {
  case Stroke:
    if( mStrokeOpacity && mStrokeWidth>0.0 ) {
      mConvStroke.width( mStrokeWidth );
      mRasterizer.add_path( mStrokeTransform );
      mRendererSolid.color( agg::rgba8(mColorRedStroke, mColorGreenStroke, mColorBlueStroke, mStrokeOpacity) );
      agg::render_scanlines( mRasterizer, mScanLine, mRendererSolid );  
    }
    break;
  case FillAndStroke:
    if( mStrokeOpacity ) {
      mRasterizer.add_path( mPathTransform );
      mRendererSolid.color( agg::rgba8(mColorRedStroke, mColorGreenStroke, mColorBlueStroke, mStrokeOpacity) );
      agg::render_scanlines( mRasterizer, mScanLine, mRendererSolid );
    }

    if( mStrokeOpacity && mStrokeWidth>0.0 ) {
      mConvStroke.width( mStrokeWidth );
      mRasterizer.add_path( mStrokeTransform );
      mRendererSolid.color( agg::rgba8(mColorRedStroke, mColorGreenStroke, mColorBlueStroke, mStrokeOpacity) );
      agg::render_scanlines( mRasterizer, mScanLine, mRendererSolid );
    }
    break;
  }
}


void wxPLDevAGG::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
	x1a=(short)(x1a/scalex); y1a=(short)(height-y1a/scaley);
	x2a=(short)(x2a/scalex);	y2a=(short)(height-y2a/scaley);

  mPath.remove_all();
  mPath.move_to( x1a, y1a );
  mPath.line_to( x2a, y2a );

  drawPath( Stroke );
}


void wxPLDevAGG::DrawPolyline( short *xa, short *ya, PLINT npts )
{
	short x1a, y1a, x2a, y2a;
  
  x2a=(short)(xa[0]/scalex); y2a=(short)(height-ya[0]/scaley);
  mPath.remove_all();
  mPath.move_to( x2a, y2a );
  for ( PLINT i=1; i<npts; i++ ) {
    x1a=x2a; y1a=y2a;
    x2a=(short)(xa[i]/scalex); y2a=(short)(height-ya[i]/scaley);
    mPath.line_to( x2a, y2a );
    if( !resizing && ownGUI ) 
      AddtoClipRegion( (int)x1a, (int)y1a, (int)x2a, (int)y2a );
  }

  drawPath( Stroke );
}


void wxPLDevAGG::ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
  mRendererBase.clear( agg::rgba8(bgr, bgg, bgb) );
}


void wxPLDevAGG::FillPolygon( PLStream *pls )
{
  short x1a, y1a, x2a, y2a;

  mPath.remove_all();
  x2a=(short)(pls->dev_x[0]/scalex); y2a=(short)(height-pls->dev_y[0]/scaley);
  mPath.move_to( x2a, y2a );
  for ( PLINT i=1; i<pls->dev_npts; i++ ) {
    x1a=x2a; y1a=y2a;
    x2a=(short)(pls->dev_x[i]/scalex); y2a=(short)(height-pls->dev_y[i]/scaley);
    mPath.line_to( x2a, y2a );
    if( !resizing && ownGUI ) 
      AddtoClipRegion( (int)x1a, (int)y1a, (int)x2a, (int)y2a );
  }
  mPath.line_to( pls->dev_x[0]/scalex, height-pls->dev_y[0]/scaley );
  mPath.close_polygon();

  drawPath( FillAndStroke );
}


void wxPLDevAGG::BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH )
{
  if( mBuffer ) {
    wxMemoryDC MemoryDC;
    wxBitmap bitmap( mBuffer->GetSubImage(wxRect(vX, vY, vW, vH)), -1 );
    MemoryDC.SelectObject( bitmap );
    dc->Blit( vX, vY, vW, vH, &MemoryDC, 0, 0 );
    MemoryDC.SelectObject( wxNullBitmap );
  }
}


void wxPLDevAGG::CreateCanvas()
{
  if( ownGUI ) {
    /* get a new wxImage (image buffer) */
    if( mBuffer )
      delete mBuffer;
    mBuffer = new wxImage( bm_width, bm_height );
    mRenderingBuffer.attach( mBuffer->GetData(), bm_width, bm_height, bm_width*3 );
  }
  else
    mRenderingBuffer.attach( mBuffer->GetData(), width, height, width*3 );

  mRendererBase.reset_clipping( true );

}


void wxPLDevAGG::SetWidth( PLStream *pls )
{
  mStrokeWidth = pls->width>0 ? pls->width : 1;  // TODO: why and when ist width 0???
}


void wxPLDevAGG::SetColor0( PLStream *pls )
{
  mColorRedStroke = pls->cmap0[pls->icol0].r;
  mColorGreenStroke = pls->cmap0[pls->icol0].g;
  mColorBlueStroke = pls->cmap0[pls->icol0].b;
  mStrokeOpacity = (wxUint8)(pls->cmap0[pls->icol0].a*255);
}


void wxPLDevAGG::SetColor1( PLStream *pls )
{
  mColorRedStroke = pls->curcolor.r;
  mColorGreenStroke = pls->curcolor.g;
  mColorBlueStroke = pls->curcolor.b;      
  mStrokeOpacity = (wxUint8)(pls->curcolor.a*255);
}


/*--------------------------------------------------------------------------*\
 *  void wx_set_buffer( PLStream* pls, wxImage* dc )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wxPLDevAGG::SetExternalBuffer( void* dc )
{
  mDC=(wxDC*)dc;  /* Add the dc to the device */
  if( mBuffer )
    delete mBuffer;
  mBuffer = new wxImage( width, height );
  mRenderingBuffer.attach( mBuffer->GetData(), width, height, width*3 );
  
  mRendererBase.reset_clipping( true );

  ready = true;
  ownGUI = false;
}


#ifdef HAVE_FREETYPE

void wxPLDevAGG::PutPixel( short x, short y, PLINT color )
{
  mBuffer->SetRGB( x, y, GetRValue(color), GetGValue(color), GetBValue(color) );   
}

void wxPLDevAGG::PutPixel( short x, short y )
{
  mBuffer->SetRGB( x, y, mColorRedStroke, mColorGreenStroke, mColorBlueStroke );
}

PLINT wxPLDevAGG::GetPixel( short x, short y )
{
  return RGB( mBuffer->GetRed( x, y ), mBuffer->GetGreen( x, y ), mBuffer->GetBlue( x, y ) );    
}

#endif // HAVE_FREETYPE


void wxPLDevAGG::PSDrawTextToDC( char* utf8_string, bool drawText )
{
  // Log_Verbose( "%s", __FUNCTION__ );
  printf( "utf8_string=%s\n", utf8_string );

  double start_x = 0.0;
  double start_y = 0.0;

  //wchar_t str[512];
  //size_t len=wxConvUTF8.ToWChar( str, 512, utf8_string );
  size_t len = strlen( utf8_string );
  char* str=utf8_string;
  printf("len=%d\n", len );

  const agg::glyph_cache* glyph;
  for( size_t i=0; i<len && str[i]; i++ ) {
    glyph = mFontManager.glyph( str[i] );
    if( glyph ) {
      if( i )
        mFontManager.add_kerning( &start_x, &start_y );
      printf( "start_x=%f, start_y=%f\n", start_x, start_y );
      mFontManager.init_embedded_adaptors( glyph, start_x, start_y );

      if( drawText ) {
        mRendererSolid.color( agg::rgba8(mColorRedStroke, mColorGreenStroke, mColorBlueStroke, mStrokeOpacity) );
        agg::render_scanlines( mFontManager.gray8_adaptor(), mFontManager.gray8_scanline(), mRendererSolid );
      }

      textHeight = textHeight>(glyph->bounds.y2-glyph->bounds.y1+yOffset/scaley) ?
                     textHeight : (glyph->bounds.y2-glyph->bounds.y1+yOffset/scaley);
      start_x += glyph->advance_x/64;
    }
  }

  textWidth += start_x;
  memset( utf8_string, '\0', max_string_length );
}


void wxPLDevAGG::PSSetFont( PLUNICODE fci )
{
  /* convert the fci to Base14/Type1 font information */
	wxString fontname=fontdir + wxString( plP_FCI2FontName(fci, TrueTypeLookup, N_TrueTypeLookup), *wxConvCurrent );
  printf("fontname=%s\n", fontdir.c_str() );

  mFontEngine.load_font( plP_FCI2FontName(fci, TrueTypeLookup, N_TrueTypeLookup), 0, agg::glyph_ren_agg_gray8 );
  mFontEngine.height( fontSize*fontScale );
  mFontEngine.width( fontSize*fontScale );
  mFontEngine.flip_y( true );
}


void wxPLDevAGG::ProcessString( PLStream* pls, EscText* args )
{
  /* Check that we got unicode, warning message and return if not */
  if( args->unicode_array_len == 0 ) {
    printf( "Non unicode string passed to a wxWidgets driver, ignoring\n" );
    return;
  }
	
  /* Check that unicode string isn't longer then the max we allow */
  if( args->unicode_array_len >= 500 ) {
    printf( "Sorry, the wxWidgets drivers only handles strings of length < %d\n", 500 );
    return;
  }
  
  /* Calculate the font size (in pixels) */
  fontSize = pls->chrht * DEVICE_PIXELS_PER_MM * 1.2;

  /* calculate rotation of text */
  plRotationShear( args->xform, &rotation, &shear, &stride );
  rotation -= pls->diorot * M_PI / 2.0;
  cos_rot = cos( rotation );
  sin_rot = sin( rotation );
  cos_shear = cos(shear);
  sin_shear = sin(shear);

  PSDrawText( args->unicode_array, args->unicode_array_len, false );
  
  agg::trans_affine mtx;
  mtx *= agg::trans_affine_translation( args->x/scalex, height-args->y/scaley );
  //mtx *= agg::trans_affine_rotation( rotation );
  //mtx *= agg::trans_affine_skewing( cos_shear, sin_shear );
  printf("textWidth=%f, textHeight=%f\n", textWidth, textHeight );

  mtx *= agg::trans_affine_translation( -args->just*textWidth, -0.5*textHeight );
  mFontEngine.transform(mtx);

  agg::conv_transform<fontManagerType::path_adaptor_type> tr( mFontManager.path_adaptor(), mtx );

  PSDrawText( args->unicode_array, args->unicode_array_len, true );

  AddtoClipRegion( 0, 0, width, height );        
}

#endif				/* PLD_wxwidgets */
