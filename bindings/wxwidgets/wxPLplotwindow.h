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

#if !defined( WXPLPLOTWINDOW_H__INCLUDED_ )
#define WXPLPLOTWINDOW_H__INCLUDED_

#include "wx/window.h"
#include "wx/dcmemory.h"
#include "wxPLplotstream.h"


/*! A plot widget which takes care of double buffering and other stuff, but does not
 *  provide an own plot API. You have to get a pointer to wxPLplotstream via the 
 *  GetStream() method to gain access to the PLplot API.
 */
class PLDLLIMPEXP wxPLplotwindow : public wxWindow
{
public:
	wxPLplotwindow( wxWindow* parent, wxWindowID id=-1, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = 0,
                  long pl_style = wxPLPLOT_NONE );   //!< Constructor.
  ~wxPLplotwindow( void );  //!< Deconstructor.

  void RenewPlot( void );   //!< Redo plot.
	void SavePlot( const wxString& driver, const wxString& filename );  //!< Save plot using a different driver.
	wxPLplotstream* GetStream()  { return m_stream; }  //!< Get pointer to wxPLplotstream of this widget.
	
protected:
  virtual void OnPaint( wxPaintEvent& event );   //!< Paint event.
	virtual void OnErase( wxEraseEvent &WXUNUSED(event) );   //!< Erase event.

private:
  // variables regarding double buffering
  wxMemoryDC* MemPlotDC;      //!< Pointer to wxMemoryDC, used for double buffering
  int MemPlotDC_width;        //!< Saved width of MemoryDC, to find out if size changed.
  int MemPlotDC_height;       //!< Saved height of MemoryDC, to find out if size changed.
  wxBitmap* MemPlotDCBitmap;  //!< Pointer to bitmap, used for double buffering.

protected:
  wxPLplotstream* m_stream;             //!< Pointer to the wxPLplotstream which belongs to this plot widget

  DECLARE_EVENT_TABLE()
};


#endif // !defined( WXPLPLOTWINDOW_H__INCLUDED_ )
