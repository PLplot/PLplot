// Copyright (C) 2008  Werner Smekal
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

#ifndef __WXWIDGETS_H__
#define __WXWIDGETS_H__

// some special wxWidgets headers
#include <wx/spinctrl.h>
#ifdef wxUSE_GRAPHICS_CONTEXT
#include <wx/dcgraph.h>

#endif
#ifndef WIN32
#include<sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#define NDEV                                                  100 // Max number of output device types in menu

//--------------------------------------------------------------------------
// Physical dimension constants used by the driver

// Virtual coordinate scaling parameter, used to do calculations at
// higher resolution.  Chosen so that maximum plP_setphy coordinate
// is 32000 which is close to but still less than the maximum possible
// which is 32767.
//
// The trick here is to do everything in device coordinates on the driver
// side, but report/receive everything in virtual coordinates to/from the
// PLplot core.
//
//#define VSCALE                   ( 40. )

// pixels per inch
//#define DEVICE_PIXELS_PER_IN     ( 80. )
//#define VIRTUAL_PIXELS_PER_IN    ( DEVICE_PIXELS_PER_IN * VSCALE )


// mm per inch
//#define MM_PER_IN                    ( 25.4 )

// pixels per mm
//#define DEVICE_PIXELS_PER_MM         ( DEVICE_PIXELS_PER_IN / MM_PER_IN )
//#define VIRTUAL_PIXELS_PER_MM        ( VIRTUAL_PIXELS_PER_IN / MM_PER_IN )

// Default dimensions of the canvas (in inches)
//#define CANVAS_WIDTH                 ( 10.0 )
//#define CANVAS_HEIGHT                ( 7.5 )

// size of plot in pixels on screen if not given
//#define PLOT_WIDTH                   ( 800 )
//#define PLOT_HEIGHT                  ( 600 )


class wxPLplotFrame;

class PLMemoryMap
{
public:
	PLMemoryMap();
	PLMemoryMap( const char *name, PLINT size, bool onlyIfExists );
	void create( const char *name, PLINT size, bool onlyIfExists );
	void close();
	~PLMemoryMap();
	char *getBuffer() { return (char*)m_buffer; }
	bool isValid() {return m_buffer != NULL; }
private:
#ifdef WIN32
	HANDLE m_mapFile;
#else
	int m_mapFile;
	PLINT m_size;
	char * m_name;
#endif
	void *m_buffer;
};

// base device class
class wxPLDevice
{
public:
    wxPLDevice( PLStream *pls, char * mf0, char * mfi, PLINT mfiSize, PLINT text, PLINT hrshsym );
    virtual ~wxPLDevice( void );

    void DrawLine( short x1a, short y1a, short x2a, short y2a );
    void DrawPolyline( short *xa, short *ya, PLINT npts );
    void ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1 = -1, PLINT y1 = -1, PLINT x2 = -1, PLINT y2 = -1 );
    void FillPolygon( PLStream *pls );
    void SetWidth( PLStream *pls );
    void SetColor( PLStream *pls );
    void SetExternalBuffer( PLStream *pls, void* buffer );
    void SetFont( PLUNICODE fci );
	void EndPage( PLStream* pls );
	void BeginPage( PLStream* pls );
	void SetSize( PLStream* pls, int width, int height );
    void ProcessString( PLStream* pls, EscText* args );

