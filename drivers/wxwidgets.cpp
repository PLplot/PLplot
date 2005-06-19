/* $Id$

   Copyright (C) 2005  Werner Smekal
   Copyright (C) 1999  Frank Huebner

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


/* TODO:
 *) patterns look bad - see x15c
 */

#include "plDevs.h"

#ifdef PLD_wxwidgets

// plplot headers
#include "plplotP.h"
#include "drivers.h"

// os specific headers
#ifdef __WIN32__
  #include <windows.h>
#endif

#ifdef __WXMAC__
	#include <Carbon/Carbon.h>
  extern "C" { void CPSEnableForegroundOperation(ProcessSerialNumber* psn); }
#endif

// C/C++ headers
#include <cstdio>

// wxwidgets headers
#include <wx/wx.h>
#include <wx/except.h>

// freetype headers and macros
#ifdef HAVE_FREETYPE_NO
  #include "plfreetype.h"
  
  #ifndef max_number_of_grey_levels_used_in_text_smoothing
    #define max_number_of_grey_levels_used_in_text_smoothing 64
  #endif
#endif

/* Physical dimensions, copied from gcw driver */
/* Drawing units per inch; 1 DU corresponds to a pen width of 1 */
#define DU_PER_IN (360.)

/* Pixels per drawing unit: The default value of 0.2 pixes per drawing 
 * unit gives 72 pixels per inch. */
#define PIXELS_PER_DU (0.2)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per mm for this device */
#define PIXELS_PER_MM (PIXELS_PER_DU * DU_PER_IN / MM_PER_IN)

/* freetype declarations */
#ifdef HAVE_FREETYPE_NO
  static void plD_pixel_wxwidgets( PLStream *pls, short x, short y );
  static void init_freetype_lv1( PLStream *pls );
  static void init_freetype_lv2( PLStream *pls );
  
  extern void plD_FreeType_init( PLStream *pls );
  extern void plD_render_freetype_text( PLStream *pls, EscText *args );
  extern void plD_FreeType_Destroy( PLStream *pls );
  extern void pl_set_extended_cmap0( PLStream *pls, int ncol0_width, int ncol0_org );
#endif

/* size of plot in pixels on screen */
#define PLOT_WIDTH 800
#define PLOT_HEIGHT 600

/* struct which contains information about device */
typedef struct {
  wxDC* dc;

  wxBitmap* bitmaps[512];
  int bitmap_counter;
  PLINT width;
  PLINT height;

  PLINT xmin;
  PLINT xmax;
  PLINT ymin;
  PLINT ymax;

	PLFLT scale;

  bool plstate_width;   // Flags indicating change of state before
  bool plstate_color0;  // device is fully initialized
  bool plstate_color1;  // taken from gcw driver

} wxPLdev;

/* wxwidgets function and class declarations */
class wxPLplotApp : public wxApp
{
public:
  virtual bool OnInit();
  void AddPLplotDevice( wxPLdev* dev )  { m_dev=dev; };
  wxPLdev* GetPLplotDevice( void )  { return m_dev; };

private:
  wxPLdev* m_dev;
};

class wxPLplotFrame : public wxFrame
{
public:
  wxPLplotFrame( const wxString& title );

  void OnQuit( wxCommandEvent& event );
  void NextPlot( wxCommandEvent& event );
  void OnPaint( wxPaintEvent& event );

private:
  int plot_counter;

  DECLARE_EVENT_TABLE()
};

enum { wxPL_Quit = wxID_EXIT, wxPL_Next=20000 };

BEGIN_EVENT_TABLE( wxPLplotFrame, wxFrame )
  EVT_MENU( wxPL_Quit, wxPLplotFrame::OnQuit )
  EVT_MENU( wxPL_Next, wxPLplotFrame::NextPlot )
  EVT_PAINT( wxPLplotFrame::OnPaint )
END_EVENT_TABLE()

IMPLEMENT_APP_NO_MAIN( wxPLplotApp ) 


