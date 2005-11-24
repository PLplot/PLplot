/* $Id$

   Copyright (C) 2005  Werner Smekal, Sjaak Verdoold
   Copyright (C) 2005  Germain Carrera Corraleche
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
 *) x02c gives a lot of warnings regarding the color palette.
    same for gd driver though. Has to do with freetype support.
 *) Freetype: font doesn't resize if plot is resized.  plfreetype
    patch being evaluated to fix this.
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
#ifdef HAVE_FREETYPE
  #include "plfreetype.h"

  static void plD_pixel_wxwidgets( PLStream *pls, short x, short y );
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
#define DEVICE_PIXELS_PER_MM (3.4)
#define VIRTUAL_PIXELS_PER_MM (DEVICE_PIXELS_PER_MM*VSCALE)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per inch */
#define DEVICE_PIXELS_PER_IN (DEVICE_PIXELS_PER_MM*MM_PER_IN)
#define VIRTUAL_PIXELS_PER_IN (VIRTUAL_PIXELS_PER_MM*MM_PER_IN)

/* Default dimensions of the canvas (in inches) */
#define CANVAS_WIDTH (10.)
#define CANVAS_HEIGHT (7.5)

/* size of plot in pixels on screen if not given */
#define PLOT_WIDTH 800
#define PLOT_HEIGHT 600
/*=========================================================================*/

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


/* wxwidgets application definition (if needed) */
class wxPLplotApp : public wxApp
{
public:
  virtual bool OnInit();
  void AddPLplotDevice( wxPLdev* dev )  { m_dev=dev; };
  wxPLdev* GetPLplotDevice( void )  { return m_dev; };

private:
  wxPLdev* m_dev;  /* app needs to know this structure */
};

/* definition of the actual window/frame shown */
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

/* event table */
BEGIN_EVENT_TABLE( wxPLplotFrame, wxFrame )
  EVT_MENU( wxPL_Quit, wxPLplotFrame::OnQuit )      /* quit program */
  EVT_MENU( wxPL_Next, wxPLplotFrame::NextPlot )    /* show next plot */
  EVT_PAINT( wxPLplotFrame::OnPaint )               /* (re)draw the plot in window */
END_EVENT_TABLE()

/* declare application without providing a main()/WinMain() function */
IMPLEMENT_APP_NO_MAIN( wxPLplotApp ) 

/* workaround against warnings for unused variables */
static inline void Use(void *) { }
#define WX_SUPPRESS_UNUSED_WARN( x ) Use( &x )

/* private functions needed by the wxwidgets Driver */
void new_bitmap( PLStream *pls );
void install_dc( PLStream *pls );
int wxPLEntry( int& argc, const wxChar **argv );




/*----------------------------------------------------------------------*\
 *  Declarations for the device.
\*----------------------------------------------------------------------*/

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
void wx_set_size( PLStream* pls, int width, int height );
int plD_errorexithandler_wxwidgets( char *errormessage );
void plD_erroraborthandler_wxwidgets( char *errormessage );



/*----------------------------------------------------------------------*\
 *  Debug functions
\*----------------------------------------------------------------------*/

/* define if you want debug output */
//#define _DEBUG
//#define _DEBUG_VERBOSE

/*--------------------------------------------------------------------------*\
 *  void Log_Verbose( char *fmt, ... )
 *
 *  Print verbose debug message to stderr (printf style).
\*--------------------------------------------------------------------------*/
void Log_Verbose( char *fmt, ... )
{
#ifdef _DEBUG_VERBOSE
  va_list args;
  va_start( args, fmt );
  fprintf( stderr, "Verbose: " );
  vfprintf( stderr, fmt, args );
  fprintf( stderr, "\n" );
  va_end( args );
  fflush( stderr );
#endif
}


/*--------------------------------------------------------------------------*\
 *  void Log_Debug( char *fmt, ... )
 *
 *  Print debug message to stderr (printf style).
\*--------------------------------------------------------------------------*/
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





/*----------------------------------------------------------------------*\
 *  In the following you'll find the driver functions which are
 *  are needed by the plplot core.
\*----------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*\
 *  void plD_dispatch_init_wxwidgets( PLDispatchTable *pdt )
 *
 *  Make driver functions known to plplot.
\*--------------------------------------------------------------------------*/
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
 *  plD_init_wxwidgets()
 *
 *  Initialize device.
