/* $Id$

   Copyright (C) 2008  Werner Smekal

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

#ifndef __WXWIDGETS_H__
#define __WXWIDGETS_H__

/* freetype headers and macros */
#ifdef HAVE_FREETYPE
  #include "plfreetype.h"

  static void plD_pixel_wxwidgets( PLStream *pls, short x, short y );
  static PLINT plD_read_pixel_wxwidgets( PLStream *pls, short x, short y );
  static void plD_set_pixel_wxwidgets( PLStream *pls, short x, short y, PLINT colour );
  static void init_freetype_lv1( PLStream *pls );
  static void init_freetype_lv2( PLStream *pls );
extern "C"
{
  extern void plD_FreeType_init( PLStream *pls );
  extern void plD_render_freetype_text( PLStream *pls, EscText *args );
  extern void plD_FreeType_Destroy( PLStream *pls );
  extern void pl_set_extended_cmap0( PLStream *pls, int ncol0_width, int ncol0_org );
}
#endif

#ifndef max_number_of_grey_levels_used_in_text_smoothing
  #define max_number_of_grey_levels_used_in_text_smoothing 64
#endif

#define NDEV	100		/* Max number of output device types in menu */

/*=========================================================================*/
/* Physical dimension constants used by the driver */

/* Virtual coordinate scaling parameter, used to do calculations at
 * higher resolution.  Chosen to be 32 for consistency with the PLplot
 * metafile (see plplotP.h).
 *
 * The trick here is to do everything in device coordinates on the driver
 * side, but report/receive everything in virtual coordinates to/from the
 * PLplot core.
 */
#define VSCALE (32.)

/* pixels per mm */
#define DEVICE_PIXELS_PER_MM (3.14961)
#define VIRTUAL_PIXELS_PER_MM (DEVICE_PIXELS_PER_MM*VSCALE)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per inch */
#define DEVICE_PIXELS_PER_IN (DEVICE_PIXELS_PER_MM*MM_PER_IN)
#define VIRTUAL_PIXELS_PER_IN (VIRTUAL_PIXELS_PER_MM*MM_PER_IN)

/* Default dimensions of the canvas (in inches) */
#define CANVAS_WIDTH (10.0)
#define CANVAS_HEIGHT (7.5)

/* size of plot in pixels on screen if not given */
#define PLOT_WIDTH (800)
#define PLOT_HEIGHT (600)

/* These need to be distinguished since the handling is slightly different. */
#define LOCATE_INVOKED_VIA_API		1
#define LOCATE_INVOKED_VIA_DRIVER	2

class wxPLplotFrame;
  
/* base device class */
class wxPLDevBase
{
public: /* methods */
	wxPLDevBase( void );
	virtual ~wxPLDevBase( void );

  // virtual functions which need to implemented
  virtual void DrawLine( short x1a, short y1a, short x2a, short y2a )=0;
  virtual void DrawPolyline( short *xa, short *ya, PLINT npts )=0;
  virtual void ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1=-1, PLINT y1=-1, PLINT x2=-1, PLINT y2=-1 )=0;
  virtual void FillPolygon( PLStream *pls )=0;
  virtual void BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH )=0;
  void AddtoClipRegion( int x1, int y1, int x2, int y2 );
  virtual void CreateCanvas()=0;
  virtual void SetWidth( PLStream *pls )=0;
  virtual void SetColor0( PLStream *pls )=0;
  virtual void SetColor1( PLStream *pls )=0;
  virtual void SetExternalBuffer( void* buffer )=0;
  virtual void PutPixel( short x, short y, PLINT color )=0;
  virtual void PutPixel( short x, short y )=0;
  virtual PLINT GetPixel( short x, short y )=0;
  virtual void ProcessString( PLStream* pls, EscText* args )=0;
  virtual void PSDrawText( PLUNICODE* ucs4, int ucs4Len, bool drawText );
  virtual void PSDrawTextToDC( char* utf8_string, bool drawText )=0;
  virtual void PSSetFont( PLUNICODE fci )=0;

