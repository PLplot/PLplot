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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA 


DESCRIPTION

  This is the Gnome Canvas Widget driver, written by Thomas J. Duck 
  following the heritage of the plplot Gnome driver.  Like all plplot
  drivers, this operates in standalone mode by default.  However, this
  driver can also be used to write to a user-supplied GnomeCanvas.

  Please see the plplot documentation for more information.


DEVELOPMENT NOTES

  Type 1 postscript text is supplied using the GNOME_CANVAS_HACKTEXT item 
  from the libgnomeprintui package.  This item was chosen because it allows
  affine transforms (e.g., rotations) on text, unlike the GNOME_CANVAS_TEXT 
  item.  There is no public api for HACKTEXT, and so I have copied the
  appropriate header file into this project.  The HACKTEXT item has been
  stable since 2001; however, this should be replaced as soon as an 
  alternative becomes available.  We have been waiting a long time for 
  this...


BUGS

  Greek symbols don't seem to work for HAS_TEXT escape function calls.

  The dashed line function doesn't yet work properly.

*/

#include "gcw.h"
#include "gnome-canvas-hacktext.h"


/* Device info */
char* plD_DEVICE_INFO_libgcw = "gcw:Gnome Canvas Widget:1:gcw:10:gcw";

/*
#define DEBUG_GCW
*/

/* Physical dimensions */

/* Drawing units per inch; 1 DU corresponds to a pen width of 1 */
#define DU_PER_IN (360.)

/* Pixels per drawing unit: The default value of 0.2 pixes per drawing 
 * unit gives 72 pixels per inch. */
#define PIXELS_PER_DU (0.2)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per mm for this device */
#define PIXELS_PER_MM (PIXELS_PER_DU * DU_PER_IN / MM_PER_IN)

/* Default dimensions of the canvas (in inches) */
#define CANVAS_WIDTH (9.0)
#define CANVAS_HEIGHT (7.0)

/* Globals */
static int text = 0;
static int aa = 1;

static DrvOpt gcw_options[] = 
  {
    {"text", DRV_INT, &text, "Use truetype fonts (text=0|1)"},
    {"aa", DRV_INT, &aa, "Use antialiasing (aa=0|1)"},
    {NULL, DRV_INT, NULL, NULL}
  };

void debug (char* msg)
{
  fprintf(stderr,msg);
  fflush(stderr);
}


/*--------------------------------------------------------------------------*\
 * plcolor_to_rgba()
 *
 * Converts from the plcolor to rgba color format.
\*--------------------------------------------------------------------------*/
static guint32 plcolor_to_rgba(PLColor color, guchar alpha)
{
  return
    ((int)(color.r) << 24)
    + ((int)(color.g) << 16)
    + ((int)(color.b) << 8)
    + alpha;
}


/*--------------------------------------------------------------------------*\
 * install_canvas()
 *
 * Creates and installs a canvas into a window on the fly.  This function
 * is called if a canvas hasn't been supplied to the driver.
\*--------------------------------------------------------------------------*/

/* Delete event callback */
gint delete_event( GtkWidget *widget,GdkEvent *event,gpointer data ) {
  return FALSE;
}

/* Destroy event calback */
void destroy(GtkWidget *widget,gpointer data) {
  gtk_main_quit ();
}

