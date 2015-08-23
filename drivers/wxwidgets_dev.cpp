// Copyright (C) 2015  Phil Rosenberg
// Copyright (C) 2005  Werner Smekal, Sjaak Verdoold
// Copyright (C) 2005  Germain Carrera Corraleche
// Copyright (C) 1999  Frank Huebner
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

#define DEBUG
#define NEED_PLDEBUG

//set this to help when debugging wxPLViewer issues
//it uses a memory map name without random characters
//and does not execute the viewer, allowing the user to do
//it in a debugger
//#define WXPLVIEWER_DEBUG

//headers needed for Rand
#ifdef WIN32
//this include must occur before any other include of stdlib.h
//due to the #define _CRT_RAND_S
#define _CRT_RAND_S
#include <stdlib.h>
#else
#include <fstream>
#endif

//plplot headers
#include "plDevs.h"
#include "wxwidgets.h" // includes wx/wx.h

// wxwidgets headers
#include <wx/dir.h>

// std and driver headers
#include <cmath>
#include <limits>


//--------------------------------------------------------------------------
// Scaler class
// This class changes the logical scale of a dc on construction and resets
// it to its original value on destruction. It is ideal for making temporary
// changes to the scale and guarenteeing that the scale gets set back.
//--------------------------------------------------------------------------
class Scaler
{
public:
    Scaler( wxDC * dc, double xScale, double yScale )
    {
        m_dc = dc;
        if ( m_dc )
        {
            dc->GetLogicalScale( &m_xScaleOld, &m_yScaleOld );
            dc->SetLogicalScale( xScale, yScale );
        }
    }
    ~Scaler( )
    {
        if ( m_dc )
            m_dc->SetLogicalScale( m_xScaleOld, m_yScaleOld );
    }
private:
    wxDC   *m_dc;
    double m_xScaleOld;
    double m_yScaleOld;
};

//--------------------------------------------------------------------------
// OriginChanger class
// This class changes the logical origin of a dc on construction and resets
// it to its original value on destruction. It is ideal for making temporary
// changes to the origin and guarenteeing that the scale gets set back.
//--------------------------------------------------------------------------
class OriginChanger
{
public:
    OriginChanger( wxDC * dc, wxCoord xOrigin, wxCoord yOrigin )
    {
        m_dc = dc;
        if ( m_dc )
        {
            dc->GetLogicalOrigin( &m_xOriginOld, &m_yOriginOld );
            dc->SetLogicalOrigin( xOrigin, yOrigin );
        }
    }
    ~OriginChanger( )
    {
        if ( m_dc )
            m_dc->SetLogicalOrigin( m_xOriginOld, m_yOriginOld );
    }
private:
    wxDC    *m_dc;
    wxCoord m_xOriginOld;
    wxCoord m_yOriginOld;
};

//--------------------------------------------------------------------------
// DrawingObjectsChanger class
// This class changes the pen and brush of a dc on construction and resets
// them to their original values on destruction. It is ideal for making temporary
// changes to the pen and brush and guarenteeing that they get set back.
//--------------------------------------------------------------------------
class DrawingObjectsChanger
{
public:
    DrawingObjectsChanger( wxDC *dc, const wxPen &pen, const wxBrush &brush )
    {
        m_dc = dc;
        if ( m_dc )
        {
            m_pen   = dc->GetPen();
            m_brush = dc->GetBrush();
            dc->SetPen( pen );
            dc->SetBrush( brush );
        }
    }
    ~DrawingObjectsChanger()
    {
        if ( m_dc )
        {
            m_dc->SetPen( m_pen );
            m_dc->SetBrush( m_brush );
        }
    }
private:
    wxDC    *m_dc;
    wxPen   m_pen;
    wxBrush m_brush;
};

//--------------------------------------------------------------------------
// TextObjectsChanger class
// This class changes the font and text colours of a dc on construction and resets
// them to their original values on destruction. It is ideal for making temporary
// changes to the text and guarenteeing that they get set back.
//--------------------------------------------------------------------------
class TextObjectsChanger
{
public:
    TextObjectsChanger( wxDC *dc, const wxFont &font, const wxColour &textForeground, const wxColour &textBackground )
    {
        m_dc = dc;
        if ( m_dc )
        {
            m_font           = dc->GetFont();
            m_textForeground = dc->GetTextForeground();
            m_textBackground = dc->GetTextBackground();
            dc->SetTextForeground( textForeground );
            dc->SetTextBackground( textBackground );
            dc->SetFont( font );
        }
    }
    TextObjectsChanger( wxDC *dc, const wxFont &font )
    {
        m_dc = dc;
        if ( m_dc )
        {
            m_font           = dc->GetFont();
            m_textForeground = dc->GetTextForeground();
            m_textBackground = dc->GetTextBackground();
            dc->SetFont( font );
        }
    }
    TextObjectsChanger( wxDC *dc, FontGrabber &fontGrabber, PLUNICODE fci, PLFLT size, bool underlined, const wxColour &textForeground, const wxColour &textBackground )
    {
        m_dc = dc;
        if ( m_dc )
        {
            wxFont font = fontGrabber.GetFont( fci, size, underlined ).getWxFont();
            m_font           = dc->GetFont();
            m_textForeground = dc->GetTextForeground();
            m_textBackground = dc->GetTextBackground();
            dc->SetTextForeground( textForeground );
            dc->SetTextBackground( textBackground );
            dc->SetFont( font );
        }
    }
    ~TextObjectsChanger()
    {
        if ( m_dc )
        {
            m_dc->SetTextForeground( m_textForeground );
            m_dc->SetTextBackground( m_textBackground );
            m_dc->SetFont( m_font );
        }
    }
private:
    wxDC     *m_dc;
    wxFont   m_font;
    wxColour m_textForeground;
    wxColour m_textBackground;
};

//--------------------------------------------------------------------------
// Clipper class
// This class changes the clipping region of a dc on construction and restores
// it to its previous region on destruction. It is ideal for making temporary
// changes to the clip region and guarenteeing that the scale gets set back.
//
// It turns out that clipping is mostly broken for wxGCDC - see
// http://trac.wxwidgets.org/ticket/17013. So there are a lot of things in
// this class to work aound those bugs. In particular you should check
// isEveryThingClipped before drawing as I'm not sure if non-overlapping
//clip regions behave properly.
//--------------------------------------------------------------------------
class Clipper
{
public:
    Clipper( wxDC * dc, const wxRect &rect )
    {
        m_dc             = dc;
        m_clipEverything = true;
        if ( m_dc )
        {
            dc->GetClippingBox( m_boxOld );
            wxRect newRect = rect;
            m_clipEverything = !( newRect.Intersects( m_boxOld )
                                  || ( m_boxOld.width == 0 && m_boxOld.height == 0 ) );
            if ( m_clipEverything )
                dc->SetClippingRegion( wxRect( -1, -1, 1, 1 ) );                 //not sure if this works
            else
                dc->SetClippingRegion( rect );
        }
    }
    ~Clipper( )
    {
        if ( m_dc )
        {
            m_dc->DestroyClippingRegion();
            m_dc->SetClippingRegion( wxRect( 0, 0, 0, 0 ) );
            m_dc->DestroyClippingRegion();
            if ( m_boxOld.width != 0 && m_boxOld.height != 0 )
                m_dc->SetClippingRegion( m_boxOld );
        }
    }
    bool isEverythingClipped()
    {
        return m_clipEverything;
    }
private:
    wxDC   *m_dc;
    wxRect m_boxOld;
    bool   m_clipEverything;
};