public: /* variables */
  bool ready;
  bool ownGUI;
  bool waiting;
  bool resizing;
  bool exit;

  int comcount;
  
  wxPLplotFrame* m_frame;
  PLINT width;
  PLINT height;
  PLINT bm_width;
  PLINT bm_height;

  PLINT xmin;
  PLINT xmax;
  PLINT ymin;
  PLINT ymax;

	PLFLT scalex;
	PLFLT scaley;

  bool plstate_width;   /* Flags indicating change of state before */
  bool plstate_color0;  /* device is fully initialized */
  bool plstate_color1;  /* taken from gcw driver */
  
  PLGraphicsIn gin;			/* Graphics input structure */
  int locate_mode;		  /* Set while in locate mode */
  bool	draw_xhair;		/* Set during xhair draws */

	/* clipping region */
	int clipminx, clipmaxx;
	int clipminy, clipmaxy;
	bool newclipregion;

  /* variables for antializing */
	int freetype;
	int smooth_text;

  const char** devDesc;		/* Descriptive names for file-oriented devices.  Malloc'ed. */
  const char** devName;		/* Keyword names of file-oriented devices. Malloc'ed. */
	int ndev;

  /* font variables */
  static const int max_string_length=500;
  wxFont* m_font;
  bool underlined;
  double fontSize;
  double fontScale;
  wxCoord textWidth, textHeight, textDescent, textLeading;
  double yOffset;
  PLINT posX, posY;
  PLFLT rotation, cos_rot, sin_rot;
  PLFLT shear, cos_shear, sin_shear;
};


class wxPLDevDC : public wxPLDevBase
{
public: /* methods */
	wxPLDevDC( void );
	~wxPLDevDC( void );

  void DrawLine( short x1a, short y1a, short x2a, short y2a );
  void DrawPolyline( short *xa, short *ya, PLINT npts );
  void ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1=-1, PLINT y1=-1, PLINT x2=-1, PLINT y2=-1 );
  void FillPolygon( PLStream *pls );
  void BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH );
  void CreateCanvas();
  void SetWidth( PLStream *pls );
  void SetColor0( PLStream *pls );
  void SetColor1( PLStream *pls );
  void SetExternalBuffer( void* buffer );
  void PutPixel( short x, short y, PLINT color );
  void PutPixel( short x, short y );
  PLINT GetPixel( short x, short y );
  void ProcessString( PLStream* pls, EscText* args );
  void PSDrawTextToDC( char* utf8_string, bool drawText );
  void PSSetFont( PLUNICODE fci );

private: /* variables */
  wxBitmap* m_bitmap;
  wxDC* m_dc;
};


#ifdef HAVE_AGG
/* antigrain headers */
#include "agg2/agg_basics.h"
#include "agg2/agg_rendering_buffer.h"
#include "agg2/agg_rasterizer_scanline_aa.h"
#include "agg2/agg_scanline_u.h"
#include "agg2/agg_scanline_bin.h"
#include "agg2/agg_conv_stroke.h"
#include "agg2/agg_pixfmt_rgb.h"
#include "agg2/agg_renderer_base.h"
#include "agg2/agg_renderer_scanline.h"
#include "agg2/agg_renderer_primitives.h"
#include "agg2/agg_path_storage.h"
#include "agg2/agg_conv_curve.h"
#include "agg2/agg_conv_contour.h"
#include "agg2/agg_font_freetype.h"

typedef agg::pixfmt_rgb24 pixfmt;
typedef agg::renderer_base<pixfmt> ren_base;
typedef agg::renderer_scanline_aa_solid<ren_base> renderer;
typedef agg::font_engine_freetype_int32 font_engine_type;
typedef agg::font_cache_manager<font_engine_type> font_manager_type;

class wxPLDevAGG : public wxPLDevBase
{
public: /* methods */
	wxPLDevAGG( void );
	~wxPLDevAGG( void );

  void DrawLine( short x1a, short y1a, short x2a, short y2a );
  void DrawPolyline( short *xa, short *ya, PLINT npts );
  void ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1=-1, PLINT y1=-1, PLINT x2=-1, PLINT y2=-1 );
  void FillPolygon( PLStream *pls );
  void BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH );
  void CreateCanvas();
  void SetWidth( PLStream *pls );
  void SetColor0( PLStream *pls );
  void SetColor1( PLStream *pls );
  void SetExternalBuffer( void* buffer );
  void PutPixel( short x, short y, PLINT color );
  void PutPixel( short x, short y );
  PLINT GetPixel( short x, short y );
  void ProcessString( PLStream* pls, EscText* args );
  void PSDrawTextToDC( char* utf8_string, bool drawText );
  void PSSetFont( PLUNICODE fci );

