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
 * - text clipping
 * - implement AddToClipRegion for text correctly
 */

#include "plDevs.h"

#ifdef PLD_wxwidgets

/* plplot headers */
#include "plplotP.h"

/* wxwidgets headers */
#include "wx/wx.h"

/* std and driver headers */
#include "wxwidgets.h"

/* only compile code if wxGraphicsContext available */
#if wxUSE_GRAPHICS_CONTEXT
#include "wx/graphics.h"
  
wxPLDevGC::wxPLDevGC( void ) : wxPLDevBase()
{
  // Log_Verbose( "%s", __FUNCTION__ );

  m_dc=NULL;
  m_bitmap=NULL;
  m_context=NULL;
  m_font=NULL;
  underlined=false;  
}


wxPLDevGC::~wxPLDevGC()
{
  // Log_Verbose( "%s", __FUNCTION__ );

  if( ownGUI ) {
    if( m_dc ) {
      ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );
      delete m_dc;
    }
    if( m_bitmap )
      delete m_bitmap;
  }

  if( m_font )
    delete m_font;
}


void wxPLDevGC::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
  // Log_Verbose( "%s", __FUNCTION__ );

	wxDouble x1=x1a/scalex;
  wxDouble y1=height-y1a/scaley;
	wxDouble x2=x2a/scalex;
  wxDouble y2=height-y2a/scaley;

  wxGraphicsPath path=m_context->CreatePath();
  path.MoveToPoint( x1, y1 );
  path.AddLineToPoint( x2, y2 );
  m_context->StrokePath( path );

  AddtoClipRegion( (int)x1, (int)y1, (int)x2, (int)y2 );  
}


void wxPLDevGC::DrawPolyline( short *xa, short *ya, PLINT npts )
{
  // Log_Verbose( "%s", __FUNCTION__ );

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

  wxDouble x, y, w, h;
  path.GetBox( &x, &y, &w, &h );
  
  AddtoClipRegion( (int)x, (int)y, (int)(x+w), (int)(y+h) );  
}


void wxPLDevGC::ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
  // Log_Verbose( "%s", __FUNCTION__ );

	wxDouble x1a, y1a, x2a, y2a;

  if( x1<0 ) x1a=0;      else x1a=x1/scalex;
  if( y1<0 ) y1a=0;      else y1a=height-y1/scaley;
  if( x2<0 ) x2a=width;  else x2a=x2/scalex;
  if( y2<0 ) y2a=height; else y2a=height-y2/scaley;

  const wxPen oldPen=m_dc->GetPen();
  const wxBrush oldBrush=m_dc->GetBrush();

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(bgr, bgg, bgb), 1, wxSOLID)) );
  m_context->SetBrush( wxBrush(wxColour(bgr, bgg, bgb)) );
  m_context->DrawRectangle( x1a, y1a, x2a-x1a, y2a-y1a ); 

  m_context->SetPen( oldPen );  
  m_context->SetBrush( oldBrush );  
   
  AddtoClipRegion( (int)x1a, (int)y1a, (int)x2a, (int)y2a );
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

  wxDouble x, y, w, h;
  path.GetBox( &x, &y, &w, &h );
  
  AddtoClipRegion( (int)x, (int)y, (int)(x+w), (int)(y+h) );  
}


void wxPLDevGC::BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  if( m_dc )
    dc->Blit( vX, vY, vW, vH, m_dc, vX, vY );
}


void wxPLDevGC::CreateCanvas()
{
  // Log_Verbose( "%s", __FUNCTION__ );

  if( ownGUI ) {
    if( !m_dc )
      m_dc = new wxMemoryDC();

    ((wxMemoryDC*)m_dc)->SelectObject( wxNullBitmap );   /* deselect bitmap */
    if( m_bitmap )
      delete m_bitmap;
    m_bitmap = new wxBitmap( bm_width, bm_height, 32 );
    ((wxMemoryDC*)m_dc)->SelectObject( *m_bitmap );   /* select new bitmap */  
  }
  
  if( m_dc )
    m_context = wxGraphicsContext::Create( *((wxMemoryDC*)m_dc) );
}


void wxPLDevGC::SetWidth( PLStream *pls )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
                                                               pls->cmap0[pls->icol0].b, (unsigned char)(pls->cmap0[pls->icol0].a*255)),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
}


void wxPLDevGC::SetColor0( PLStream *pls )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
                                                               pls->cmap0[pls->icol0].b, (unsigned char)(pls->cmap0[pls->icol0].a*255)),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
  m_context->SetBrush( wxBrush(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g, pls->cmap0[pls->icol0].b,
                                        (unsigned char)(pls->cmap0[pls->icol0].a*255))) );
}


