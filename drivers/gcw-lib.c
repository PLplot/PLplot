/* gcw-lib - Library routines for the Gnome Canvas Widget device driver.

  Copyright (C) 2005  Thomas J. Duck
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

DESCRIPTION

  The gcw-lib API allows users to interact with the GCW driver.  The
  library routines are also invoked by the driver itself when needed.

*/

#include "gcw.h"

guint32 plcolor_to_rgba(PLColor color, guchar alpha);

/*--------------------------------------------------------------------------*\
 * gcw_set_canvas()
 *
 * Adds a canvas to the newly created stream.  
 *
 * The associated device is attached to the canvas as the property "dev".
\*--------------------------------------------------------------------------*/
void gcw_set_canvas(PLStream* pls,GnomeCanvas* canvas)
{
  GcwPLdev* dev = pls->dev;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) plexit("GCW driver: Canvas not found");

  /* Add the canvas to the device */
  dev->canvas=canvas;

  /* Attach the device to the canvas widget */
  g_object_set_data(G_OBJECT(canvas),"dev",(gpointer)dev);

  /* Determine if the canvas is antialiased */
  g_object_get(G_OBJECT(canvas),"aa",&(dev->aa),NULL);

  /* Add the background to the canvas and move it to the back */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_background = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL)
    )) {
    plabort("GCW driver <gcw_set_canvas>: Canvas item not created");
  }

  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_background),"path",NULL,NULL);

  if(!GNOME_IS_CANVAS_ITEM(
    dev->background = gnome_canvas_item_new(
		        dev->group_background,
			GNOME_TYPE_CANVAS_RECT,
			"x1", 0.0,
			"y1", -dev->height,
			"x2", dev->width,
			"y2", 0.0,
			"fill-color-rgba",
			plcolor_to_rgba(pls->cmap0[0],0xFF),
			"width-units", 0.0,
			NULL)
    )) {
    plabort("GCW driver <gcw_set_canvas>: Canvas item not created");
  }

  gnome_canvas_item_lower_to_bottom(GNOME_CANVAS_ITEM(dev->group_background));

  /* Add the foreground to the canvas and move it to the front */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_foreground = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL)
    )) {
    plabort("GCW driver <gcw_set_canvas>: Canvas item not created");
  }

  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_foreground),"path",NULL,NULL);

  gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(dev->group_foreground));

  /* Set the canvas width and height */
  gcw_set_canvas_aspect(canvas,CANVAS_WIDTH/CANVAS_HEIGHT);

  /* Get the colormap */
  dev->colormap = gtk_widget_get_colormap(GTK_WIDGET(dev->canvas));
}


/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_aspect()
 *
 * Sets the aspect (the canvas width/height) by changing the region on the 
 * canvas that is visible. Note that the user will have to correctly set the 
 * plplot viewport to take this into account.
\*--------------------------------------------------------------------------*/
void gcw_set_canvas_aspect(GnomeCanvas* canvas,PLFLT aspect)
{
  GcwPLdev* dev;
  gdouble default_aspect = CANVAS_WIDTH/CANVAS_HEIGHT;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_aspect>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL )
    plabort("GCW driver <gcw_set_canvas_aspect>: Device not found");

  /* Set the new width and height */
  if(aspect < default_aspect) {
    dev->width = PIXELS_PER_IN*CANVAS_WIDTH * (gdouble)(aspect)/default_aspect;
    dev->height = PIXELS_PER_IN * CANVAS_HEIGHT;
  }
  else {
    dev->height = PIXELS_PER_IN*CANVAS_HEIGHT*default_aspect/(gdouble)(aspect);
    dev->width = PIXELS_PER_IN * CANVAS_WIDTH;
  }

  /* Resize the background */
  g_object_set(G_OBJECT(dev->background),"x1",0.,"y1",-dev->height,
	       "x2",dev->width,"y2",0.,NULL);
}

/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_zoom()
 *
 * Sets the zoom magnification on the canvas and resizes the widget
 * appropriately.
\*--------------------------------------------------------------------------*/
void gcw_set_canvas_zoom(GnomeCanvas* canvas,PLFLT magnification)
{
  GcwPLdev* dev;

  gdouble curmag=1.,dum;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_zoom>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_set_canvas_zoom>: Device not found");

  /* Get the current magnification */
  if(dev->zoom_is_initialized) gnome_canvas_c2w(canvas,1,0,&curmag,&dum);
  curmag = 1./curmag;

  gnome_canvas_set_pixels_per_unit(canvas,magnification*curmag);

  gtk_widget_set_size_request(GTK_WIDGET(canvas),
			      (dev->width+1)*magnification*curmag,
			      (dev->height+2)*magnification*curmag);

  gnome_canvas_set_scroll_region(canvas,0,-dev->height,
				 dev->width+1,1);

  dev->zoom_is_initialized = TRUE;
}