private: /* variables */
  wxDC* m_dc;
  wxImage* m_buffer;
  agg::rendering_buffer *m_rendering_buffer;
//	font_engine_type m_font_engine;
//  font_manager_type m_font_manager;
//	agg::conv_curve<font_manager_type::path_adaptor_type> m_curves;
//  agg::conv_contour<agg::conv_curve<font_manager_type::path_adaptor_type> > m_contour;

  double m_strokewidth;
  wxUint8 m_StrokeOpacity;
  unsigned char m_colredstroke;
  unsigned char m_colgreenstroke;
  unsigned char m_colbluestroke;
  unsigned char m_colredfill;
  unsigned char m_colgreenfill;
  unsigned char m_colbluefill;
};
#endif


#if wxUSE_GRAPHICS_CONTEXT
class wxPLDevGC : public wxPLDevBase
{
public: /* methods */
	wxPLDevGC( void );
	~wxPLDevGC( void );

  void DrawLine( short x1a, short y1a, short x2a, short y2a );
  void DrawPolyline( short *xa, short *ya, PLINT npts );
  void ClearBackground( PLINT bgr, PLINT bgg, PLINT bgb, PLINT x1=-1, PLINT y1=-1, PLINT x2=-1, PLINT y2=-1 );
  void FillPolygon( PLStream *pls );
  void BlitRectangle( wxPaintDC* dc, int vX, int vY, int vW, int vH );
  void CreateCanvas();
  void SetWidth( PLStream *pls );
  void SetColor0( PLStream *pls );
  void SetColor1( PLStream *pls );
  void SetExternalBuffer( void* buffer );
  void PutPixel( short x, short y, PLINT color );
  void PutPixel( short x, short y );
  PLINT GetPixel( short x, short y );
  void ProcessString( PLStream* pls, EscText* args );
  void PSDrawTextToDC( char* utf8_string, bool drawText );
  void PSSetFont( PLUNICODE fci );

private: /* variables */
  wxBitmap* m_bitmap;
  wxDC* m_dc;
  wxGraphicsContext* m_context;
  
  /* text colors */
  unsigned char textRed, textGreen, textBlue;
};
#endif


struct dev_entry {    
  wxString dev_name;
  wxString dev_menu_short;
  wxString dev_menu_long;
  wxString dev_file_app;
};



/* after how many commands the window should be refreshed */
#define MAX_COMCOUNT 5000

/* wxwidgets application definition (if needed) */
WX_DEFINE_ARRAY( wxPLplotFrame*, wxArrayOfwxPLplotFrame );
class wxPLplotApp : public wxApp
{
public:
  virtual bool OnInit();
  /* virtual int OnExit(); */
  void SetExitFlag( bool flag=true ) { exit=flag; };
  bool GetExitFlag( void ) { return exit; };
  void SetAdvanceFlag( bool flag=true ) { advance=flag; };
  bool GetAdvanceFlag( void ) { return advance; };
  void SetRefreshFlag( bool flag=true );
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

/* definition of the actual window/frame shown */
class wxPLplotWindow : public wxWindow
{
public:
	wxPLplotWindow( wxWindow* parent, PLStream *pls );

  void SetRefreshFlag( bool flag=true ) { refresh=flag; };
  bool GetRefreshFlag( void ) { return refresh; };
  
private:
  void OnPaint( wxPaintEvent& event );
  void OnChar( wxKeyEvent& event );
  void OnIdle( wxIdleEvent& event );
	void OnErase( wxEraseEvent& WXUNUSED(event) );
  void OnSize( wxSizeEvent & WXUNUSED(event) );
  void OnMaximize( wxMaximizeEvent & WXUNUSED(event) );
  void OnMouse( wxMouseEvent& event );
  void DrawCrosshair();
  
  PLStream *m_pls;
  wxPLDevBase* m_dev;  /* windows needs to know this structure */
  bool refresh;
  bool xhair_drawn;
  int mouse_x, mouse_y, old_mouse_x, old_mouse_y;