//--------------------------------------------------------------------------
// class Rand
// This is a simple random number generator class, created soley so that
// random numbers can be generated in this file without "contaminating" the
// global series of random numbers with a new seed.
// It uses an algorithm that apparently used to be used in gcc rand()
// provided under GNU LGPL v2.1.
//--------------------------------------------------------------------------
class Rand
{
public:
    Rand()
    {
#ifdef WIN32
        rand_s( &m_seed );
#else
        std::fstream fin( "/dev/random", std::ios::in );
        fin.read( (char *) ( &m_seed ), sizeof ( m_seed ) );
        fin.close();
#endif
    }
    Rand( unsigned int seed )
    {
        m_seed = seed;
    }
    unsigned int operator()()
    {
        unsigned int next = m_seed;
        int          result;

        next  *= 1103515245;
        next  += 12345;
        result = (unsigned int) ( next / max ) % 2048;

        next    *= 1103515245;
        next    += 12345;
        result <<= 10;
        result  ^= (unsigned int) ( next / max ) % 1024;

        next    *= 1103515245;
        next    += 12345;
        result <<= 10;
        result  ^= (unsigned int) ( next / max ) % 1024;

        m_seed = next;

        return result;
    }
    static const unsigned int max = 65536;
private:
    unsigned int m_seed;
};

void plFontToWxFontParameters( PLUNICODE fci, PLFLT scaledFontSize, wxFontFamily &family, int &style, int &weight, int &pt )
{
    unsigned char plFontFamily, plFontStyle, plFontWeight;

    plP_fci2hex( fci, &plFontFamily, PL_FCI_FAMILY );
    plP_fci2hex( fci, &plFontStyle, PL_FCI_STYLE );
    plP_fci2hex( fci, &plFontWeight, PL_FCI_WEIGHT );

    family = fontFamilyLookup[plFontFamily];
    style  = fontStyleLookup[plFontStyle];
    weight = fontWeightLookup[plFontWeight];
    pt     = ROUND( scaledFontSize );
}

Font::Font ( )
{
    m_fci        = 0;
    m_size       = std::numeric_limits<PLFLT>::quiet_NaN();
    m_underlined = false;
    m_hasFont    = false;
}

Font::Font( PLUNICODE fci, PLFLT size, bool underlined, bool createFontOnConstruction )
{
    m_fci        = fci;
    m_size       = size;
    m_underlined = underlined;
    m_hasFont    = false;
    if ( createFontOnConstruction )
        createFont();
}

void Font::createFont()
{
    wxFontFamily family;
    int          style;
    int          weight;
    int          pt;
    plFontToWxFontParameters( m_fci, m_size, family, style, weight, pt );

    m_font    = wxFont( pt, family, style, weight, m_underlined, wxEmptyString, wxFONTENCODING_DEFAULT );
    m_hasFont = true;
}

wxFont Font::getWxFont()
{
    if ( !m_hasFont )
        createFont();
    return m_font;
}

bool operator ==( const Font &lhs, const Font &rhs )
{
    return lhs.getFci() == rhs.getFci()
           && lhs.getSize() == rhs.getSize()
           && lhs.getUnderlined() == rhs.getUnderlined();
}

//--------------------------------------------------------------------------
//  FontGrabber::FontGrabber( )
//
//  Default constructor
//--------------------------------------------------------------------------
FontGrabber::FontGrabber( )
{
    m_lastWasCached = false;
}

//--------------------------------------------------------------------------
//  Font FontGrabber::GetFont( PLUNICODE fci )
//
//  Get the requested font either fresh of from the cache.
//--------------------------------------------------------------------------
Font FontGrabber::GetFont( PLUNICODE fci, PLFLT scaledFontSize, bool underlined )
{
    Font newFont( fci, scaledFontSize, underlined );
    if ( m_prevFont == newFont )
    {
        m_lastWasCached = true;
        return m_prevFont;
    }

    m_lastWasCached = false;

    return m_prevFont = newFont;
}

//--------------------------------------------------------------------------
//  wxPLDevice::wxPLDevice( void )
//
//  Constructor of the standard wxWidgets device based on the wxPLDevBase
//  class. Only some initialisations are done.
//--------------------------------------------------------------------------
wxPLDevice::wxPLDevice( PLStream *pls, char * mfo, PLINT text, PLINT hrshsym )
    : m_plplotEdgeLength( PLFLT( SHRT_MAX ) )
{
    m_fixedAspect = false;

    m_lineSpacing = 1.0;

    m_dc = NULL;

    m_prevSingleCharString       = 0;
    m_prevSingleCharStringWidth  = 0;
    m_prevSingleCharStringHeight = 0;

    if ( mfo )
        strcpy( m_mfo, mfo );
    else
    //assume we will be outputting to the default
    //memory map until we are given a dc to draw to
#ifdef WXPLVIEWER_DEBUG
        strcpy( m_mfo, "plplotMemoryMap" );
#else
        strcpy( m_mfo, "plplotMemoryMap??????????" );
#endif

    // be verbose and write out debug messages
#ifdef _DEBUG
    pls->verbose = 1;
    pls->debug   = 1;
#endif

    pls->color       = 1;                               // Is a color device
    pls->dev_flush   = 1;                               // Handles flushes
    pls->dev_fill0   = 1;                               // Can handle solid fills
    pls->dev_fill1   = 0;                               // Can't handle pattern fills
    pls->dev_dash    = 0;
    pls->dev_clear   = 1;                               // driver supports clear
    pls->plbuf_write = 1;                               // use the plot buffer!
    pls->termin      = ( strlen( m_mfo ) ) > 0 ? 0 : 1; // interactive device unless we are writing to memory - pretty sure this is an unused option though
    pls->graphx      = GRAPHICS_MODE;                   //  This indicates this is a graphics driver. PLPlot with therefore cal pltext() before outputting text, however we currently have not implemented catching that text.

    if ( text )
    {
        pls->dev_text    = 1; // want to draw text
        pls->dev_unicode = 1; // want unicode
        if ( hrshsym )
            pls->dev_hrshsym = 1;
    }


    // Set up physical limits of plotting device in plplot internal units
    // we just tell plplot we are the maximum plint in both dimensions
    //which gives us the best resolution
    plP_setphy( (PLINT) 0, (PLINT) SHRT_MAX,
        (PLINT) 0, (PLINT) SHRT_MAX );

    // set dpi and page size defaults if the user has not already set
    // these with -dpi or -geometry command line options or with
    // plspage.

    if ( pls->xdpi <= 0. || pls->ydpi <= 0. )
    {
        // Use recommended default DPI
        plspage( PLPLOT_DEFAULT_DPI, PLPLOT_DEFAULT_DPI, 0, 0, 0, 0 );
    }

    if ( pls->xlength == 0 || pls->ylength == 0 )
    {
        // Use recommended default pixel width and height.
        plspage( 0., 0., PLPLOT_DEFAULT_WIDTH_PIX, PLPLOT_DEFAULT_HEIGHT_PIX, 0, 0 );
    }
    m_localBufferPosition = 0;

    SetSize( pls, plsc->xlength, plsc->ylength );

    if ( pls->dev_data )
        SetDC( pls, (wxDC *) pls->dev_data );
    else
        SetupMemoryMap();

    //this must be the absolute last thing that is done
    //so that if an exception is thrown pls->dev remains as NULL
    pls->dev = (void *) this;
}