/* workaround against warnings for unused variables */
static inline void Use(void *) { }
#define WX_SUPPRESS_UNUSED_WARN(x) Use(&x)


/************************************************************************************
 ** driver declarations
 ************************************************************************************/
 
/* Device info */
char* plD_DEVICE_INFO_wxwidgets = "wxwidgets:wxWidgets DC:1:wxwidgets:51:wxwidgets";

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


/* font look-up table, copied from aquaterm driver */

/* #define WX_N_Type1Lookup 30
const FCI_to_FontName_Table WX_Type1Lookup[WX_N_Type1Lookup] = {
	{0x10000000, "Arial"},
	{0x10000001, "Times-Roman"},
	{0x10000002, "Courier"},
	{0x10000003, "Times-Roman"},
	{0x10000004, "LucidaGrande Regular"},
	{0x10000010, "Arial-Oblique"},
	{0x10000011, "Times-Italic"},
	{0x10000012, "Courier-Oblique"},
	{0x10000013, "Times-Italic"},
	{0x10000014, "LucidaGrande Regular"},
	{0x10000020, "Arial-Oblique"},
	{0x10000021, "Times-Italic"},
	{0x10000022, "Courier-Oblique"},
	{0x10000023, "Times-Italic"},
	{0x10000024, "LucidaGrande Regular"},
	{0x10000100, "Arial-Bold"},
	{0x10000101, "Times-Bold"},
	{0x10000102, "Courier-Bold"},
	{0x10000103, "Times-Bold"},
	{0x10000104, "LucidaGrande Regular"},
	{0x10000110, "Arial-BoldOblique"},
	{0x10000111, "Times-BoldItalic"},
	{0x10000112, "Courier-BoldOblique"},
	{0x10000113, "Times-BoldItalic"},
	{0x10000114, "LucidaGrande Regular"},
	{0x10000120, "Arial-BoldOblique"},
	{0x10000121, "Times-BoldItalic"},
	{0x10000122, "Courier-BoldOblique"},
	{0x10000123, "Times-BoldItalic"},
	{0x10000124, "LucidaGrande Regular"}
}; */


/************************************************************************************
 ** debug functions
 ************************************************************************************/

// define if you want debug output
//#define _DEBUG

// print verbose message to stderr
void Log_Verbose( char *fmt, ... )
{
#ifdef _DEBUG
  va_list args;
  va_start( args, fmt );
  fprintf( stderr, "Verbose: " );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  va_end( args );
  fflush( stderr );
#endif
}


// print debug message to stderr
void Log_Debug( char *fmt, ... )
{
#ifdef _DEBUG
  va_list args;
  va_start( args, fmt );
  fprintf( stderr, "Debug: " );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  va_end( args );
  fflush( stderr );
#endif
}




/************************************************************************************
 ** wxwindows specific functions
 ************************************************************************************/