/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_size()
 *
 * Sets the canvas size by setting the aspect and zoom.
 *
 * Width and height are both measured in inches.
\*--------------------------------------------------------------------------*/
void gcw_set_canvas_size(GnomeCanvas* canvas,PLFLT width,PLFLT height)
{
  PLFLT magnification;
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_size>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_set_canvas_size>: Device not found");

  gcw_set_canvas_aspect(canvas,width/height);

  magnification = width/(PLFLT)(dev->width);

  gcw_set_canvas_zoom(canvas,magnification);
}


/*--------------------------------------------------------------------------*\
 * gcw_get_canvas_viewport()
 *
 * Given a plplot viewport, this routine adjusts it taking the device
 * aspect into account.
\*--------------------------------------------------------------------------*/
void gcw_get_canvas_viewport(GnomeCanvas* canvas,PLFLT xmin1,PLFLT xmax1,
			     PLFLT ymin1,PLFLT ymax1,PLFLT* xmin2,PLFLT* xmax2,
			     PLFLT* ymin2,PLFLT* ymax2)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_viewport>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_set_canvas_viewport>: Device not found");

  *xmin2 = xmin1*(PLFLT)(dev->width)/(PIXELS_PER_IN*CANVAS_WIDTH);
  *xmax2 = xmax1*(PLFLT)(dev->width)/(PIXELS_PER_IN*CANVAS_WIDTH);
  *ymin2 = ymin1*(PLFLT)(dev->height)/(PIXELS_PER_IN*CANVAS_HEIGHT);
  *ymax2 = ymax1*(PLFLT)(dev->height)/(PIXELS_PER_IN*CANVAS_HEIGHT);
}


/*--------------------------------------------------------------------------*\
 * gcw_use_text()
 *
 * Used to turn text usage on and off.
\*--------------------------------------------------------------------------*/
void gcw_use_text(GnomeCanvas* canvas,PLINT use_text)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_use_text>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_text>: Device not found");

#ifdef HAVE_FREETYPE
  dev->use_text = (gboolean)use_text;

  /* Use a hack to the plplot escape mechanism to update text handling */
  plP_esc(PLESC_HAS_TEXT,NULL);
#endif

}


/*--------------------------------------------------------------------------*\
 * gcw_use_fast_rendering()
 *
 * Used to turn fast rendering on and off.  This matters in 
 * plD_polyline_gcw, where fast rendering can cause errors on the
 * GnomeCanvas.
\*--------------------------------------------------------------------------*/
void gcw_use_fast_rendering(GnomeCanvas* canvas,PLINT use_fast_rendering)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_use_fast_rendering>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_fast_rendering>: Device not found");

  dev->use_fast_rendering = (gboolean)use_fast_rendering;
}


/*--------------------------------------------------------------------------*\
 * gcw_use_pixmap()
 *
 * Used to turn pixmap usage on and off for polygon fills (used during
 * shading calls).
\*--------------------------------------------------------------------------*/
void gcw_use_pixmap(GnomeCanvas* canvas,PLINT use_pixmap)
{
  GcwPLdev* dev;

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_pixmap>: Device not found");

  dev->use_pixmap=(gboolean)use_pixmap;

  /* Allocate the pixmap */
  if(use_pixmap) {
    dev->pixmap = gdk_pixmap_new(NULL,dev->width,
				 dev->height,gdk_visual_get_best_depth());
  }

  dev->pixmap_has_data = FALSE;
}


/*--------------------------------------------------------------------------*\
 * gcw_use_*_group()
 *
 * Used to switch which groups plplot is writing to.  The choices are:
 *
 *  background - persistent (never clears)
 *  foreground - persistent (never clears)
 *  default - shown when the page is advanced, cleared for next page
 *
\*--------------------------------------------------------------------------*/
void gcw_use_background_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_background_group>: Device not found");

  dev->group_current = dev->group_background;
}
void gcw_use_foreground_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_foreground_group>: Device not found");

  dev->group_current = dev->group_foreground;
}
void gcw_use_default_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_default_group>: Device not found");

  dev->group_current = dev->group_hidden;
}


/*--------------------------------------------------------------------------*\
 * plcolor_to_rgba()
 *
 * Converts from the plcolor to rgba color format.
\*--------------------------------------------------------------------------*/
guint32 plcolor_to_rgba(PLColor color, guchar alpha)
{
  return
    ((int)(color.r) << 24)
    + ((int)(color.g) << 16)
    + ((int)(color.b) << 8)
    + alpha;
}

