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

//plplot headers
#include "plDevs.h"
#include "wxwidgets.h" // includes wx/wx.h

// wxwidgets headers
#include <wx/dir.h>



// std and driver headers
#include <cmath>


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
        dc->GetLogicalScale( &m_xScaleOld, &m_yScaleOld );
        dc->SetLogicalScale( xScale, yScale );
    }
    ~Scaler( )
    {
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
    OriginChanger( wxDC * dc, long xOrigin, long yOrigin )
    {
        m_dc = dc;
        dc->GetLogicalOrigin( &m_xOriginOld, &m_yOriginOld );
        dc->SetLogicalOrigin( xOrigin, yOrigin );
    }
    ~OriginChanger( )
    {
        m_dc->SetLogicalOrigin( m_xOriginOld, m_yOriginOld );
    }
private:
    wxDC *m_dc;
    long m_xOriginOld;
    long m_yOriginOld;
};

//--------------------------------------------------------------------------
//  wxPLDevice::wxPLDevice( void )
//
//  Constructor of the standard wxWidgets device based on the wxPLDevBase
//  class. Only some initialisations are done.
//--------------------------------------------------------------------------
wxPLDevice::wxPLDevice( PLStream *pls, char * mfo, PLINT text, PLINT hrshsym )
    : m_plplotEdgeLength( PLFLT( SHRT_MAX ) )
{
    m_plstate_width = false;
    m_plstate_color = false;

    m_fixedAspect = false;
    //locate_mode = 0;
    //draw_xhair  = false;

    m_font        = NULL;
    m_lineSpacing = 1.0;
    m_underlined  = false;

    m_dc = NULL;

    if ( mfo )
        strcpy( m_mfo, mfo );
    else
        //assume we will be outputting to the default
        //memory map until we are given a dc to draw to
        //strcpy(m_mfo, "plplotMemoryMap");
        strcpy( m_mfo, "plplotMemoryMap??????????" );

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
    pls->dev = (void *) this;


    // Set up physical limits of plotting device in plplot internal units
    // we just tell plplot we are the maximum plint in both dimensions
    //which gives us the best resolution
    plP_setphy( (PLINT) 0, (PLINT) SHRT_MAX,
        (PLINT) 0, (PLINT) SHRT_MAX );

    // set dpi and page size defaults
    // the user might have already set this with plspage
    // so only override zeros
    if ( pls->xdpi == 0 && pls->ydpi == 0 )
    {
        pls->xdpi = 90;
        pls->ydpi = 90;
    }
    else if ( pls->xdpi == 0 )
        pls->xdpi = pls->ydpi;
    else if ( pls->ydpi == 0 )
        pls->ydpi = pls->xdpi;

    PLINT width  = pls->xlength;
    PLINT height = pls->ylength;
    if ( width == 0 && height == 0 )
    {
        width  = 900;
        height = 675;
    }
    else if ( width == 0 )
        width = height;
    else if ( height == 0 )
        height = width;

    SetSize( pls, width, height );

    m_localBufferPosition = 0;
    m_begunRendering      = false;
}


//--------------------------------------------------------------------------
//  wxPLDevice::~wxPLDevice( void )
//
//  The deconstructor frees memory allocated by the device.
//--------------------------------------------------------------------------
wxPLDevice::~wxPLDevice()
{
    if ( m_font )
        delete m_font;
}


