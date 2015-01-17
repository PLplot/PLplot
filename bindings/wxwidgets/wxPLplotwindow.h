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

#include "plplot.h"
#include "wxPLplotstream.h"
#include <wx/window.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>

// A plplot wxWindow template. To create an actual plplot wxWindow use
// the type of wxWindow you wish to inherit from at the template parameter
// For example to create a plplot wxFrame create a wxPLplotwindow<wxFrame>.
template <class WXWINDOW>
class wxPLplotwindow : public WXWINDOW
{
public:
    wxPLplotwindow( bool useGraphicsContext = true,
		wxString mapFile = wxT(""), PLINT mapFileSize = 0 );                //!< Constructor.
    ~wxPLplotwindow( void );                                                //!< Destructor.

    void RenewPlot( void );                                                 //!< Redo plot.
    bool SavePlot( const wxString& driver, const wxString& filename );      //!< Save plot using a different driver.
    wxPLplotstream* GetStream()  { return &m_stream; }                       //!< Get pointer to wxPLplotstream of this widget.
   void setUseGraphicsContext( bool useGraphicsContext );

protected:
    virtual void OnPaint( wxPaintEvent& event );                //!< Paint event
    virtual void OnSize( wxSizeEvent & event );     //!< Size event
	virtual void OnErase( wxEraseEvent &event );                //!< Background erase event
    wxPLplotstream m_stream;           //!< Pointer to the wxPLplotstream which belongs to this plot widget

private:
    bool       m_useGraphicsContext; //!< Flag to indicate whether we should use a wxGCDC
};


//! Constructor initialises all pointers to NULL and sets sizes to zero, the first resize
// will initialise these. It also sets up the event handling.
//
template<class WXWINDOW>
wxPLplotwindow<WXWINDOW>::wxPLplotwindow( bool useGraphicsContext,
		wxString mapFile, PLINT mapFileSize )
{
	m_stream.Create( NULL, WXWINDOW::GetClientSize().GetWidth(), WXWINDOW::GetClientSize().GetHeight(),
		wxPLPLOT_DRAW_TEXT, mapFile, mapFileSize );

	setUseGraphicsContext( useGraphicsContext );

	WXWINDOW::SetBackgroundStyle( wxBG_STYLE_CUSTOM );

	//We use connect instead of Bind for compatiblity with wxWidgets 2.8
	//but should move to bind in the future.
	WXWINDOW::Connect( wxEVT_SIZE, wxSizeEventHandler(wxPLplotwindow<WXWINDOW>::OnSize) );
	WXWINDOW::Connect( wxEVT_PAINT, wxPaintEventHandler(wxPLplotwindow<WXWINDOW>::OnPaint) );
	WXWINDOW::Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(wxPLplotwindow<WXWINDOW>::OnErase) );
	//Bind( wxEVT_SIZE, &wxPLplotwindow<WXWINDOW>::OnSize, this );
	//Bind( wxEVT_PAINT, &wxPLplotwindow<WXWINDOW>::OnPaint, this );
}


//! Deconstructor takes care that all is deleted in the correct order.
//
template<class WXWINDOW>
wxPLplotwindow<WXWINDOW>::~wxPLplotwindow( void )
{
}


//! In the OnPaint Method we check if the Windows was resized (will be moved to OnSize() sometimes
//  later), we also implement our own double buffering here (since the PLplot wxWidgets driver draws
//  into a wxMemoryDC)
//
template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnPaint( wxPaintEvent &WXUNUSED( event ) )
{
	wxAutoBufferedPaintDC dc( this );
	int width = WXWINDOW::GetClientSize().GetWidth();
	int height = WXWINDOW::GetClientSize().GetHeight();

	wxDC *pldc = &dc;
#ifdef wxUSE_GRAPHICS_CONTEXT
	wxGCDC gcdc( dc );
	if ( m_useGraphicsContext )
	{
		pldc = &gcdc;
	}
#endif

	m_stream.SetSize( WXWINDOW::GetClientSize().GetWidth(), WXWINDOW::GetClientSize().GetHeight() );
	m_stream.SetDC( pldc ); // This causes a redraw.
	m_stream.SetDC( NULL ); //Reset to NULL to avaoid writing to the wxGCDC after it has been destroyed

}


template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnSize( wxSizeEvent& WXUNUSED( event ) )
{
	//Invalidate the whole window so it is all redrawn, otherwise only
	//newly exposed parts of the window get redrawn
	RenewPlot();
}


template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::OnErase( wxEraseEvent& WXUNUSED( event ) )
{
	//Do nothing. This stops screen flicker.
}


//! Redo the whole plot.
//
template<class WXWINDOW>
void wxPLplotwindow<WXWINDOW>::RenewPlot( void )
{
	WXWINDOW::Refresh();
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
void wxPLplotwindow<WXWINDOW>::setUseGraphicsContext( bool useGraphicsContext )
{
	if( useGraphicsContext != m_useGraphicsContext )
	{
		m_useGraphicsContext = useGraphicsContext;
		RenewPlot();
	}
}

#endif // !defined( WXPLPLOTWINDOW_H__INCLUDED_ )