void new_bitmap( PLStream *pls )
{
  Log_Verbose( "new_bitmap" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  ((wxMemoryDC*)dev->dc)->SelectObject( wxNullBitmap );
  dev->bitmap_counter++;
  dev->bitmaps[dev->bitmap_counter-1] = new wxBitmap( dev->width, dev->height, -1 );
  ((wxMemoryDC*)dev->dc)->SelectObject( *(dev->bitmaps[dev->bitmap_counter-1]) );
}


void install_dc( PLStream *pls )
{
  Log_Verbose( "install_dc" );

#ifdef __WXMAC__
  ProcessSerialNumber psn;

  GetCurrentProcess( &psn );
  CPSEnableForegroundOperation( &psn );
  SetFrontProcess( &psn );
#endif

  wxInitialize();
  
  wxPLdev* dev = (wxPLdev*)pls->dev;
  dev->dc = new wxMemoryDC();
	new_bitmap( pls );
  plD_bop_wxwidgets( pls );

  // set origin
  dev->dc->SetDeviceOrigin( (int)(dev->xmin*dev->scale), (int)(dev->height-(dev->ymin*dev->scale)) );
  
  // set axis orientation
  dev->dc->SetAxisOrientation( true, true );    // x-axis from left to right, y-axis from bottom to top
}
 

int wxPLEntry( int& argc, const wxChar **argv )
{
  Log_Verbose( "Own wxPLEntry" );

  wxLog::GetActiveTarget();

  wxTRY
  {
    if ( !wxTheApp->CallOnInit() )
      return -1;

    class CallOnExit
    {
    public:
      ~CallOnExit() { wxTheApp->OnExit(); }
    } callOnExit;
    WX_SUPPRESS_UNUSED_WARN(callOnExit);

    return wxTheApp->OnRun();
  }
  wxCATCH_ALL( wxTheApp->OnUnhandledException(); return -1; )
}


bool wxPLplotApp::OnInit()
{
  Log_Verbose( "wxPLplotApp::OnInit" );

  wxPLplotFrame* frame = new wxPLplotFrame( _T("wxWidgets PLplot App") );
  frame->SetClientSize( PLOT_WIDTH, PLOT_HEIGHT );
  frame->Show( true );
  frame->Raise();
	
  return true;
}


wxPLplotFrame::wxPLplotFrame( const wxString& title )
             : wxFrame( NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
                        wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION |
                        wxCLOSE_BOX | wxCLIP_CHILDREN )
{
  Log_Verbose( "wxPLplotFrame::wxPLplotFrame" );

  plot_counter=1;
    
  wxMenu* fileMenu = new wxMenu;
  fileMenu->Append( wxPL_Next, _T("&Next plot\tCtrl-N"), _T("Next plot") );
  fileMenu->Append( wxPL_Quit, _T("E&xit\tAlt-X"), _T("Quit this program") );

  wxMenuBar* menuBar = new wxMenuBar();
  menuBar->Append( fileMenu, _T("&File") );
  SetMenuBar(menuBar);
}


void wxPLplotFrame::OnQuit( wxCommandEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::OnQuit" );

  wxGetApp().ExitMainLoop();
}


void wxPLplotFrame::NextPlot( wxCommandEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::NextPlot" );
    
  wxPLdev* dev = wxGetApp().GetPLplotDevice();

  if( ++plot_counter > dev->bitmap_counter-1 )
    plot_counter = 1;

  Refresh();
}


void wxPLplotFrame::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::OnPaint" );

  int width, height;
  GetClientSize( &width, &height );

  wxPLdev* dev = wxGetApp().GetPLplotDevice();
  
  wxMemoryDC MemoryDC;
  MemoryDC.SelectObject( *(dev->bitmaps[plot_counter-1]) );  
  SetTitle( wxString::Format( _("wxWidgets PLplot App - Plot %d of %d"), plot_counter, dev->bitmap_counter-1 ) );
  
  wxPaintDC dc(this);
  dc.BeginDrawing();
  dc.Blit( 0, 0, dev->width, dev->height, &MemoryDC, 0, 0 );
  dc.EndDrawing();
  
  MemoryDC.SelectObject( wxNullBitmap );  
}
 

/************************************************************************************
 ** driver functions
 ************************************************************************************/

void plD_dispatch_init_wxwidgets( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr  = "wxWidgets DC";
    pdt->pl_DevName  = "wxwidgets";
#endif
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 51;
    pdt->pl_init     = (plD_init_fp)     plD_init_wxwidgets;
    pdt->pl_line     = (plD_line_fp)     plD_line_wxwidgets;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_wxwidgets;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_wxwidgets;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_wxwidgets;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_wxwidgets;
    pdt->pl_state    = (plD_state_fp)    plD_state_wxwidgets;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_wxwidgets;
}


/*--------------------------------------------------------------------------*\
 * plD_init_wxwin()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/
void
plD_init_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_init_wxwidgets()" );

  /* Default dimensions of the canvas (in inches) */
	static PLFLT canvas_width = 9.0;
	static PLFLT canvas_height;  // defined later

  wxPLdev* dev;