void install_canvas(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;
  GtkWidget *window;
  GnomeCanvas *canvas;

#ifdef DEBUG_GCW
  debug("<install_canvas>\n");
#endif

  if(dev->window==NULL) {

    /* Create a new window and prepare a notebook for it */

    /* Initialize */
    g_type_init();
    gtk_init(0,NULL);

    /* Create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dev->window = window;
    gtk_window_set_title(GTK_WINDOW(window),"");

    /* Connect the signal handlers to the window decorations */
    g_signal_connect(G_OBJECT(window),"delete_event",
		     G_CALLBACK(delete_event),NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);

    /* Create the new notebook */
    dev->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(dev->notebook),TRUE);

    /* Put the notebook into the window */
    gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(dev->notebook));
  }

  /* Install a new canvas in the notebook*/
  if(aa) {
    canvas = GNOME_CANVAS(gnome_canvas_new_aa());
  }
  else
    canvas = GNOME_CANVAS(gnome_canvas_new());

  gcw_set_canvas(pls,canvas);

  gcw_set_canvas_zoom(canvas,1.5);
  gtk_notebook_append_page(GTK_NOTEBOOK(dev->notebook),GTK_WIDGET(canvas),
			   NULL);

  /* Display everything */
  gtk_widget_show_all(dev->window);
}


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

  /* Add the canvas to the device */
  dev->canvas=canvas;

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Attach the device to the canvas widget */
  g_object_set_data(G_OBJECT(canvas),"dev",(gpointer)dev);

  /* Add the background to the canvas and move it to the back */
  dev->group_background = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL);

  dev->background = gnome_canvas_item_new(
		        dev->group_background,
			gnome_canvas_rect_get_type(),
			"x1", 0.0,
			"y1", -dev->height,
			"x2", dev->width,
			"y2", 0.0,
			"fill-color-rgba",
			plcolor_to_rgba(pls->cmap0[0],0xFF),
			"width-units", 0.0,
			NULL);
  gnome_canvas_item_lower_to_bottom(GNOME_CANVAS_ITEM(dev->group_background));

  /* Add the foreground to the canvas and move it to the front */
  dev->group_foreground = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL);
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
  PLFLT default_aspect = CANVAS_WIDTH/CANVAS_HEIGHT;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_aspect>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  /* Set the new width and height */
  if(aspect < default_aspect) {
    dev->width = PIXELS_PER_DU*DU_PER_IN*CANVAS_WIDTH*aspect/default_aspect;
    dev->height = PIXELS_PER_DU * DU_PER_IN * CANVAS_HEIGHT;
  }
  else {
    dev->height = PIXELS_PER_DU*DU_PER_IN*CANVAS_HEIGHT*default_aspect/aspect;
    dev->width = PIXELS_PER_DU * DU_PER_IN * CANVAS_WIDTH;
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

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_zoom>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  gnome_canvas_set_pixels_per_unit(canvas,magnification);

  gtk_widget_set_size_request(GTK_WIDGET(canvas),
			      (dev->width+1)*magnification,
			      (dev->height+2)*magnification);

  gnome_canvas_set_scroll_region(canvas,0,-dev->height,
				 dev->width+1,1);

  dev->zoom_is_initialized = TRUE;
}


/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_size()
 *
 * Sets the canvas size by setting the aspect and zoom
\*--------------------------------------------------------------------------*/
void gcw_set_canvas_size(GnomeCanvas* canvas,PLFLT width,PLFLT height)
{
  PLFLT magnification;
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_set_canvas_size>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  gcw_set_canvas_aspect(canvas,width/height);

  magnification = width/dev->width;

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

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  *xmin2 = xmin1*dev->width/(PIXELS_PER_DU * DU_PER_IN * CANVAS_WIDTH);
  *xmax2 = xmax1*dev->width/(PIXELS_PER_DU * DU_PER_IN * CANVAS_WIDTH);
  *ymin2 = ymin1*dev->height/(PIXELS_PER_DU * DU_PER_IN * CANVAS_HEIGHT);
  *ymax2 = ymax1*dev->height/(PIXELS_PER_DU * DU_PER_IN * CANVAS_HEIGHT);
}