protected:
    void DrawText( PLUNICODE* ucs4, int ucs4Len, bool drawText );;
    void DrawTextSection( char* utf8_string, bool drawText );

	//The DC we will draw on if given by the user
    wxDC       *m_dc;

	//Size and Scale
	//As far as plplot is concerned the size of the window is SHRT_MAX by
	//SHRT_MAX which gives us the best resolution.
	const PLFLT  m_plplotEdgeLength;
    PLFLT        m_width; //native width
    PLFLT        m_height; //native height
    PLFLT        m_scalex; //conversion from native width to plplotEdgeLength
    PLFLT        m_scaley; //conversion from native height to plplotEdgeLength
	//PLFLT        m_ppi; //pixels per inch (actually conversion from native size to inches)

	// Flags indicating change of state before we have a DC.
    bool         m_plstate_width;
    bool         m_plstate_color;

	//these are not being used at the current time
    //PLGraphicsIn gin;                   // Graphics input structure
    //int          locate_mode;           // Set while in locate mode
    //bool         draw_xhair;            // Set during xhair draws

    // font variables
    static const int m_max_string_length = 500;
    wxFont          *m_font;
    bool             m_underlined;
    PLFLT            m_fontSize;
    PLFLT            m_fontScale;
    wxCoord          m_textWidth, m_textHeight, m_textDescent, m_textLeading;
    PLUNICODE        m_fci;

    //Text positioning related variables
    wxCoord          m_superscriptHeight; //distance between superscript top and baseline
	wxCoord          m_subscriptDepth; //distance between subscript base and baseline
    PLFLT            m_lineSpacing;
    PLFLT            m_yOffset;
    PLINT            m_posX;
	PLINT            m_posY;
    PLFLT            m_rotation;
	PLFLT            m_cos_rot;
	PLFLT            m_sin_rot;
    PLFLT            m_shear;
	PLFLT            m_cos_shear;
	PLFLT            m_sin_shear;
    PLFLT            m_stride;

	//variables for dealing with sending/receiving commands
	//via a memory map
	char           m_mfo[256];
	char           m_mfi[256];
	PLMemoryMap    m_inputMemoryMap;
	PLMemoryMap    m_outputMemoryMap;
	PLINT          m_inputSize;
};


struct dev_entry
{
    wxString dev_name;
    wxString dev_menu_short;
    wxString dev_menu_long;
    wxString dev_file_app;
    bool     pixelDevice;
};



// after how many commands the window should be refreshed
#define MAX_COMCOUNT    10000

// wxwidgets application definition (if needed)
/*
WX_DEFINE_ARRAY( wxPLplotFrame *, wxArrayOfwxPLplotFrame );
class wxPLplotApp : public wxApp
{
public:
    virtual bool OnInit();
    // virtual int OnExit();
    void SetExitFlag( bool flag = true ) { exit = flag; };
    bool GetExitFlag( void ) { return exit; };
    void SetAdvanceFlag( bool flag = true ) { advance = flag; };
    bool GetAdvanceFlag( void ) { return advance; };
    void SetRefreshFlag( bool flag = true );
    void AddFrame( wxPLplotFrame* frame ) { FrameArray.Add( frame ); };
    void RemoveFrame( wxPLplotFrame* frame ) { FrameArray.Remove( frame ); };
    size_t FrameCount( void ) { return FrameArray.GetCount(); }
    void OnIdle( wxIdleEvent& event );

private:
    bool exit;
    bool advance;
    wxArrayOfwxPLplotFrame FrameArray;

    DECLARE_EVENT_TABLE()
};

// definition of the actual window/frame shown
class wxPLplotWindow : public wxWindow
{
public:
    wxPLplotWindow( wxWindow* parent, PLStream *pls );

    void SetRefreshFlag( bool flag = true ) { refresh = flag; };
    bool GetRefreshFlag( void ) { return refresh; };
    void SetOrientation( int rot );

private:
    void OnPaint( wxPaintEvent& event );
    void OnChar( wxKeyEvent& event );
    void OnIdle( wxIdleEvent& event );
    void OnErase( wxEraseEvent & WXUNUSED( event ) );
    void OnSize( wxSizeEvent & WXUNUSED( event ) );
    void OnMaximize( wxMaximizeEvent & WXUNUSED( event ) );
    void OnMouse( wxMouseEvent& event );
    void Locate( void );
    void DrawCrosshair();

    PLStream   * m_pls;
    wxPLDevBase* m_dev; // windows needs to know this structure
    bool       refresh;
    bool       xhair_drawn;
    int        mouse_x, mouse_y, old_mouse_x, old_mouse_y;

    DECLARE_EVENT_TABLE()
};


// declaration of the actual window/frame shown
class wxPLplotFrame : public wxFrame
{
public:
    wxPLplotFrame( const wxString& title, PLStream* pls );
    void OnMenu( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );
    bool SavePlot( const char* filename, const char* devname, int width, int height );
    void SetRefreshFlag( bool flag = true ) { m_window->SetRefreshFlag( flag ); };
    bool GetRefreshFlag( void ) { return m_window->GetRefreshFlag(); };

private:
    wxPanel       * m_panel;
    wxPLplotWindow* m_window;
    wxPLDevBase   * m_dev; // frame needs to know this structure

    DECLARE_EVENT_TABLE()
};

// menu ids
enum { wxPL_Save = 10000, wxPL_Next = 10100, wxPL_Locate, wxPL_Orientation_0, wxPL_Orientation_90,
       wxPL_Orientation_180, wxPL_Orientation_270 };


// Pixel size dialog
class wxGetSizeDialog : public wxDialog
{
public:
    // constructors and destructors
    wxGetSizeDialog( wxWindow *parent, wxWindowID id, const wxString &title,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxDEFAULT_DIALOG_STYLE,
                     int width = 800, int height = 600 );

    int getWidth()  { return spinControlWidth->GetValue(); }
    int getHeight()  { return spinControlHeight->GetValue(); }

private:
    wxSpinCtrl* spinControlWidth;
    wxSpinCtrl* spinControlHeight;

private:
    DECLARE_EVENT_TABLE()
};


// Use this macro if you want to define your own main() or WinMain() function
// and call wxEntry() from there.
#define IMPLEMENT_PLAPP_NO_MAIN( appname )                                                                                                        \
    wxAppConsole * wxPLCreateApp()                                                                                                                \
    {                                                                                                                                             \
        wxAppConsole::CheckBuildOptions( WX_BUILD_OPTIONS_SIGNATURE,                                                                              \
            "your program" );                                                                                                                     \
        return new appname;                                                                                                                       \
    }                                                                                                                                             \
    wxAppInitializer                                                                                                                              \
    wxAppInitializer( (wxAppInitializerFunction) ( wxApp::GetInitializerFunction() == NULL ? wxPLCreateApp : wxApp::GetInitializerFunction() ) ); \
    DECLARE_PLAPP( appname )                                                                                                                      \
    appname & wxPLGetApp() { return *wx_static_cast( appname *, wxApp::GetInstance() ); }

#define DECLARE_PLAPP( appname )    extern appname &wxPLGetApp();

// workaround against warnings for unused variables
static inline void Use( void * )
{
}
#define WX_SUPPRESS_UNUSED_WARN( x )    Use( &x )
*/