\*--------------------------------------------------------------------------*/
void
plD_init_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_init_wxwidgets()" );

  wxPLdev* dev;

#ifdef HAVE_FREETYPE
  static int freetype=1;
  static int smooth_text=1;
#endif

#ifdef HAVE_FREETYPE
  DrvOpt wx_options[] = {{"text", DRV_INT, &freetype, "Use driver text (FreeType)"},
                         {"smooth", DRV_INT, &smooth_text, "Turn text smoothing on (1) or off (0)"},
              			     {NULL, DRV_INT, NULL, NULL}};
#endif

  pls->color = 1;		/* Is a color device */
  pls->dev_fill0 = 1;		/* Can handle solid fills */
  pls->dev_fill1 = 0;		/* Can't handle pattern fills */
  pls->dev_dash = 0;
  pls->plbuf_write = 1;    /* use the plot buffer! */

// be verbose and write out debug messages
#ifdef _DEBUG
  pls->verbose = 1;
  pls->debug = 1;
#else
  pls->verbose = 0;
  pls->debug = 0;    
#endif    

  /* allocate memory for the device storage */
  if( (dev = (wxPLdev* )malloc(sizeof(wxPLdev))) == NULL) {
    fprintf( stdout, "Insufficient memory\n" );
    exit( 0 );
  }
  memset( dev, 0, sizeof(wxPLdev) );
  pls->dev = (void*)dev;
  
#ifdef HAVE_FREETYPE
  /* Check for and set up driver options */
  plParseDrvOpts( wx_options );

  if( freetype ) {
    pls->dev_text = 1; /* want to draw text */
    pls->dev_unicode = 1; /* want unicode */

    init_freetype_lv1( pls );
    FT_Data* FT=(FT_Data *)pls->FT;
    FT->want_smooth_text=smooth_text;
  }
#endif  
  
  /* initialize device storage */
  dev->dc = NULL;
  if( pls->xlength <= 0 || pls->ylength <=0 )
    plspage( 0.0, 0.0, (PLINT)(CANVAS_WIDTH*DEVICE_PIXELS_PER_IN), (PLINT)(CANVAS_HEIGHT*DEVICE_PIXELS_PER_IN), 0, 0 );

  dev->width=pls->xlength;
  dev->height=pls->ylength;
  dev->bitmap_counter=0;

  /* If portrait mode, apply a rotation and set freeaspect */
  if( pls->portrait ) {
    plsdiori( (PLFLT)(4 - ORIENTATION) );
    pls->freeaspect = 1;
  }
  
  Log_Verbose( "settings" );

  /* Set the number of pixels per mm */
  plP_setpxl( (PLFLT)VIRTUAL_PIXELS_PER_MM, (PLFLT)VIRTUAL_PIXELS_PER_MM );

  /* Set up physical limits of plotting device (in drawing units) */
  plP_setphy( (PLINT)0, (PLINT)(CANVAS_WIDTH*VIRTUAL_PIXELS_PER_IN),
	            (PLINT)0, (PLINT)(CANVAS_HEIGHT*VIRTUAL_PIXELS_PER_IN) );

  // get physical device limits coordinates
  plP_gphy( &dev->xmin, &dev->xmax, &dev->ymin, &dev->ymax );

  // setting scale factors
  if( ((PLFLT)dev->width/dev->height) < ((PLFLT)CANVAS_WIDTH/CANVAS_HEIGHT) )
    dev->scale=(PLFLT)(dev->xmax-dev->xmin)/(dev->width);
  else
    dev->scale=(PLFLT)(dev->ymax-dev->ymin)/(dev->height);  

  /* set dpi */    
  plspage(VIRTUAL_PIXELS_PER_IN/dev->scale, VIRTUAL_PIXELS_PER_IN/dev->scale, 0, 0, 0, 0);

  /* Set the initialization state monitors to false */
  dev->plstate_width = false;
  dev->plstate_color0 = false;
  dev->plstate_color1 = false;

	/* Set wx error handler for various errors in plplot*/
  //plsexit(plD_errorexithandler_wxwidgets);
  //plsabort(plD_erroraborthandler_wxwidgets);
  
#ifdef HAVE_FREETYPE
  if( pls->dev_text )
    init_freetype_lv2(pls);
#endif

}