  DECLARE_EVENT_TABLE()
};

/* definition of the actual window/frame shown */

class wxPLplotFrame : public wxFrame
{
public:
  wxPLplotFrame( const wxString& title, PLStream* pls );
  void OnMenu( wxCommandEvent& event );
  void OnClose( wxCloseEvent& event );
  bool SavePlot( const char* filename, const char* devname, int width,  int height );
  void SetRefreshFlag( bool flag=true ) { m_window->SetRefreshFlag( flag ); };
  bool GetRefreshFlag( void ) { return m_window->GetRefreshFlag(); };

private:
  wxPanel* m_panel;
  wxPLplotWindow* m_window;
  wxPLDevBase* m_dev;  /* frame needs to know this structure */

  DECLARE_EVENT_TABLE()
};

enum { wxPL_Save=10000, wxPL_Next=10100 };

/* Use this macro if you want to define your own main() or WinMain() function
   and call wxEntry() from there. */
#define IMPLEMENT_PLAPP_NO_MAIN(appname)                                      \
    wxAppConsole *wxPLCreateApp()                                             \
    {                                                                       \
        wxAppConsole::CheckBuildOptions( WX_BUILD_OPTIONS_SIGNATURE,         \
                                         "your program" );                    \
        return new appname;                                                 \
    }                                                                       \
    wxAppInitializer                                                        \
        wxAppInitializer((wxAppInitializerFunction) wxPLCreateApp);        \
    DECLARE_APP(appname)                                                    \
    appname& wxGetApp() { return *wx_static_cast(appname*, wxApp::GetInstance()); }

#define DECLARE_APP(appname) extern appname& wxGetApp();

/* workaround against warnings for unused variables */
static inline void Use(void *) { }
#define WX_SUPPRESS_UNUSED_WARN( x ) Use( &x )

/* private functions needed by the wxwidgets Driver */
static void install_buffer( PLStream *pls );
static void wxRunApp( PLStream *pls, bool runonce=false );
static void GetCursorCmd( PLStream *pls, PLGraphicsIn *ptr );

/*----------------------------------------------------------------------*\
 *  Declarations for the device.
\*----------------------------------------------------------------------*/

void plD_init_wxwidgets		(PLStream *);
void plD_line_wxwidgets		(PLStream *, short, short, short, short);
void plD_polyline_wxwidgets	(PLStream *, short *, short *, PLINT);
void plD_eop_wxwidgets		(PLStream *);
void plD_bop_wxwidgets		(PLStream *);
void plD_tidy_wxwidgets		(PLStream *);
void plD_state_wxwidgets	(PLStream *, PLINT);
void plD_esc_wxwidgets		(PLStream *, PLINT, void *);

static void fill_polygon( PLStream *pls );
void wx_set_dc( PLStream* pls, wxDC* dc );
void wx_set_buffer( PLStream* pls, wxImage* buffer );
void wx_set_size( PLStream* pls, int width, int height );
int plD_errorexithandler_wxwidgets( char *errormessage );
void plD_erroraborthandler_wxwidgets( char *errormessage );

/*----------------------------------------------------------------------*\
 *  Debug functions
\*----------------------------------------------------------------------*/

/* define if you want debug output */
// #define _DEBUG //*/
/* #define _DEBUG_VERBOSE //*/
void Log_Verbose( const char *fmt, ... );
void Log_Debug( const char *fmt, ... );


/*---------------------------------------------------------------------
  Font style and weight lookup tables
  ---------------------------------------------------------------------*/
const wxFontFamily fontFamilyLookup[5] = {
  wxFONTFAMILY_SWISS,        // sans-serif
  wxFONTFAMILY_ROMAN,        // serif
  wxFONTFAMILY_TELETYPE,     // monospace
  wxFONTFAMILY_SCRIPT,       // script
  wxFONTFAMILY_ROMAN         // symbol
};

const int fontStyleLookup[3] = {
  wxFONTFLAG_DEFAULT,        // upright
  wxFONTFLAG_ITALIC,         // italic
  wxFONTFLAG_SLANT           // oblique
};

const int fontWeightLookup[2] = {
  wxFONTFLAG_DEFAULT,       // medium
  wxFONTFLAG_BOLD           // bold
};

#endif /* __WXWIDGETS_H__ */