/*--------------------------------------------------------------------------*\
 * gcw_use_text()
 *
 * Used to turn text usage on and off.
\*--------------------------------------------------------------------------*/
void gcw_use_text(GnomeCanvas* canvas,gboolean use_text)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<gcw_use_text>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  dev->use_text = use_text;

  /* Use a hack to the plplot escape mechanism to update text handling */
  plP_esc(PLESC_HAS_TEXT,NULL);
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

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  dev->group_current = dev->group_background;
}
void gcw_use_foreground_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  dev->group_current = dev->group_foreground;
}
void gcw_use_default_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

  if(!GNOME_IS_CANVAS(canvas)) {
    fprintf(stderr,"\n\n*** GCW driver error: canvas not found.\n");
    return;
  }

  /* Retrieve the device */
  dev = g_object_get_data(G_OBJECT(canvas),"dev");

  dev->group_current = dev->group_hidden;
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
    pdt->pl_MenuStr  = "Gnome Canvas Widget";
    pdt->pl_DevName  = "gcw";
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
}


/*--------------------------------------------------------------------------*\
 * plD_init_gcw()
 *
 * Initializes the device.
 *
 * This routine is invoked by a call to plinit, along with others:
 *
 *   <plD_init_gcw>
 *   <plD_bop_gcw>
 *   <plD_esc_gcw>: PLESC_GRAPH
 *   <plD_state_gcw>: PLSTATE_COLOR0
 *
\*--------------------------------------------------------------------------*/

void plD_init_gcw(PLStream *pls)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW
  debug("<plD_init_gcw>\n");
#endif

  /* Parse the driver options */
  plParseDrvOpts(gcw_options);

  pls->termin = 1;      /* Is an interactive terminal */
  pls->dev_flush = 1;   /* Handle our own flushes */
  pls->plbuf_write = 1; /* Use plot buffer to replot to another device */
  pls->width = 1;
  pls->dev_clear = 1;   /* Handle plclear() */
  pls->dev_fill0 = 1;	/* Handle solid fills */
  pls->dev_dash = 0;	/* Handle dashed lines */

  /* Create the device */
  if((dev = g_malloc(sizeof(GcwPLdev))) == NULL) {
    fprintf(stdout,"Insufficient memory\n");
    exit(0);
  }
  pls->dev = dev;

  /* Set the number of pixels per mm */
  plP_setpxl((PLFLT)PIXELS_PER_MM,(PLFLT)PIXELS_PER_MM);

  /* Set up physical limits of plotting device (in drawing units) */
  plP_setphy((PLINT)0,(PLINT)CANVAS_WIDTH*DU_PER_IN,
	     (PLINT)0,(PLINT)CANVAS_HEIGHT*DU_PER_IN);

  /* Set the magnification */
  dev->magnification = 1.;

  /* Set the width and height for the device */
  dev->width = PIXELS_PER_DU * DU_PER_IN * CANVAS_WIDTH;
  dev->height = PIXELS_PER_DU * DU_PER_IN * CANVAS_HEIGHT;

  /* Set text handling */
  if(aa && text) {
    pls->dev_text = TRUE;
    dev->use_text = TRUE;
  }
  else {
    pls->dev_text = FALSE;
    dev->use_text = FALSE;
  }

  /* Only initialize the zoom after all other initialization is complete */
  dev->zoom_is_initialized = FALSE;

  /* Set the device canvas pointer and window */
  dev->canvas = NULL;
  dev->window = NULL;

  /* Initialize the visible and hidden groups.  All of the plplot plotting
   * commands are drawn to the hidden group.  When the page is finalized,
   * the group is made visible, and the old group destroyed. */
  dev->group_visible=NULL;
  dev->group_hidden=NULL;
  dev->group_background=NULL;
  dev->group_foreground=NULL;
  dev->group_current=NULL;
}


/*--------------------------------------------------------------------------*\
 * plD_polyline_gcw()
 *
 * Draw a polyline in the current color from (x1,y1) to (x2,y2).
 *
\*--------------------------------------------------------------------------*/

void plD_polyline_gcw(PLStream *pls, short *x, short *y, PLINT npts)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;
  GdkGC* gc;
  GdkPoint* gdkpoints;
  guint i;

