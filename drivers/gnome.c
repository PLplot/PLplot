/* $Id$

        PLplot GnomeCanvas device driver.
*/
#include "plplot/plDevs.h"
#include "plplot/plplotP.h"

#include <gtk/gtk.h>                                                       
#include <glib.h>                                                          
#include <pthread.h>                                                       

#include <gnome.h>

#include <unistd.h>

#define DEBUG

#undef ANTIALISED_CANVAS

/* Physical dimensions */

/* Unit of drawing (1/360 in).  Corresponds to the pen widht = 1 */
#define DRAWING_UNIT (0.0027777778)

/* How many pixels in a drawing unit (defualt value) */
#define PIXELS_PER_DU (0.2)

/* mm per inch */
#define MM_PER_IN (25.4)

/* Default dimensions of the canvas (in inches) */
#define WIDTH (9)
#define HEIGHT (6.5)

#ifdef PLD_gnome

void
debug (char* msg)
{
  //      printf (msg);
  //      fflush (stdout);
}

G_LOCK_DEFINE_STATIC (canvas);

static pthread_t tid;

static guint gnome_is_initialized = FALSE;

// FIXME : Terribel global variable hack
static GtkStatusbar* sb;

typedef enum {
  GNOME_PLDEV_LOCATE_MODE,
  GNOME_PLDEV_ZOOM_MODE,
  GNOME_PLDEV_NONE_MODE
} GnomePLdevCanvasMode;
  
typedef struct {
  GnomeCanvas* canvas;
  GnomePLdevCanvasMode mode;
  GnomeCanvasItem* hlocline;
  double hpos;
  GnomeCanvasItem* vlocline;
  double vpos;
  GnomeCanvasItem* zoomrect;
  PLGraphicsIn gin;
  guint context;
  double width;
  double height;
} GnomePLdevPage;

typedef struct {
  GtkWidget* parent;
  GtkWidget* root;
  GtkNotebook* notebook;
  GtkStatusbar* statusbar;
  guint npages;
  GnomePLdevPage** page;
  guint parent_is_from_driver;
} GnomePLdev;

void
gnome_pldev_adopt (PLStream* pls, GtkContainer* parent) 
{
  GnomePLdev* dev = pls->dev;
  GtkWidget* root;

  if (dev->parent_is_from_driver) {
    root = dev->root;
    gtk_object_ref (GTK_OBJECT (root));
    gtk_container_remove (GTK_CONTAINER (dev->parent), root);
    gtk_widget_destroy (dev->parent);
    dev->parent_is_from_driver = FALSE;
  }
  gtk_container_add (parent, root);
  dev->parent = GTK_WIDGET (parent);
  gtk_object_unref (GTK_OBJECT (root));
}

static
void change_mode (GnomePLdevPage* page, GnomePLdevCanvasMode mode)
{
  guint context = page->context;

  page->mode = mode;

  switch (mode) {
  case GNOME_PLDEV_LOCATE_MODE:
    // FIXME : Terribel global variable hack
    gtk_statusbar_pop (sb, context);
    // FIXME : Terribel global variable hack
    gtk_statusbar_push (sb, context, "Locate Mode");
    break;
  case GNOME_PLDEV_ZOOM_MODE:
    // FIXME : Terribel global variable hack
    gtk_statusbar_pop (sb, context);
    // FIXME : Terribel global variable hack
    gtk_statusbar_push (sb, context, "Zoom Mode");
    break;
  default:
    break;
  }
}

static
void *init(void *args)                                          
{
  gdk_threads_enter();
  gtk_main();
  gdk_threads_leave ();
  debug ("Leaving init\n");
}
 
static void 
quit (void)
{
  
  gdk_threads_enter ();
  gtk_main_quit ();
  gdk_threads_leave ();
  debug ("Quitting\n");

  pthread_exit(NULL);
}

