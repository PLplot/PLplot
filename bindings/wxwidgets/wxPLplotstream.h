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

#if !defined( WXPLPLOTSTREAM_H__INCLUDED_ )
#define WXPLPLOTSTREAM_H__INCLUDED_

#include "plstream.h"

// forward declarations
class wxImage;
class wxDC;
  
/*! Style options for wxPLplotstream:
 *  wxPLPLOT_NONE: no option
 *  wxPLPLOT_FREETYPE: use freetype library instead of Hershey fonts
 *  wxPLPLOT_SMOOTHTEXT: antialiase font (if freetype library is used)
 *  wxPLPLOT_ANTIALIZED: antialize plot (if agg library is available)
 */
enum wxPLplotstreamstyle {
    wxPLPLOT_NONE = 0,
    wxPLPLOT_FREETYPE = 1,
    wxPLPLOT_SMOOTHTEXT = 2,
    wxPLPLOT_ANTIALIZED = 4
};

/*! wxPLplotstream is inherited from plstream, which is the C++ interface
 *  to the plplot API. The documentation of this interface is described in
 *  the PLplot manual, not here. 
 */
class PLDLLIMPEXP wxPLplotstream : public plstream
{
public:
  wxPLplotstream( wxDC *dc, int width, int height, long style = wxPLPLOT_NONE );  //!< Constructor.
  void set_stream();   //!< Calls some code before every PLplot command.
	void SetSize( int width, int height );   //!< Set new size of plot area.
	void RenewPlot();   //!< Redo plot.

private:
	wxDC* m_dc;   //!< Pointer to wxDC to plot into.
  wxImage* m_image;   //!< pointer to wxImage
	int m_width;   //!< Width of dc/plot area.
	int m_height;   //!< Height of dc/plot area.
  long m_style;   //!< style of this plot
};


#endif // !defined( WXPLPLOTSTREAM_H__INCLUDED_ )