#ifdef HAVE_FREETYPE_NO
  static int freetype=1;
  static int smooth_text=1;
  FT_Data *FT;
#endif
  
  pls->color = 1;		/* Is a color device */
  pls->dev_fill0 = 1;		/* Handle solid fills */
  pls->dev_fill1 = 0;		/* Handle pattern fills */
  pls->dev_dash = 0;

  /* Set the initialization state monitors to false */
  dev->plstate_width = false;
  dev->plstate_color0 = false;
  dev->plstate_color1 = false;

  // be verbose and write out debug messages
#ifdef _DEBUG
  pls->verbose = 1;
  pls->debug = 1;
#else
  pls->verbose = 0;
  pls->debug = 0;    
#endif    

#ifdef HAVE_FREETYPE_NO
  if( freetype ) {
    pls->dev_text = 1; /* want to draw text */
    pls->dev_unicode = 1; /* want unicode */

    init_freetype_lv1(pls);
    FT=(FT_Data *)pls->FT;
    FT->want_smooth_text=smooth_text;
  }
#endif
  
  /* allocate memory for the device storage */
  if( (dev = (wxPLdev* )malloc(sizeof(wxPLdev))) == NULL) {
    fprintf(stdout,"Insufficient memory\n");
    exit(0);
  }
  memset( dev, 0, sizeof(wxPLdev) );
  pls->dev = (void*)dev;
  
  /* initialize device storage */
  dev->dc = NULL;
  if( pls->xlength <= 0 || pls->ylength <=0 ) {
		pls->xlength = PLOT_WIDTH;
		pls->ylength = PLOT_HEIGHT;
  }

  dev->width=pls->xlength;
  dev->height=pls->ylength;
  dev->bitmap_counter=0;
  
  canvas_width = 9.0;
  canvas_height = pls->ylength*canvas_width/pls->xlength;
  
  /* Set the number of pixels per mm */
  plP_setpxl( (PLFLT)PIXELS_PER_MM, (PLFLT)PIXELS_PER_MM );

  /* Set up physical limits of plotting device (in drawing units) */
  plP_setphy( (PLINT)0, (PLINT)(canvas_width*DU_PER_IN),
	            (PLINT)0, (PLINT)(canvas_height*DU_PER_IN) );

  // get physical device limits coordinates
  plP_gphy( &dev->xmin, &dev->xmax, &dev->ymin, &dev->ymax );

  // setting scale factors
  dev->scale=(PLFLT)(dev->width)/(dev->xmax-dev->xmin);
	
#ifdef HAVE_FREETYPE_NO
  if( pls->dev_text )
    init_freetype_lv2( pls );
#endif
}


/*--------------------------------------------------------------------------*\
 * The remaining driver functions are to be filled with code.
\*--------------------------------------------------------------------------*/

void plD_line_wxwidgets( PLStream *pls, short x1a, short y1a, short x2a, short y2a )
{
  //Log_Verbose( "plD_line_wxwidgets(x1a=%d, y1a=%d, x2a=%d, y2a=%d)", x1a, y1a, x2a, y2a );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc == NULL )
    install_dc( pls );
  
  dev->dc->DrawLine( (wxCoord) (x1a*dev->scale), (wxCoord) (y1a*dev->scale),
                     (wxCoord) (x2a*dev->scale), (wxCoord) (y2a*dev->scale) );
}


void plD_polyline_wxwidgets( PLStream *pls, short *xa, short *ya, PLINT npts )
{
  //Log_Verbose( "plD_polyline_wxwidgets()" );

  // should be changed to use the wxDC::DrawLines function?
  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc == NULL )
    install_dc( pls );

  dev->dc->BeginDrawing();
  for( PLINT i=1; i<npts; i++ )
    dev->dc->DrawLine( (wxCoord) (xa[i-1]*dev->scale), (wxCoord) (ya[i-1]*dev->scale),
                       (wxCoord) (xa[i]*dev->scale), (wxCoord) (ya[i]*dev->scale) );
  dev->dc->EndDrawing();
}