static gint 
canvas_pressed_cb(GnomeCanvasItem *item, GdkEvent *event,
		  GnomePLdevPage* page)
{
  static guint dragging;
  guint move;
  GdkCursor *cursor;
  char buffer[128];
  PLGraphicsIn* gin = &(page->gin);

  move = FALSE;
  switch (event->type) {

  case GDK_2BUTTON_PRESS:

  case GDK_3BUTTON_PRESS:

  case GDK_BUTTON_PRESS:
    
    if (event->button.button == 1) {

      gnome_canvas_item_show (page->hlocline);
      gnome_canvas_item_raise_to_top (page->hlocline);
      
      gnome_canvas_item_show (page->vlocline);
      gnome_canvas_item_raise_to_top (page->vlocline);
      
      move = TRUE;

      cursor = gdk_cursor_new (GDK_CROSSHAIR);
      gnome_canvas_item_grab(item,
			     GDK_POINTER_MOTION_MASK | 
			     GDK_BUTTON_RELEASE_MASK,
			     cursor,
			     event->button.time);
      gdk_cursor_destroy(cursor);

      // FIXME : Terribel global variable hack
      gtk_statusbar_push (sb, page->context, "");

      dragging = TRUE;
    }
    //    printf ("Button press\n");
    //    fflush (stdout);
    break;

  case GDK_MOTION_NOTIFY:
    if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) 
      move = TRUE;
    //    printf ("Mouse motion\n");
    //    fflush (stdout);
    break;
    
  case GDK_BUTTON_RELEASE:
    if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
      gnome_canvas_item_ungrab(item, event->button.time);
      dragging = FALSE;
      gnome_canvas_item_hide (page->hlocline);
      gnome_canvas_item_hide (page->vlocline);
      // FIXME : Terribel global variable hack
      gtk_statusbar_pop (sb, page->context);
      //      printf ("Button release\n");
      //      fflush (stdout);
    }
    break;
    
  default:
    //    printf ("Other event\n");
    //    fflush (stdout);
    break;
    
  }

  if (move) {

    gnome_canvas_item_move (page->hlocline,
			    0.0, event->button.y - page->vpos);    
    page->vpos = event->button.y;
    
    gnome_canvas_item_move (page->vlocline,
			    event->button.x - page->hpos, 0.0);    
    page->hpos = event->button.x;

    gin->dX = page->hpos / page->width;
    gin->dY = page->vpos / (- page->height);

    plTranslateCursor (gin);

    sprintf (buffer, "   x = %f   y = %f", gin->wX, gin->wY);
    // FIXME : Terribel global variable hack
    gtk_statusbar_pop (sb, page->context);
    // FIXME : Terribel global variable hack
    gtk_statusbar_push (sb, page->context, buffer);

  }

  return FALSE;
}


static gint 
delete_event_cb (GtkWidget* widget, GdkEventAny* e, gpointer data)
{
  printf ("Entered delete_event_cb\n");
  fflush (stdout);
  gdk_threads_enter ();
  gtk_widget_destroy (widget);
  gdk_threads_leave ();
  quit ();
  return FALSE;
}

static gint
item_event(GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
  switch (event->type) {
  case GDK_BUTTON_PRESS:
    printf("item_event\n");
    fflush (stdout);
    break;
  }

  return TRUE;
}

static void
setup_item(GnomeCanvasItem *item)
{
  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      NULL);
}

static double ppu = 1.0;

static gint
key_cb (GtkWidget* widget, GdkEventKey* event, PLStream* pls)
{
  GnomePLdev* dev = pls->dev;
  GnomePLdevPage* page;
  GtkNotebook* notebook = dev->notebook;
  GtkStatusbar* statusbar = dev->statusbar;
  guint curpage;

  if (notebook != NULL)
    curpage = gtk_notebook_get_current_page (notebook) - 1;
  else
    curpage = 0;

  page = dev->page[curpage];
  
  switch (event->keyval) {
  case GDK_minus:
    ppu /= 1.4142;
    break;
  case GDK_equal:
    ppu *= 1.4142;
    break;
  case GDK_q:
    gdk_threads_enter ();
    gtk_signal_emit_by_name
      (GTK_OBJECT (gtk_widget_get_ancestor
		   (GTK_WIDGET (widget), GTK_TYPE_WINDOW)),
       "destroy");
    gdk_threads_leave ();
    break;
  case GDK_l:
    gdk_threads_enter ();
    change_mode (page, GNOME_PLDEV_LOCATE_MODE);
    gdk_threads_leave ();
    break;
  case GDK_z:
    gdk_threads_enter ();
    change_mode (page, GNOME_PLDEV_ZOOM_MODE);
    gdk_threads_leave ();
    break;
  default:
    break;
  }
  gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (widget), ppu);
  return TRUE;
}

static void
page_switch (GtkNotebook *notebook, GtkNotebookPage *page, gint page_num,
	     gpointer dev)
{
  guint curpage;
  GnomeCanvas* canvas;

  canvas = ((GnomePLdev*)(dev))->page[page_num]->canvas;
  gtk_widget_grab_focus (GTK_WIDGET (canvas));
}

