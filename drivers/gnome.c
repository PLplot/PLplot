/* $Id$

        PLplot GnomeCanvas device driver.
*/
#include "plplot/plDevs.h"
#include "plplot/plplotP.h"

#include <gtk/gtk.h>                                                       
#include <glib.h>                                                          
#include <pthread.h>                                                       
#include <math.h>

#include <gnome.h>

#include <unistd.h>

//#undef DEBUG
#define DEBUG

#undef ANTIALISED_CANVAS

/* Physical dimensions */

/* Unit of drawing (1/360 in).  Corresponds to the pen widht = 1 */
#define DRAWING_UNIT (0.0027777778)

/* How many pixels in a drawing unit (defualt value) */
#define PIXELS_PER_DU (0.2)

/* mm per inch */
#define MM_PER_IN (25.4)

/* pixels per mm */
#define PIXELS_PER_MM (PIXELS_PER_DU / DRAWING_UNIT / MM_PER_IN)

/* Default dimensions of the canvas (in inches) */
#define WIDTH (9)
#define HEIGHT (6.5)

/* Magnification factor for the PLplot physical dimensions */
#define MAG_FACTOR 10

#ifdef PLD_gnome

void
debug (char* msg)
{
#ifdef DEBUG
  printf (msg);
  fflush (stdout);
#endif
}

G_LOCK_DEFINE_STATIC (canvas);

static pthread_t tid;

static guint gnome_is_initialized = FALSE;

// FIXME : Terrible global variable hack
static GtkStatusbar* sb;

typedef enum {
  GNOME_PLDEV_LOCATE_MODE,
  GNOME_PLDEV_ZOOM_MODE,
  GNOME_PLDEV_NONE_MODE
} GnomePLdevCanvasMode;

typedef struct {
  guint cmap;
  gdouble color;
} ItemColor;

typedef struct {
  GtkScrolledWindow* sw;
  GnomeCanvas* canvas;
  GnomePLdevCanvasMode mode;
  GnomeCanvasItem* hlocline;
  double hpos;
  GnomeCanvasItem* vlocline;
  double vpos;
  GnomeCanvasItem* zoomrect;
  PLGraphicsIn gin;
  guint context;
  GtkAdjustment* hadj;
  GtkAdjustment* vadj;
  double width;
  double height;
  double ppu;
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
    // FIXME : Terrible global variable hack
    gtk_statusbar_pop (sb, context);
    // FIXME : Terrible global variable hack
    gtk_statusbar_push (sb, context, "Locate Mode");
    break;
  case GNOME_PLDEV_ZOOM_MODE:
    // FIXME : Terrible global variable hack
    gtk_statusbar_pop (sb, context);
    // FIXME : Terrible global variable hack
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
  pthread_exit(NULL);
}

static gboolean
quit_dialog (void)
{
  GtkWidget* dialog;
  gint answer;

  fflush (stderr);

  dialog = gnome_dialog_new ("PLplot Gnome driver",
			     GNOME_STOCK_BUTTON_OK,
			     GNOME_STOCK_BUTTON_CANCEL,
			     NULL);
  
  gtk_box_pack_start (GTK_BOX (GNOME_DIALOG (dialog)->vbox),
		      gtk_label_new ("Quit PLplot?"), 
		      TRUE, 
		      TRUE,
		      0);
  
  gtk_widget_show_all (GTK_WIDGET (GNOME_DIALOG (dialog)->vbox));

  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);  
  gnome_dialog_set_default (GNOME_DIALOG (dialog), 0);

  answer = gnome_dialog_run_and_close (GNOME_DIALOG (dialog));

  if (answer == 0) {
    return TRUE;
  }
  else
    return FALSE;
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
  ItemColor* color;
  GnomeCanvasItem* item_at_cursor;

  move = FALSE;

  switch (event->type) {
    
  case GDK_2BUTTON_PRESS:
    
  case GDK_3BUTTON_PRESS:
    
  case GDK_BUTTON_PRESS:
    
    if (event->button.button == 1) {

      gnome_canvas_item_raise_to_top (page->hlocline);
      gnome_canvas_item_raise_to_top (page->vlocline);
      
      move = TRUE;

      cursor = gdk_cursor_new (GDK_CROSSHAIR);
      gnome_canvas_item_grab(item,
			     GDK_POINTER_MOTION_MASK | 
			     GDK_BUTTON_RELEASE_MASK,
			     cursor,
			     event->button.time);
      gdk_cursor_destroy(cursor);
      
      // FIXME : Terrible global variable hack
      gtk_statusbar_push (sb, page->context, "");

      dragging = TRUE;
    }
    break;

  case GDK_MOTION_NOTIFY:
    if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) 
      move = TRUE;
    break;
    
  case GDK_BUTTON_RELEASE:
    if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
      gnome_canvas_item_ungrab(item, event->button.time);
      dragging = FALSE;
      gnome_canvas_item_hide (page->hlocline);
      gnome_canvas_item_hide (page->vlocline);
      // FIXME : Terrible global variable hack
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

    gnome_canvas_item_hide (page->hlocline);
    gnome_canvas_item_hide (page->vlocline);

    item_at_cursor = gnome_canvas_get_item_at (page->canvas,
					       event->button.x,
					       event->button.y);

    if (item_at_cursor != NULL)
      color = (ItemColor *) gtk_object_get_data (GTK_OBJECT (item_at_cursor),
						 "color");
    else
      color = NULL;

    gnome_canvas_item_show (page->hlocline);
    gnome_canvas_item_show (page->vlocline);

    gnome_canvas_item_move (page->hlocline,
			    0.0, event->button.y - page->vpos);    
    page->vpos = event->button.y;
    
    gnome_canvas_item_move (page->vlocline,
			    event->button.x - page->hpos, 0.0);    
    page->hpos = event->button.x;

    gin->dX = page->hpos / page->width;
    gin->dY = page->vpos / (- page->height);

    plTranslateCursor (gin);

    if (color == NULL) 
      sprintf (buffer, "");
    else
      if (color->cmap == 0)
	sprintf (buffer, "   x = %f   y = %f   color = %d (cmap0)",
		 gin->wX, gin->wY, (int) color->color);
      else
	sprintf (buffer, "   x = %f   y = %f   color = %f (cmap1)",
		 gin->wX, gin->wY, color->color);

    // FIXME : Terrible global variable hack
    gtk_statusbar_pop (sb, page->context);
    // FIXME : Terrible global variable hack
    gtk_statusbar_push (sb, page->context, buffer);

  }

  return FALSE;
}


