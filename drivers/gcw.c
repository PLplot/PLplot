/* gcw-driver - PLplot Gnome Canvas Widget device driver.

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


DESCRIPTION

  This is the Gnome Canvas Widget driver, written by Thomas J. Duck 
  following the heritage of the PLplot Gnome driver by Rafael Laboissiere.  
  Like all PLplot drivers, this operates in standalone mode by default.  
  However, this driver can also be used to write to a user-supplied 
  GnomeCanvas.

  Please see the PLplot documentation for more information.


DEVELOPMENT NOTES

  Truetype text is supplied using the PLPLOT_CANVAS_HACKTEXT item,
  which was cloned from gnome-print.  This text item was chosen because 
  it rotates and scales under a zoom correctly and easily.

  It would be better to use GNOME_CANVAS_TEXT, but currently 
  (4 March 2005) it doesn't rotate or scale under a zoom on the 
  GnomeCanvas.  GNOME_CANVAS_TEXT uses Pango, and rotations were only
  recently implemented in the Pango API (i.e., Fall 2004).  If the
  Pango API is used directly, the bounding box doesn't rotate with the 
  text on GnomeCanvas, which results in clipping.  It is likely that
  GnomeCanvas is not querying the bounding box from Pango correctly,
  and is not directing Pango to scale.  So, GnomeCanvas needs to be 
  updated to deal with Pango properly.

  Another problem is that drawing polylines on the Gnome Canvas sometimes
  results in an 'attempt to put segment in horiz list twice' error.
  The workaround here is to plot single line segments only, but this
  results in a performance hit.  This problem will need to be corrected
  in the GnomeCanvas.


KNOWN BUGS

  PLplot test suite problems:

    1) Example x10c does not clip the text (there is no text clipping).

    2) Example x17c, the strip chart demo, doesn't do a strip chart 
       (try the xwin driver to see how it should work).  Strip charts 
       are fundamentally incompatible with the tabbed window design of
       the GCW driver.  Use the PlplotCanvas to create animations 
       instead.
*/

#include <sys/stat.h>

#include "gcw.h"
#include "plplotcanvas-hacktext.h"

#ifdef HAVE_FREETYPE

#include "plfreetype.h"
#include "plfci-truetype.h"

/* Font lookup table that is constructed in plD_FreeType_init*/
extern FCI_to_FontName_Table FontLookup[N_TrueTypeLookup];

extern void plD_FreeType_init(PLStream *pls);
extern void plD_FreeType_Destroy(PLStream *pls);

#endif /* HAVE_FREETYPE */


/* Device info */
char* plD_DEVICE_INFO_gcw = "gcw:Gnome Canvas Widget:1:gcw:10:gcw";

/* Global driver options */

#ifdef HAVE_FREETYPE
static PLINT text = 1;
#else
static PLINT text = 0;
#endif

static PLINT hrshsym = 0;
static PLINT replot = 1;

static DrvOpt gcw_options[] = 
  {
    {"text", DRV_INT, &text, "Use truetype fonts (text=0|1)"},
    {"hrshsym", DRV_INT, &hrshsym, "Use Hershey symbol set (hrshsym=0|1)"},
    {"replot", DRV_INT, &replot, "Allow replotting to other devices (replot=0|1)"},
    {NULL, DRV_INT, NULL, NULL}
  };


/*********************
 * Utility functions *
 *********************/

guint32 plcolor_to_rgba(PLColor color, guchar alpha)
{
  return
    ((int)(color.r) << 24)
    + ((int)(color.g) << 16)
    + ((int)(color.b) << 8)
    + alpha;
}


/*--------------------------------------------------------------------------*\
 * plD_dispatch_init_gcw()
 *
 * Initializes the dispatch table.
\*--------------------------------------------------------------------------*/

void plD_open_gcw(PLStream *pls);
void plD_init_gcw(PLStream *);
void plD_line_gcw(PLStream *, short, short, short, short);
void plD_polyline_gcw(PLStream *, short *, short *, PLINT);
void plD_eop_gcw(PLStream *);
void plD_bop_gcw(PLStream *);
void plD_tidy_gcw(PLStream *);
void plD_state_gcw(PLStream *, PLINT);
void plD_esc_gcw(PLStream *, PLINT, void *);