#ifdef DEBUG_GCW
  debug("<plD_polyline_gcw>\n");
#endif

  if(npts >= 2) {

    if(dev->canvas==NULL) install_canvas(pls);
    canvas = dev->canvas;

    if(dev->group_hidden==NULL) plD_bop_gcw(pls);
    group = dev->group_current;

    points = gnome_canvas_points_new(npts);

    for ( i = 0; i < npts; i++ ) {
      points->coords[2*i] = ((double) x[i]) * PIXELS_PER_DU;
      points->coords[2*i + 1] = ((double) -y[i]) * PIXELS_PER_DU;
    }

    item=gnome_canvas_item_new(group,
			       gnome_canvas_line_get_type (),
			       "cap_style", GDK_CAP_ROUND,
			       "join-style", GDK_JOIN_ROUND,
			       "points", points,
			       "fill-color-rgba",dev->color,
			       "width-units",pls->width*PIXELS_PER_DU*3,
			       NULL);
    
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

#ifdef DEBUG_GCW
  debug("<plD_line_gcw>\n");
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
  GnomeCanvasItem* item;
  GnomeCanvasGroup* group;

  int i;
  short x, y;

#ifdef DEBUG_GCW
  debug("<plD_eop_gcw>\n");
#endif

  if(dev->canvas!=NULL) {

    canvas = dev->canvas;

    /* Make the hidden group visible */
    gnome_canvas_item_show((GnomeCanvasItem*)(dev->group_hidden));

    /* Destroy the current visible group */
    if(dev->group_visible!=NULL) {
      gtk_object_destroy((GtkObject*)(dev->group_visible));
      dev->group_visible = NULL;
    }

    /* Name the hidden group as visible */
    dev->group_visible = dev->group_hidden;
    dev->group_hidden=NULL;

    /* Update the canvas */
    canvas->need_update = 1;
    gnome_canvas_update_now (canvas);
    
    /* If the driver is creating its own canvasses, set dev->canvas to be
     * NULL now in order to force creation of a new canvas when the next
     * drawing call is made.
     */
    if(dev->window!=NULL) {
      dev->canvas=NULL;
      dev->group_visible=NULL;
      dev->group_hidden=NULL;
      dev->group_background=NULL;
      dev->group_foreground=NULL;
      dev->group_current=NULL;
    }
  }
}


/*--------------------------------------------------------------------------*\
 * plD_bop_gcw()
 *
 * Set up for the next page.
 *
 * This routine is invoked by a call to plinit, along with others:
 *
 *   <plD_init_gcw>
 *   <plD_bop_gcw>
 *   <plD_esc_gcw>: PLESC_GRAPH
 *   <plD_state_gcw>: PLSTATE_COLOR0
 *
\*--------------------------------------------------------------------------*/

void plD_bop_gcw(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;

#ifdef DEBUG_GCW
  debug("<plD_bop_gcw>\n");
#endif

  if(dev->canvas!=NULL) { 
    canvas = dev->canvas;

    /* Make sure the zoom is initialized */
    if(!dev->zoom_is_initialized) gcw_set_canvas_zoom(canvas,1.);

    /* Creat a new hidden group; all new drawing will be to this group */
    dev->group_hidden = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL);
    gnome_canvas_item_hide((GnomeCanvasItem*)(dev->group_hidden));

    /* Set the hidden group as current unless it is fore or background */
    if( (dev->group_current != dev->group_foreground) &&
	(dev->group_current != dev->group_background) )
      dev->group_current = dev->group_hidden;

    /* Make sure the foreground group is at the front, and the background
     * group is at the back 
     */
    gnome_canvas_item_raise_to_top(
			    GNOME_CANVAS_ITEM(dev->group_foreground));
    gnome_canvas_item_lower_to_bottom(
			    GNOME_CANVAS_ITEM(dev->group_background));
  }
}


