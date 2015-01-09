// Copyright (C) 2005  Werner Smekal
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

#if !defined ( WXPLPLOTWINDOW_H__INCLUDED_ )
#define WXPLPLOTWINDOW_H__INCLUDED_

#include <wx/dcmemory.h>
#include <wx/dcclient.h>

class wxPLplotstream;

// A plplot wxWindow template. To create an actual plplot wxWindow use
// the type of wxWindow you wish to inherit from at the template parameter
// For example to create a plplot wxFrame create a wxPLplotwindow<wxFrame>.
// You must also create the event table somewhere in your application
// using the PL_EVENT_TABLE macro below
template <class WXWINDOW>
class wxPLplotwindow : public WXWINDOW
{
public:
    wxPLplotwindow( int pl_style = wxPLPLOT_NONE );                         //!< Constructor.
    ~wxPLplotwindow( void );                                                //!< Destructor.

    void RenewPlot( void );                                                 //!< Redo plot.
    bool SavePlot( const wxString& driver, const wxString& filename );      //!< Save plot using a different driver.
    wxPLplotstream* GetStream()  { return m_stream; }                       //!< Get pointer to wxPLplotstream of this widget.
    int getBackend()  { return m_backend; }
	void loadFile( wxString fileName );

protected:
    virtual void OnPaint( wxPaintEvent& event );                //!< Paint event
    virtual void OnErase( wxEraseEvent & WXUNUSED( event ) );   //!< Erase event
    virtual void OnSize( wxSizeEvent & WXUNUSED( event ) );     //!< Size event
    wxPLplotstream* m_stream;           //!< Pointer to the wxPLplotstream which belongs to this plot widget

private:
    // variables regarding double buffering
    wxMemoryDC m_memPlotDC;        //!< Pointer to wxMemoryDC, used for double buffering
    int       m_width;            //!< Saved width of plot, to find out if size changed.
    int       m_height;           //!< Saved height of plot, to find out if size changed.
    int       m_bitmapWidth;        //!< Width of bitmap, only changed if plot gets bigger
    int       m_bitmapHeight;       //!< Height of bitmap, only changed if plot gets bigger
    wxBitmap  * m_memPlotDCBitmap;  //!< Pointer to bitmap, used for double buffering.
};


//! Constructor initialises all pointers to NULL and sets sizes to zero, the first resize
// will initialise these. It also sets up the event handling.
//
template<class WXWINDOW>
wxPLplotwindow<WXWINDOW>::wxPLplotwindow( int pl_style )
{


	m_memPlotDCBitmap = NULL;
	m_stream = new wxPLplotstream( &m_memPlotDC, 0, 0, pl_style );
	m_width = 0;
	m_height = 0;
	m_bitmapWidth = 0;
	m_bitmapHeight = 0;

	SetBackgroundStyle( wxBG_STYLE_CUSTOM );

	Bind( wxEVT_SIZE, &wxPLplotwindow<WXWINDOW>::OnSize, this );
	Bind( wxEVT_PAINT, &wxPLplotwindow<WXWINDOW>::OnPaint, this );
}


//! Deconstructor takes care that all is deleted in the correct order.
//
template<class WXWINDOW>
wxPLplotwindow<WXWINDOW>::~wxPLplotwindow( void )
{
	m_memPlotDC.SelectObject( wxNullBitmap );

    if ( m_memPlotDCBitmap )
        delete m_memPlotDCBitmap;

    if ( m_stream )
        delete m_stream;
}


//! In the OnPaint Method we check if the Windows was resized (will be moved to OnSize() sometimes
//  later), we also implement our own double buffering here (since the PLplot wxWidgets driver draws
//  into a wxMemoryDC)
//
template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnPaint( wxPaintEvent &WXUNUSED( event ) )
{
    wxPaintDC dc( this );
    dc.Blit( 0, 0, m_width, m_height, &m_memPlotDC, 0, 0 );
}


template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnSize( wxSizeEvent& WXUNUSED( event ) )
{
    int width, height;
    GetClientSize( &width, &height );

    // Check if the window was resized
    if ( ( m_width != width ) || ( m_height != height ) )
    {
        if ( ( width > m_bitmapWidth ) || ( height > m_bitmapHeight ) )
        {
            m_bitmapWidth  = m_bitmapWidth > width ? m_bitmapWidth : width;
            m_bitmapHeight = m_bitmapHeight > height ? m_bitmapHeight : height;

            m_memPlotDC.SelectObject( wxNullBitmap );
            if ( m_memPlotDCBitmap )
                delete m_memPlotDCBitmap;
            m_memPlotDCBitmap = new wxBitmap( m_bitmapWidth, m_bitmapHeight, -1 );
            m_memPlotDC.SelectObject( *m_memPlotDCBitmap );
        }

        m_stream->SetSize( width, height );
        m_stream->RenewPlot();

        m_width  = width;
        m_height = height;
    }
    else
    {
        m_stream->Update();
        Refresh( false );
    }
}


//! Together with "SetBackgroundStyle( wxBG_STYLE_CUSTOM );" in the constructor this method
//  is responsible that the background is not erased in order to prevent flickering.
//
template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnErase( wxEraseEvent &WXUNUSED( event ) )
{
}


//! Redo the whole plot.
//
template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::RenewPlot( void )
{
    if ( m_stream )
    {
        m_stream->RenewPlot();
        Refresh( false );
    }
}


//! Save plot.
//
template<class WXWINDOW>
bool wxPLplotwindow<WXWINDOW>::SavePlot( const wxString& devname, const wxString& filename )
{
    int  pls, pls_save;
    FILE *sfile;

    if ( ( sfile = fopen( filename.mb_str(), "wb+" ) ) == NULL )
    {
        return false;
    }

    plgstrm( &pls );
    plmkstrm( &pls_save );
    if ( pls_save < 0 )
    {
        fclose( sfile );
        return false;
    }
    plsdev( devname.mb_str() );
    plsfile( sfile );

    plspage( 0., 0., 800, 600, 0, 0 );
    plcpstrm( pls, 0 );
    pladv( 0 );
    plreplot();
    plend1();
    plsstrm( pls );

    return true;
}

template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::loadFile( wxString fileName )
{

}

#endif // !defined( WXPLPLOTWINDOW_H__INCLUDED_ )