//--------------------------------------------------------------------------
//  void wxPLDevice::DrawLine( short x1a, short y1a, short x2a, short y2a )
//
//  Draw a line from (x1a, y1a) to (x2a, y2a).
//--------------------------------------------------------------------------
void wxPLDevice::DrawLine( short x1a, short y1a, short x2a, short y2a )
{
    if ( !m_dc )
    {
        if ( !m_begunRendering )
        {
            //create the memory map for external communication
            SetupMemoryMap();
            m_begunRendering = true;
        }
        return;
    }

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
    {
        if ( !m_begunRendering )
        {
            //create the memory map for external communication
            SetupMemoryMap();
            m_begunRendering = true;
        }
        return;
    }

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

    PLINT bgr, bgg, bgb;                  // red, green, blue
    plgcolbg( &bgr, &bgg, &bgb );         // get background color information

    x1 = x1 < 0 ? 0 : x1;
    x2 = x2 < 0 ? m_plplotEdgeLength : x2;
    y1 = y1 < 0 ? 0 : y1;
    y2 = y2 < 0 ? m_plplotEdgeLength : y2;

    PLINT         x      = MIN( x1, x2 );
    PLINT         y      = MIN( y1, y2 );
    PLINT         width  = abs( x1 - x2 );
    PLINT         height = abs( y1 - y2 );

    const wxPen   oldPen   = m_dc->GetPen();
    const wxBrush oldBrush = m_dc->GetBrush();

    m_dc->SetPen( wxNullPen );

    m_dc->SetBrush( wxBrush( wxColour( bgr, bgg, bgb ) ) );
    m_dc->DrawRectangle( x, y, width, height );

    m_dc->SetPen( oldPen );
    m_dc->SetBrush( oldBrush );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::FillPolygon( PLStream *pls )
//
//  Draw a filled polygon.
//--------------------------------------------------------------------------
void wxPLDevice::FillPolygon( PLStream *pls )
{
    if ( !m_dc )
    {
        if ( !m_begunRendering )
        {
            //create the memory map for external communication
            SetupMemoryMap();
            m_begunRendering = true;
        }
        return;
    }

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
    //Note that calls to this function before we have a DC should be honoured
    //Save a flag to grab the value from the PLStream after creation.
    if ( !m_dc )
    {
        m_plstate_width = true;
        return;
    }
    PLFLT width = ( pls->width > 0.0 ? pls->width : 1.0 ) * m_scale;
    m_dc->SetPen( *( wxThePenList->FindOrCreatePen( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b ),
                         width, wxSOLID ) ) );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetColor0( PLStream *pls )
//
//  Set color from colormap 0.
//--------------------------------------------------------------------------
void wxPLDevice::SetColor( PLStream *pls )
{
    //Note that calls to this function before we have a DC should be honoured
    //Save a flag to grab the value from the PLStream after creation.
    if ( !m_dc )
    {
        m_plstate_color = true;
        return;
    }
    PLFLT width = ( pls->width > 0.0 ? pls->width : 1.0 ) * m_scale;
    m_dc->SetPen( *( wxThePenList->FindOrCreatePen( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a * 255 ),
                         width, wxSOLID ) ) );
    m_dc->SetBrush( wxBrush( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a * 255 ) ) );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetExternalBuffer( void* dc )