void plD_eop_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_eop_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->bitmap_counter > 0 )
  	new_bitmap( pls );
}


void plD_bop_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_bop_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc != NULL ) {
    // clear background (with black colour)
    dev->dc->SetBackground( *wxBLACK_BRUSH );
    dev->dc->Clear();
  
    /* Replay escape calls that come in before PLESC_DEVINIT.  All of them
     * required a DC that didn't exist yet.
     */
    if( dev->plstate_width )
      plD_state_wxwidgets( pls, PLSTATE_WIDTH );
    if( dev->plstate_color0 )
      plD_state_wxwidgets( pls, PLSTATE_COLOR0 );
    if( dev->plstate_color1 )
      plD_state_wxwidgets( pls, PLSTATE_COLOR1 );
    dev->plstate_width = false;
    dev->plstate_color0 = false;
    dev->plstate_color1 = false;

		// why this? xwin driver has this
	  //pls->page++;
	}
}


void plD_tidy_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_tidy_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

#ifdef HAVE_FREETYPE_NO
  if( pls->dev_text ) {
    FT_Data *FT=(FT_Data *)pls->FT;
    plscmap0n( FT->ncol0_org );
    plD_FreeType_Destroy( pls );
  }
#endif
  
  if( dev->bitmap_counter > 0 ) {
    Log_Verbose( "Calling wxEntry()..." );
    ((wxMemoryDC*)dev->dc)->SelectObject( wxNullBitmap );
    delete dev->dc;
    wxGetApp().AddPLplotDevice( dev );

#ifdef __WXMSW__    
    wxSetInstance( GetModuleHandle(NULL) );
    wxApp::m_nCmdShow = 0;
    // wxEntry( GetModuleHandle(NULL), NULL, NULL, 0 );
#endif
    int argc=1;
    const wxChar* argv[2];
    argv[0] = _("test");
    argv[1] = NULL;
#ifdef __WXMSW__    
    wxEntry( argc, argv );
#else
    wxPLEntry( argc, argv );
#endif
      
    Log_Verbose( "After wxEntry()..." );
    for( int i=0; i<dev->bitmap_counter; i++ )
      delete dev->bitmaps[i];
    Log_Verbose( "delete bitmaps" );
      
    wxUninitialize();
    Log_Verbose( "wxUninitialize()" );
  }
}


void plD_state_wxwidgets( PLStream *pls, PLINT op )
{
  //Log_Verbose( "plD_state_wxwidgets(op=%d)", op );

  wxPLdev* dev = (wxPLdev*)pls->dev;
  int width;

  switch( op ) {
  case PLSTATE_WIDTH:  // 1
    if( dev->dc!=NULL)
      dev->dc->SetPen( wxPen(wxColour(pls->curcolor.r, pls->curcolor.g,
                                      pls->curcolor.b),
                             pls->width) );
    else
      dev->plstate_width = true;
    break;
  case PLSTATE_COLOR0:  // 2
    if( dev->dc!=NULL) {
      dev->dc->SetPen( wxPen(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
                                      pls->cmap0[pls->icol0].b),
                             pls->width) );
      dev->dc->SetBrush( wxBrush(wxColour(pls->cmap0[pls->icol0].r, pls->cmap0[pls->icol0].g,
                                          pls->cmap0[pls->icol0].b)) );
    } else
      dev->plstate_color0 = true;
    break;
  case PLSTATE_COLOR1:  // 3
    if( dev->dc!=NULL) {
      dev->dc->SetPen( wxPen(wxColour(pls->curcolor.r, pls->curcolor.g,
                                      pls->curcolor.b),
                             pls->width) );
      dev->dc->SetBrush( wxBrush(wxColour(pls->curcolor.r, pls->curcolor.g,
                                          pls->curcolor.b)) );
    } else 
      dev->plstate_color1 = true;
    break;
  default:
    if( dev->dc==NULL)
      install_dc( pls );
  }
}