static gint 
delete_event_cb (GtkWidget* widget, GdkEventAny* e, gpointer data)
{
  if (quit_dialog () == TRUE) {
    gtk_widget_destroy (widget);
    gtk_main_quit ();
  }
  return TRUE;
}


static gint
item_event(GnomeCanvasItem *item, GdkEvent *event, gpointer data)
{
  switch (event->type) {
  case GDK_BUTTON_PRESS:
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

static void
add_to_adj (GtkAdjustment* adj, gfloat add)
{
  gfloat value;

  value = adj->value + add;
  gtk_adjustment_set_value (adj, value);
}

static gint
key_cb (GtkWidget* widget, GdkEventKey* event, PLStream* pls)
{
  GnomePLdev* dev = pls->dev;
  GnomePLdevPage* page;
  GtkNotebook* notebook = dev->notebook;
  GtkStatusbar* statusbar = dev->statusbar;
  guint curpage;

  if (notebook != NULL)
    curpage = gtk_notebook_get_current_page (notebook);
  else
    curpage = 0;

  page = dev->page[curpage];
  
  switch (event->keyval) {
  case GDK_minus:
    page->ppu /= 1.4142;
    break;
  case GDK_equal:
    page->ppu *= 1.4142;
    break;
  case GDK_Return:
    if (quit_dialog () == TRUE)
      gtk_main_quit ();
    break;
  case GDK_l:
    //gdk_threads_enter ();
    change_mode (page, GNOME_PLDEV_LOCATE_MODE);
    //gdk_threads_leave ();
    break;
  case GDK_z:
    //gdk_threads_enter ();
    change_mode (page, GNOME_PLDEV_ZOOM_MODE);
    //gdk_threads_leave ();
    break;
  case GDK_Page_Up:
    if (curpage != 0)
      gtk_notebook_set_page (notebook, curpage - 1);
    break;
  case GDK_Page_Down:
    gtk_notebook_set_page (notebook, curpage + 1);
    break;
  case GDK_Right:
    add_to_adj (page->hadj, page->ppu);
    break;
  case GDK_Left:
    add_to_adj (page->hadj, -page->ppu);
    break;
  case GDK_Up:
    add_to_adj (page->vadj, page->ppu);
    break;
  case GDK_Down:
    add_to_adj (page->vadj, -page->ppu);
    break;
  default:
    break;
  }

  page->hadj->step_increment = page->ppu;
  page->hadj->step_increment = page->ppu;
  gtk_adjustment_changed (page->hadj);
  gtk_adjustment_changed (page->vadj);
  gnome_canvas_set_pixels_per_unit (GNOME_CANVAS (widget), page->ppu);

  return TRUE;
}

static void
page_switch (GtkNotebook *notebook, GtkNotebookPage *page, gint page_num,
	     gpointer dev)
{
  guint curpage;
  GnomeCanvas* canvas;

  canvas = ((GnomePLdev*)(dev))->page[page_num]->canvas;
  if (canvas != NULL)
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
set_color (GnomeCanvasItem* item, guint cmap, gdouble color)
{
  ItemColor* colorp;

  colorp = g_malloc (sizeof (ItemColor));
  colorp->cmap = cmap;
  colorp->color = color;
  
  gtk_object_set_data (GTK_OBJECT (item), "color", colorp);
}

static void
new_page (PLStream* pls)
{
  GtkAdjustment* adj;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* background;
  GnomeCanvas* canvas;
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvasPoints* points;
  guint np;
  guint32 loclinecolor;
  char buffer[32];

  dev = pls->dev;
  page = g_malloc (sizeof (GnomePLdevPage));
  page->mode = GNOME_PLDEV_LOCATE_MODE;
  page->ppu = 1.0;

  np = dev->npages;

  gdk_threads_enter ();

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

  gtk_widget_pop_visual();
  gtk_widget_pop_colormap();

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

  set_color (background, 0, 0.0);

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

  gtk_signal_connect (GTK_OBJECT (canvas), "key_press_event",
		      GTK_SIGNAL_FUNC (key_cb), pls);

  page->hadj = GTK_ADJUSTMENT (GTK_LAYOUT (canvas)->hadjustment);
  page->vadj = GTK_ADJUSTMENT (GTK_LAYOUT (canvas)->vadjustment);

  page->sw = GTK_SCROLLED_WINDOW (gtk_scrolled_window_new (page->hadj,
							   page->vadj));

  gtk_scrolled_window_set_policy (page->sw,
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);

  gtk_container_add (GTK_CONTAINER (page->sw), GTK_WIDGET (canvas));

  if (np == 0) 
    dev->page = g_malloc (sizeof (GnomePLdevPage*));
  else
    dev->page = g_realloc (dev->page,
			   (np+1) * sizeof (GnomePLdevPage*));

  
  dev->page[np] = page;
  
  gtk_notebook_set_show_tabs (dev->notebook, (np > 0));

  sprintf (buffer, "Page %d", np+1);
  gtk_notebook_append_page (dev->notebook, GTK_WIDGET (page->sw),
			    gtk_label_new (buffer));

  //  adj = gtk_scrolled_window_get_vadjustment (page->hadj);
  //  page->hadj->value = 0.0;
  //page->hadj->step_increment = 1.0;
  //  page->vadj = adj;
  //gtk_scrolled_window_set_vadjustment (page->sw, adj);

  //adj = gtk_scrolled_window_get_hadjustment (page->sw);
  //page->vadj->value = 0.0;
  //page->vadj->step_increment = 1.0;
  //  page->hadj = adj;
  //gtk_scrolled_window_set_hadjustment (page->sw, adj);

  gtk_widget_show_all (dev->parent);

  gtk_notebook_set_page (dev->notebook, -1);

  gdk_threads_leave ();

  dev->npages++;

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
  
  dev->notebook = GTK_NOTEBOOK (gtk_notebook_new ());

  gtk_signal_connect (GTK_OBJECT (dev->notebook), "switch_page",
    		      GTK_SIGNAL_FUNC (page_switch), dev);
  
  gtk_notebook_set_scrollable (dev->notebook, TRUE);
  
  gtk_box_pack_start (GTK_BOX (dev->root), GTK_WIDGET (dev->notebook),
		      TRUE, TRUE, 0);
  
  gtk_widget_show_all (GTK_WIDGET (dev->notebook));
  
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
  double phys2canvas = MAG_FACTOR * PIXELS_PER_MM;

  pls->termin = 1;		/* Is an interactive terminal */
  pls->dev_flush = 1;		/* Handle our own flushes */
  pls->dev_fill0 = 1;		/* Handle solid fills */
  pls->dev_dash = 1;		/* Handle dashed lines */
  pls->plbuf_write = 0;	        /* No plot buffer */
  pls->width = 1;
  
  /* The real meat of the initialization done here */

  //  atexit (do_quit);

  /* init threads */                                                       
  g_thread_init (NULL);                                                     

  if (pls->dev == NULL) {

    if (! gnome_is_initialized ) {

      gnome_init ("GnomePLplotDriver", "0.0.1", 1, argv);
      gnome_sound_shutdown ();

    }

    gdk_rgb_init ();
    
    gnome_pldev_create (pls);
    
    dev = pls->dev;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    dev->parent = window;

    gtk_signal_connect (GTK_OBJECT (window),
			"delete_event",
			GTK_SIGNAL_FUNC (delete_event_cb),
			NULL);
    
    gtk_window_set_title (GTK_WINDOW (window), "Gnome PLplot Driver");

    gtk_window_set_policy (GTK_WINDOW (window), TRUE, TRUE, TRUE);

    gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);

    gtk_container_add (GTK_CONTAINER (window), dev->root);

    gtk_widget_show_all (window);

  }

  plP_setpxl (phys2canvas, phys2canvas);
  plP_setphy (0, MAG_FACTOR * WIDTH/DRAWING_UNIT,
	      0, MAG_FACTOR * HEIGHT/DRAWING_UNIT);

  gnome_is_initialized = TRUE;

  pthread_create (&tid, NULL, init, NULL); 

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

  gdk_threads_enter ();

  dev = pls->dev;
  
  page = dev->page[dev->npages-1];

  canvas = page->canvas;

  group = gnome_canvas_root (canvas);

  points = gnome_canvas_points_new (npts);

  for ( i = 0; i < npts; i++ ) {
    points->coords[2*i] = ((double) x[i]/MAG_FACTOR) * PIXELS_PER_DU;
    points->coords[2*i + 1] = ((double) -y[i]/MAG_FACTOR) * PIXELS_PER_DU;;
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

  set_color (item, 0, pls->icol0);

  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  gnome_canvas_points_unref (points);

  gdk_threads_leave ();

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

  dev = pls->dev;

  gdk_threads_enter ();

  page = dev->page[dev->npages-1];

  canvas = page->canvas;

  group = gnome_canvas_root (canvas);

  points = gnome_canvas_points_new (pls->dev_npts);

  for ( i = 0; i < pls->dev_npts; i++ ) {
    points->coords[2*i] =
      ((double) pls->dev_x[i]/MAG_FACTOR) * PIXELS_PER_DU;
    points->coords[2*i + 1] =
      ((double) -pls->dev_y[i]/MAG_FACTOR) * PIXELS_PER_DU;
  }

  item = gnome_canvas_item_new(group,
                                gnome_canvas_polygon_get_type (),
                                "points", points,
                                "fill_color_rgba",
 				plcolor_to_rgba (pls->curcolor, 0xFF),
                                "width_units", 0.0,
                                NULL);

  set_color (item, 1, ((double) pls->icol1)/pls->ncol1);

  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  gnome_canvas_points_unref (points);

  gdk_threads_leave ();

}  

static void 
dashed_line (PLStream* pls)
{
  GnomePLdev* dev;
  GnomePLdevPage* page;
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GnomeCanvas* canvas;
  guint i;
  gchar* dash_list;

  dev = pls->dev;

  gdk_threads_enter ();

  page = dev->page[dev->npages-1];

  canvas = page->canvas;

  group = gnome_canvas_root (canvas);

  points = gnome_canvas_points_new (pls->dev_npts);

  for (i = 0; i < pls->dev_npts; i++) {
    points->coords[2*i] =
      ((double) pls->dev_x[i]/MAG_FACTOR) * PIXELS_PER_DU;
    points->coords[2*i + 1] =
      ((double) -pls->dev_y[i]/MAG_FACTOR) * PIXELS_PER_DU;
  }

  dash_list = g_malloc (sizeof (gchar) * 2 * pls->nms);
  for (i = 0; i < pls->nms; i++) {
    dash_list[2*i] = (gchar) ceil ((pls->mark[i]/1e3) * PIXELS_PER_MM);
    dash_list[2*i+1] = (gchar) floor ((pls->space[i]/1e3) * PIXELS_PER_MM);
    fflush(stdout);
  }

  gdk_gc_set_dashes (canvas->pixmap_gc, 0, dash_list, 2*pls->nms);

  g_free (dash_list);
  
  item = gnome_canvas_item_new (group,
                                gnome_canvas_line_get_type (),
				"cap_style", GDK_CAP_BUTT,
				"join_style", GDK_JOIN_ROUND,
				"line_style", GDK_LINE_ON_OFF_DASH,
                                "points", points,
				"fill_color_rgba",
				plcolor_to_rgba (pls->curcolor, 0xFF),
                                "width_units",
				MAX ((double) pls->width, 3.0) * PIXELS_PER_DU,
                                NULL);

  set_color (item, 0, (double) pls->icol0);

  gtk_signal_connect (GTK_OBJECT (item), "event",
                      (GtkSignalFunc) canvas_pressed_cb,
                      page);

  gnome_canvas_points_unref (points);

  gdk_threads_leave ();

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

  case PLESC_CLEAR:
    clear (pls);
    break;    

  case PLESC_DASH:
    dashed_line (pls);
    break;    

  case PLESC_FILL:
    fill_polygon(pls);
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