//--------------------------------------------------------------------------
//  wxPLDevice::~wxPLDevice( void )
//
//  The deconstructor frees memory allocated by the device.
//--------------------------------------------------------------------------
wxPLDevice::~wxPLDevice()
{
    if ( m_outputMemoryMap.isValid() )
    {
        MemoryMapHeader *header = (MemoryMapHeader *) ( m_outputMemoryMap.getBuffer() );
        header->completeFlag = 1;
    }
}

//--------------------------------------------------------------------------
//  void wxPLDevice::PreDestructorTidy( PLStream *pls )
//
//  This function performs any tidying up that requires a PLStream. should
//  be called before the destructor obviously
//--------------------------------------------------------------------------
void wxPLDevice::PreDestructorTidy( PLStream *pls )
{
    if ( !m_dc && pls->nopause )
        TransmitBuffer( pls, transmissionClose );
}

//--------------------------------------------------------------------------
//  void wxPLDevice::DrawLine( short x1a, short y1a, short x2a, short y2a )
//
//  Draw a line from (x1a, y1a) to (x2a, y2a).
//--------------------------------------------------------------------------
void wxPLDevice::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
    if ( !m_dc )
        return;

    Clipper clipper( m_dc, GetClipRegion().GetBox() );
    Scaler  scaler( m_dc, 1.0 / m_scale, 1.0 / m_scale );
    DrawingObjectsChanger drawingObjectsChanger( m_dc, m_pen, m_brush );
    m_dc->DrawLine( (wxCoord) ( m_xAspect * x1a ), (wxCoord) ( m_yAspect * ( m_plplotEdgeLength - y1a ) ),
        (wxCoord) ( m_xAspect * x2a ), (wxCoord) ( m_yAspect * ( m_plplotEdgeLength - y2a ) ) );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::DrawPolyline( short *xa, short *ya, PLINT npts )