/*--------------------------------------------------------------------------*\
 *  void plD_line_wxwidgets( PLStream *pls, short x1a, short y1a,
 *													 short x2a, short y2a )
 *
 *  Draws a line from (x1a, y1a) to (x2a, y2a).
\*--------------------------------------------------------------------------*/
void plD_line_wxwidgets( PLStream *pls, short x1a, short y1a, short x2a, short y2a )
{
  Log_Verbose( "plD_line_wxwidgets(x1a=%d, y1a=%d, x2a=%d, y2a=%d)", x1a, y1a, x2a, y2a );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc == NULL )
    install_dc( pls );
  
  Log_Verbose( "plD_line_wxwidgets(x1a=%d, y1a=%d, x2a=%d, y2a=%d)", (wxCoord)(x1a/dev->scale),
							 (wxCoord)(dev->height-y1a/dev->scale), (wxCoord)(x2a/dev->scale), (wxCoord)(dev->height-y2a/dev->scale) );
  dev->dc->DrawLine( (wxCoord) (x1a/dev->scale), (wxCoord) (dev->height-y1a/dev->scale),
                     (wxCoord) (x2a/dev->scale), (wxCoord) (dev->height-y2a/dev->scale) );
}


/*--------------------------------------------------------------------------*\
 *  void plD_polyline_wxwidgets( PLStream *pls, short *xa, short *ya,
 *															 PLINT npts )
 *
 *  Draw a poly line - points are in xa and ya arrays.
\*--------------------------------------------------------------------------*/
void plD_polyline_wxwidgets( PLStream *pls, short *xa, short *ya, PLINT npts )
{
  Log_Verbose( "plD_polyline_wxwidgets()" );

  // should be changed to use the wxDC::DrawLines function?
  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc == NULL )
    install_dc( pls );

  dev->dc->BeginDrawing();
  for( PLINT i=1; i<npts; i++ )
    dev->dc->DrawLine( (wxCoord) (xa[i-1]/dev->scale), (wxCoord) (dev->height-ya[i-1]/dev->scale),
                       (wxCoord) (xa[i]/dev->scale), (wxCoord) (dev->height-ya[i]/dev->scale) );
  dev->dc->EndDrawing();
}


/*--------------------------------------------------------------------------*\
 *  void plD_eop_wxwidgets( PLStream *pls )
 *
 *  End of Page. This function is called if a "end of page" is send by the
 *  user. This command is ignored if we have the plot embedded in a 
 *  wxWidgets application, but a new bitmap is created if we use a
 *  command line executable.
\*--------------------------------------------------------------------------*/
void plD_eop_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_eop_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->bitmap_counter > 0 )
  	new_bitmap( pls );
}