void plD_dispatch_init_gcw( PLDispatchTable *pdt )
{

#ifdef DEBUG_GCW_1
  gcw_debug("<plD_dispatch_init_gcw>\n");
#endif

#ifndef ENABLE_DYNDRIVERS
  pdt->pl_MenuStr  = "Gnome Canvas Widget";
  pdt->pl_DevName  = "gcw";
#endif
  pdt->pl_type     = plDevType_Interactive;
  pdt->pl_seq      = 1;
  pdt->pl_init     = (plD_init_fp)     plD_init_gcw;
  pdt->pl_line     = (plD_line_fp)     plD_line_gcw;
  pdt->pl_polyline = (plD_polyline_fp) plD_polyline_gcw;
  pdt->pl_eop      = (plD_eop_fp)      plD_eop_gcw;
  pdt->pl_bop      = (plD_bop_fp)      plD_bop_gcw;
  pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_gcw;
  pdt->pl_state    = (plD_state_fp)    plD_state_gcw;
  pdt->pl_esc      = (plD_esc_fp)      plD_esc_gcw;

#ifdef DEBUG_GCW_1
  gcw_debug("</plD_dispatch_init_gcw>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * plD_init_gcw()
 *
 * Initializes the device.
 *
 * This routine is invoked by a call to plinit.
 *
\*--------------------------------------------------------------------------*/

void plD_init_gcw(PLStream *pls)
{
  GcwPLdev* dev;

  PLINT width, height, tmp;

  PLColor bgcolor = pls->cmap0[0];

#ifdef DEBUG_GCW_1
  gcw_debug("<plD_init_gcw>\n");
#endif

  /* Parse the driver options */
  plParseDrvOpts(gcw_options);

  /* Set up the stream */
  pls->termin = 1;      /* Is an interactive terminal */
  pls->dev_flush = 1;   /* Handle our own flushes */
  pls->plbuf_write = replot; /* Use plot buffer to replot to another device */
  pls->width = 1;
  pls->dev_clear = 0;   /* Handle plclear() */
  pls->dev_fill0 = 1;	/* Handle solid fills */

  /* Create the device */
  if((dev = g_malloc(sizeof(GcwPLdev))) == NULL) 
    plexit("GCW driver <plD_init_gcw>: Cannot create device");
  pls->dev = dev;

  /* Set text handling */
#ifdef HAVE_FREETYPE
  if(text) {
    pls->dev_text = TRUE;
    pls->dev_unicode = TRUE;
    if(hrshsym) pls->dev_hrshsym = 1;

    /* Initialize freetype */
    plD_FreeType_init(pls);
  }
  else {
    pls->dev_text = FALSE;
    pls->dev_unicode = FALSE;
  }
#else
  pls->dev_text = FALSE;
  pls->dev_unicode = FALSE;
#endif

  /* Set up pixmap support */
  dev->use_pixmap = (gboolean)(!pls->nopixmap);
  dev->pixmap_has_data = FALSE;

  /* Initialize the device colors */
  dev->color = plcolor_to_rgba(pls->cmap0[pls->icol0],0xFF);
  dev->bgcolor.red=(guint16)(bgcolor.r/255.*65535); 
  dev->bgcolor.green=(guint16)(bgcolor.b/255.*65535); 
  dev->bgcolor.blue=(guint16)(bgcolor.g/255.*65535);

  /* Set the device canvas and window pointers */
  dev->canvas = NULL;
  dev->background = NULL;
  dev->gc = NULL;
  dev->colormap = NULL;
  dev->window = NULL;
  dev->notebook = NULL;
  dev->statusbar = NULL;
  dev->filew = NULL;
    
  /* Initialize the Canvas groups.  All of the plplot plotting
   * commands are drawn to the hidden group.  When the page is finalized,
   * the group is made visible, and the old group destroyed. The persistent
   * group is never erased, and always plotted at the very front.
   */
  dev->group_visible=NULL;
  dev->group_hidden=NULL;
  dev->group_persistent=NULL;

  /* Assume that pladv should completeley refresh the page */
  dev->use_persistence = FALSE;

  /* Set the initialization state monitors to FALSE */
  dev->plstate_width = FALSE;
  dev->plstate_color0 = FALSE;
  dev->plstate_color1 = FALSE;

  /* Initialize gtk */
  gtk_init(0,NULL);

  /* Set up the physical device in the next series of commands.  It is very 
   * important to do this properly, because many PLplot routines depend on
   * physical coordinates (e.g., dashed lines, hatched areas, the
   * replot mechanism, hidden line removal, etc.
   *
   * Note that coordinates in the driver are measured in device units,
   * which correspond to the pixel size on a typical screen.  The coordinates
   * reported and received from the PLplot core, however, are in virtual
   * coordinates, which is just a scaled version of the device coordinates.
   * This strategy is used so that the calculations in the PLplot
   * core are performed at reasonably high resolution.
   *
   */
  if (pls->xlength > 0 && pls->ylength > 0) {
    /* xlength and length are the dimensions specified using -geometry
     * on the command line, in device coordinates.
     */
    width = pls->xlength;
    height = pls->ylength;
  }
  else {    
    width = (PLINT)(CANVAS_WIDTH*DEVICE_PIXELS_PER_IN);
    height = (PLINT)(CANVAS_HEIGHT*DEVICE_PIXELS_PER_IN);
  }

  /* If portrait mode, apply a rotation and set freeaspect */
  if(pls->portrait) {
    plsdiori((PLFLT)(4 - ORIENTATION));
    pls->freeaspect = 1;
  }

  /* Setup the page size for this device.  Very important for any driver! */
  gcw_set_device_size(width,height);

  /* Install a canvas... unless plsc->hack is set, which is a driver-specific
   * hack that indicates a PLESC_DEVINIT escape call will provide us with a 
   * canvas to use.  This hack is used by the PlplotCanvas.
   */
  if(!pls->hack) {
    dev->allow_resize = FALSE; /* The size is set an should not be changed */
    gcw_install_canvas(NULL);
  }
  else dev->allow_resize = TRUE; /* Resizing allowed for canvasses
				  * provided via PLESC_DEVINIT */


#ifdef DEBUG_GCW_1
  gcw_debug("</plD_init_gcw>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * plD_polyline_gcw()
 *
 * Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/

void plD_polyline_gcw(PLStream *pls, short *x, short *y, PLINT npts)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvasPoints* points;
  GnomeCanvasPoints pts;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;

  GdkPoint* gdkpoints;

  PLINT i;

  gdouble width;
  guint32 color;

#ifdef DEBUG_GCW_2
  gcw_debug("<plD_polyline_gcw />\n");
#endif

  if(!GNOME_IS_CANVAS(dev->canvas))
    plexit("GCW driver <plD_polyline_gcw>: Canvas not found");
  canvas = dev->canvas;

  if(dev->use_persistence) group = dev->group_persistent;
  else group = dev->group_hidden;

  if(dev->use_pixmap && !dev->use_persistence) { /* Write to bg pixmap */

    if((gdkpoints = (GdkPoint*)malloc(npts*sizeof(GdkPoint)))==NULL)
      plabort("GCW driver <plD_polyline_gcw>: Could not create gdkpoints");

    if(!pls->portrait) {
      for(i=0;i<npts;i++) {
	gdkpoints[i].x = (gint)(x[i]/VSCALE);
	gdkpoints[i].y = (gint)(dev->height-y[i]/VSCALE);
      }
    }
    else { /* Swap x and y for portrait mode */
      for(i=0;i<npts;i++) {
	gdkpoints[i].x = (gint)(dev->height-y[i]/VSCALE);
	gdkpoints[i].y = (gint)(dev->width-x[i]/VSCALE);
      }
    }

    gdk_draw_lines(dev->background,dev->gc,gdkpoints,npts);

    dev->pixmap_has_data = TRUE;

    free(gdkpoints);
  }
  else { /* Draw Canvas lines */

    /* Put the data in a points structure */
    if( (points = gnome_canvas_points_new(npts)) == NULL )
      plabort("GCW driver <plD_polyline_gcw>: Cannot create points");
    if(!pls->portrait) {
      for ( i = 0; i < npts; i++ ) {
	points->coords[2*i] = (gdouble)(x[i]/VSCALE);
	points->coords[2*i + 1] = (gdouble)(-y[i]/VSCALE);
      }
    }
    else { /* Swap x and y for portrait mode */
      for ( i = 0; i < npts; i++ ) {
	points->coords[2*i] = (gdouble)(dev->height-y[i]/VSCALE);
	points->coords[2*i + 1] = (gdouble)(-x[i]/VSCALE);
      }
    }
    
    /* Get the pen width and color */
    width = pls->width;
    color = dev->color;


    /* Workaround for the 'attempt to put segment in horiz list twice'
     * from libgnomecanvas:
     *
     *   Plot a series of line segments rather than a single polyline.
     *
     * This slows rendering down a considerable amount.  However, it is 
     * unclear what else can be done.  Libgnomecanvas should be able to 
     * deal with all valid data; bizarre plotting errors happen along with
     * this error.
     *
     * Note that instead of allocating a series of points structures, 
     * we just refer to the original one from a separate struct 
     * (GnomeCanvas does not hold a reference to the points structure).
     */

    pts.num_points = 2;
    pts.ref_count = 1;
    pts.coords = points->coords;
      
    for(i=0;i<npts-1;i++) {
      pts.coords=&(points->coords[2*i]);

      if(!GNOME_IS_CANVAS_ITEM(
	    item=gnome_canvas_item_new(group,
				       GNOME_TYPE_CANVAS_LINE,
				       "cap_style", GDK_CAP_ROUND,
				       "join-style", GDK_JOIN_ROUND,
				       "points", &pts,
				       "fill-color-rgba", color,
				       "width-units", width,
				       NULL)
      )) {
	plwarn("GCW driver <plD_polyline_gcw>: Canvas item not created.");
      }
    }

    /* Free the points structure */
    gnome_canvas_points_free(points);
  }
}


/*--------------------------------------------------------------------------*\
 * plD_line_gcw()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void plD_line_gcw(PLStream *pls, short x1, short y1, short x2, short y2)
{
  short x[2];
  short y[2];

#ifdef DEBUG_GCW_2
  gcw_debug("<plD_line_gcw />\n");
#endif

  x[0] = x1;
  x[1] = x2;
  y[0] = y1;
  y[1] = y2;

  plD_polyline_gcw(pls, x, y, (PLINT) 2);
}


/*--------------------------------------------------------------------------*\
 * plD_eop_gcw()
 *
 * End of page.
\*--------------------------------------------------------------------------*/

void plD_eop_gcw(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;

  GdkPixbuf* pixbuf;
  GnomeCanvasItem* item;
  GnomeCanvasGroup* group;

  gdouble dx, dy;

  gint count=1,n;
  
  void *save_state;

  PLINT width,height;

  if(!GNOME_IS_CANVAS(dev->canvas))
    plexit("GCW driver <plD_eop_gcw>: Canvas not found");
  canvas = dev->canvas;

  /* Ignore if there is no hidden group.  This means BOP has not been
   * called yet.
   */
  if(!GNOME_IS_CANVAS_GROUP(dev->group_hidden)) return;

#ifdef DEBUG_GCW_1
  gcw_debug("<plD_eop_gcw>\n");
#endif

  if(dev->use_persistence) group = dev->group_persistent;
  else group = dev->group_hidden;

  /* Retrieve the device width and height of the canvas */
  width = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-width");
  height = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-height");

  if(dev->pixmap_has_data) {

    /* Render the pixmap to a pixbuf on the canvas. */
    if(!GDK_IS_PIXBUF(pixbuf=gdk_pixbuf_get_from_drawable(NULL,
			      dev->background,
			      dev->colormap,
			      0,0,
			      0,0,
			      width,height))) {
      plwarn("GCW driver <plD_eop_gcw>: Can't draw pixmap into pixbuf.");
    }
    else { /* Pixbuf creation succeeded */

      if(!GNOME_IS_CANVAS_ITEM(
	    item = gnome_canvas_item_new(dev->group_hidden,
				         GNOME_TYPE_CANVAS_PIXBUF,
				         "pixbuf",pixbuf,
				         "x", 1.,
				         "y", (gdouble)(-height+1.),
				         "width", (gdouble)(width),
				         "height", (gdouble)(height),
				         NULL)
      )) {
	plwarn("GCW driver <plD_eop_gcw>: Canvas item not created.");
      }

      /* Free the pixbuf */
      g_object_unref(pixbuf);
    }
  }
  else {

    /* Use a rectangle for the background instead (faster) */
    if(!GNOME_IS_CANVAS_ITEM(
	  item = gnome_canvas_item_new(
		       dev->group_hidden,
		       GNOME_TYPE_CANVAS_RECT,
		       "x1", 0.,
		       "y1", (gdouble)(-height),
		       "x2", (gdouble)(width),
		       "y2", 0.,
		       "fill-color-rgba", plcolor_to_rgba(pls->cmap0[0],0xFF),
		       "width-units", 0.,
		       NULL)
    )) {
      plabort("GCW driver <pld_eop_gcw>: Canvas item not created");
    }
  }

  /* Move the persistent group to the front */
  gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(dev->group_persistent));

  /* Move the background to the back */
  if(GNOME_IS_CANVAS_ITEM(item)) gnome_canvas_item_lower_to_bottom(item);
    
  /* Make the hidden group visible */
  gnome_canvas_item_show(GNOME_CANVAS_ITEM(dev->group_hidden));

  /* Destroy the old visible group */
  if(GNOME_IS_CANVAS_GROUP(dev->group_visible)) {
    gtk_object_destroy((GtkObject*)(dev->group_visible));
    dev->group_visible = NULL;
  }

  /* Clear the background pixmap */
  if(!dev->use_persistence && dev->pixmap_has_data) gcw_clear_background();

  /* Name the hidden group as visible */
  dev->group_visible = dev->group_hidden;
  dev->group_hidden=NULL;
  
  /* Update the canvas */
  canvas->need_update = 1;
  gnome_canvas_update_now(canvas);

  /*
   * Copy the plot buffer for future reference, otherwise it is 
   * thrown out.
   */
 
  save_state = g_object_get_data(G_OBJECT(canvas),"plotbuf");
  save_state = (void *)plbuf_save(pls, save_state);

  /* Attach the saved state to the canvas */
  g_object_set_data(G_OBJECT(canvas),"plotbuf",(gpointer)save_state);

  /* If the driver is creating its own canvasses, set dev->canvas to be
   * NULL now in order to force creation of a new canvas when the next
   * drawing call is made.  The new canvas will be placed in a new
   * notebook page.
   */
  if(dev->window!=NULL) {
    dev->canvas = NULL;
    dev->group_visible = NULL;
    dev->group_hidden = NULL;
    dev->group_persistent = NULL;
  }
  
#ifdef DEBUG_GCW_1
  gcw_debug("</plD_eop_gcw>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * plD_bop_gcw()
 *
 * Set up for the next page.
 *
\*--------------------------------------------------------------------------*/

void plD_bop_gcw(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;

  if(!GNOME_IS_CANVAS(dev->canvas)) {
    if(pls->hack) return; /* Wait for a canvas via DEVINIT */
    else gcw_install_canvas(NULL);
  }
  canvas = dev->canvas;

#ifdef DEBUG_GCW_1
  gcw_debug("<plD_bop_gcw>\n");
#endif

  /* Replay escape calls that come in before PLESC_DEVINIT.  Some of them
   * required a Canvas that didn't exist yet.
   */
  if(dev->plstate_width)  plD_state_gcw(pls, PLSTATE_WIDTH);
  if(dev->plstate_color0) plD_state_gcw(pls, PLSTATE_COLOR0);
  if(dev->plstate_color1) plD_state_gcw(pls, PLSTATE_COLOR1);
  dev->plstate_width = FALSE;
  dev->plstate_color0 = FALSE;
  dev->plstate_color1 = FALSE;
  
  /* Creat a new hidden group; all new drawing will be to this group */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_hidden = GNOME_CANVAS_GROUP(gnome_canvas_item_new(
					     gnome_canvas_root(canvas),
					     gnome_canvas_clipgroup_get_type(),
					     "x",0.,
					     "y",0.,
					     NULL))
    )) {
    plexit("GCW driver <plD_bop_gcw>: Canvas group cannot be created");
  }
  
  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_hidden),"path",NULL,NULL);
  
  /* Hide this group until drawing is done */
  gnome_canvas_item_hide(GNOME_CANVAS_ITEM(dev->group_hidden));

#ifdef DEBUG_GCW_1
  gcw_debug("</plD_bop_gcw>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * plD_tidy_gcw()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void plD_tidy_gcw(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<plD_tidy_gcw>\n");
#endif

#ifdef HAVE_FREETYPE
  if (pls->dev_text) {
    FT_Data *FT=(FT_Data *)pls->FT;
    plscmap0n(FT->ncol0_org);
    plD_FreeType_Destroy(pls);
  }
#endif

  if(dev->window!=NULL) {
    gtk_main ();
  }

#ifdef DEBUG_GCW_1
  gcw_debug("</plD_tidy_gcw>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * plD_state_gcw()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
 *
 * Note that PLplot sometimes tries to change states before the device is
 * fully initialized (i.e., via PLESC_DEVINIT).  We must keep track of
 * such attempts, and invoke the state change during the next call to
 * plD_bop_gcw.
 *
\*--------------------------------------------------------------------------*/

void plD_state_gcw(PLStream *pls, PLINT op)
{
  GcwPLdev* dev = pls->dev;
  char opname[20],msg[100];

#ifdef DEBUG_GCW_1
  if(op==PLSTATE_WIDTH) strcpy(opname,"PLSTATE_WIDTH");
  else if(op==PLSTATE_COLOR0) strcpy(opname,"PLSTATE_COLOR0");
  else if(op==PLSTATE_COLOR1) strcpy(opname,"PLSTATE_COLOR1");
  else if(op==PLSTATE_FILL) strcpy(opname,"PLSTATE_FILL");
  else if(op==PLSTATE_CMAP0) strcpy(opname,"PLSTATE_CMAP0");
  else if(op==PLSTATE_CMAP1) strcpy(opname,"PLSTATE_CMAP1");
  else strcpy(opname,"unknown");
  sprintf(msg,"<plD_state_gcw />: %s\n",opname);
  gcw_debug(msg);
#endif

  switch (op) {

    case PLSTATE_WIDTH:
      if(GNOME_IS_CANVAS(dev->canvas)) {
	if(dev->use_pixmap) {
	  gdk_gc_set_line_attributes(dev->gc, pls->width,
				     GDK_LINE_SOLID,
				     GDK_CAP_BUTT,
				     GDK_JOIN_MITER);
	}
      }
      else dev->plstate_width = TRUE;
      break;

    case PLSTATE_COLOR0:
      if(GNOME_IS_CANVAS(dev->canvas)) {
	dev->color = plcolor_to_rgba(pls->cmap0[pls->icol0],0xFF);
	if(dev->use_pixmap) gcw_set_gdk_color();
      }
      else dev->plstate_color0 = TRUE;
      break;

    case PLSTATE_COLOR1:
      if(GNOME_IS_CANVAS(dev->canvas)) {
	dev->color = plcolor_to_rgba(pls->cmap1[pls->icol1],0xFF);
	if(dev->use_pixmap) gcw_set_gdk_color();
      }
      else dev->plstate_color1 = TRUE;
      break;

    case PLSTATE_FILL:
      break;

    case PLSTATE_CMAP0:
      break;

    case PLSTATE_CMAP1:
      break;

    default: 
      break;
  }
}


/*--------------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fills the polygon defined by the given points.  Used for shade
 * plotting.  Only solid fills are allowed.
\*--------------------------------------------------------------------------*/

static void fill_polygon (PLStream* pls)
{
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;
  
  PLINT i;

  GdkPoint* gdkpoints;

  PLINT tmp;

#ifdef DEBUG_GCW_2
  gcw_debug("<fill_polygon />\n");
#endif

  if(!GNOME_IS_CANVAS(dev->canvas))
    plexit("GCW driver <fill_polygon>: Canvas not found");
  canvas = dev->canvas;

  if(dev->use_persistence) group = dev->group_persistent;
  else group = dev->group_hidden;

  if(dev->use_pixmap && !dev->use_persistence) { /* Write to a pixmap */

    if((gdkpoints = (GdkPoint*)malloc(pls->dev_npts*sizeof(GdkPoint)))==NULL)
      plabort("GCW driver <fill_polygon>: Could not create gdkpoints");

    if(!pls->portrait) {
      for(i=0;i<pls->dev_npts;i++) {
	gdkpoints[i].x = (gint)(pls->dev_x[i]/VSCALE);
	gdkpoints[i].y = (gint)(dev->height-pls->dev_y[i]/VSCALE);
      }
    }
    else { /* Swap x and y for portrait mode */
      for(i=0;i<pls->dev_npts;i++) {
	gdkpoints[i].x = (gint)(dev->height-pls->dev_y[i]/VSCALE);
	gdkpoints[i].y = (gint)(dev->width-pls->dev_x[i]/VSCALE);
      }
    }

    gdk_draw_polygon(dev->background,dev->gc,TRUE,gdkpoints,pls->dev_npts);
    
    dev->pixmap_has_data = TRUE;

    free(gdkpoints);
  }
  else { /* Use Gnome Canvas polygons */

    if( (points = gnome_canvas_points_new (pls->dev_npts)) == NULL )
      plabort("GCW driver <fill_polygon>: Could not create points");

    if(!pls->portrait) {
      for (i=0; i<pls->dev_npts; i++) {
	points->coords[2*i] = (gdouble)(pls->dev_x[i]/VSCALE);
	points->coords[2*i + 1] = (gdouble)(-pls->dev_y[i]/VSCALE);
      }
    }
    else { /* Swap x and y for portrait mode */
      for (i=0; i<pls->dev_npts; i++) {
	points->coords[2*i] = (gdouble)(dev->height-pls->dev_y[i]/VSCALE);
	points->coords[2*i + 1] = (gdouble)(-pls->dev_x[i]/VSCALE);
      } 
    }

    if(!GNOME_IS_CANVAS_ITEM(
      item = gnome_canvas_item_new (group,
				    GNOME_TYPE_CANVAS_POLYGON,
				    "points", points,
				    "fill-color-rgba",dev->color,
				    /* "outline-color-rgba",dev->color, */
				    NULL)
      )) {
      plwarn("GCW driver <fill_polygon>: Canvas item not created.");
    }
  
    gnome_canvas_points_free(points);


    /* Draw a thin outline for each polygon; note that doing this
     * using the "outline-color-rgba" property above can result in 
     * Canvas errors.
     */
    tmp = pls->width;
    pls->width=1;
    plD_polyline_gcw(pls,pls->dev_x,pls->dev_y,pls->dev_npts);
    pls->width = tmp;
  }
}

#ifdef HAVE_FREETYPE
/*--------------------------------------------------------------------------*\
 * proc_str()
 *
 * Handles call to draw text on the canvas when the HAS_TEXT escape funtion
 * case is invoked.
 *
 * This routine is unicode enabled, and requires freetype.
\*--------------------------------------------------------------------------*/

static void proc_str(PLStream *pls, EscText *args)
{
  PLFLT *t = args->xform; /* Transform matrix for string */

  GnomeCanvasGroup* group;
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;

  PLUNICODE fci; /* The unicode font characterization integer */
  guchar *fontname = NULL;
  gint font_size;
  GnomeFont *font;
  GnomeFontFace *face;
  GnomeGlyphList *glyphlist;
  guint Nglyphs;

  gdouble affine_baseline[6] = {0.,0.,0.,0.,0.,0.}; /* Affine transforms */
  gdouble affine_translate[6] = {0.,0.,0.,0.,0.,0.};
  gdouble affine_rotate[6] = {0.,0.,0.,0.,0.,0.};
  gdouble affine_plplot[6] = {0.,0.,0.,0.,0.,0.};

  GnomeCanvasItem* item[200]; /* List of string segments */
  gdouble width[200],height[200]; /* Height and width of string segment */
  gdouble up_list[200]; /* Indicates sub/sup position of string segment */
  gdouble up=0,scale=1; /* Used to create superscripts and subscripts */

  ArtDRect bbox; /* Bounding box for each segment to get width & height */

  const PLUNICODE *text; /* The text and pointers to it */
  guint i=0,Ntext; /* The text index and maximum length */

  char esc; /* The escape character */

  guint N=0; /* The number of text segments */
  gdouble total_width=0,sum_width=0;

  guint symbol;


#ifdef DEBUG_GCW_2
  gcw_debug("<proc_str>\n");
#endif

  if(!GNOME_IS_CANVAS(dev->canvas))
    plexit("GCW driver <proc_str>: Canvas not found");
  canvas = dev->canvas;

  if(dev->use_persistence) group = dev->group_persistent;
  else group = dev->group_hidden;

  /* Retrieve the escape character */
  plgesc(&esc);

  /* Put the transform matrix values in the order expected by libart.
   * Note that the plplot transform matrix only has a rotation and shear;
   * plplot's rotation direction and shear are opposite from that expected 
   * by libart, hence the negative signs below.
   */
  affine_plplot[0] = t[0];  /* cos(theta) */
  affine_plplot[1] = -t[2]; /* sin(theta) */
  affine_plplot[2] = -t[1]; /* a cos(theta) - sin(theta) */
  affine_plplot[3] = t[3];  /* a sin(theta) + cos(theta) */

  /* Font size: size is in pixels but chrht is in mm.  Why the extra factor? */
  font_size = (gint)(pls->chrht*DEVICE_PIXELS_PER_MM*1.5);

  /* Determine the default font */
  plgfci(&fci);
  fontname = plP_FCI2FontName(fci, FontLookup, N_TrueTypeLookup);
  if (fontname == NULL) {
    plabort("GCW driver <proc_str>: FCI inconsistent with TrueTypeLookup");
  }

  /* Retrieve the font face */
  face = gnome_font_face_find_from_filename(fontname,0);

  /* Get the unicode string */
  text = args->unicode_array;
  Ntext = (guint)(args->unicode_array_len);

  /* Process the string: Break it into segments of constant font and size,
   * making sure we process control characters as we come to them.  Save
   * the extra information that will allow us to place the text on the
   * canvas.
   */
  while(i<Ntext) {

    /* Process the next character */

    if(text[i] & PL_FCI_MARK) { /* Is it a font characterization index? */

      /* Determine the font name */
      fontname = plP_FCI2FontName(text[i], FontLookup, N_TrueTypeLookup);
      if (fontname == NULL) {
	plabort("GCW driver <proc_str>: FCI inconsistent with "
		"TrueTypeLookup");
      }

      /* Retrieve the font face */
      gnome_font_unref(face); /* We already have a face */
      face = gnome_font_face_find_from_filename(fontname,0);

      i++; /* Move ahead to the next character */

    }
    else {

      if(text[i] == esc) { /* Check for escape sequences */

	/* Process escape sequence */

	i++; /* Move on to next character */
	if(i>=Ntext) {
	  plwarn("GCW driver <proc_str>: Invalid escape sequence "
		 "provided in text.");
	  return;
	}

	switch(text[i]) {

	case '#': /* <esc><esc>; this should translate to a hash */
	  break;  /* Watch out for it later */

	/* Move to lower sub/sup position */
	case 'd':
	case 'D':
	  if(up>0.) scale *= 1.25;  /* Subscript scaling parameter */
	  else scale *= 0.8;  /* Subscript scaling parameter */
	  up -= font_size / 2.;
	  break;

	/* Move to higher sub/sup position */
	case 'u':
	case 'U':
	  if(up<0.) scale *= 1.25;  /* Subscript scaling parameter */
	  else scale *= 0.8;  /* Subscript scaling parameter */
	  up += font_size / 2.;
	  break;

	/* Ignore the next sequences */

        /* Overline */
	case '+':

	/* Underline */
	case '-':

	/* Backspace */
	case 'b':
	case 'B':
	  plwarn("GCW driver <proc_str>: '+', '-', and 'b' text "
		 "escape sequences not processed.");
	  break;

	} /* switch(text[i]) */

	if(text[i]!='#') i++; /* Move ahead to the next character */

      } /* if(text[i] == esc) */
    } /* if(text[i] & PL_FCI_MARK) */


    if(i==Ntext) continue; /* End of string */

    /* Save the sub/sup position */
    up_list[N] = up;

    /* Get the font */
    font = gnome_font_face_get_font_default(face,font_size*scale);
    /* printf("\n\nfont name = %s\n\n",gnome_font_get_name(font)); */

    /* Create the glyphlist for this text segment */
    glyphlist = gnome_glyphlist_new ();
    gnome_glyphlist_font(glyphlist, font);
    gnome_glyphlist_color(glyphlist,dev->color);
    gnome_glyphlist_advance(glyphlist, TRUE);
    gnome_glyphlist_kerning(glyphlist, 0.);
    gnome_glyphlist_letterspace(glyphlist, 0.);

    /* Free the font */
    gnome_font_unref(font);

    /* Move along to the next escape or FCI character, stuffing 
     * everything else into the glyphlist.
     */
    Nglyphs=0;
    while(i<Ntext && !(text[i] & PL_FCI_MARK)) {

      /* Differentiate between ## and escape sequences */
      if(text[i]==esc) {
	if( !(i>0 && text[i-1]==esc) ) break;
      }

      gnome_glyphlist_glyph(glyphlist, 
			    gnome_font_lookup_default(font,text[i]));
      i++; Nglyphs++;
    }

    if(Nglyphs) {

      /* Determine the bounding box of the text */
      gnome_glyphlist_bbox(glyphlist,NULL,0,&bbox);
      width[N] = bbox.x1-bbox.x0;
      height[N] = bbox.y1-bbox.y0;

      /* Keep track of the total string width so that we can justify it */
      total_width += width[N];
      if(N!=0) total_width += 2; /* Add a little extra space */

      /* Create the canvas text item */
      if(!GNOME_IS_CANVAS_ITEM(
        item[N] = gnome_canvas_item_new (group,
				       PLPLOT_TYPE_CANVAS_HACKTEXT,
				       "glyphlist",glyphlist,
				       "fill-color-rgba",dev->color,
				       "x",0.,
				       "y",0.,
				       NULL)
	)) {
	plabort("GCW driver <proc_str>: Canvas item not created");
      }

      /* Free the glyphlist */
      gnome_glyphlist_unref(glyphlist);
      
      /* Advance to next string segment */
      N++;
    } /* if(Nglyphs) */


    /* Don't overflow buffer */
    if(N==200 && i<Ntext)
      plabort("GCW driver <proc_str>: too many text segments");

  } /* while(i<Ntext) */

  /* We have all of the string segments.  Place each on the canvas 
   * appropriately.
   */
  for(i=0;i<N;i++) {

    /* Calculate and apply the affine transforms */
    art_affine_rotate(affine_rotate,90.*(pls->diorot-pls->portrait));
    if(!pls->portrait) {
      art_affine_translate(affine_baseline,
			   -total_width*args->just + sum_width,
			   height[0]/2.5-up_list[i]);
      art_affine_translate(affine_translate,
			   args->x/VSCALE,-args->y/VSCALE);
    }
    else { /* Swap x and y for portrait mode */
      art_affine_translate(affine_baseline,
			   -total_width*args->just + sum_width,
			   height[0]/2.5-up_list[i]);
      art_affine_translate(affine_translate,
			   dev->height-args->y/VSCALE,-args->x/VSCALE);
    }
    gnome_canvas_item_affine_relative(item[i],affine_translate);
    gnome_canvas_item_affine_relative(item[i],affine_rotate);
    gnome_canvas_item_affine_relative(item[i],affine_plplot);
    gnome_canvas_item_affine_relative(item[i],affine_baseline);

    /* Keep track of the position in the string */
    sum_width += width[i];
    if(i!=N-1) sum_width += 2; /* Add a little extra space */
  }

#ifdef DEBUG_GCW_2
  gcw_debug("</proc_str>\n");
#endif
}
#endif /*HAVE_FREETYPE */


/*--------------------------------------------------------------------------*\
 * plD_esc_gcw()
 *
 * Escape functions.
 *
\*--------------------------------------------------------------------------*/

void plD_esc_gcw(PLStream *pls, PLINT op, void *ptr)
{
  GcwPLdev* dev = pls->dev;

#ifdef DEBUG_GCW_1
  char opname[20], msg[100];
  if(op==PLESC_DEVINIT) strcpy(opname,"PLESC_DEVINIT");
  else if(op==PLESC_CLEAR) strcpy(opname,"PLESC_CLEAR");
  else if(op==PLESC_FILL) strcpy(opname,"PLESC_FILL");
  else if(op==PLESC_HAS_TEXT) strcpy(opname,"PLESC_HAS_TEXT");
  else if(op==PLESC_GRAPH) strcpy(opname,"PLESC_GRAPH");
  else strcpy(opname,"unknown");
  sprintf(msg,"<plD_esc_gcw />: %s\n",opname);
  gcw_debug(msg);
#endif

  switch(op) {

  case PLESC_DEVINIT:
    gcw_init_canvas(GNOME_CANVAS(ptr));
    pls->hack=0;
    break;

  case PLESC_CLEAR:
    break;

  case PLESC_FILL:
    fill_polygon(pls);
    break;

#ifdef HAVE_FREETYPE
  case PLESC_HAS_TEXT:
    proc_str(pls, ptr); /* Draw the text */
    break;
#endif

  case PLESC_GRAPH:
    break;

  default:
    break;
  }
}