//
//  Draw a poly line - coordinates are in the xa and ya arrays.
//--------------------------------------------------------------------------
void wxPLDevice::DrawPolyline( short *xa, short *ya, PLINT npts )
{
    if ( !m_dc )
        return;
    Clipper clipper( m_dc, GetClipRegion().GetBox() );
    Scaler  scaler( m_dc, 1.0 / m_scale, 1.0 / m_scale );
    DrawingObjectsChanger drawingObjectsChanger( m_dc, m_pen, m_brush );
    for ( PLINT i = 1; i < npts; i++ )
        m_dc->DrawLine( m_xAspect * xa[i - 1], m_yAspect * ( m_plplotEdgeLength - ya[i - 1] ),
            m_xAspect * xa[i], m_yAspect * ( m_plplotEdgeLength - ya[i] ) );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::ClearBackground( PLStream* pls, PLINT bgr, PLINT bgg, PLINT bgb,
//                                   PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
//
//  Clear parts ((x1,y1) to (x2,y2)) of the background in color (bgr,bgg,bgb).
//--------------------------------------------------------------------------
void wxPLDevice::ClearBackground( PLStream* pls, PLINT x1, PLINT y1, PLINT x2, PLINT y2 )
{
    if ( !m_dc )
        return;

    x1 = x1 < 0 ? 0 : x1;
    x2 = x2 < 0 ? m_plplotEdgeLength : x2;
    y1 = y1 < 0 ? 0 : y1;
    y2 = y2 < 0 ? m_plplotEdgeLength : y2;

    PLINT x      = MIN( x1, x2 );
    PLINT y      = MIN( y1, y2 );
    PLINT width  = abs( x1 - x2 );
    PLINT height = abs( y1 - y2 );

    if ( width > 0 && height > 0 )
    {
        PLINT    r, g, b;
        PLFLT    a;
        plgcolbga( &r, &g, &b, &a );
        wxColour bgColour( r, g, b, a * 255 );
        DrawingObjectsChanger changer( m_dc, wxPen( bgColour, 0 ), wxBrush( bgColour ) );
        m_dc->DrawRectangle( x, y, width, height );
    }
}


//--------------------------------------------------------------------------
//  void wxPLDevice::FillPolygon( PLStream *pls )
//
//  Draw a filled polygon.
//--------------------------------------------------------------------------
void wxPLDevice::FillPolygon( PLStream *pls )
{
    if ( !m_dc )
        return;

    //edge the polygon with a 0.5 pixel line to avoid seams. This is a
    //bit of a bodge really but this is a difficult problem
    wxPen edgePen( m_brush.GetColour(), m_scale, wxSOLID );
    DrawingObjectsChanger changer( m_dc, edgePen, m_brush );
    //DrawingObjectsChanger changer(m_dc, wxNullPen, m_brush );
    Scaler  scaler( m_dc, 1.0 / m_scale, 1.0 / m_scale );
    wxPoint *points = new wxPoint[pls->dev_npts];
    wxCoord xoffset = 0;
    wxCoord yoffset = 0;

    for ( int i = 0; i < pls->dev_npts; i++ )
    {
        points[i].x = (int) ( m_xAspect * pls->dev_x[i] );
        points[i].y = (int) ( m_yAspect * ( m_plplotEdgeLength - pls->dev_y[i] ) );
    }

    if ( pls->dev_eofill )
    {
        m_dc->DrawPolygon( pls->dev_npts, points, xoffset, yoffset, wxODDEVEN_RULE );
    }
    else
    {
        m_dc->DrawPolygon( pls->dev_npts, points, xoffset, yoffset, wxWINDING_RULE );
    }
    delete[] points;
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetWidth( PLStream *pls )
//
//  Set the width of the drawing pen.
//--------------------------------------------------------------------------
void wxPLDevice::SetWidth( PLStream *pls )
{
    PLFLT width = ( pls->width > 0.0 ? pls->width : 1.0 ) * m_scale;
    m_pen = wxPen( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b,
            pls->curcolor.a * 255 ), width, wxSOLID );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetColor( PLStream *pls )
//
//  Set color from PLStream.
//--------------------------------------------------------------------------
void wxPLDevice::SetColor( PLStream *pls )
{
    PLFLT width = ( pls->width > 0.0 ? pls->width : 1.0 ) * m_scale;
    m_pen = wxPen( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b,
            pls->curcolor.a * 255 ), width, wxSOLID );
    m_brush = wxBrush( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b,
            pls->curcolor.a * 255 ) );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetDC( PLStream *pls, void* dc )
//
//  Adds a dc to the device. In that case, the drivers doesn't provide
//  a GUI.
//--------------------------------------------------------------------------
void wxPLDevice::SetDC( PLStream *pls, wxDC* dc )
{
    if ( m_outputMemoryMap.isValid() )
        throw( "wxPLDevice::SetDC The DC must be set before initialisation. The device is outputting to a separate viewer" );
    m_dc = dc;                 // Add the dc to the device
    m_useDcTextTransform = false;
    m_gc = NULL;
    if ( m_dc )
    {
#if wxVERSION_NUMBER >= 2902
        m_useDcTextTransform = m_dc->CanUseTransformMatrix();
#endif
        //If we don't have wxDC tranforms we can use the
        //underlying wxGCDC if this is a wxGCDC
        if ( !m_useDcTextTransform )
        {
            //check to see if m_dc is a wxGCDC by using RTTI
            wxGCDC *gcdc = NULL;
            try
            {
                //put this in a try block as I'm not sure if it will throw if
                //RTTI is switched off
                gcdc = dynamic_cast< wxGCDC* >( m_dc );
            }
            catch ( ... )
            {
            }
            if ( gcdc )
                m_gc = gcdc->GetGraphicsContext();
        }

        strcpy( m_mfo, "" );
        SetSize( pls, m_width, m_height );  //call with our current size to set the scaling
        pls->has_string_length = 1;         // Driver supports string length calculations, if we have a dc to draw on
    }
    else
    {
        pls->has_string_length = 0;         //if we have no device to draw on we cannot check string size
    }
}

PLFLT getTextOffset( PLINT superscriptLevel, PLFLT baseFontSize )
{
    if ( superscriptLevel == 0 )
        return 0;
    else
    {
        PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
        if ( superscriptLevel > 0 )
            return getTextOffset( superscriptLevel - 1, baseFontSize ) + baseFontSize * fontScale / 2.;
        else
            return getTextOffset( superscriptLevel + 1, baseFontSize ) - baseFontSize * fontScale * 0.8 / 2.;
    }
}

//--------------------------------------------------------------------------
//  void wxPLDevice::DrawText( PLUNICODE* ucs4, int ucs4Len, bool drawText )
//
//  This is the function to draw text. Pass in a unicde string and its
//  length and set drawText to true to actually draw the text or false to
//  just get text metrics. This function will process the string for inline
//  style change escapes and newlines.
//--------------------------------------------------------------------------
void wxPLDevice::DrawTextLine( PLUNICODE* ucs4, int ucs4Len, PLFLT baseFontSize, bool drawText, PLINT &superscriptLevel, bool &underlined )
{
    if ( !m_dc && drawText )
        return;

    char utf8_string[m_max_string_length];
    char utf8[5];
    memset( utf8_string, '\0', m_max_string_length );

    // Get PLplot escape character
    char plplotEsc;
    plgesc( &plplotEsc );

    //Reset the size metrics
    m_textWidth         = 0;
    m_textHeight        = 0;
    m_superscriptHeight = 0;
    m_subscriptDepth    = 0;

    int i = 0;
    while ( i < ucs4Len )
    {
        if ( ucs4[i] < PL_FCI_MARK )                // not a font change
        {
            if ( ucs4[i] != (PLUNICODE) plplotEsc ) // a character to display
            {
                ucs4_to_utf8( ucs4[i], utf8 );
                strncat( utf8_string, utf8,
                    sizeof ( utf8_string ) - strlen( utf8_string ) - 1 );
                i++;
                continue;
            }
            i++;
            if ( ucs4[i] == (PLUNICODE) plplotEsc ) // a escape character to display
            {
                ucs4_to_utf8( ucs4[i], utf8 );
                strncat( utf8_string, utf8,
                    sizeof ( utf8_string ) - strlen( utf8_string ) - 1 );
                i++;
                continue;
            }
            else
            {
                if ( ucs4[i] == (PLUNICODE) 'u' ) // Superscript
                {                                 // draw string so far
                    PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
                    PLFLT yOffset   = getTextOffset( superscriptLevel, baseFontSize ) * m_yScale;
                    DrawTextSection( utf8_string, baseFontSize * fontScale, yOffset, underlined, drawText );

                    ++superscriptLevel;
                    fontScale = pow( 0.8, abs( superscriptLevel ) );
                    if ( m_dc )
                        m_dc->SetFont( m_fontGrabber.GetFont( m_fci, baseFontSize * fontScale, underlined ).getWxFont() );
                }
                if ( ucs4[i] == (PLUNICODE) 'd' ) // Subscript
                {                                 // draw string so far
                    PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
                    PLFLT yOffset   = getTextOffset( superscriptLevel, baseFontSize ) * m_yScale;
                    DrawTextSection( utf8_string, baseFontSize * fontScale, yOffset, underlined, drawText );

                    --superscriptLevel;
                    fontScale = pow( 0.8, abs( superscriptLevel ) );
                    if ( m_dc )
                        m_dc->SetFont( m_fontGrabber.GetFont( m_fci, baseFontSize * fontScale, underlined ).getWxFont() );
                }
                if ( ucs4[i] == (PLUNICODE) '-' ) // underline
                {                                 // draw string so far
                    PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
                    PLFLT yOffset   = getTextOffset( superscriptLevel, baseFontSize ) * m_yScale;
                    DrawTextSection( utf8_string, baseFontSize * fontScale, yOffset, underlined, drawText );

                    underlined = !underlined;
                    if ( m_dc )
                        m_dc->SetFont( m_fontGrabber.GetFont( m_fci, baseFontSize * fontScale, underlined ).getWxFont() );
                }
                if ( ucs4[i] == (PLUNICODE) '+' ) // overline
                {                                 // not implemented yet
                }
                i++;
            }
        }
        else // a font change
        {
            // draw string so far
            PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
            PLFLT yOffset   = getTextOffset( superscriptLevel, baseFontSize ) * m_yScale;
            DrawTextSection( utf8_string, baseFontSize * fontScale, yOffset, underlined, drawText );

            // get new font
            m_fci = ucs4[i];
            if ( m_dc )
                m_dc->SetFont( m_fontGrabber.GetFont( m_fci, baseFontSize * fontScale, underlined ).getWxFont() );
            i++;
        }
    }

    //we have reached the end of the string. Draw the remainder.
    PLFLT fontScale = pow( 0.8, abs( superscriptLevel ) );
    PLFLT yOffset   = getTextOffset( superscriptLevel, baseFontSize ) * m_yScale;
    DrawTextSection( utf8_string, baseFontSize * fontScale, yOffset, underlined, drawText );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::DrawTextSection( char* utf8_string, bool drawText )
//
//  Draw a section of text. This should already have been processed and
//  split into sections so that the text passed in has no style changes or
//  newines.
//--------------------------------------------------------------------------
void wxPLDevice::DrawTextSection( char* utf8_string, PLFLT scaledFontSize, PLFLT yOffset, bool underlined, bool drawText )
{
    if ( !m_dc && drawText )
        return;

    wxCoord  w, h, d, l;

    wxString str = wxString::FromUTF8( utf8_string );
    //wxString str( wxConvUTF8.cMB2WC( utf8_string ), *wxConvCurrent );

    if ( m_dc )
        m_dc->GetTextExtent( str, &w, &h, &d, &l );

    if ( !m_dc && m_outputMemoryMap.isValid() )
    {
        MemoryMapHeader *header = (MemoryMapHeader *) ( m_outputMemoryMap.getBuffer() );
        header->textSizeInfo.written = false;
        plFontToWxFontParameters( m_fci, scaledFontSize, header->textSizeInfo.family, header->textSizeInfo.style,
            header->textSizeInfo.weight, header->textSizeInfo.pt );
        header->textSizeInfo.underlined = underlined;
        bool   gotResponse = false;
        TransmitBuffer( NULL, transmissionRequestTextSize );
        size_t counter = 0;
        while ( !gotResponse && counter < 1000 )
        {
            gotResponse = header->textSizeInfo.written;
            ++counter;
            wxMilliSleep( 1 );
        }
        if ( counter == 1000 )
            plwarn( "Failed to get text size from wxPLViewer - timeout" );

        w = header->textSizeInfo.width;
        h = header->textSizeInfo.height;
    }

    if ( drawText )
    {
        //if we are using wxDC transforms or the wxGC, then the transformations
        //have already been applied
        if ( m_gc )
            m_gc->DrawText( str, m_textWidth, -yOffset / m_yScale );
        else if ( m_useDcTextTransform )
            m_dc->DrawText( str, m_textWidth, -yOffset / m_yScale );
        else
        {
            //If we are stuck with a wxDC that has no transformation abilities then
            // all we can really do is rotate the text - this is a bit of a poor state
            // really, but to be honest it is better than defaulting to hershey for all
            // text
            if ( m_rotation == 0 )
                m_dc->DrawRotatedText( str, (wxCoord) ( m_posX + m_textWidth ),
                    (wxCoord) ( m_height - (wxCoord) ( m_posY + yOffset / m_yScale ) ),
                    m_rotation * 180.0 / M_PI );
            else
                m_dc->DrawRotatedText( str,
                    (wxCoord) ( m_posX - yOffset / m_yScale * sin( m_rotation ) + m_textWidth * cos( m_rotation ) ),
                    (wxCoord) ( m_height - (wxCoord) ( m_posY + yOffset * cos( m_rotation ) / m_yScale ) - m_textWidth * sin( m_rotation ) ),
                    m_rotation * 180.0 / M_PI );
        }
    }

    m_textWidth += w;

    //keep track of the height of superscript text, the depth of subscript
    //text and the height of regular text
    if ( yOffset > 0.0001 )
    {
        //determine the height the text would have if it were full size
        double currentOffset = yOffset;
        double currentHeight = h;
        while ( currentOffset > 0.0001 )
        {
            currentOffset -= m_yScale * scaledFontSize / 2.;
            currentHeight *= 1.25;
        }
        m_textHeight = (wxCoord) m_textHeight > ( currentHeight )
                       ? m_textHeight
                       : currentHeight;
        //work out the height including superscript
        m_superscriptHeight = m_superscriptHeight > ( currentHeight + yOffset / m_yScale )
                              ? m_superscriptHeight
                              : static_cast<int>( ( currentHeight + yOffset / m_yScale ) );
    }
    else if ( yOffset < -0.0001 )
    {
        //determine the height the text would have if it were full size
        double currentOffset = yOffset;
        double currentHeight = h;
        double currentDepth  = d;
        while ( currentOffset < -0.0001 )
        {
            currentOffset += m_yScale * scaledFontSize * 1.25 / 2.;
            currentHeight *= 1.25;
            currentDepth  *= 1.25;
        }
        m_textHeight = (wxCoord) m_textHeight > currentHeight ? m_textHeight : currentHeight;
        //work out the additional depth for subscript note an assumption has been made
        //that the font size of (non-superscript and non-subscript) text is the same
        //along a line. Currently there is no escape to change font size mid string
        //so this should be fine
        m_subscriptDepth = (wxCoord) m_subscriptDepth > ( ( -yOffset / m_yScale + h + d ) - ( currentDepth + m_textHeight ) )
                           ? m_subscriptDepth
                           : ( ( -yOffset / m_yScale + h + d ) - ( currentDepth + m_textHeight ) );
        m_subscriptDepth = m_subscriptDepth > 0 ? m_subscriptDepth : 0;
    }
    else
        m_textHeight = (wxCoord) m_textHeight > ( h ) ? m_textHeight : h;

    memset( utf8_string, '\0', m_max_string_length );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::ProcessString( PLStream* pls, EscText* args )
//
//  This is the main function which processes the unicode text strings.
//  Font size, rotation and color are set, width and height of the
//  text string is determined and then the string is drawn to the canvas.
//--------------------------------------------------------------------------
void wxPLDevice::ProcessString( PLStream* pls, EscText* args )
{
    if ( !m_dc && !m_outputMemoryMap.isValid() )
        return;

    m_textWidth   = 0;
    m_textHeight  = 0;
    m_textDescent = 0;
    m_textLeading = 0;

    //for text, work in native coordinates, partly to avoid rewriting existing code
    //but also because we should get better text hinting for screen display I think.
    //The scaler object sets the scale to the new value until it is destroyed
    //when this function exits.
    Scaler scaler( m_dc, 1.0, 1.0 );

    //Also move the origin so the text region buts up to the dc top, not the bottom
    OriginChanger originChanger( m_dc, 0, wxCoord( m_height - m_plplotEdgeLength / m_yScale ) );

    // Check that we got unicode, warning message and return if not
    if ( args->unicode_array_len == 0 )
    {
        printf( "Non unicode string passed to the wxWidgets driver, ignoring\n" );
        return;
    }

    // Check that unicode string isn't longer then the max we allow
    if ( args->unicode_array_len >= 500 )
    {
        printf( "Sorry, the wxWidgets drivers only handles strings of length < %d\n", 500 );
        return;
    }

    // Calculate the font size (in pt)
    // Plplot saves it in mm (bizarre units!)
    PLFLT baseFontSize = pls->chrht * 72.0 / 25.4;

    // Use PLplot core routine to get the corners of the clipping rectangle
    PLINT rcx[4], rcy[4];
    difilt_clip( rcx, rcy );

    wxPoint cpoints[4];
    for ( int i = 0; i < 4; i++ )
    {
        cpoints[i].x = rcx[i] / m_xScale;
        cpoints[i].y = m_height - rcy[i] / m_yScale;
    }

    Clipper   clipper( m_dc, wxRegion( 4, cpoints ).GetBox() );

    PLUNICODE *lineStart     = args->unicode_array;
    int       lineLen        = 0;
    bool      lineFeed       = false;
    bool      carriageReturn = false;
    wxCoord   paraHeight     = 0;
    // Get the curent font
    PLINT     superscriptLevel = 0;
    bool      underlined       = 0;
    plgfci( &m_fci );
    //set the font up, we use a textObjectChanger here so that the font returns
    //to its original value on exit
    TextObjectsChanger textObjectsChanger( m_dc, m_fontGrabber, m_fci, baseFontSize, underlined,
                                           wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a * 255 ),
                                           wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a * 255 ) );
    bool lastFontWasCached = m_fontGrabber.lastWasCached();

    //draw each line of text individually and record the width of the paragraph
    wxCoord paragraphWidth = 0;
    while ( lineStart != args->unicode_array + args->unicode_array_len )
    {
        //remember the text parameters so they can be restored
        double    lineStartSuperscriptLevel = superscriptLevel;
        PLUNICODE lineStartFci        = m_fci;
        bool      lineStartUnderlined = underlined;


        while ( lineStart + lineLen != args->unicode_array + args->unicode_array_len
                && *( lineStart + lineLen ) != (PLUNICODE) '\n' )
        {
            lineLen++;
        }

        //set line feed for the beginning of this line and
        //carriage return for the end
        lineFeed       = carriageReturn;
        carriageReturn = lineStart + lineLen != args->unicode_array + args->unicode_array_len
                         && *( lineStart + lineLen ) == (PLUNICODE) ( '\n' );
        if ( lineFeed )
            paraHeight += m_textHeight + m_subscriptDepth;


        // determine extent of text
        m_posX = args->x / m_xScale;
        m_posY = args->y / m_yScale;

        if ( args->unicode_array_len == 1
             && args->unicode_array[0] == m_prevSingleCharString
             && lastFontWasCached )
        {
            m_textWidth  = m_prevSingleCharStringWidth;
            m_textHeight = m_prevSingleCharStringHeight;
        }
        else
        {
            DrawTextLine( lineStart, lineLen, baseFontSize, false, superscriptLevel, underlined );
        }
        paragraphWidth = MAX( paragraphWidth, m_textWidth );

        if ( lineFeed && m_superscriptHeight > m_textHeight )
            paraHeight += m_superscriptHeight - m_textHeight;

        // actually draw text, resetting the font first
        if ( !pls->get_string_length && m_dc )
        {
            superscriptLevel = lineStartSuperscriptLevel;
            m_fci            = lineStartFci;
            underlined       = lineStartUnderlined;
            m_dc->SetFont( m_fontGrabber.GetFont( m_fci, pow( 0.8, abs( superscriptLevel ) ) * baseFontSize, underlined ).getWxFont() );


            // calculate rotation of text
            PLFLT shear;
            PLFLT stride;
            plRotationShear( args->xform, &m_rotation, &shear, &stride );
            m_rotation -= pls->diorot * M_PI / 2.0;
            PLFLT cos_rot   = cos( m_rotation );
            PLFLT sin_rot   = sin( m_rotation );
            PLFLT cos_shear = cos( shear );
            PLFLT sin_shear = sin( shear );

            //Set the transform if possible and draw the text
            if ( m_gc )
            {
                wxGraphicsMatrix originalMatrix = m_gc->GetTransform();

                m_gc->Translate( args->x / m_xScale, m_height - args->y / m_yScale );             //move to text starting position
                wxGraphicsMatrix matrix = m_gc->CreateMatrix(
                    cos_rot * stride, -sin_rot * stride,
                    cos_rot * sin_shear + sin_rot * cos_shear,
                    -sin_rot * sin_shear + cos_rot * cos_shear,
                    0.0, 0.0 );                                                                                 //create rotation transformation matrix
                m_gc->ConcatTransform( matrix );                                                                //rotate
                m_gc->Translate( -args->just * m_textWidth, -0.5 * m_textHeight + paraHeight * m_lineSpacing ); //move to set alignment

                DrawTextLine( lineStart, lineLen, baseFontSize, true, superscriptLevel, underlined );
                m_gc->SetTransform( originalMatrix );
            }
#if wxVERSION_NUMBER >= 2902
            else if ( m_useDcTextTransform )
            {
                wxAffineMatrix2D originalMatrix = m_dc->GetTransformMatrix();

                wxAffineMatrix2D newMatrix = originalMatrix;
                newMatrix.Translate( args->x / m_xScale, m_height - args->y / m_yScale );
                wxAffineMatrix2D textMatrix;
                textMatrix.Set( wxMatrix2D( cos_rot * stride, -sin_rot * stride,
                        cos_rot * sin_shear + sin_rot * cos_shear,
                        -sin_rot * sin_shear + cos_rot * cos_shear ),
                    wxPoint2DDouble( 0.0, 0.0 ) );
                newMatrix.Concat( textMatrix );
                newMatrix.Translate( -args->just * m_textWidth, -0.5 * m_textHeight + paraHeight * m_lineSpacing );

                m_dc->SetTransformMatrix( newMatrix );
                DrawTextLine( lineStart, lineLen, baseFontSize, true, superscriptLevel, underlined );
                m_dc->SetTransformMatrix( originalMatrix );
            }
#endif
            else
            {
                m_posX = (PLINT) ( args->x / m_xScale - ( args->just * m_textWidth ) * cos_rot - ( 0.5 * m_textHeight - paraHeight * m_lineSpacing ) * sin_rot );                             //move to set alignment
                m_posY = (PLINT) ( args->y / m_yScale - ( args->just * m_textWidth ) * sin_rot + ( 0.5 * m_textHeight - paraHeight * m_lineSpacing ) * cos_rot );
                DrawTextLine( lineStart, lineLen, baseFontSize, true, superscriptLevel, underlined );
            }
        }

        lineStart += lineLen;
        if ( carriageReturn )
            lineStart++;
        lineLen = 0;
    }

    //set the size of the string in mm
    if ( pls->get_string_length )
        pls->string_length = paragraphWidth * m_xScale / pls->xpmm;

    //save the width if we only had one character
    if ( args->unicode_array_len == 1 )
    {
        m_prevSingleCharStringWidth  = paragraphWidth;
        m_prevSingleCharStringHeight = m_textHeight;
        m_prevSingleCharString       = args->unicode_array[0];
    }
}

//--------------------------------------------------------------------------
//  void wxPLDevice::EndPage( PLStream* pls )
//  End the page. This is the point that we write the buffer to the memory
//  mapped file if needed
//--------------------------------------------------------------------------
void wxPLDevice::EndPage( PLStream* pls )
{
    if ( !m_dc )
    {
        if ( pls->nopause )
            TransmitBuffer( pls, transmissionEndOfPageNoPause );
        else
            TransmitBuffer( pls, transmissionEndOfPage );
        return;
    }
}

//--------------------------------------------------------------------------
//  void wxPLDevice::BeginPage( PLStream* pls )
//  Sets up for transfer in case it is needed and sets the current state
//--------------------------------------------------------------------------
void wxPLDevice::BeginPage( PLStream* pls )
{
    if ( !m_dc )
    {
        m_localBufferPosition = 0;
        TransmitBuffer( NULL, transmissionBeginPage );
    }

    // Get the starting colour, width and font from the stream
    SetWidth( pls );
    SetColor( pls );

    //clear the page
    ClearBackground( pls );
}

//--------------------------------------------------------------------------
//  void wxPLDevice::SetSize( PLStream* pls )
//  Set the size of the page, scale parameters and the dpi
//--------------------------------------------------------------------------
void wxPLDevice::SetSize( PLStream* pls, int width, int height )
{
    //we call BeginPage, before we fiddle with fixed aspect so that the
    //whole background gets filled
    // get boundary coordinates in plplot units
    PLINT xmin;
    PLINT xmax;
    PLINT ymin;
    PLINT ymax;
    plP_gphy( &xmin, &xmax, &ymin, &ymax );
    //split the scaling into an overall scale, the same in both dimensions
    //and an aspect part which differs in both directions.
    //We will apply the aspect ratio part, and let the DC do the overall
    //scaling. This gives us subpixel accuray, but ensures line thickness
    //remains consistent in both directions
    m_xScale = width > 0 ? (PLFLT) ( xmax - xmin ) / (PLFLT) width : 0.0;
    m_yScale = height > 0 ? (PLFLT) ( ymax - ymin ) / (PLFLT) height : 0.0;
    m_scale  = MIN( m_xScale, m_yScale );

    if ( !m_fixedAspect )
    {
        m_xAspect = m_scale / m_xScale;
        m_yAspect = m_scale / m_yScale;
    }
    else
    {
        //now sort out the fixed aspect and reset the logical scale if needed
        if ( PLFLT( height ) / PLFLT( width ) > m_yAspect / m_xAspect )
        {
            m_scale  = m_xScale * m_xAspect;
            m_yScale = m_scale / m_yAspect;
        }
        else
        {
            m_scale  = m_yScale * m_yAspect;
            m_xScale = m_scale / m_xAspect;
        }
    }

    m_width      = ( xmax - xmin ) / m_xScale;
    pls->xlength = PLINT( m_width + 0.5 );
    m_height     = ( ymax - ymin ) / m_yScale;
    pls->ylength = PLINT( m_height + 0.5 );

    // Set the number of plplot pixels per mm
    plP_setpxl( m_plplotEdgeLength / m_width * pls->xdpi / 25.4, m_plplotEdgeLength / m_height * pls->ydpi / 25.4 );

    pls->aspect = m_xAspect / m_yAspect;

    // redraw the plot
    if ( m_dc && pls->plbuf_buffer )
        plreplot();
}


void wxPLDevice::FixAspectRatio( bool fix )
{
    m_fixedAspect = fix;
}

void wxPLDevice::Flush( PLStream *pls )
{
    if ( !m_dc )
        TransmitBuffer( pls, transmissionComplete );
}

//This function transmits the remaining buffer to the gui program via a memory map
//If the buffer is too big for the memory map then it will loop round waiting for
//the gui to catch up each time
// note that this function can be called with pls set to NULL for transmissions
//of jsut a flag for e.g. page end or begin
void wxPLDevice::TransmitBuffer( PLStream* pls, unsigned char transmissionType )
{
    if ( !m_outputMemoryMap.isValid() )
        return;
    size_t       amountToCopy = pls ? pls->plbuf_top - m_localBufferPosition : 0;
    bool         first        = true;
    size_t       counter      = 0;
    const size_t counterLimit = 10000;
    const size_t headerSize   = sizeof ( transmissionType ) + sizeof ( size_t );
    bool         completed    = false;
    while ( !completed && counter < counterLimit )
    {
        //if we are doing multiple loops then pause briefly before we
        //lock to give the reading application a chance to spot the
        //change.
        if ( !first )
            wxMilliSleep( 10 );
        first = false;


        size_t copyAmount = 0;
        size_t freeSpace  = 0;
        //lock the mutex so reading and writing don't overlap
        try
        {
            PLNamedMutexLocker lock( &m_mutex );
            MemoryMapHeader &  mapHeader = *(MemoryMapHeader *) m_outputMemoryMap.getBuffer();

            //check how much free space we have before the end of the buffer
            //or if we have looped round how much free space we have before
            //we reach the read point
            freeSpace = m_outputMemoryMap.getSize() - mapHeader.writeLocation;
            // if readLocation is at the beginning then don't quite fill up the buffer
            if ( mapHeader.readLocation == plMemoryMapReservedSpace )
                --freeSpace;

            //if the free space left in the file is less than that needed for the header then
            //just tell the GUI to skip the rest of the file so it can start again at the
            //beginning of the file.
            if ( freeSpace <= headerSize )
            {
                if ( mapHeader.readLocation > mapHeader.writeLocation )                //don't overtake the read buffer
                    freeSpace = 0;
                else if ( mapHeader.readLocation == plMemoryMapReservedSpace )         // don't catch up exactly with the read buffer
                    freeSpace = 0;
                else
                {
                    //send a skip end of file command and move back to the beginning of the file
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                        (void *) ( &transmissionSkipFileEnd ), sizeof ( transmissionSkipFileEnd ) );
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                    counter = 0;
                    plwarn( "wxWidgets wrapping buffer" );
                    continue;
                }
            }

            //if this is a beginning of page  then send a beginning of page flag first
            if ( transmissionType == transmissionBeginPage )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionBeginPage ), sizeof ( transmissionBeginPage ) );
                mapHeader.writeLocation += sizeof ( transmissionBeginPage );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                counter = 0;
                if ( amountToCopy == 0 )
                    completed = true;
                transmissionType = transmissionRegular;
                continue;
            }

            //if this is a end of page and we have completed
            //the buffer then send a end of page flag first
            if ( ( transmissionType == transmissionEndOfPage || transmissionType == transmissionEndOfPageNoPause )
                 && amountToCopy == 0 )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionType ), sizeof ( transmissionType ) );
                mapHeader.writeLocation += sizeof ( transmissionType );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                counter   = 0;
                completed = true;
                continue;
            }

            if ( transmissionType == transmissionLocate && amountToCopy == 0 )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionLocate ), sizeof ( transmissionLocate ) );
                mapHeader.writeLocation += sizeof ( transmissionLocate );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                mapHeader.locateModeFlag = 1;
                counter   = 0;
                completed = true;
                continue;
            }

            if ( transmissionType == transmissionRequestTextSize )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionRequestTextSize ), sizeof ( transmissionRequestTextSize ) );
                mapHeader.writeLocation += sizeof ( transmissionRequestTextSize );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                counter   = 0;
                completed = true;
                continue;
            }
            if ( transmissionType == transmissionClose )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionType ), sizeof ( transmissionType ) );
                mapHeader.writeLocation += sizeof ( transmissionType );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                counter   = 0;
                completed = true;
                continue;
            }

            //if we have looped round stay 1 character behind the read buffer - it makes it
            //easier to test whether the reading has caught up with the writing or vica versa
            if ( mapHeader.writeLocation < mapHeader.readLocation && mapHeader.readLocation > 0 )
                freeSpace = mapHeader.readLocation - mapHeader.writeLocation - 1;

            if ( freeSpace > headerSize )
            {
                //decide exactly how much to copy
                copyAmount = MIN( amountToCopy, freeSpace - headerSize );

                //copy the header and the amount we can to the buffer
                if ( copyAmount != amountToCopy )
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                        (char *) ( &transmissionPartial ), sizeof ( transmissionPartial ) );
                else
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                        (char *) ( &transmissionComplete ), sizeof ( transmissionComplete ) );
                if ( pls )
                {
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation + sizeof ( transmissionComplete ),
                        (char *) ( &copyAmount ), sizeof ( copyAmount ) );
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation + headerSize,
                        (char *) pls->plbuf_buffer + m_localBufferPosition, copyAmount );
                    m_localBufferPosition   += copyAmount;
                    mapHeader.writeLocation += copyAmount + headerSize;
                    if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                        mapHeader.writeLocation = plMemoryMapReservedSpace;
                    amountToCopy -= copyAmount;
                    counter       = 0;
                }
                if ( amountToCopy == 0 && transmissionType != transmissionEndOfPage
                     && transmissionType != transmissionLocate
                     && transmissionType != transmissionEndOfPageNoPause )
                    completed = true;
            }
            else
            {
                ++counter;
            }
        }
