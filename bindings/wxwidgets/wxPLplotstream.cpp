/* $Id$

   Copyright (C) 2005  Werner Smekal

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

#include "plplotP.h" 
#include "wxPLplotstream.h"
#include "wx/image.h"
#include "wx/dcmemory.h"

/*! Constructor of wxPLplotstream class which is inherited from plstream.
 *  Here we set the driver (wxwidgets :), and tell plplot in which dc to
 *  plot to and the size of the canvas. We also check and set several
 *  device style options.
 */
wxPLplotstream::wxPLplotstream( wxDC *dc, int width, int height, int style ) :
                m_dc(dc), m_width(width), m_height(height), m_style(style)
{
  ::plstream();
  InitStream();
  cmd( PLESC_DEVINIT, (void*)m_dc );
}

void wxPLplotstream::InitStream()
{
  const size_t bufferSize=256;

  sdev( "wxwidgets" );
  spage( 0.0, 0.0, m_width, m_height, 0, 0 );

  // use freetype, antialized canvas?
  char drvopt[bufferSize], buffer[bufferSize];
  drvopt[0]='\0';
#ifdef WX_TEMP_HAVE_FREETYPE_IS_ON  
  sprintf( buffer, "freetype=%d,smooth=%d,",
            m_style & wxPLPLOT_FREETYPE ? 1 : 0,
            m_style & wxPLPLOT_SMOOTHTEXT ? 1 : 0 );
  strcat( drvopt, buffer );
#endif  
  
  int backend;
  switch( m_style )
  {
  case wxPLPLOT_BACKEND_GC:
    backend=2;
    break;
  case wxPLPLOT_BACKEND_AGG:
    backend=1;
    break;
  default:
    backend=0;
    break;
  }
  sprintf( buffer, "backend=%d", backend );
  strncat( drvopt, buffer, bufferSize-strlen(drvopt) );

  SetOpt( "-drvopt", drvopt );
  
  init();
}



/*! This is the overloaded set_stream() function, where we could have some
 *  code processed before every call of a plplot functions, since set_stream()
 *  is called before every plplot function. Not used in the moment.
 */
void wxPLplotstream::set_stream()
{
  plstream::set_stream();
}


/*! Call this method if the size of the dc changed (because of resizing)
 *  to set the new size. You need to call RenewPlot afterwards.
 */
void wxPLplotstream::SetSize( int width, int height )
{
	m_width=width;
	m_height=height;

  wxSize size( m_width, m_height );
  cmd( PLESC_RESIZE, (void*)&size );
}


/*! Clear the background and replot everything. TODO: actually this should
 *  not be necessary, but bop() is not working as it should?
 */
void wxPLplotstream::RenewPlot()
{
  cmd( PLESC_CLEAR, NULL );
  replot();

#ifdef WX_TEMP_HAVE_AGG_IS_ON  
  if( m_style & wxPLPLOT_ANTIALIZED ) {
    wxMemoryDC MemoryDC;
    wxBitmap bitmap( *m_image, -1 );
    MemoryDC.SelectObject( bitmap );
    m_dc->Blit( 0, 0, m_width, m_height, &MemoryDC, 0, 0 );
    MemoryDC.SelectObject( wxNullBitmap );
  }
#endif
}


/* After calling plot commands it is not sure, that the dc 
 * gets updated properly, therefore you need to call this function.
 */
void wxPLplotstream::Update()
{
#ifdef WX_TEMP_HAVE_AGG_IS_ON  
  if( m_style & wxPLPLOT_ANTIALIZED ) {
    wxMemoryDC MemoryDC;
    wxBitmap bitmap( *m_image, -1 );
    MemoryDC.SelectObject( bitmap );
    m_dc->Blit( 0, 0, m_width, m_height, &MemoryDC, 0, 0 );
    MemoryDC.SelectObject( wxNullBitmap );
  }
#endif
}

