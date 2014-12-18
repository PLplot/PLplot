// December 17, 2014
//
// PLplot driver for OpenGL (http://www. ??)
//
// Copyright (C) 2014 Chris Marshall
// Copyright (C) 2014 Hazen Babcock
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
//

//---------------------------------------------
// Header files, defines and local variables
// ---------------------------------------------

#include <stdarg.h>

// PLplot header files

#include "plplotP.h"
#include "drivers.h"

// constants

#define SVG_Default_X      720
#define SVG_Default_Y      540

#define POINTS_PER_INCH    72

#define MAX_STRING_LEN     1000

// local variables

PLDLLIMPEXP_DRIVER const char* plD_DEVICE_INFO_opengl = "opengl:OpenGL Driver:1:opengl:80:opengl";

// Note:
// All plot specific variables should go in this structure.
// PLplot will hold on to this. When there are multiple plots
// open at the same time this structure is how this driver
// will no which plot to update.
//
typedef struct
{
    int   plotXSize;
    int   plotYSize;
} OPENGL;

//-----------------------------------------------
// function declarations
// -----------------------------------------------

// PLplot interface functions

void plD_dispatch_init_opengl( PLDispatchTable *pdt );
void plD_init_opengl( PLStream * );
void plD_line_opengl( PLStream *, short, short, short, short );
void plD_polyline_opengl( PLStream *, short *, short *, PLINT );
void plD_eop_opengl( PLStream * );
void plD_bop_opengl( PLStream * );
void plD_tidy_opengl( PLStream * );
void plD_state_opengl( PLStream *, PLINT );
void plD_esc_opengl( PLStream *, PLINT, void * );

//--------------------------------------------------------------------------
// plD_dispatch_init_opengl()
//
// Initialize device dispatch table
//--------------------------------------------------------------------------

void plD_dispatch_init_opengl( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = "OpenGL";
    pdt->pl_DevName = "opengl";
#endif
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 80;
    pdt->pl_init     = (plD_init_fp) plD_init_opengl;
    pdt->pl_line     = (plD_line_fp) plD_line_opengl;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_opengl;
    pdt->pl_eop      = (plD_eop_fp) plD_eop_opengl;
    pdt->pl_bop      = (plD_bop_fp) plD_bop_opengl;
    pdt->pl_tidy     = (plD_tidy_fp) plD_tidy_opengl;
    pdt->pl_state    = (plD_state_fp) plD_state_opengl;
    pdt->pl_esc      = (plD_esc_fp) plD_esc_opengl;
}

//--------------------------------------------------------------------------
// plD_init_opengl()
//
// Initialize device
//--------------------------------------------------------------------------

void plD_init_opengl( PLStream *pls )
{
    OPENGL *aStream;

    // Note:
    // The pls structure contains all the relevant info for the plot.

    pls->termin  = 0;           // not an interactive device
    pls->color   = 1;           // supports color

    // Note:
    // For now, have PLplot render text. This text is really ugly though
    // so after the initial goal of getting a window w/ a plot this will
    // be the next thing that needs to work.
    pls->dev_text     = 0;      // handles text
    pls->dev_unicode  = 0;      // wants text as unicode

    // Note:
    // We also should address these later on.
    pls->dev_fill0    = 0;      // driver generates solid fills
    pls->dev_gradient = 0;      // driver renders gradient

    // Note:
    // Saving OpenGL specific driver information in the dev field of pls.
    if ( pls->dev != NULL )
        free( (void *) pls->dev );

    pls->dev = calloc( 1, (size_t) sizeof ( OPENGL ) );
    if ( pls->dev == NULL )
        plexit( "plD_init_opengl: Out of memory." );

    aStream = (OPENGL *) pls->dev;

    // Note:
    // 0,0 means use our default window size. Anything else means PLplot
    // wants some other size (in pixels).
    if ( pls->xlength <= 0 || pls->ylength <= 0 )
    {
        aStream->plotXSize = SVG_Default_X;
        aStream->plotYSize = SVG_Default_Y;
    }
    else
    {
        aStream->plotXSize = pls->xlength;
        aStream->plotYSize = pls->ylength;
    }

    // Note:
    // One trick that is used by the drivers that support anti-aliasing is to
    // configure PLplot to plot on a much larger surface than the actual plot
    // window. The driver than downscales the PLplot commands (line, etc.) to
    // fit into the window.
    // Example: cairo.c, line 1324.
}