void plD_esc_wxwidgets( PLStream *pls, PLINT op, void *ptr )
{
  Log_Verbose( "plD_esc_wxwidgets(op=%d, ptr=%x)", op, ptr );

  switch (op) {

  case PLESC_FILL:
    fill_polygon( pls );
    break;

  case PLESC_DEVINIT:
    wx_set_dc( pls, (wxDC*)ptr );
    break;

#ifdef HAVE_FREETYPE_NO
  case PLESC_HAS_TEXT:
    plD_render_freetype_text( pls, (EscText *)ptr );
    break;
#endif

  default:
    break;
  }
}

/*--------------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*--------------------------------------------------------------------------*/

static void fill_polygon( PLStream *pls )
{
  Log_Verbose( "fill_polygon(), npts=%d, x[0]=%d, y[0]=%d", pls->dev_npts, pls->dev_y[0], pls->dev_y[0] );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc != NULL) {
    wxPoint *points = new wxPoint[pls->dev_npts];
  
    for( int i=0; i < pls->dev_npts; i++ ) {
      points[i].x=(int) (pls->dev_x[i]*dev->scale);
      points[i].y=(int) (pls->dev_y[i]*dev->scale);
    }
  
    dev->dc->DrawPolygon( pls->dev_npts, points );
    delete[] points;
  }
}


/*--------------------------------------------------------------------------*\
 * wx_set_dc()
 *
 * Adds a dc to the newly created stream.  
 *
 * The associated device is attached to the canvas as the property "dev".
\*--------------------------------------------------------------------------*/
void wx_set_dc( PLStream* pls, wxDC* dc )
{
  Log_Verbose( "wx_set_dc()" );
  
  wxPLdev* dev = (wxPLdev*)pls->dev;
  dev->dc=dc;  /* Add the dc to the device */

  /* initialize device storage */
  if( pls->xlength <= 0 || pls->ylength <=0 ) {
		pls->xlength = PLOT_WIDTH;
		pls->ylength = PLOT_HEIGHT;
  }

  dev->width=pls->xlength;
  dev->height=pls->ylength;
  
  PLFLT canvas_width = 9.0;
  PLFLT canvas_height = pls->ylength*canvas_width/pls->xlength;
  
  /* Set up physical limits of plotting device (in drawing units) */
  plP_setphy( (PLINT)0, (PLINT)(canvas_width*DU_PER_IN),
	            (PLINT)0, (PLINT)(canvas_height*DU_PER_IN) );

  // get physical device limits coordinates
  plP_gphy( &dev->xmin, &dev->xmax, &dev->ymin, &dev->ymax );

  // setting scale factors
  dev->scale=(PLFLT)(dev->width)/(dev->xmax-dev->xmin);

  // set origin
  dev->dc->SetDeviceOrigin( (int)(dev->xmin*dev->scale), (int)(dev->height-(dev->ymin*dev->scale)) );
  
  // set axis orientation
  dev->dc->SetAxisOrientation( true, true );    // x-axis from left to right, y-axis from bottom to top
}

#ifdef HAVE_FREETYPE_NO

/*----------------------------------------------------------------------*\
 *  void plD_pixel_wxwidgets (PLStream *pls, short x, short y)
 *
 *  callback function, of type "plD_pixel_fp", which specifies how a single
 *  pixel is set in the current colour.
\*----------------------------------------------------------------------*/
void plD_pixel_wxwidgets( PLStream *pls, short x, short y )
{
  Log_Verbose( "plD_pixel_wxwidgets" );

  wxPLdev *dev=(wxPLdev*)pls->dev;
  dev->dc->DrawPoint( x, y );
}


