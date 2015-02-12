// Copyright (C) 2015  Phil Rosenberg
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

#ifndef WXPLFRAME_H
#define WXPLFRAME_H

#include <wx/wx.h>
#include <wx/frame.h>
#include "wxPLplotstream.h"
#include "wxPLplotwindow.h"
#include <vector>
#include "../drivers/wxwidgets_comms.h"

//Note that this header is not part of the PLPlot public API and is likely to
//change without warning


class wxPlFrame : public wxPLplotwindow<wxFrame>
{
public:
	static const int ID_FILE_EXIT;
	static const int ID_HELP_ABOUT;
	static const int ID_PAGE_NEXT;
	static const int ID_PAGE_PREV;
	static const int ID_CHECK_TIMER;

	wxPlFrame( wxWindow *parent, wxWindowID id, const wxString &title, wxString file, long fileSize,
		const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE,
		const wxString &name=wxFrameNameStr );
	~wxPlFrame();
private:
	void setupMenus();
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	void OnNextPage( wxCommandEvent& event );
	void OnPrevPage( wxCommandEvent& event );
	void OnCheckTimer( wxTimerEvent &event );
	void OnMouse( wxMouseEvent &event );
	void OnKey( wxKeyEvent &event );
	void SetPageAndUpdate( size_t page = size_t(-1) );
	std::vector<std::vector<char>> m_pageBuffers;
	std::vector<bool> m_bufferValidFlags;
	size_t m_viewingPage;
	size_t m_writingPage;
	char * m_transferBuffer;
	wxString m_file;
	size_t m_fileSize;

	PLMemoryMap m_memoryMap;
	PLNamedMutex m_mutex;
	wxTimer m_checkTimer;

	wxPoint m_cursorPosition;
	bool m_locateMode;
	size_t m_plottedBufferAmount;

	DECLARE_EVENT_TABLE();
};
#endif // APEX_MAINFRAME_H