//--------------------------------------------------------------------------
//  Declarations for the device.
//--------------------------------------------------------------------------

void plD_init_wxwidgets( PLStream * );
void plD_init_wxpng( PLStream * );
void plD_line_wxwidgets( PLStream *, short, short, short, short );
void plD_polyline_wxwidgets( PLStream *, short *, short *, PLINT );
void plD_eop_wxwidgets( PLStream * );
void plD_bop_wxwidgets( PLStream * );
void plD_tidy_wxwidgets( PLStream * );
void plD_state_wxwidgets( PLStream *, PLINT );
void plD_esc_wxwidgets( PLStream *, PLINT, void * );

void wx_set_dc( PLStream* pls, wxDC* dc );
void wx_set_buffer( PLStream* pls, wxImage* buffer );

//--------------------------------------------------------------------------
//  Debug functions
//--------------------------------------------------------------------------

// define if you want debug output
// #define _DEBUG //
// #define _DEBUG_VERBOSE //
void Log_Verbose( const char *fmt, ... );
void Log_Debug( const char *fmt, ... );


//--------------------------------------------------------------------------
// Font style and weight lookup tables
//--------------------------------------------------------------------------
const wxFontFamily fontFamilyLookup[5] = {
    wxFONTFAMILY_SWISS,      // sans-serif
    wxFONTFAMILY_ROMAN,      // serif
    wxFONTFAMILY_TELETYPE,   // monospace
    wxFONTFAMILY_SCRIPT,     // script
    wxFONTFAMILY_SWISS       // symbol
};

const int          fontStyleLookup[3] = {
    wxFONTFLAG_DEFAULT,      // upright
    wxFONTFLAG_ITALIC,       // italic
    wxFONTFLAG_SLANT         // oblique
};

const int          fontWeightLookup[2] = {
    wxFONTFLAG_DEFAULT,     // medium
    wxFONTFLAG_BOLD         // bold
};

#endif // __WXWIDGETS_H__