static guint32
plcolor_to_rgba (PLColor color, guchar alpha)
{
  return
    ((int)(color.r) << 24)
    + ((int)(color.g) << 16)
    + ((int)(color.b) << 8)
    + alpha;
}

static guint32
plcolor_to_rgba_inv (PLColor color, guchar alpha)
{
  return 
    ((int)(255 - color.r) << 24)
    + ((int)(255 - color.g) << 16)
    + ((int)(255 - color.b) << 8)
    + alpha;
}

static void
new_page (PLStream* pls)
{
  GnomeCanvasGroup* group;
  GnomeCanvasItem* background;
  GnomeCanvas* canvas;
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvasPoints* points;
  GtkScrolledWindow* sw;
  guint np;
  guint32 loclinecolor;
  char buffer[32];

  debug("Entered new_page\n");

  dev = pls->dev;
  page = g_malloc (sizeof (GnomePLdevPage));
  page->mode = GNOME_PLDEV_LOCATE_MODE;

  np = dev->npages;

  debug("Before gdk_init_rgb\n");

  gdk_threads_enter ();

  debug("After gdk_init_rgb\n");

#ifdef ANTIALISED_CANVAS

  gtk_widget_push_visual(gdk_rgb_get_visual());
  gtk_widget_push_colormap(gdk_rgb_get_cmap());
  canvas = GNOME_CANVAS (gnome_canvas_new_aa ());

#else

  gtk_widget_push_visual(gdk_imlib_get_visual());
  gtk_widget_push_colormap(gdk_imlib_get_colormap());
  canvas = GNOME_CANVAS (gnome_canvas_new ());

#endif

  page->canvas = canvas;

  page->width = PIXELS_PER_DU * WIDTH / DRAWING_UNIT;
  page->height = PIXELS_PER_DU * HEIGHT / DRAWING_UNIT;

  gnome_canvas_set_scroll_region(GNOME_CANVAS(canvas),
				 0, -page->height, page->width, 0);

  debug("After scroll_region\n");

  gtk_widget_pop_visual();
  gtk_widget_pop_colormap();

  debug("Before gnome_canvas_root\n");

  group = gnome_canvas_root (GNOME_CANVAS (canvas));

  background = gnome_canvas_item_new (group,
				      gnome_canvas_rect_get_type(),
				      "x1", 0.0,
				      "y1", -page->height,
				      "x2", page->width,
				      "y2", 0.0, 
				      "fill_color", (pls->cmap0[0]).name,
				      "width_units", 0.0,
				      NULL);

  points = gnome_canvas_points_new (2);

  points->coords[0] = 0.0;
  points->coords[1] = 0.0;
  points->coords[2] = page->width;
  points->coords[3] = 0.0;

  loclinecolor = plcolor_to_rgba_inv (pls->cmap0[0], 0xF0),
  page->hlocline =
    gnome_canvas_item_new (group,
			   gnome_canvas_line_get_type(),
			   "points", points,
			   "fill_color_rgba", loclinecolor,
			   "width_units", 1.0,
			   NULL);
  gnome_canvas_item_hide (page->hlocline);
  page->hpos = 0.0;

  points->coords[0] = 0.0;
  points->coords[1] = -page->height;
  points->coords[2] = 0.0;
  points->coords[3] = 0.0;

  page->vlocline =
    gnome_canvas_item_new (group,
			   gnome_canvas_line_get_type(),
			   "points", points,
			   "fill_color_rgba", loclinecolor,
			   "width_units", 1.0,
			   NULL);
  gnome_canvas_item_hide (page->vlocline);
  page->vpos = 0.0;

  gnome_canvas_points_unref (points);
  points = gnome_canvas_points_new (5);

  points->coords[0] = 0.0;
  points->coords[1] = 0.0;
  points->coords[2] = 0.0;
  points->coords[3] = 0.0;
  points->coords[4] = 0.0;
  points->coords[5] = 0.0;
  points->coords[6] = 0.0;
  points->coords[7] = 0.0;
  points->coords[8] = 0.0;
  points->coords[9] = 0.0;

  page->zoomrect =
    gnome_canvas_item_new (group,
			   gnome_canvas_line_get_type(),
			   "points", points,
			   "fill_color_rgba",
			   plcolor_to_rgba_inv (pls->cmap0[0], 0xF0),
			   "width_units", 1.0,
			   NULL);

  gnome_canvas_item_hide (page->zoomrect);
  page->hpos = 0.0;

  gnome_canvas_points_unref (points);

  page->context = np+1;

  change_mode (page, GNOME_PLDEV_LOCATE_MODE);

  gtk_signal_connect (GTK_OBJECT (background), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  debug("After setup_item\n");

  //gtk_window_set_focus (GTK_WINDOW (window), canvas);

  gtk_signal_connect (GTK_OBJECT (canvas), "key_press_event",
		      GTK_SIGNAL_FUNC (key_cb), pls);

  sw = GTK_SCROLLED_WINDOW (gtk_scrolled_window_new (NULL, NULL));

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);

  gtk_container_add (GTK_CONTAINER (sw), GTK_WIDGET (canvas));

  if (np == 0) 
    dev->page = g_malloc (sizeof (GnomePLdevPage*));
  else
    dev->page = g_realloc (dev->page,
			   (np+1) * sizeof (GnomePLdevPage*));

  
  debug ("After g_realloc\n");

  dev->page[np] = page;
  
  debug ("Before gtk_notebook_append_page\n");

  gtk_notebook_set_show_tabs (dev->notebook, (np > 0));

  sprintf (buffer, "Page %d", np+1);
  gtk_notebook_append_page (dev->notebook, GTK_WIDGET (sw),
			    gtk_label_new (buffer));

  gtk_widget_show_all (dev->parent);

  gtk_notebook_set_page (dev->notebook, -1);

  gdk_threads_leave ();

  dev->npages++;

  debug("End of new_page\n");

}