/*--------------------------------------------------------------------------*\
 *  void plD_bop_wxwidgets( PLStream *pls )
 *
 *  Begin of page. Before any plot command, this function is called, If we
 *  have already a dc the background is cleared in background color and some
 *  state calls are resent - this is because at the first call of this
 *  function, a dc does most likely not exist, but state calls are recorded
 *  and when a new dc is created this function is called again.
\*--------------------------------------------------------------------------*/
void plD_bop_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_bop_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc != NULL ) {
    // clear background
		PLINT bgr, bgg, bgb;  // red, green, blue
		plgcolbg( &bgr, &bgg, &bgb);  // get background color information
    dev->dc->SetBackground( wxBrush(wxColour(bgr, bgg, bgb)) );
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


/*--------------------------------------------------------------------------*\
 *  void plD_tidy_wxwidgets( PLStream *pls )
 *
 *  This function is called, if all plots are done. If this driver is used
 *  from a command line executable, we actually let wxWidgets take over 
 *  control - a window'll be opened and the plots are shown.
\*--------------------------------------------------------------------------*/
void plD_tidy_wxwidgets( PLStream *pls )
{
  Log_Verbose( "plD_tidy_wxwidgets()" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

#ifdef HAVE_FREETYPE
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
#endif
    int argc=1;
    const wxChar* argv[2];
    argv[0] = _("test");
    argv[1] = NULL;
    wxPLEntry( argc, argv );
      
    Log_Verbose( "After wxEntry()..." );
    for( int i=0; i<dev->bitmap_counter; i++ )
      delete dev->bitmaps[i];
    Log_Verbose( "delete bitmaps" );
      
    wxUninitialize();
    Log_Verbose( "wxUninitialize()" );
  }
}


/*--------------------------------------------------------------------------*\
 *  void plD_state_wxwidgets( PLStream *pls, PLINT op )
 *
 *  Handler for several state codes. Here we take care of setting the width
 *  and color of the pen.
\*--------------------------------------------------------------------------*/
void plD_state_wxwidgets( PLStream *pls, PLINT op )
{
  Log_Verbose( "plD_state_wxwidgets(op=%d)", op );

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


/*--------------------------------------------------------------------------*\
 *  void plD_esc_wxwidgets( PLStream *pls, PLINT op, void *ptr )
 *
 *  Handler for several escape codes. Here we take care of filled polygons,
 *  XOR or copy mode, initialize device (install dc from outside), and if 
 *  there is freetype support, rerendering of text.
\*--------------------------------------------------------------------------*/
void plD_esc_wxwidgets( PLStream *pls, PLINT op, void *ptr )
{
  Log_Verbose( "plD_esc_wxwidgets(op=%d, ptr=%x)", op, ptr );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  switch (op) {

  case PLESC_FILL:
    fill_polygon( pls );
    break;

	case PLESC_XORMOD:
		// switch between wxXOR and wxCOPY
    if( dev->dc!=NULL) {
			if( dev->dc->GetLogicalFunction() == wxCOPY )
				dev->dc->SetLogicalFunction( wxXOR );    
			else if( dev->dc->GetLogicalFunction() == wxXOR )
				dev->dc->SetLogicalFunction( wxCOPY );
		}
		break;

	case PLESC_DEVINIT:
    wx_set_dc( pls, (wxDC*)ptr );
    break;

#ifdef HAVE_FREETYPE
  case PLESC_HAS_TEXT:
    plD_render_freetype_text( pls, (EscText *)ptr );
    break;
#endif

  case PLESC_RESIZE: {
    	wxSize* size=(wxSize*)ptr;
    	wx_set_size( pls, size->GetWidth(), size->GetHeight() );
		}
    break;

	case PLESC_CLEAR: {
			PLINT bgr, bgg, bgb;  // red, green, blue
			plgcolbg( &bgr, &bgg, &bgb );  // get background color information
    	dev->dc->SetBackground( wxBrush(wxColour(bgr, bgg, bgb)) );
    	dev->dc->Clear();
		}
		break;
  
  default:
    break;
  }
}


/*--------------------------------------------------------------------------*\
 *  static void fill_polygon( PLStream *pls )
 *
 *  Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*--------------------------------------------------------------------------*/
static void fill_polygon( PLStream *pls )
{
  Log_Verbose( "fill_polygon(), npts=%d, x[0]=%d, y[0]=%d", pls->dev_npts, pls->dev_y[0], pls->dev_y[0] );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  if( dev->dc != NULL) {
    wxPoint *points = new wxPoint[pls->dev_npts];
  
    for( int i=0; i < pls->dev_npts; i++ ) {
      points[i].x=(int) (pls->dev_x[i]/dev->scale);
      points[i].y=(int) (dev->height-pls->dev_y[i]/dev->scale);
    }
  
    dev->dc->DrawPolygon( pls->dev_npts, points );
    delete[] points;
  }
}


/*--------------------------------------------------------------------------*\
 *  void wx_set_size( PLStream* pls, int width, int height )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wx_set_size( PLStream* pls, int width, int height )
{
  Log_Verbose( "wx_set_size()" );
  
  wxPLdev* dev = (wxPLdev*)pls->dev;

  // clear background if we have a dc, since it's invalid (TODO: why, since in bop
  // it must be cleared anyway?)
  if( dev->dc != NULL ) {
		PLINT bgr, bgg, bgb;  // red, green, blue
		plgcolbg( &bgr, &bgg, &bgb);  // get background color information
    dev->dc->SetBackground( wxBrush(wxColour(bgr, bgg, bgb)) );
    dev->dc->Clear();
	}

	// set new size
  dev->width = width;
  dev->height = height;

  /*set scale factor */
  if( ((PLFLT)dev->width/dev->height) < ((PLFLT)CANVAS_WIDTH/CANVAS_HEIGHT) )
    dev->scale=(PLFLT)(dev->xmax-dev->xmin)/dev->width;
  else
    dev->scale=(PLFLT)(dev->ymax-dev->ymin)/dev->height;  
    
    /* recalculate the dpi used in calculation of fontsize */
  pls->xdpi = VIRTUAL_PIXELS_PER_IN/dev->scale;
  pls->ydpi = VIRTUAL_PIXELS_PER_IN/dev->scale;
    
  // freetype parameters must also be changed
#ifdef HAVE_FREETYPE  
  FT_Data *FT=(FT_Data *)pls->FT;
  FT->scale=dev->scale;   
  FT->ymax=dev->height; 
#endif
}


/*--------------------------------------------------------------------------*\
 *  void wx_set_dc( PLStream* pls, wxDC* dc )
 *
 *  Adds a dc to the stream. The associated device is attached to the canvas
 *  as the property "dev".
\*--------------------------------------------------------------------------*/
void wx_set_dc( PLStream* pls, wxDC* dc )
{
  Log_Verbose( "wx_set_dc()" );
  
  wxPLdev* dev = (wxPLdev*)pls->dev;
  dev->dc=dc;  /* Add the dc to the device */

	/* replay begin of page call and state settings */
	plD_bop_wxwidgets( pls );
}


/*----------------------------------------------------------------------*\
 *  int plD_errorexithandler_wxwidgets( char *errormessage )
 *
 *  If an PLplot error occurs, this function shows a dialog regarding
 *  this error and than exits.
\*----------------------------------------------------------------------*/
int plD_errorexithandler_wxwidgets( char *errormessage )
{  
	wxMessageDialog dialog( 0,wxString(errormessage, *wxConvCurrent),wxString("wxPlot error",*wxConvCurrent),wxOK );
	dialog.ShowModal();
	plend();
	return 0;
}

/*----------------------------------------------------------------------*\
 *  void plD_erroraborthandler_wxwidgets( char *errormessage )
 *
 *  If an PLplot error occurs, this function shows a dialog regarding
 *  this error.
\*----------------------------------------------------------------------*/
void plD_erroraborthandler_wxwidgets( char *errormessage )
{  
	wxMessageDialog dialog( 0,(wxString(errormessage, *wxConvCurrent)+ wxString(" aborting operation...", *wxConvCurrent)), wxString("wxPlot error",*wxConvCurrent), wxOK );
  dialog.ShowModal();
}


#ifdef HAVE_FREETYPE

/*----------------------------------------------------------------------*\
 *  static void plD_pixel_wxwidgets( PLStream *pls, short x, short y )
 *
 *  callback function, of type "plD_pixel_fp", which specifies how a single
 *  pixel is set in the current colour.
\*----------------------------------------------------------------------*/
static void plD_pixel_wxwidgets( PLStream *pls, short x, short y )
{
  Log_Verbose( "plD_pixel_wxwidgets" );

  wxPLdev *dev=(wxPLdev*)pls->dev;

  if( dev->dc == NULL )
    install_dc( pls );

  Log_Verbose( "Draw Pixel @ %d, %d\n", x, y );	
  dev->dc->DrawPoint( x, y );
  Log_Verbose( "Pixel Drawn\n" );	
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

	plD_FreeType_init( pls );

	FT_Data *FT=(FT_Data *)pls->FT;
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
  
  FT->scale=dev->scale;   
  FT->ymax=dev->height;
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







/*----------------------------------------------------------------------*\
 *  This part includes wxWidgets specific functions, which allow to
 *  open a window from the command line, if needed.
\*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*\
 *  void new_bitmap( PLStream *pls )
 *
 *  For every new plot a new bitmap is allocated so that you can
 *  browse through the different plots.
\*----------------------------------------------------------------------*/
void new_bitmap( PLStream *pls )
{
  Log_Verbose( "new_bitmap" );

  wxPLdev* dev = (wxPLdev*)pls->dev;

  ((wxMemoryDC*)dev->dc)->SelectObject( wxNullBitmap );   // deselect bitmap
  dev->bitmap_counter++;
  dev->bitmaps[dev->bitmap_counter-1] = new wxBitmap( dev->width, dev->height, -1 );
  ((wxMemoryDC*)dev->dc)->SelectObject( *(dev->bitmaps[dev->bitmap_counter-1]) );   // select new bitmap
}


/*----------------------------------------------------------------------*\
 *  void install_dc( PLStream *pls )
 *
 *  If this driver is called from a command line executable (and not
 *  from within a wxWidgets program), this function prepares a DC and a
 *  bitmap to plot into.
\*----------------------------------------------------------------------*/
void install_dc( PLStream *pls )
{
  Log_Verbose( "install_dc" );

  // this hack enables to have a GUI on Mac OSX even if the 
  // program was called from the command line (and isn't a bundle)
#ifdef __WXMAC__
  ProcessSerialNumber psn;

  GetCurrentProcess( &psn );
  CPSEnableForegroundOperation( &psn );
  SetFrontProcess( &psn );
#endif

  // initialize wxWidgets
  wxInitialize();

  // get a DC and a bitmap  
  wxPLdev* dev = (wxPLdev*)pls->dev;
  dev->dc = new wxMemoryDC();
	new_bitmap( pls );

  // replay command we may have missed
  plD_bop_wxwidgets( pls );
}
 

/*----------------------------------------------------------------------*\
 *  int wxPLEntry( int& argc, const wxChar **argv )
 *
 *  This is a hacked wxEntry-function, so that wxUninitialize is not
 *  called twice. Here we actually start the wxApplication.
\*----------------------------------------------------------------------*/
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

    return wxTheApp->OnRun();   // start wxWidgets application
  }
  wxCATCH_ALL( wxTheApp->OnUnhandledException(); return -1; )
}