#ifdef WIN32
        catch ( DWORD )
        {
            plwarn( "Locking mutex failed when trying to communicate with wxPLViewer." );
            break;
        }
#endif
        catch ( ... )
        {
            plwarn( "Unknown error when trying to communicate with wxPLViewer." );
            break;
        }
    }
    if ( counter == counterLimit )
    {
        plwarn( "Communication timeout with wxPLViewer - disconnecting" );
        m_outputMemoryMap.close();
    }
}

void wxPLDevice::SetupMemoryMap()
{
    if ( strlen( m_mfo ) > 0 )
    {
        const size_t mapSize = 1024 * 1024;
        //create a memory map to hold the data and add it to the array of maps
        int          nTries = 0;
        char         mapName[PLPLOT_MAX_PATH];
        char         mutexName[PLPLOT_MAX_PATH];
        static Rand  randomGenerator;         // make this static so that rapid repeat calls don't use the same seed
        while ( nTries < 10 )
        {
            for ( int i = 0; i < strlen( m_mfo ); ++i )
            {
                if ( m_mfo[i] == '?' )
                    mapName[i] = 'A' + (char) ( randomGenerator() % 26 );
                else
                    mapName[i] = m_mfo[i];
            }
            mapName[strlen( m_mfo )] = '\0';
            //truncate it earlier if needed
            if ( strlen( m_mfo ) > PLPLOT_MAX_PATH - 4 )
                mapName[PLPLOT_MAX_PATH - 4] = '\0';
            pldebug( "wxPLDevice::SetupMemoryMap", "nTries = %d, mapName = %s\n", nTries, mapName );
            strcpy( mutexName, mapName );
            strcat( mutexName, "mut" );
            pldebug( "wxPLDevice::SetupMemoryMap", "nTries = %d, mutexName = %s\n", nTries, mutexName );
            m_outputMemoryMap.create( mapName, mapSize, false, true );
            if ( m_outputMemoryMap.isValid() )
                m_mutex.create( mutexName );
            if ( !m_mutex.isValid() )
                m_outputMemoryMap.close();
            if ( m_outputMemoryMap.isValid() )
                break;
            ++nTries;
        }
        //m_outputMemoryMap.create( m_mfo, pls->plbuf_top, false, true );
        //check the memory map is valid
        if ( !m_outputMemoryMap.isValid() )
        {
            plwarn( "Error creating memory map for wxWidget instruction transmission. The plots will not be displayed" );
            return;
        }

        //zero out the reserved area
        MemoryMapHeader *header = (MemoryMapHeader *) ( m_outputMemoryMap.getBuffer() );
        header->readLocation   = plMemoryMapReservedSpace;
        header->writeLocation  = plMemoryMapReservedSpace;
        header->viewerOpenFlag = 0;
        header->locateModeFlag = 0;
        header->completeFlag   = 0;

        //try to find the wxPLViewer executable, in the first instance just assume it
        //is in the path.
        //wxString exeName = wxT( "/nfs/see-fs-02_users/earpros/usr/src/plplot-plplot/build/utils/wxPLViewer" );
        wxString exeName = wxT( "wxPLViewer" );
        if ( plInBuildTree() )
        {
            //if we are in the build tree check for the needed exe in there
            wxArrayString files;
            wxString      utilsDir = wxString( wxT( BUILD_DIR ) ) + wxString( wxT( "/utils" ) );
            wxDir::GetAllFiles( utilsDir, &files, exeName, wxDIR_FILES | wxDIR_DIRS );
            if ( files.size() == 0 )
                wxDir::GetAllFiles( utilsDir, &files, exeName + wxT( ".exe" ), wxDIR_FILES | wxDIR_DIRS );
            if ( files.size() > 0 )
                exeName = files[0];
        }
        else
        {
            //check the plplot bin install directory
            wxArrayString files;
            wxDir::GetAllFiles( wxT( BIN_DIR ), &files, exeName, wxDIR_FILES | wxDIR_DIRS );
            if ( files.size() == 0 )
                wxDir::GetAllFiles( wxT( BIN_DIR ), &files, exeName + wxT( ".exe" ), wxDIR_FILES | wxDIR_DIRS );
            if ( files.size() > 0 )
                exeName = files[0];
        }
        //Run the wxPlViewer with command line parameters telling it the location and size of the buffer
        wxString command;
        command << wxT( "\"" ) << exeName << wxT( "\" " ) << wxString( mapName, wxConvUTF8 ) << wxT( " " ) <<
            mapSize << wxT( " " ) << m_width << wxT( " " ) << m_height;
#ifndef WXPLVIEWER_DEBUG
#ifdef WIN32

        if ( wxExecute( command, wxEXEC_ASYNC ) == 0 )
            plwarn( "Failed to run wxPLViewer - no plots will be shown" );
#else   //WIN32
        //Linux doesn't like using wxExecute without a wxApp, so use system instead
        command << wxT( " &" );
        system( command.mb_str() );
#endif  //WIN32
        size_t   maxTries = 1000;
#else //WXPLVIEWER_DEBUG
        wxString runMessage;
        runMessage << "Begin Running wxPLViewer in the debugger now to continue. Use the parameters: plplotMemoryMap " <<
            mapSize << " " << m_width << " " << m_height;
        fprintf( stdout, runMessage );
        size_t maxTries = 100000;
#endif  //WXPLVIEWER_DEBUG
        //wait until the viewer signals it has opened the map file
        size_t  counter      = 0;
        size_t &viewerSignal = header->viewerOpenFlag;
        while ( counter < maxTries && viewerSignal == 0 )
        {
            wxMilliSleep( 10 );
            ++counter;
        }
        if ( viewerSignal == 0 )
            plwarn( "wxPLViewer failed to signal it has found the shared memory." );
    }
}

