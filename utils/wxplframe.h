#ifndef WXPLFRAME_H
#define WXPLFRAME_H

#include <wx/wx.h>
#include <wx/frame.h>
#include "wxPLplotstream.h"
#include "wxPLplotwindow.h"
#include <vector>

//Note that this header is not part of the PLPlot public API and is likely to
//change without warning


//data transmission codes
const unsigned char transmissionRegular = 0;
const unsigned char transmissionSkipFileEnd = 1;
const unsigned char transmissionEndOfPage = 2;
const unsigned char transmissionBeginPage = 3;
const unsigned char transmissionLocate = 4;
const unsigned char transmissionPartial = 5;
const unsigned char transmissionComplete = 6;

const PLINT plMemoryMapReservedSpace = 2*sizeof( size_t );

class PLMemoryMap
{
public:
	PLMemoryMap();
	PLMemoryMap( const char *name, PLINT size, bool mustExist, bool mustNotExist );
	void create( const char *name, PLINT size, bool mustExist, bool mustNotExist );
	void close();
	~PLMemoryMap();
	char *getBuffer() { return (char*)m_buffer; }
	bool isValid() {return m_buffer != NULL; }
	size_t getSize() {return m_size; }
private:
#ifdef WIN32
	HANDLE m_mapFile;
#else
	int m_mapFile;
	char * m_name;
#endif
	size_t m_size;
	void *m_buffer;
};

class PLNamedMutex
{
public:
	PLNamedMutex();
	PLNamedMutex( const char *name, bool aquireOnCreate = false );
	~PLNamedMutex();
	void create(const char *name, bool aquireOnCreate = false );
	void clear();
	void aquire();
	bool aquire( unsigned long millisecs );
	bool aquireNoWait();
	void release();
	bool isValid();
private:
#ifdef WIN32
	HANDLE m_mutex;
#else
#endif
};

class PLNamedMutexLocker
{
public:
	PLNamedMutexLocker( PLNamedMutex *mutex );
	~PLNamedMutexLocker();
private:
	 PLNamedMutex *m_mutex;
	 //remove default constructors
	 PLNamedMutexLocker();
	 PLNamedMutexLocker( const PLNamedMutexLocker & );
	 PLNamedMutexLocker & operator = ( const PLNamedMutex & );
};

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
	std::vector<std::vector<char>> m_pageBuffers;
	std::vector<bool> m_pageCompleteFlags;
	size_t m_viewingPage;
	size_t m_writingPage;
	char * m_transferBuffer;
	wxString m_file;
	size_t m_fileSize;

	PLMemoryMap m_memoryMap;
	PLNamedMutex m_mutex;
	wxTimer m_checkTimer;

	DECLARE_EVENT_TABLE();
};
#endif // APEX_MAINFRAME_H