//
//  Adds a dc to the device. In that case, the drivers doesn't provide
//  a GUI.
//--------------------------------------------------------------------------
void wxPLDevice::SetExternalBuffer( PLStream *pls, void* dc )
{
    m_dc             = (wxDC *) dc; // Add the dc to the device
    m_useDcTransform = false;
    m_gc             = NULL;
    if ( m_dc )
    {
#if wxVERSION_NUMBER >= 2902
        m_useDcTransform = m_dc->CanUseTransformMatrix();
#endif
        //If we don't have wxDC tranforms we can use the
        //underlying wxGCDC if this is a wxGCDC
        if ( !m_useDcTransform )
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
        SetSize( pls, m_width, m_height );         //call with our current size to set the scaling
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
void wxPLDevice::DrawText( PLUNICODE* ucs4, int ucs4Len, bool drawText )
{
    if ( !m_dc )
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
                    DrawTextSection( utf8_string, drawText );

                    // change font scale
                    if ( m_yOffset < -0.0001 )
                        m_fontScale *= 1.25; // Subscript scaling parameter
                    else
                        m_fontScale *= 0.8;  // Subscript scaling parameter
                    SetFont( m_fci );

                    m_yOffset += m_yScale * m_fontSize * m_fontScale / 2.;
                }
                if ( ucs4[i] == (PLUNICODE) 'd' ) // Subscript
                {                                 // draw string so far
                    DrawTextSection( utf8_string, drawText );

                    // change font scale
                    double old_fontScale = m_fontScale;
                    if ( m_yOffset > 0.0001 )
                        m_fontScale *= 1.25; // Subscript scaling parameter
                    else
                        m_fontScale *= 0.8;  // Subscript scaling parameter
                    SetFont( m_fci );

                    m_yOffset -= m_yScale * m_fontSize * old_fontScale / 2.;
                }
                if ( ucs4[i] == (PLUNICODE) '-' ) // underline
                {                                 // draw string so far
                    DrawTextSection( utf8_string, drawText );

                    m_underlined = !m_underlined;
                    SetFont( m_fci );
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
            DrawTextSection( utf8_string, drawText );

            // get new font
            m_fci = ucs4[i];
            SetFont( m_fci );
            i++;
        }
    }

    DrawTextSection( utf8_string, drawText );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::DrawTextSection( char* utf8_string, bool drawText )
//
//  Draw a section of text. This should already have been processed and
//  split into sections so that the text passed in has no style changes or
//  newines.
//--------------------------------------------------------------------------
void wxPLDevice::DrawTextSection( char* utf8_string, bool drawText )
{
    if ( !m_dc )
        return;

    wxCoord  w, h, d, l;

    wxString str( wxConvUTF8.cMB2WC( utf8_string ), *wxConvCurrent );

    m_dc->GetTextExtent( str, &w, &h, &d, &l );

    if ( drawText )
    {
        //if we are using wxDC transforms or the wxGC, then the transformations
        //have already been applied
        if ( m_gc )
            m_gc->DrawText( str, m_textWidth, 0.0 );
        else if ( m_useDcTransform )
            m_dc->DrawText( str, m_textWidth, 0 );
        else
        {
            //If we are stuck with a wxDC that has no transformation abilities then
            // all we can really do is rotate the text - this is a bit of a poor state
            // really, but to be honest it is better than defaulting to hershey for all
            // text
            if ( m_rotation == 0 )
                m_dc->DrawRotatedText( str, (wxCoord) ( m_posX + m_textWidth ),
                    (wxCoord) ( m_height - (wxCoord) ( m_posY + m_yOffset / m_yScale ) ),
                    m_rotation * 180.0 / M_PI );
            else
                m_dc->DrawRotatedText( str,
                    (wxCoord) ( m_posX - m_yOffset / m_yScale * sin( m_rotation ) + m_textWidth * cos( m_rotation ) ),
                    (wxCoord) ( m_height - (wxCoord) ( m_posY + m_yOffset * cos( m_rotation ) / m_yScale ) - m_textWidth * sin( m_rotation ) ),
                    m_rotation * 180.0 / M_PI );
        }
    }

    m_textWidth += w;

    //keep track of the height of superscript text, the depth of subscript
    //text and the height of regular text
    if ( m_yOffset > 0.0001 )
    {
        //determine the height the text would have if it were full size
        double currentOffset = m_yOffset;
        double currentHeight = h;
        while ( currentOffset > 0.0001 )
        {
            currentOffset -= m_yScale * m_fontSize * m_fontScale / 2.;
            currentHeight *= 1.25;
        }
        m_textHeight = (wxCoord) m_textHeight > ( currentHeight )
                       ? m_textHeight
                       : currentHeight;
        //work out the height including superscript
        m_superscriptHeight = m_superscriptHeight > ( currentHeight + m_yOffset / m_yScale )
                              ? m_superscriptHeight
                              : static_cast<int>( ( currentHeight + m_yOffset / m_yScale ) );
    }
    else if ( m_yOffset < -0.0001 )
    {
        //determine the height the text would have if it were full size
        double currentOffset = m_yOffset;
        double currentHeight = h;
        double currentDepth  = d;
        while ( currentOffset < -0.0001 )
        {
            currentOffset += m_yScale * m_fontSize * m_fontScale * 1.25 / 2.;
            currentHeight *= 1.25;
            currentDepth  *= 1.25;
        }
        m_textHeight = (wxCoord) m_textHeight > currentHeight ? m_textHeight : currentHeight;
        //work out the additional depth for subscript note an assumption has been made
        //that the font size of (non-superscript and non-subscript) text is the same
        //along a line. Currently there is no escape to change font size mid string
        //so this should be fine
        m_subscriptDepth = (wxCoord) m_subscriptDepth > ( ( -m_yOffset / m_yScale + h + d ) - ( currentDepth + m_textHeight ) )
                           ? m_subscriptDepth
                           : ( ( -m_yOffset / m_yScale + h + d ) - ( currentDepth + m_textHeight ) );
        m_subscriptDepth = m_subscriptDepth > 0 ? m_subscriptDepth : 0;
    }
    else
        m_textHeight = (wxCoord) m_textHeight > ( h ) ? m_textHeight : h;

    memset( utf8_string, '\0', m_max_string_length );
}


//--------------------------------------------------------------------------
//  void wxPLDevice::SetFont( PLUNICODE fci )
//
//  Set font defined by fci.
//--------------------------------------------------------------------------
void wxPLDevice::SetFont( PLUNICODE fci )
{
    if ( !m_dc )
        return;

    unsigned char fontFamily, fontStyle, fontWeight;

    plP_fci2hex( fci, &fontFamily, PL_FCI_FAMILY );
    plP_fci2hex( fci, &fontStyle, PL_FCI_STYLE );
    plP_fci2hex( fci, &fontWeight, PL_FCI_WEIGHT );

    if ( m_font )
        delete m_font;

    //create the font based on point size ( with 1 point = 1/72 inch )
    m_font = wxFont::New( (int) ( m_fontSize * m_fontScale < 4 ? 4 : m_fontSize * m_fontScale ),
        fontFamilyLookup[fontFamily],
        fontStyleLookup[fontStyle] | fontWeightLookup[fontWeight] );
    m_font->SetUnderlined( m_underlined );
    m_dc->SetFont( *m_font );
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
    if ( !m_dc )
    {
        if ( !m_begunRendering )
        {
            //create the memory map for external communication
            SetupMemoryMap();
            m_begunRendering = true;
        }
        return;
    }

    //for text, work in native coordinates, partly to avoid rewriting existing code
    //but also because we should get better text hinting for screen display I think.
    //The scaler object sets the scale to the new value until it is destroyed
    //when this function exits.
    Scaler scaler( m_dc, 1.0, 1.0 );

    //Also move the origin so the text region buts up to the dc top, not the bottom
    OriginChanger originChanger( m_dc, 0, m_height - m_plplotEdgeLength / m_yScale );

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
    m_fontSize = pls->chrht * 72.0 / 25.4;

    // Use PLplot core routine to get the corners of the clipping rectangle
    PLINT rcx[4], rcy[4];
    difilt_clip( rcx, rcy );

    wxPoint cpoints[4];
    for ( int i = 0; i < 4; i++ )
    {
        cpoints[i].x = rcx[i] / m_xScale;
        cpoints[i].y = m_height - rcy[i] / m_yScale;
    }
    wxDCClipper clip( *m_dc, wxRegion( 4, cpoints ) );

    // Set font color
    m_dc->SetTextForeground( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b ) );
    m_dc->SetTextBackground( wxColour( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b ) );

    PLUNICODE *lineStart     = args->unicode_array;
    int       lineLen        = 0;
    bool      lineFeed       = false;
    bool      carriageReturn = false;
    wxCoord   paraHeight     = 0;
    // Get the curent font
    m_fontScale = 1.0;
    m_yOffset   = 0.0;
    plgfci( &m_fci );
    SetFont( m_fci );
    while ( lineStart != args->unicode_array + args->unicode_array_len )
    {
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

        //remember the text parameters so they can be restored
        double    startingFontScale = m_fontScale;
        double    startingYOffset   = m_yOffset;
        PLUNICODE startingFci       = m_fci;

        // determine extent of text
        m_posX = args->x / m_xScale;
        m_posY = args->y / m_yScale;
        DrawText( lineStart, lineLen, false );

        if ( lineFeed && m_superscriptHeight > m_textHeight )
            paraHeight += m_superscriptHeight - m_textHeight;

        // actually draw text, resetting the font first
        m_fontScale = startingFontScale;
        m_yOffset   = startingYOffset;
        m_fci       = startingFci;
        SetFont( m_fci );


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

            DrawText( lineStart, lineLen, true );
            m_gc->SetTransform( originalMatrix );
        }
#if wxVERSION_NUMBER >= 2902
        else if ( m_useDcTransform )
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
            DrawText( lineStart, lineLen, true );
            m_dc->SetTransformMatrix( originalMatrix );
        }