void wxPLDevice::Locate( PLStream* pls, PLGraphicsIn *graphicsIn )
{
    if ( !m_dc && m_outputMemoryMap.isValid() )
    {
        MemoryMapHeader *header = (MemoryMapHeader *) ( m_outputMemoryMap.getBuffer() );
        TransmitBuffer( pls, transmissionLocate );
        bool            gotResponse = false;
        while ( !gotResponse )
        {
            wxMilliSleep( 100 );
            PLNamedMutexLocker lock( &m_mutex );
            gotResponse = header->locateModeFlag == 0;
        }

        PLNamedMutexLocker lock( &m_mutex );
        *graphicsIn = header->graphicsIn;
    }
    else
    {
        plwarn( "plGetCursor cannot be used when the user supplies a wxDC or until wxPLViewer is initialised" );
        graphicsIn->dX = -1;
        graphicsIn->dY = -1;
        graphicsIn->pX = -1;
        graphicsIn->pY = -1;
    }
}

//--------------------------------------------------------------------------
// wxRegion wxPLDevice::GetClipRegion()
// Gets the current clip region from plplot as a wxRegion
//--------------------------------------------------------------------------

wxRegion wxPLDevice::GetClipRegion()
{
    PLINT rcx[4], rcy[4];
    difilt_clip( rcx, rcy );

    wxPoint cpoints[4];
    for ( int i = 0; i < 4; i++ )
    {
        cpoints[i].x = rcx[i] / m_xScale;
        cpoints[i].y = m_height - rcy[i] / m_yScale;
    }
    return wxRegion( 4, cpoints );
}