/*--------------------------------------------------------------------------*\
 * plD_tidy_gcw()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void plD_tidy_gcw(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;

#ifdef DEBUG_GCW
  debug("<plD_tidy_gcw>\n");
#endif

  if(dev->window!=NULL) {
    gtk_main ();
    exit(0); /* Terrible hack to avoid segfault - why does it happen? */
  }
}


/*--------------------------------------------------------------------------*\
 * plD_state_gcw()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
 *
 * This routine is invoked by a call to plinit, along with others:
 *
 *   <plD_init_gcw>
 *   <plD_bop_gcw>
 *   <plD_esc_gcw>: PLESC_GRAPH
 *   <plD_state_gcw>: PLSTATE_COLOR0
 *
\*--------------------------------------------------------------------------*/

void plD_state_gcw(PLStream *pls, PLINT op)
{
  GcwPLdev* dev = pls->dev;
  char msg[100];

#ifdef DEBUG_GCW
  sprintf(msg,"<plD_state_gcw>: %d\n",op);
  debug(msg);
#endif

  switch (op) {
    case (1): /* PLSTATE_WIDTH */
      if(dev->canvas==NULL) install_canvas(pls);
      break;
    case (2): /* PLSTATE_COLOR0 */
      dev->color = plcolor_to_rgba(pls->cmap0[pls->icol0],0xFF);
      break;
    case (3): /* PLSTATE_COLOR1 */
      if(dev->canvas==NULL) install_canvas(pls);
      dev->color = plcolor_to_rgba(pls->cmap1[pls->icol1],0xFF);
      break;
    case (4): /* PLSTATE_FILL */
      if(dev->canvas==NULL) install_canvas(pls);
      break;
    case (5): /* PLSTATE_CMAP0 */
      if(dev->canvas==NULL) install_canvas(pls);
      break;
    case (6): /* PLSTATE_CMAP1 */
      if(dev->canvas==NULL) install_canvas(pls);
      break;
    default: 
      if(dev->canvas==NULL) install_canvas(pls);
      break;
  }
}


/*--------------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fills the polygon defined by the given points.  Used for shade
 * plotting.  Only solid fills are allowed.
 *
\*--------------------------------------------------------------------------*/