#endif
        else
        {
            m_posX = (PLINT) ( args->x / m_xScale - ( args->just * m_textWidth ) * cos_rot - ( 0.5 * m_textHeight - paraHeight * m_lineSpacing ) * sin_rot );             //move to set alignment
            m_posY = (PLINT) ( args->y / m_yScale - ( args->just * m_textWidth ) * sin_rot + ( 0.5 * m_textHeight - paraHeight * m_lineSpacing ) * cos_rot );
            DrawText( lineStart, lineLen, true );
        }


        lineStart += lineLen;
        if ( carriageReturn )
            lineStart++;
        lineLen = 0;
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
        if ( !m_begunRendering )
        {
            //create the memory map for external communication
            SetupMemoryMap();
            m_begunRendering = true;
        }
        TransmitBuffer( pls, transmissionEndOfPage );
        return;
    }
}

//--------------------------------------------------------------------------
//  void wxPLDevice::BeginPage( PLStream* pls )
//  Clears the page and applys any state changes waiting to be applied
//--------------------------------------------------------------------------
void wxPLDevice::BeginPage( PLStream* pls )
{
    if ( !m_dc )
    {
        m_localBufferPosition = 0;
        TransmitBuffer( NULL, transmissionBeginPage );
        return;
    }

    ClearBackground( pls );

    // Replay escape calls that come in before PLESC_DEVINIT.  All of them
    // required a DC that didn't exist yet.
    //
    if ( m_plstate_width )
        SetWidth( pls );
    if ( m_plstate_color )
        SetColor( pls );

    m_plstate_width = false;
    m_plstate_color = false;
}