//--------------------------------------------------------------------------
// plD_bop_opengl()
//
// Set up for the next page.
//--------------------------------------------------------------------------

void plD_bop_opengl( PLStream *pls )
{
    OPENGL *aStream;

    aStream = pls->dev;

    // Note:
    // Clear the window with the background color.
    // (pls->cmap0[0].r, pls->cmap0[0].g, pls->cmap0[0].g, pls->cmap0[0].a)
    // Example: cairo.c, line 1722

    printf("bop_opengl\n");
}

//--------------------------------------------------------------------------
// plD_line_opengl()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//--------------------------------------------------------------------------

void plD_line_opengl( PLStream *pls, short x1a, short y1a, short x2a, short y2a )
{
    OPENGL *aStream;

    aStream = pls->dev;

    // Note:
    // The color is in pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a.
    // The desired line width is in pls->width.
    // Example (color & line width): cairo.c 1402
    // Example (drawing the line): cairo.c, line 445

    printf("line_opengl\n");
}

//--------------------------------------------------------------------------
// plD_polyline_opengl()
//
// Draw a polyline in the current color.
//--------------------------------------------------------------------------

void plD_polyline_opengl( PLStream *pls, short *xa, short *ya, PLINT npts )
{
    OPENGL *aStream;

    aStream = pls->dev;

    // Note:
    // This has the same setup (line color, width) as plD_line_opengl.
    // Example: cairo.c, line 1427
    
    printf("polyline_opengl\n");
}

//--------------------------------------------------------------------------
// plD_eop_opengl()
//
// End of page
//--------------------------------------------------------------------------

void plD_eop_opengl( PLStream *pls )
{
    OPENGL *aStream;

    aStream = pls->dev;

    // Note:
    // Event loop here. For better or worse, the way most of PLPlot interactive 
    // drivers work is they take over here. When the use calls plend() or plend1()
    // the process lands here and does not return until the plot is closed.
    //
    // A future extension might be a variant of this driver (like extcairo or 
    // extqt) plot into in a user supplied OpenGL context.

    printf("eop_opengl\n");
}

//--------------------------------------------------------------------------
// plD_tidy_opengl()
//
// Close graphics file or otherwise clean up.
//--------------------------------------------------------------------------

void plD_tidy_opengl( PLStream *pls )
{
    OPENGL *aStream;

    aStream = pls->dev;

    // Note:
    // Close window, clean up the plotting memory etc. here.

    printf("tidy_opengl\n");
}

//--------------------------------------------------------------------------
// plD_state_opengl()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//
// Nothing is done here because these attributes are aquired from
// PLStream for each element that is drawn.
//--------------------------------------------------------------------------

void plD_state_opengl( PLStream *PL_UNUSED( pls ), PLINT PL_UNUSED( op ) )
{
  // Note:
  // Yes we really don't have to do anything here.
}

//--------------------------------------------------------------------------
// plD_esc_opengl()
//
// Escape function.
//--------------------------------------------------------------------------

void plD_esc_opengl( PLStream *pls, PLINT op, void *ptr )
{
  // Note:
  // This is catch all for everything that is not drawing a line.
  // When we get to text handling that will go here.
  // When we get to filled polygons that will go here.
  // When we get to requests (from PLplot) for a mouse click that
  //   event loop will go here.
    switch ( op )
    {
    case PLESC_ARC: // Draw an arc, either filled or outline
      break;
    case PLESC_FILL:      // fill polygon
        break;
    case PLESC_FLUSH:    // forced update of the window
      break;
    case PLESC_GETC:     // get cursor position
      break;
    case PLESC_GRADIENT:  // render gradient inside polygon
        break;
    case PLESC_HAS_TEXT:  // render text
        break;
    }

    printf("esc_opengl\n");
}