/*----------------------------------------------------------------------*\
 *  void init_freetype_lv1 (PLStream *pls)
 *
 *  "level 1" initialisation of the freetype library.
 *  "Level 1" initialisation calls plD_FreeType_init(pls) which allocates
 *  memory to the pls->FT structure, then sets up the pixel callback
 *  function.
\*----------------------------------------------------------------------*/
static void init_freetype_lv1( PLStream *pls )
{
  Log_Verbose( "init_freetype_lv1" );

	FT_Data *FT;

	plD_FreeType_init( pls );

	FT=(FT_Data *)pls->FT;
	FT->pixel = (plD_pixel_fp)plD_pixel_wxwidgets;
}

/*----------------------------------------------------------------------*\
 *  void init_freetype_lv2 (PLStream *pls)
 *
 *  "Level 2" initialisation of the freetype library.
 *  "Level 2" fills in a few setting that aren't public until after the
 *  graphics sub-syetm has been initialised.
 *  The "level 2" initialisation fills in a few things that are defined
 *  later in the initialisation process for the GD driver.
 *
 *  FT->scale is a scaling factor to convert co-ordinates. This is used by
 *  the GD and other drivers to scale back a larger virtual page and this
 *  eliminate the "hidden line removal bug". Set it to 1 if your device
 *  doesn't have scaling.
 *
 *  Some coordinate systems have zero on the bottom, others have zero on
 *  the top. Freetype does it one way, and most everything else does it the
 *  other. To make sure everything is working ok, we have to "flip" the
 *  coordinates, and to do this we need to know how big in the Y dimension
 *  the page is, and whether we have to invert the page or leave it alone.
 *
 *  FT->ymax specifies the size of the page FT->invert_y=1 tells us to
 *  invert the y-coordinates, FT->invert_y=0 will not invert the
 *  coordinates.
\*----------------------------------------------------------------------*/

static void init_freetype_lv2( PLStream *pls )
{
  Log_Verbose( "init_freetype_lv2" );

  wxPLdev *dev=(wxPLdev *)pls->dev;
  FT_Data *FT=(FT_Data *)pls->FT;
  
  FT->scale=*dev->scale;   
  FT->ymax=dev->ymax;  // this should be the right parameter
  FT->invert_y=1;
  FT->smooth_text=0;
  
  if (FT->want_smooth_text==1)    /* do we want to at least *try* for smoothing ? */
  {
    FT->ncol0_org=pls->ncol0;                                   /* save a copy of the original size of ncol0 */
    FT->ncol0_xtra=16777216-(pls->ncol1+pls->ncol0);            /* work out how many free slots we have */
    FT->ncol0_width=FT->ncol0_xtra/(pls->ncol0-1);              /* find out how many different shades of anti-aliasing we can do */
    if (FT->ncol0_width>4)     /* are there enough colour slots free for text smoothing ? */
       {
        if (FT->ncol0_width>max_number_of_grey_levels_used_in_text_smoothing)
           FT->ncol0_width=max_number_of_grey_levels_used_in_text_smoothing;                 /* set a maximum number of shades */
        plscmap0n(FT->ncol0_org+(FT->ncol0_width*pls->ncol0));      /* redefine the size of cmap0 */
    /* the level manipulations are to turn off the plP_state(PLSTATE_CMAP0)
    * call in plscmap0 which (a) leads to segfaults since the GD image is
    * not defined at this point and (b) would be inefficient in any case since
    * setcmap is always called later (see plD_bop_png) to update the driver
    * color palette to be consistent with cmap0. */
         {
          PLINT level_save;
          level_save = pls->level;
          pls->level = 0;
          pl_set_extended_cmap0(pls, FT->ncol0_width, FT->ncol0_org); /* call the function to add the extra cmap0 entries and calculate stuff */
          pls->level = level_save;
         }
        FT->smooth_text=1;      /* Yippee ! We had success setting up the extended cmap0 */
      }
    else
      plwarn("Insufficient colour slots available in CMAP0 to do text smoothing.");
  }
}

#endif

#else
int pldummy_wxwidgets()
{
    return 0;
}

#endif				/* PLD_wxwin */