/*----------------------------------------------------------------------*\
 *  bool wxPLplotApp::OnInit()
 *
 *  This method is called before the applications gets control. A frame
 *  is created here.
\*----------------------------------------------------------------------*/
bool wxPLplotApp::OnInit()
{
  Log_Verbose( "wxPLplotApp::OnInit" );

  wxPLplotFrame* frame = new wxPLplotFrame( _T("wxWidgets PLplot App") );
  frame->SetClientSize( m_dev->width, m_dev->height );
  frame->Show( true );
  frame->Raise();
	
  return true;
}


/*----------------------------------------------------------------------*\
 *  wxPLplotFrame::wxPLplotFrame( const wxString& title )
 *
 *  Constructor of wxPLplotFrame, where we create the menu.
\*----------------------------------------------------------------------*/
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
  SetMenuBar( menuBar );
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotFrame::OnQuit( wxCommandEvent& WXUNUSED(event) )
 *
 *  Event method, which is called if user wants to quit. We quit.
\*----------------------------------------------------------------------*/
void wxPLplotFrame::OnQuit( wxCommandEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::OnQuit" );

  wxGetApp().ExitMainLoop();
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotFrame::NextPlot( wxCommandEvent& WXUNUSED(event) )
 *
 *  Event method where, if we have more than one plot, the next plot 
 *  is selected. Refresh() takes care, that the new plot is shown.
\*----------------------------------------------------------------------*/
void wxPLplotFrame::NextPlot( wxCommandEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::NextPlot" );
    
  wxPLdev* dev = wxGetApp().GetPLplotDevice();

  // increment counter, if we have passed the last plot
  // show the first plot again
  if( ++plot_counter > dev->bitmap_counter-1 )
    plot_counter = 1;

  Refresh();
}


/*----------------------------------------------------------------------*\
 *  void wxPLplotFrame::OnPaint( wxPaintEvent& WXUNUSED(event) )
 *
 *  Event method where the plots are actually drawn. Since the plots
 *  are already drawn into bitmaps, which just copy them into to client
 *  area. This method is also called, if (part) of the client area was
 *  invalidated and a refresh is necessary.
\*----------------------------------------------------------------------*/
void wxPLplotFrame::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
  Log_Verbose( "wxPLplotFrame::OnPaint" );

  int width, height;
  GetClientSize( &width, &height );

  wxPLdev* dev = wxGetApp().GetPLplotDevice();
  
  // select bitmap
  wxMemoryDC MemoryDC;
  MemoryDC.SelectObject( *(dev->bitmaps[plot_counter-1]) );  
  
  SetTitle( wxString::Format( _T("wxWidgets PLplot App - Plot %d of %d"), plot_counter, dev->bitmap_counter-1 ) );
  
  // copy bitmap into client area
  wxPaintDC dc(this);
  dc.BeginDrawing();
  dc.Blit( 0, 0, dev->width, dev->height, &MemoryDC, 0, 0 );
  dc.EndDrawing();
  
  MemoryDC.SelectObject( wxNullBitmap );  
}
 
#endif

#else
int pldummy_wxwidgets()
{
    return 0;
}

#endif				/* PLD_wxwidgets */