//--------------------------------------------------------------------------
//  void wxPLDevice::SetSize( PLStream* pls )
//  Set the size of the page, scale parameters and the dpi
//--------------------------------------------------------------------------
void wxPLDevice::SetSize( PLStream* pls, int width, int height )
{
    // set new size and scale parameters
    m_width  = pls->xlength = width;
    m_height = pls->ylength = height;

    // get boundary coordinates in plplot units
    PLINT xmin;
    PLINT xmax;
    PLINT ymin;
    PLINT ymax;
    plP_gphy( &xmin, &xmax, &ymin, &ymax );
    m_xScale = m_width > 0 ? (PLFLT) ( xmax - xmin ) / (PLFLT) width : 0.0;
    m_yScale = m_height > 0 ? (PLFLT) ( ymax - ymin ) / (PLFLT) height : 0.0;

    //split the scaling into an overall scale, the same in both dimensions
    //and an aspect part which differs in both directions.
    //We will apply the aspect ratio part, and let the DC do the overall
    //scaling. This gives us subpixel accuray, but ensures line thickness
    //remains consistent in both directions

    //Initially assume we have not got fixed aspect, until after we call clear.
    //This ensures the whole plot gets cleared. But save the original aspect
    PLFLT xAspectOriginal = m_xAspect;
    PLFLT yAspectOriginal = m_yAspect;
    m_scale   = MIN( m_xScale, m_yScale );
    m_xAspect = m_scale / m_xScale;
    m_yAspect = m_scale / m_yScale;

    // Set the number of plplot pixels per mm
    plP_setpxl( m_plplotEdgeLength / m_width * pls->xdpi / 25.4, m_plplotEdgeLength / m_height * pls->xdpi / 25.4 );

    //we call BeginPage, before we fiddle with fixed aspect so that the
    //whole background gets filled
    if ( m_dc )
    {
        //we use the dc to do the basic scaling as it then gives us subpixel accuracy
        m_dc->SetLogicalScale( 1.0 / m_scale, 1.0 / m_scale );
        BeginPage( pls );
    }

    //now sort out the fixed aspect and reset the logical scale if needed

    if ( m_fixedAspect )
    {
        m_xAspect = xAspectOriginal;
        m_yAspect = yAspectOriginal;
        if ( PLFLT( m_height ) / PLFLT( m_width ) > m_yAspect / m_xAspect )
        {
            m_scale  = m_xScale * m_xAspect;
            m_yScale = m_scale / m_yAspect;
        }
        else
        {
            m_scale  = m_yScale * m_yAspect;
            m_xScale = m_scale / m_xAspect;
        }
        if ( m_dc )
            m_dc->SetLogicalScale( 1.0 / m_scale, 1.0 / m_scale );
    }

    // redraw the plot
    if ( m_dc )
        plreplot();
}