static void fill_polygon (PLStream* pls)
{
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GdkGC* gc;
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;
  GdkPoint* gdkpoints;
  guint i;

#ifdef DEBUG_GCW
  debug("<fill_polygon>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);
  group = dev->group_current;

  points = gnome_canvas_points_new (pls->dev_npts);

  for (i=0; i<pls->dev_npts; i++) {
    points->coords[2*i] = ((double) pls->dev_x[i]) * PIXELS_PER_DU;
    points->coords[2*i + 1] = ((double) -pls->dev_y[i]) * PIXELS_PER_DU;
  }

  item = gnome_canvas_item_new (group,
				GNOME_TYPE_CANVAS_POLYGON,
				"points", points,
				"fill-color-rgba",dev->color,
				"outline-color-rgba",dev->color,
				NULL);
  
  gnome_canvas_points_free (points);
}


/*--------------------------------------------------------------------------*\
 * dashed_line()
 *
 * Handles call for dashed line.  Currently there is no facility for
 * setting dash lengths on the Gdk-type GnomeCanvas; there is no dash
 * facility whatsoever for antialiased GnomeCanvas objects.
\*--------------------------------------------------------------------------*/

static void dashed_line(PLStream* pls)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;

/*   gchar* dash_list; */
/*   gint dash_list_len; */

  guint i;

#ifdef DEBUG_GCW
  debug("<dashed_line>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);
  group = dev->group_current;

/*   /\* Save the dash list in a handy construct *\/ */
/*   dash_list_len = 2 * pls->nms; */
/*   dash_list = g_malloc (dash_list_len * sizeof(gchar)); */
/*   for (i = 0; i < pls->nms; i++) { */
/*     dash_list[2*i] = (gchar) ceil ((pls->mark[i]/1e3) * PIXELS_PER_MM); */
/*     dash_list[2*i+1] = (gchar) floor ((pls->space[i]/1e3) * PIXELS_PER_MM); */
/*   } */


  /* Save the points list */
  points = gnome_canvas_points_new (pls->dev_npts);
  for (i = 0; i < pls->dev_npts; i++) {
    points->coords[2*i] = ((double) pls->dev_x[i]) * PIXELS_PER_DU;
    points->coords[2*i+1] = ((double) -pls->dev_y[i]) * PIXELS_PER_DU;
  }

  item = gnome_canvas_item_new (group,
				GNOME_TYPE_CANVAS_LINE,
				"cap_style", GDK_CAP_BUTT,
				"join_style", GDK_JOIN_ROUND,
                                "points", points,
 				"line-style", GDK_LINE_ON_OFF_DASH,
				"fill-color-rgba",dev->color,
                                "width-units",pls->width*PIXELS_PER_DU*3,
                                NULL);

  gnome_canvas_points_free (points);
}


/*--------------------------------------------------------------------------*\
 * clear()
 *
 * Handles call to clear the canvas
\*--------------------------------------------------------------------------*/

static void clear (PLStream* pls)
{
  GcwPLdev* dev = pls->dev;
  GnomeCanvasGroup* group;

#ifdef DEBUG_GCW
  debug("<clear>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);

  if(dev->group_visible!=NULL) {
    gtk_object_destroy((GtkObject*)(dev->group_visible));
    dev->group_visible = NULL;
  }

  if(dev->group_hidden!=NULL){
    gtk_object_destroy((GtkObject*)(dev->group_hidden));
    dev->group_hidden = NULL;
  }
}

/*--------------------------------------------------------------------------*\
 * adobe_symbol_enc (in)
 *
 * Transforms the character in from the PLplot Greek convention into the
 * Adobe encoding for the Symbol font.
\*--------------------------------------------------------------------------*/

static char adobe_symbol_enc (char in) 
{
  char retval = in;
  
  switch (in) {
  case 'C': retval = 'X'; break;
  case 'Y': retval = 'H'; break;
  case 'H': retval = 'Q'; break;
  case 'Q': retval = 'Y'; break;
  case 'X': retval = 'C'; break;
  case 'c': retval = 'x'; break;    
  case 'y': retval = 'h'; break;
  case 'h': retval = 'q'; break;
  case 'q': retval = 'y'; break;
  case 'x': retval = 'c'; break;
  }
    
  return retval;
}

/*--------------------------------------------------------------------------*\
 * proc_str()
 *
 * Handles call to draw text on the canvas when the HAS_TEXT escape funtion
 * case is invoked.
 *
\*--------------------------------------------------------------------------*/

void proc_str(PLStream *pls, EscText *args)
{
  PLFLT *t = args->xform, tt[4]; /* Transform matrices */
  PLFLT theta, shear;  /* Rotation angle and shear from the matrix */
  PLINT clxmin, clxmax, clymin, clymax; /* Clip limits */

  gint size;
  double width[200],height[200];
  char *fontname;
  char *symbolfontname = "Numbus Sans L Standard Symbols L";

  gdouble affine_baseline[6] = {0.,0.,0.,0.,0.,0.};
  gdouble affine_shear[6] = {0.,0.,0.,0.,0.,0.};
  gdouble affine_rotate[6] = {0.,0.,0.,0.,0.,0.};
  gdouble affine_translate[6] = {0.,0.,0.,0.,0.,0.};

  GnomeCanvasGroup* group;
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;

  GnomeCanvasItem* item[200];
  GnomeFont *font;
  GnomeFontFace *face;
  GnomeGlyphList *glyphlist;

  ArtDRect bbox;

  char *str,*c1,*c2;
  int i,N=0;
  double total_width=0,sum_width=0;

  double advance = 1.5;
  double up=0,scale=1;
  double up_list[200];

  gboolean flag = FALSE;

  gboolean symbol = FALSE;

  char esc,c;

#ifdef DEBUG_GCW
  debug("<proc_str>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);
  group = dev->group_current;

  /* The transform matrix has only rotations and shears; extract them */

  /* Note: For some reason, I have to divide the shear by a correction
   * factor of 1.28.  Why?
   */

  theta = acos(t[0]) * 180. / PI;  /* Determine the rotation (in degrees)... */
  if (t[2] < 0.) theta *= -1.;     /* ... and sign ... */
  if(cos(theta*PI/180.)<0.000001)  /* ... and shear */
    shear = t[3]/sin(theta*PI/180.) * 180. / PI / 1.28;
  else shear = (t[1]+sin(theta*PI/180.))/cos(theta*PI/180.) * 180. / PI /1.28;

  /* Apply plplot difilt transformations */
  difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);

  /* Font height: size is in pixels. chrht is in mm.
   * The factor at the end matches the font size to plplot's native
   * font size.
   */
  size = (gint)(pls->chrht/MM_PER_IN*DU_PER_IN*PIXELS_PER_DU/3.4);
  advance *= (float)size / 11.;

  /* apply transformations */
  difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);

  /* Determine the font */
  switch (pls->cfont) {
    case (1): 
      fontname = "Nimbus Sans L Regular"; 
      break;
    case (2): 
      fontname = "Nimbus Roman No9 L Regular";
      break;
    case (3): 
      fontname = "Nimbus Roman No9 L Regular Italic"; 
      break;
    case (4): 
      fontname = "Nimbus Sans L Regular Italic"; 
      break;
    default:
      fontname = "Nimbus Sans L Regular"; 
      break;
  }

  /* Make a copy of the string */
  if ( (str = strdup (args->string)) == NULL) {
    fprintf(stderr,"\n\n*** GCW driver error: insufficient memory.\n");
    return;
  }

  c1 = c2 = str; 

  plgesc(&esc);

  while(!flag) {

    if(*c1 == esc) {
      c1++; c2++;

      if (*c1 == esc) { /* <esc><esc> */
	c2++;
      }
      else switch (*c1) {

      case 'f':
	c1++; c2++;
	switch (*c1) {
	case 'n': 
	  fontname = "Nimbus Sans L Regular"; 
	  break;
	case 'r': 
	  fontname = "Nimbus Roman No9 L Regular"; 
	  break;
	case 'i': 
	  fontname = "Nimbus Roman No9 L Regular Italic"; 
	  break;
	case 's':
	  fontname = "URW Chancery L"; 
	  break;
	default:
	  fontname = "Nimbus Sans L Regular"; 
	  break;
	}
	c1++; c2++;
	break;
	
      case 'd':
	if(up>0.) scale *= 1.25;  /* Subscript scaling parameter */
	else scale *= 0.8;  /* Subscript scaling parameter */
	up -= size / 2.;
	c1++; c2++;
	break;

      case 'u':
	if(up<0.) scale *= 1.25;  /* Subscript scaling parameter */
	else scale *= 0.8;  /* Subscript scaling parameter */
	up += size / 2.;
	c1++; c2++;
	break;

      case 'g':
	c1++; c2++;
	*c1 = adobe_symbol_enc(*c1);
	symbol = TRUE;
	break;

	/* ignore the next sequences */

      case '+':
      case '-':
      case 'b':
	plwarn("'+', '-', and 'b' text escape sequences not processed.");
	c1++; c2++;
	break;

      case '(':
	plwarn("'g(...)' text escape sequence not processed.");
	while (*c1++ != ')' && *c1!='\0') c2++;
	break;
      }
    }

    up_list[N] = up;

    /* Move c2 along to the next stop point */
    if(symbol) c2++;
    while(!symbol && *c2!='\0' && *c2!=esc) c2++;

    if(*c2=='\0') flag = TRUE;
    else {
      c = *c2;
      *c2 = '\0';
    }

    /* Get the font */
    if(symbol) face = gnome_font_face_find_closest(symbolfontname);
    else face = gnome_font_face_find_closest(fontname);

    font = gnome_font_face_get_font_default(face,size*scale);

/*     if(symbol) */
/*       printf("\nFONT: %s (wanted %s)\n",gnome_font_get_name(font),symbolfontname); */
/*     else */
    if(symbol)
      printf("\nFONT: %s (wanted %s)\n",gnome_font_get_name(font),symbolfontname);

    /* Get the glyphs */
    glyphlist = gnome_glyphlist_from_text_dumb(font,dev->color,0.,0.,c1);

    /* Replace the deleted character */
    if(!flag) *c2 = c;

    /* Determine the bounding box of the text */
    gnome_glyphlist_bbox(glyphlist,NULL,0,&bbox);
    width[N] = bbox.x1-bbox.x0;
    height[N] = bbox.y1-bbox.y0;

    total_width += width[N];

    /* Create the canvas text item */
    item[N] = gnome_canvas_item_new (group,
				     GNOME_TYPE_CANVAS_HACKTEXT,
				     "glyphlist",glyphlist,
				     "fill-color-rgba",dev->color,
				     "x",0.,
				     "y",0.,
				     NULL);

    /* Advance */
    symbol = FALSE;
    c1=c2;
    N++;
  }

  total_width += (N-1)*advance;

  for(i=0;i<N;i++) {

    /* Calculate and apply the affine transforms */
    art_affine_translate(affine_baseline,
			 -total_width*args->just + sum_width,
			 height[0]/2.5-up_list[i]);
    art_affine_shear(affine_shear,-shear);
    art_affine_rotate(affine_rotate,-theta);
    art_affine_translate(affine_translate,
			 args->x*PIXELS_PER_DU,-args->y*PIXELS_PER_DU);
    gnome_canvas_item_affine_relative(item[i],affine_translate);
    gnome_canvas_item_affine_relative(item[i],affine_rotate);
    gnome_canvas_item_affine_relative(item[i],affine_shear);
    gnome_canvas_item_affine_relative(item[i],affine_baseline);

    sum_width += width[i] + advance;
  }
}
/*--------------------------------------------------------------------------*\
 * plD_esc_gcw()
 *
 * Escape functions.
 *
 * This routine is invoked by a call to plinit, along with others:
 *
 *   <plD_init_gcw>
 *   <plD_bop_gcw>
 *   <plD_esc_gcw>: PLESC_GRAPH
 *   <plD_state_gcw>: PLSTATE_COLOR0

\*--------------------------------------------------------------------------*/

void plD_esc_gcw(PLStream *pls, PLINT op, void *ptr)
{
  GcwPLdev* dev = pls->dev;

  char msg[100];

#ifdef DEBUG_GCW
  sprintf(msg,"<plD_esc_gcw>: %d\n",op);
  debug(msg);
#endif

  switch (op) {

  case PLESC_DEVINIT:
    gcw_set_canvas(pls,GNOME_CANVAS(ptr));
    break;

  case PLESC_CLEAR:
    clear (pls);
    break;

  case PLESC_DASH:
    dashed_line (pls);
    break;

  case PLESC_FILL:
    fill_polygon(pls);
    break;

  case PLESC_HAS_TEXT:
    if(ptr!=NULL) proc_str(pls, ptr); /* Draw the text */
    else { /* Assume this was a request to change the text handling */
      if(dev->use_text) pls->dev_text = 1; /* Allow text handling */
      else pls->dev_text = 0; /* Disallow text handling */
    }
    break;

  case PLESC_GRAPH:
    break;

  default:
    break;
  }
}
