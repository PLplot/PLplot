/* gcw - PLplot Gnome Canvas Widget device driver.

  Copyright (C) 2004, 2005  Thomas J. Duck
  Copyright (C) 2004  Rafael Laboissiere
  All rights reserved.

NOTICE

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
  USA 
*/

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

typedef struct {
  GnomeCanvas* canvas;                /* The canvas to draw on */
  GnomeCanvasItem* background;        /* The background of the canvas */

  GnomeCanvasGroup* group_background; /* Background group for plot items */
  GnomeCanvasGroup* group_foreground; /* Foreground group for plot items */
  GnomeCanvasGroup* group_visible;    /* Visible group, removed at next eop */
  GnomeCanvasGroup* group_hidden;     /* Hidden group --> visible at eop */
  GnomeCanvasGroup* group_current;    /* A pointer to one of the above */

  GtkWidget* window;         /* A window used in standalone mode */
  GtkWidget* notebook;       /* A notebook pager in the window */
  GtkWidget* statusbar=NULL; /* A statusbar for the window */
  GtkWidget *filew=NULL;     /* A file widget chooser when Save is pressed */

  GdkColormap* colormap;
  guint32 color;

  gdouble width;  /* Width of the canvas in device pixels */
  gdouble height; /* Height of the canvas in device pixels */

  PLINT pen_color; /* Current pen color */
  PLINT pen_width; /* Current pen width */

  gboolean zoom_is_initialized;

  gboolean use_text;           /* Flag to use TrueType text */
  gboolean use_fast_rendering; /* Flag for fastest (but buggy) rendering */

  gboolean aa;                 /* Flag for an antialiased Canvas */

  gboolean use_pixmap;         /* Flags a pixmap should be used for shades */
  GdkPixmap* pixmap;           /* The pixmap */
  gboolean pixmap_has_data;    /* Flags the pixmap has data */

} GcwPLdev;

/* Physical dimensions */

/* Virtual coordinate scaling parameter, used to do calculations at
 * higher resolution.  Chosen to be 32 for consistency with the PLplot
 * metafile (see plplotP.h).
 *
 * The trick here is to do everything in device coordinates on the driver
 * side, but report/receive everything in virtual coordinates to/from the
 * PLplot core.
 */
#define VSCALE (32.)

/* pixels per mm; note DPMM = 4. in plplotP.h */
#define DEVICE_PIXELS_PER_MM (DPMM)
#define VIRTUAL_PIXELS_PER_MM (DPMM*VSCALE)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per inch */
#define DEVICE_PIXELS_PER_IN (DEVICE_PIXELS_PER_MM*MM_PER_IN)
#define VIRTUAL_PIXELS_PER_IN (VIRTUAL_PIXELS_PER_MM*MM_PER_IN)

/* Default dimensions of the canvas (in inches) */
#define CANVAS_WIDTH (10.)
#define CANVAS_HEIGHT (7.5)

/* The zoom factor for 100% zoom in */
#define ZOOM100 (1.0)
#define ZOOMSTEP (1.25)


/*=========================================================================*/
/* GCW "Gnome Canvas Widget" Library prototypes.  The functions are defined
 * in gcw-lib.c. 
 */

/* Public_functions */
/* void gcw_install_canvas(PLStream *pls, GnomeCanvas *canvas); */
void gcw_set_canvas_size(GnomeCanvas* canvas,PLFLT width,PLFLT height);
void gcw_use_text(GnomeCanvas* canvas,PLINT use_text);
void gcw_use_fast_rendering(GnomeCanvas* canvas,PLINT use_fast_rendering);
void gcw_use_pixmap(GnomeCanvas* canvas,PLINT use_pixmap);
void gcw_use_foreground_group(GnomeCanvas* canvas);
void gcw_use_background_group(GnomeCanvas* canvas);
void gcw_use_default_group(GnomeCanvas* canvas);

/* Private functions */
void gcw_init_canvas(PLStream* pls,GnomeCanvas* canvas);
void gcw_set_canvas_aspect(GnomeCanvas* canvas,PLFLT aspect);
void gcw_set_canvas_zoom(GnomeCanvas* canvas,PLFLT magnification);
void gcw_get_canvas_viewport(GnomeCanvas* canvas,PLFLT xmin1,PLFLT xmax1,
			     PLFLT ymin1,PLFLT ymax1,PLFLT* xmin2,PLFLT* xmax2,
			     PLFLT* ymin2,PLFLT* ymax2);

#endif /* __GCW_H__ */
