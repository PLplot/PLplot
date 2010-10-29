// gcw - PLplot Gnome Canvas Widget device driver.
//
// Copyright (C) 2004, 2005  Thomas J. Duck
// Copyright (C) 2004  Rafael Laboissiere
// All rights reserved.
//
// NOTICE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
// USA
//

#ifndef __GCW_H__
#define __GCW_H__

#include "plDevs.h"
#include "plplotP.h"
#include "drivers.h"

#include <math.h>
#include <stdio.h>
#include <sys/stat.h>

#include <glib.h>
#include <gtk/gtk.h>
#include <libgnomecanvas/libgnomecanvas.h>
#include <libart_lgpl/libart.h>
#include <libgnomeprint/gnome-print.h>


// #define DEBUG_GCW_1
// #define DEBUG_GCW_2
// #define ASSERT_GCW


//--------------------------------------------------------------------------
// GcwPLdev - the PLplot device structure for the GCW driver

typedef struct
{
    GnomeCanvas     * canvas;           // The canvas to draw on
    GdkPixmap       * background;       // The background
    GdkGC           * gc;               // A graphics context for the pixmap

    GnomeCanvasGroup* group_visible;    // Visible group, removed at next eop
    GnomeCanvasGroup* group_hidden;     // Hidden group --> visible at eop
    GnomeCanvasGroup* group_persistent; // Persistent group, at from and never erased

    gboolean        use_persistence;    // Flags the persistent group should be used,
                                        // and that it and the background should not be
                                        // erased when the page is advanced.
                                        //

    GtkWidget  * window;                // A window used in standalone mode
    GtkWidget  * notebook;              // A notebook pager in the window
    GtkWidget  * statusbar;             // A statusbar for the window
    GtkWidget  * filew;                 // A file widget chooser when Save is pressed

    guint32    color;                   // Current pen color
    GdkColor   bgcolor;                 // Background color (shouldn't change)
    GdkColormap* colormap;              // The gdk colormap for the canvas

    PLINT      width;                   // Width of the canvas in device pixels
    PLINT      height;                  // Height of the canvas in device pixels

    PLINT      pen_color;               // Current pen color
    PLINT      pen_width;               // Current pen width

    gboolean   use_pixmap;              // Flags pixmap use for lines and fills
    gboolean   pixmap_has_data;         // Flags that the pixmap has data

    gboolean   plstate_width;           // Flags indicating change of state before
    gboolean   plstate_color0;          //  device is fully initialized
    gboolean   plstate_color1;

    gboolean   allow_resize; // Flags whether device resizing is allowed
} GcwPLdev;


//--------------------------------------------------------------------------
// Physical dimension constants used by the driver

// Virtual coordinate scaling parameter, used to do calculations at
// higher resolution.  Chosen to be 32 for consistency with the PLplot
// metafile (see plplotP.h).
//
// The trick here is to do everything in device coordinates on the driver
// side, but report/receive everything in virtual coordinates to/from the
// PLplot core.
//
#define VSCALE                   ( 32. )

// pixels per mm
#define DEVICE_PIXELS_PER_MM     ( 3.4 )
#define VIRTUAL_PIXELS_PER_MM    ( DEVICE_PIXELS_PER_MM * VSCALE )

// mm per inch
#define MM_PER_IN                ( 25.4 )

// pixels per inch
#define DEVICE_PIXELS_PER_IN     ( DEVICE_PIXELS_PER_MM * MM_PER_IN )
#define VIRTUAL_PIXELS_PER_IN    ( VIRTUAL_PIXELS_PER_MM * MM_PER_IN )

// Default dimensions of the canvas (in inches)
#define CANVAS_WIDTH             ( 10. )
#define CANVAS_HEIGHT            ( 7.5 )

// The zoom factor for 100% zoom in
#define ZOOM100                  ( 1.0 )
#define ZOOMSTEP                 ( 1.25 )


//--------------------------------------------------------------------------
// GCW "Gnome Canvas Widget" Library prototypes (see gcw-lib.c)

// Public_functions

void gcw_use_text( PLINT use_text );
void gcw_use_pixmap( PLINT use_pixmap );
void gcw_use_hrshsym( PLINT use_hrshsym );

// Private functions

void gcw_debug( char* msg );
PLDLLIMPEXP_GNOME2 void gcw_set_gdk_color();
PLDLLIMPEXP_GNOME2 void gcw_clear_background();
PLDLLIMPEXP_GNOME2 void gcw_init_canvas( GnomeCanvas* canvas );
PLDLLIMPEXP_GNOME2 void gcw_install_canvas( GnomeCanvas *canvas );
PLDLLIMPEXP_GNOME2 void gcw_set_device_size( PLINT width, PLINT height );
void gcw_set_canvas_size( GnomeCanvas* canvas, PLINT width, PLINT height );
void gcw_set_canvas_zoom( GnomeCanvas* canvas, PLFLT magnification );
void gcw_use_persistence( PLINT use_persistence );

#endif // __GCW_H__