void
gnome_pldev_create (PLStream* pls) 
{
  GnomePLdev* dev;
  GtkWidget* vbox;

  dev = g_malloc (sizeof (GnomePLdev));
  pls->dev = dev;
  dev->npages = 0;
  vbox = gtk_vbox_new (FALSE, 0);
  dev->root = vbox;

  dev->statusbar = GTK_STATUSBAR (gtk_statusbar_new ());
  sb = dev->statusbar;

  gtk_box_pack_end (GTK_BOX (dev->root), GTK_WIDGET (dev->statusbar),
		    FALSE, FALSE, 0);
  
  debug("Before gtk_notebook_new\n");

  dev->notebook = GTK_NOTEBOOK (gtk_notebook_new ());

  // FIXME : this crashed the driver when the 4th page is displayed 
  gtk_signal_connect (GTK_OBJECT (dev->notebook), "switch_page",
    		      GTK_SIGNAL_FUNC (page_switch), dev);
  
  gtk_notebook_set_scrollable (dev->notebook, TRUE);
  
  debug ("Before gtk_box_pack_start\n");

  gtk_box_pack_start (GTK_BOX (dev->root), GTK_WIDGET (dev->notebook),
		      TRUE, TRUE, 0);
  
  debug ("After gtk_box_pack_start\n");

  debug ("Before gtk_widget_show_all\n");

  gtk_widget_show_all (GTK_WIDGET (dev->notebook));
  
  debug ("Before gtk_notebook_set_page\n");


  //  new_page (pls);
  debug("(gnome_pldev_create): After new_page\n");

  dev->parent_is_from_driver = FALSE;
}

void plD_open_gnome(PLStream *pls);

/*--------------------------------------------------------------------------*\
 * plD_init_gnome()
 *
 * Initialize device.
 * X-dependent stuff done in plD_open_gnome() and Init().
\*--------------------------------------------------------------------------*/

void
plD_init_gnome (PLStream *pls)
{
  char* argv[] = { "" };
  GnomePLdev* dev;
  GtkWidget* window;
  double phys2canvas = PIXELS_PER_DU / DRAWING_UNIT / MM_PER_IN;

  pls->termin = 1;		/* Is an interactive terminal */
  pls->dev_flush = 1;		/* Handle our own flushes */
  pls->dev_fill0 = 1;		/* Handle solid fills */
  pls->plbuf_write = 0;	        /* No plot buffer */
  pls->width = 1;
  
  /* The real meat of the initialization done here */

  atexit (quit);

  /* init threads */                                                       
  g_thread_init (NULL);                                                     

  if (pls->dev == NULL) {

    if (! gnome_is_initialized ) {
      debug ("Before gnome_init\n");

      gnome_init ("GnomePLplotDriver", "0.0.1", 1, argv);
      gnome_sound_shutdown ();

      debug ("After gnome_init\n");

    }
      debug ("Before gnome_pldev_create\n");

      gdk_rgb_init ();

    gnome_pldev_create (pls);

      debug ("After gnome_pldev_create\n");

    dev = pls->dev;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    dev->parent = window;

    gtk_signal_connect (GTK_OBJECT (window),
			"destroy_event",
			GTK_SIGNAL_FUNC (delete_event_cb),
			NULL);

    gtk_window_set_title (GTK_WINDOW (window), "Gnome PLplot Driver");

    gtk_window_set_policy (GTK_WINDOW (window), TRUE, TRUE, TRUE);

    gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);

    gtk_container_add (GTK_CONTAINER (window), dev->root);

    gtk_widget_show_all (window);

  }

  plP_setpxl (phys2canvas, phys2canvas);
  plP_setphy (0, WIDTH/DRAWING_UNIT, 0, HEIGHT/DRAWING_UNIT);

  gnome_is_initialized = TRUE;

      debug ("Before pthread_create\n");

      pthread_create (&tid, NULL, init, NULL); 

      debug ("After pthread_create\n");


}