void wxPLDevGC::SetColor1( PLStream *pls )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(pls->curcolor.r, pls->curcolor.g,
                                                              pls->curcolor.b, (unsigned char)(pls->curcolor.a*255)),
                                                     pls->width>0 ? pls->width : 1, wxSOLID)) );
  m_context->SetBrush( wxBrush(wxColour(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b,
                                        (unsigned char)(pls->curcolor.a*255))) );
}


/*--------------------------------------------------------------------------*\
 *  void wx_set_dc( PLStream* pls, wxDC* dc )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wxPLDevGC::SetExternalBuffer( void* dc )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  m_dc=(wxDC*)dc;  /* Add the dc to the device */
  m_context = wxGraphicsContext::Create( *((wxMemoryDC*)m_dc) );
  ready = true;
  ownGUI = false;
}


void wxPLDevGC::PutPixel( short x, short y, PLINT color )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  const wxPen oldpen=m_dc->GetPen();
  m_context->SetPen( *(wxThePenList->FindOrCreatePen(wxColour(GetRValue(color), GetGValue(color), GetBValue(color)),
                                                     1, wxSOLID)) );
  //m_context->DrawPoint( x, y );
  AddtoClipRegion( x, y, x, y );
  m_context->SetPen( oldpen );
}

void wxPLDevGC::PutPixel( short x, short y )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  //m_dc->DrawPoint( x, y );
  AddtoClipRegion( x, y, x, y );
}

PLINT wxPLDevGC::GetPixel( short x, short y )
{
  // Log_Verbose( "%s", __FUNCTION__ );

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

void wxPLDevGC::PSDrawTextToDC( char* utf8_string, bool drawText )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  wxDouble w, h, d, l;

  wxString str(wxConvUTF8.cMB2WC(utf8_string), *wxConvCurrent);
  m_context->GetTextExtent( str, &w, &h, &d, &l );
  if( drawText ) {
    m_context->DrawText( str, 0, -yOffset/scaley );
    m_context->Translate( w, 0 );
  }
 
  textWidth += w;
  textHeight = textHeight>(h+yOffset/scaley) ? textHeight : (h+yOffset/scaley);
  memset( utf8_string, '\0', max_string_length );
}


void wxPLDevGC::PSSetFont( PLUNICODE fci )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  unsigned char fontFamily, fontStyle, fontWeight;

  plP_fci2hex( fci, &fontFamily, PL_FCI_FAMILY );
  plP_fci2hex( fci, &fontStyle, PL_FCI_STYLE );
  plP_fci2hex( fci, &fontWeight, PL_FCI_WEIGHT );  
  if( m_font )
    delete m_font;
  m_font=wxFont::New(fontSize*fontScale, fontFamilyLookup[fontFamily],
                         fontStyleLookup[fontStyle] & fontWeightLookup[fontWeight] );
  m_font->SetUnderlined( underlined );
  m_context->SetFont( *m_font, wxColour(textRed, textGreen, textBlue) );
}


void wxPLDevGC::ProcessString( PLStream* pls, EscText* args )
{
  // Log_Verbose( "%s", __FUNCTION__ );

  /* Check that we got unicode, warning message and return if not */
  if( args->unicode_array_len == 0 ) {
    printf( "Non unicode string passed to a cairo driver, ignoring\n" );
    return;
  }
	
  /* Check that unicode string isn't longer then the max we allow */
  if( args->unicode_array_len >= 500 ) {
    printf( "Sorry, the wxWidgets drivers only handles strings of length < %d\n", 500 );
    return;
  }
  
  /* Calculate the font size (in pixels) */
  fontSize = pls->chrht * VIRTUAL_PIXELS_PER_MM/scaley * 1.3;

  /* text color */
  textRed=pls->cmap0[pls->icol0].r;
  textGreen=pls->cmap0[pls->icol0].g;
  textBlue=pls->cmap0[pls->icol0].b;
  
  /* calculate rotation of text */
  plRotationShear( args->xform, &rotation, &shear );
  rotation -= pls->diorot * M_PI / 2.0;
  cos_rot = cos( rotation );
  sin_rot = sin( rotation );
  cos_shear = cos(shear);
  sin_shear = sin(shear);
 
  /* determine extend of text */
  PSDrawText( args->unicode_array, args->unicode_array_len, false );
 
  /* actually draw text */
  m_context->PushState();
  wxGraphicsMatrix matrix=m_context->CreateMatrix(  cos_rot, -sin_rot,
                                                    cos_rot * sin_shear + sin_rot * cos_shear,
                                                   -sin_rot * sin_shear + cos_rot * cos_shear,
                                                    args->x/scalex-args->just*textWidth,
                                                    height-args->y/scaley-0.5*textHeight );
  m_context->SetTransform( matrix );
  PSDrawText( args->unicode_array, args->unicode_array_len, true );
  m_context->PopState();

  AddtoClipRegion( 0, 0, width, height );        
}

#endif

#endif				/* PLD_wxwidgets */