void wxPLDevice::FixAspectRatio( bool fix )
{
    m_fixedAspect = fix;
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
            //size_t &readLocation = *((size_t*)(m_outputMemoryMap.getBuffer()));
            //size_t &writeLocation = *((size_t*)(m_outputMemoryMap.getBuffer())+1);
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
            if ( transmissionType == transmissionEndOfPage && amountToCopy == 0 )
            {
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                    (void *) ( &transmissionEndOfPage ), sizeof ( transmissionEndOfPage ) );
                mapHeader.writeLocation += sizeof ( transmissionEndOfPage );
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
                mapHeader.writeLocation += sizeof ( transmissionEndOfPage );
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                *( (size_t *) ( m_outputMemoryMap.getBuffer() ) + 3 ) = 1;
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
                        (char *) ( &transmissionPartial ), sizeof ( copyAmount ) );
                else
                    memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation,
                        (char *) ( &transmissionComplete ), sizeof ( copyAmount ) );
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation + sizeof ( transmissionType ),
                    (char *) ( &copyAmount ), sizeof ( copyAmount ) );
                memcpy( m_outputMemoryMap.getBuffer() + mapHeader.writeLocation + headerSize,
                    (char *) pls->plbuf_buffer + m_localBufferPosition, copyAmount );
                m_localBufferPosition   += copyAmount;
                mapHeader.writeLocation += copyAmount + headerSize;
                if ( mapHeader.writeLocation == m_outputMemoryMap.getSize() )
                    mapHeader.writeLocation = plMemoryMapReservedSpace;
                amountToCopy -= copyAmount;
                counter       = 0;
                if ( amountToCopy == 0 && transmissionType != transmissionEndOfPage
                     && transmissionType != transmissionLocate )
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
        while ( nTries < 10 )
        {
            for ( int i = 0; i < strlen( m_mfo ); ++i )
            {
                if ( m_mfo[i] == '?' )                               //this is reall a poor imitation of a random number generator.
                    mapName[i] = 'A' + clock() % 26;                 //Using C++11 generators would be better, but are not supported
                //on some enterpise linux systems at the moment
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
        *( (size_t *) ( m_outputMemoryMap.getBuffer() ) )     = plMemoryMapReservedSpace;
        *( (size_t *) ( m_outputMemoryMap.getBuffer() ) + 1 ) = plMemoryMapReservedSpace;
        *( (size_t *) ( m_outputMemoryMap.getBuffer() ) + 2 ) = 0;
        *( (size_t *) ( m_outputMemoryMap.getBuffer() ) + 3 ) = 0;

        //try to find the wxPLViewer executable, in the first instance just assume it
        //is in the path.
        wxString exeName = wxT( "wxPLViewer" );
        if ( plInBuildTree() )
        {
            //if we are in the build tree check for the needed exe in there
            wxArrayString files;
            wxDir::GetAllFiles( wxT( BUILD_DIR ), &files, exeName, wxDIR_FILES | wxDIR_DIRS );
            if ( files.size() == 0 )
                wxDir::GetAllFiles( wxT( BUILD_DIR ), &files, exeName + wxT( ".exe" ), wxDIR_FILES | wxDIR_DIRS );
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
#ifdef WIN32
        if ( wxExecute( command, wxEXEC_ASYNC ) == 0 )
            plwarn( "Failed to run wxPLViewer - no plots will be shown" );
#else
        //Linux doesn't like using wxExecute without a wxApp, so use system instead
        command << wxT( " &" );
        system( command.mb_str() );
#endif
        //wait until the viewer signals it has opened the map file
        size_t  counter      = 0;
        size_t &viewerSignal = *( (size_t *) m_outputMemoryMap.getBuffer() + 2 );
        while ( counter < 1000 && viewerSignal == 0 )
        {
            wxMilliSleep( 10 );
        }
        if ( viewerSignal == 0 )
            plwarn( "wxPLViewer failed to signal it has found the shared memory." );



        //begin the first page
        TransmitBuffer( NULL, transmissionBeginPage );
    }
}

void wxPLDevice::Locate( PLStream* pls, PLGraphicsIn *graphicsIn )
{
    if ( !m_dc && m_begunRendering && m_outputMemoryMap.isValid() )
    {
        TransmitBuffer( pls, transmissionLocate );
        bool gotResponse = false;
        while ( !gotResponse )
        {
            wxMilliSleep( 100 );
            PLNamedMutexLocker lock( &m_mutex );
            gotResponse = *( (size_t *) ( m_outputMemoryMap.getBuffer() ) + 3 ) == 0;
        }

        PLNamedMutexLocker lock( &m_mutex );
        *graphicsIn = *(PLGraphicsIn *) ( ( (size_t *) m_outputMemoryMap.getBuffer() ) + 4 );
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