/*--------------------------------------------------------------------------*\
 * plD_polyline_gnome()
 *
 * Draw a polyline in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_polyline_gnome(PLStream *pls, short *x, short *y, PLINT npts)
{
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;
  guint i;

  debug ("Entered plD_polyline_gnome\n");

  debug ("Before threads_enter\n");

  gdk_threads_enter ();

  debug ("Before getting dev\n");

  dev = pls->dev;
  
  debug ("Got dev\n");


  //  printf ("npages = %d\n", dev->npages);
  //  fflush (stdout);

  page = dev->page[dev->npages-1];

  debug ("Before canvas = page->canvas\n");

  canvas = page->canvas;

  debug ("Before gnome_canvas_root\n");

  group = gnome_canvas_root (canvas);

  debug ("After gnome_canvas_root\n");

  points = gnome_canvas_points_new (npts);

  for ( i = 0; i < npts; i++ ) {
    points->coords[2*i] = ((double) x[i]) * PIXELS_PER_DU;
    points->coords[2*i + 1] = ((double) -y[i]) * PIXELS_PER_DU;;
  }

  item = gnome_canvas_item_new (group,
                                gnome_canvas_line_get_type (),
				"cap_style", GDK_CAP_ROUND,
				"join_style", GDK_JOIN_ROUND,
                                "points", points,
				"fill_color_rgba",
				plcolor_to_rgba (pls->curcolor, 0xFF),
                                "width_units",
				MAX ((double) pls->width, 3.0) * PIXELS_PER_DU,
                                NULL);

  debug ("After gnome_canvas_item_new\n");

  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  //  setup_item (item);

  gnome_canvas_points_unref (points);

  gdk_threads_leave ();

  debug ("Left plD_polyline_gnome\n");

}

/*--------------------------------------------------------------------------*\
 * plD_line_gnome()
 *
 * Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/

void
plD_line_gnome(PLStream *pls, short x1, short y1, short x2, short y2)
{
  short x[2];
  short y[2];

  x[0] = x1;
  x[1] = x2;
  y[0] = y1;
  y[1] = y2;

  plD_polyline_gnome(pls, x, y, (PLINT) 2);
}

/*--------------------------------------------------------------------------*\
 * plD_eop_gnome()
 *
 * End of page.  User must hit return (or third mouse button) to continue.
\*--------------------------------------------------------------------------*/

void
plD_eop_gnome(PLStream *pls)
{
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvas* canvas;

  //  static int i;

  dev = pls->dev;

  gdk_threads_enter ();

  page = dev->page[dev->npages-1];
  canvas = page->canvas;

  canvas->need_update = 1;
  gnome_canvas_update_now (canvas);

  gdk_threads_leave ();

  //  printf("eop #%d\n", i++);
  //  fflush (stdout);

  //  getchar();
}

/*--------------------------------------------------------------------------*\
 * plD_bop_gnome()
 *
 * Set up for the next page.
\*--------------------------------------------------------------------------*/

void
plD_bop_gnome(PLStream *pls)
{
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvas* canvas;

  new_page (pls);
  
  dev = pls->dev;

  //  printf("npages = %d\n", dev->npages);
  //  fflush (stdout);

  gdk_threads_enter ();
  
  page = dev->page[dev->npages-1];
  canvas = page->canvas;
  
  canvas->need_update = 1;
  gnome_canvas_update_now (canvas);
  
  gdk_threads_leave ();
  
  pls->page++;
}

/*--------------------------------------------------------------------------*\
 * plD_tidy_gnome()
 *
 * Close graphics file
\*--------------------------------------------------------------------------*/

void
plD_tidy_gnome(PLStream *pls)
{
  debug("plD_tidy_gnome");

  pthread_join (tid, NULL);
}

/*--------------------------------------------------------------------------*\
 * plD_state_gnome()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/

void 
plD_state_gnome(PLStream *pls, PLINT op)
{
  switch (op) {
    
  case PLSTATE_WIDTH:
    break;
    
  case PLSTATE_COLOR0:
    break;

  case PLSTATE_COLOR1:
    break;

  case PLSTATE_CMAP0:
    break;

  case PLSTATE_CMAP1:
    break;
  }
}

static void 
fill_polygon (PLStream* pls)
{
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;
  guint i;

  debug ("Entered fill_polygon\n");

  dev = pls->dev;

  debug ("Got dev\n");
  
  gdk_threads_enter ();

  //  printf("npages = %d\n", dev->npages);
  //  fflush (stdout);

  page = dev->page[dev->npages-1];

  debug ("Got page\n");
  
  canvas = page->canvas;

  debug ("Got canvas\n");
  
  debug ("Before gnome_canvas_root\n");

  group = gnome_canvas_root (canvas);

  points = gnome_canvas_points_new (pls->dev_npts);

  for ( i = 0; i < pls->dev_npts; i++ ) {
    points->coords[2*i] = ((double) pls->dev_x[i]) * PIXELS_PER_DU;
    points->coords[2*i + 1] = ((double) -pls->dev_y[i]) * PIXELS_PER_DU;
  }

  debug ("Before gnome_canvas_item_new\n");

  item = gnome_canvas_item_new(group,
                                gnome_canvas_polygon_get_type (),
                                "points", points,
                                "fill_color_rgba",
 				plcolor_to_rgba (pls->curcolor, 0xFF),
                                "width_units", 0.0,
                                NULL);

  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  //  setup_item (item);

  gnome_canvas_points_unref (points);

  gdk_threads_leave ();

  debug ("Left fill_polygon\n");

}  

/*--------------------------------------------------------------------------*\
 * plD_esc_gnome()
 *
 * Escape function.
 *
 * Functions:
 *
 *	PLESC_EH	Handle pending events
 *	PLESC_EXPOSE	Force an expose
 *	PLESC_FILL	Fill polygon
 *	PLESC_FLUSH	Flush X event buffer
 *	PLESC_GETC	Get coordinates upon mouse click
 *	PLESC_REDRAW	Force a redraw
 *	PLESC_RESIZE	Force a resize
 * 	PLESC_XORMOD 	set/reset xor mode
\*--------------------------------------------------------------------------*/

static void
clear (PLStream* pls)
{
  GnomePLdev* dev = pls->dev;
  GnomePLdevPage* page;
  GnomeCanvas* canvas;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* rect;

  fflush(stdout);

  page = dev->page[pls->page-1];
  canvas = page->canvas;
  group = gnome_canvas_root (canvas);

  rect = gnome_canvas_item_new (group,
				gnome_canvas_rect_get_type(),
				"x1", (double) (pls->sppxmi) * PIXELS_PER_DU,
				"y1", (double) -(pls->sppyma) * PIXELS_PER_DU,
				"x2", (double) (pls->sppxma) * PIXELS_PER_DU,
				"y2", (double) -(pls->sppymi) * PIXELS_PER_DU,
				"fill_color", (pls->cmap0[0]).name,
				"width_units", 0.0,
				NULL);
  gnome_canvas_item_raise_to_top (rect);

  gtk_signal_connect (GTK_OBJECT (rect), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

}  

void
plD_esc_gnome(PLStream *pls, PLINT op, void *ptr)
{
  dbug_enter("plD_esc_gnome");
  
  switch (op) {
  case PLESC_EH:
    break;
    
  case PLESC_EXPOSE:
    break;
    
  case PLESC_FILL:
    fill_polygon(pls);
    break;
    
  case PLESC_FLUSH:
    break;
    
  case PLESC_GETC:
    break;
    
  case PLESC_REDRAW:
    break;

  case PLESC_RESIZE:
    break;

  case PLESC_XORMOD:
    break;

  case PLESC_DOUBLEBUFFERING:
    break;    

  case PLESC_CLEAR:
    clear (pls);
    break;    
  }
}

#else
int
pldummy_gnome()
{
    return 0;
}

#endif				/* PLD_gnome */
