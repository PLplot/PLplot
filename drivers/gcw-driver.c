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

  Text clipping is not completely working, although it is implemented
  here.  Proper text clipping will require Pango, as it is not
  implemented for Hacktext.

  PLplot test suite problems:

    1) Example x10c does not clip the text.

    2) Example x17c, the strip chart demo, doesn't do a strip chart 
       (try the xwin driver to see how it should work).  This example
       probably isn't so important for this driver, which gives 
       alternative and more flexible ways of drawing animations.

    3) Example x20c freezes during the drawing of Lena, likely due to 
       too many items on the canvas.

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

/*
#define DEBUG_GCW
*/

/*
#define ASSERT_GCW
*/

#define POLYGON_GCW

#define OUTLINE_POLYGON_GCW

/* Global driver options */

static PLINT aa = 1;

#ifdef HAVE_FREETYPE
static PLINT text = 1;
#else
static PLINT text = 0;
#endif

static PLINT hrshsym = 0;
static PLINT fast = 0; 
static PLINT pixmap = 0;
static PLINT replot = 1;

static DrvOpt gcw_options[] = 
  {
    {"aa", DRV_INT, &aa, "Use antialiased canvas (aa=0|1)"},
    {"text", DRV_INT, &text, "Use truetype fonts (text=0|1)"},
    {"hrshsym", DRV_INT, &hrshsym, "Use Hershey symbol set (hrshsym=0|1)"},
    {"fast", DRV_INT, &fast, "Use fast rendering (fast=0|1)"},
    {"pixmap", DRV_INT, &pixmap, "Use pixmap for plotting shades (pixmap=0|1)"},
    {"replot", DRV_INT, &replot, "Allow replotting to other devices (replot=0|1)"},
    {NULL, DRV_INT, NULL, NULL}
  };

GtkWidget *filew=NULL, *statusbar=NULL;

void debug (char* msg)
{
  fprintf(stderr,msg);
  fflush(stderr);
}

/*--------------------------------------------------------------------------*\
 * Callback functions
 *
 * Employed when this driver is used in standalone mode.
 *
\*--------------------------------------------------------------------------*/

/*******************************
 * Main window event callbacks *
 *******************************/

/* Delete event callback */
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data ) {
  if(filew!=NULL)
    g_signal_emit_by_name( G_OBJECT(filew),"delete-event");
  return FALSE;
}

/* Destroy event calback */
void destroy(GtkWidget *widget, gpointer data) {
  gtk_main_quit ();
}

/******************
 * Zoom callbacks *
 ******************/

/* All-purpose zoom callback.  Referred to by specific zoom callbacks given
 *  below.
 */
void zoom(gpointer data, gint flag) {

  gint n;

  GtkNotebook *notebook;
  GnomeCanvas *canvas;
  GtkWidget *scrolled_window;
  GList *list;

  GcwPLdev* dev;

  gdouble curmag,dum;

  /* Get the current canvas */
  notebook = GTK_NOTEBOOK(data);
  n = gtk_notebook_get_current_page(notebook);
  scrolled_window = gtk_notebook_get_nth_page(notebook,n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <zoom>: Device not found");

  /* Determine the new magnification */
  if(flag==2) /* Zoom in */
    gcw_set_canvas_zoom(canvas,ZOOMSTEP);
  else if(flag==0) /* Zoom out */
    gcw_set_canvas_zoom(canvas,1./ZOOMSTEP);
  else { /* Zoom 100 */
    /* Get the current magnification */
    if(dev->zoom_is_initialized) gnome_canvas_c2w(canvas,1,0,&curmag,&dum);
    curmag = 1./curmag;
    gcw_set_canvas_zoom(canvas,(PLFLT)(ZOOM100/curmag));
  }

  /* Set the focus on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(dev->notebook));
}


/* Callback when zoom in button is pressed */
void zoom_in(GtkWidget *widget, gpointer data ) {
  zoom(data,2);
}

/* Callback when zoom 100 button is pressed */
void zoom_100(GtkWidget *widget, gpointer data ) {
  zoom(data,1);
}

/* Callback when zoom out button is pressed */
void zoom_out(GtkWidget *widget, gpointer data ) {
  zoom(data,0);
}

/*****************************
 * File selection callbacks. *
 *****************************/

/* Callback to OK file selection.  Retrieves the selected filename and 
 * replays the plot buffer to it through the associated driver. 
 */
void file_ok_sel(GtkWidget *w, gpointer data)
{
  gchar *fname;
  FILE *f;
  guint n;
  char devname[10],str[100];

  PLINT cur_strm, new_strm;

  GtkNotebook *notebook;
  GnomeCanvas *canvas;
  GtkWidget *scrolled_window;
  GList *list;

  GtkWidget *fs;

  gdouble curmag,dum;

  GcwPLdev* dev;

  PLINT icol0,icol1,ncol0,ncol1;
  PLColor *cmap0,*cmap1;

  gboolean errflag = FALSE;

  struct stat buf;

  GtkDialog *dialog;
  GtkWidget *hbox,*message,*icon;
  gint result;

  guint context;
  
  /* Get the current canvas */
  notebook = GTK_NOTEBOOK(data);
  n = gtk_notebook_get_current_page(notebook);
  scrolled_window = gtk_notebook_get_nth_page(notebook,n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Get the file name */
  fs = GTK_WIDGET(g_object_get_data(G_OBJECT(notebook),"fs"));
  fname = strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  if(fname==NULL) plabort("GCW driver <file_ok_sel>: Cannot obtain filename");

  /* Check to see if the file already exists, and respond appropriately */
  if(stat(fname,&buf)==0) {

    /* Confirm the user wants to overwrite the existing file */
    dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("",
		          GTK_WINDOW(filew),
			  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                          GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, NULL));

    message = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(message),
			 "<span size=\"large\"><b>File exists.  "
			 "Overwrite?</b></span>");

    icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION,
				    GTK_ICON_SIZE_DIALOG);

    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbox), icon, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox), message, TRUE, TRUE, 10);
    gtk_container_add(GTK_CONTAINER(dialog->vbox),hbox);

    gtk_widget_show_all(GTK_WIDGET(dialog));

    result = gtk_dialog_run(dialog);
    gtk_widget_destroy(GTK_WIDGET(dialog));
    if(result==GTK_RESPONSE_REJECT) return;
  }

  /* Hide the file selection widget */
  gtk_widget_hide(GTK_WIDGET(fs));

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <file_sel_ok>: Device not found");

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(notebook));

  /* Test that we can open and write to the file */
  if((f=fopen(fname,"w"))==NULL)
    plabort("GCW driver <file_ok_sel>: Cannot open output file");
  fclose(f);
  remove(fname);

  /* Get the file extension and make sure we recognize it.  Allow names
   * of display devices as well.
   *
   * Note that we can't replot to xwin or tk devices, which can't open a
   * display for some reason.
   *
   */
  n = strlen(fname);
  if     (strcasecmp(&fname[n-3],"png")==0)  sprintf(devname,"png");
  else if(strcasecmp(&fname[n-3],"gif")==0)  sprintf(devname,"gif");
  else if(strcasecmp(&fname[n-3],"jpg")==0)  sprintf(devname,"jpg");
  else if(strcasecmp(&fname[n-4],"jpeg")==0) sprintf(devname,"jpeg");
  else if(strcasecmp(&fname[n-2],"ps")==0)   sprintf(devname,"ps");
  else if(strcasecmp(&fname[n-3],"psc")==0)  sprintf(devname,"psc");
  else if(strcasecmp(&fname[n-4],"xwin")==0) sprintf(devname,"xwin");
  else if(strcasecmp(&fname[n-3],"gcw")==0) sprintf(devname,"gcw");
  else if(strcasecmp(&fname[n-2],"tk")==0) sprintf(devname,"tk");
  else {
    if(statusbar!=NULL) {
      context=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),"PLplot");
      gtk_statusbar_push(GTK_STATUSBAR(statusbar), context,
			 " WARNING: File type not recognized (unknown "
			 "extension).  Use one of ps, psc, png, jpg, or "
			 "gif."
			 );
      return;
    }
    else plabort("GCW driver <file_ok_sel>: File type not recognized");
  }
  /* Check that we are set up appropriately for device */
  if( strcmp(devname,"ps")==0 || strcmp(devname,"psc")==0 ) {
    if(plsc->dev_hrshsym != 1)
      if(statusbar!=NULL) {
	context=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
					     "PLplot");

	gtk_statusbar_push(GTK_STATUSBAR(statusbar),context,
			   " NOTE: Use '-drvopt hrshsym' in command-line "
			   "arguments if symbols are missing in saved "
			   "PostScript files."
			   );
      }
      else
	plwarn("GCW driver: Use '-drvopt hrshsym' if symbols are missing in "
	       "saved PostScript files.");
  }
  if( strcmp(devname,"xwin")==0 || strcmp(devname,"tk")==0 ) {
    if(plsc->dev_text != 0)
      plwarn("GCW driver: Use '-drvopt text=0'");
  }

  /* Hack: Swap in the saved tempfile and colormaps */
  f = plsc->plbufFile;
  plsc->plbufFile = g_object_get_data(G_OBJECT(canvas),"plotbuf");
  if(plsc->plbufFile == NULL) errflag=TRUE;

  icol0=plsc->icol0;
  if( g_object_get_data(G_OBJECT(canvas),"icol0") != NULL )
    plsc->icol0 = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"icol0");
  else errflag=TRUE;

  ncol0=plsc->ncol0;
  if( g_object_get_data(G_OBJECT(canvas),"ncol0") != NULL )
    plsc->ncol0 = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"ncol0");
  else errflag=TRUE;
    
  cmap0=plsc->cmap0;
  if( g_object_get_data(G_OBJECT(canvas),"cmap0") )
    plsc->cmap0 = (PLColor*)g_object_get_data(G_OBJECT(canvas),"cmap0");
  else errflag=TRUE;

  icol1=plsc->icol1;
  if( g_object_get_data(G_OBJECT(canvas),"icol1") != NULL )
    plsc->icol1 = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"icol1");
  else errflag=TRUE;

  ncol1=plsc->ncol1;
  if( g_object_get_data(G_OBJECT(canvas),"ncol1") != NULL )
    plsc->ncol1 = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"ncol1");
  else errflag=TRUE;

  cmap1=plsc->cmap1;
  if( g_object_get_data(G_OBJECT(canvas),"cmap1") != NULL )
    plsc->cmap1 = (PLColor*)g_object_get_data(G_OBJECT(canvas),"cmap1");
  else errflag=TRUE;

  if(!errflag) {
    /* Get the current stream and make a new one */
    plgstrm(&cur_strm);
    plmkstrm(&new_strm);

    plsfnam(fname); /* Set the file name */

    plsdev(devname); /* Set the device */

    plcpstrm(cur_strm, 0); /* copy old stream parameters to new stream */

    plreplot();            /* do the save by replaying the plot buffer */

    plend1();              /* finish the device */

    plsstrm(cur_strm);     /* return to previous stream */
  }
  else plwarn("GCW driver <file_ok_sel>: Cannot set up output stream.");

  /* Reverse the swap */
  plsc->plbufFile = f;
  plsc->icol0 = icol0;
  plsc->ncol0 = ncol0;
  plsc->cmap0 = cmap0;
  plsc->icol1 = icol1;
  plsc->ncol1 = ncol1;
  plsc->cmap1 = cmap1;
}

/* Callback to cancel file selection */
void file_cancel_sel(GtkWidget *w, gpointer data)
{
  GtkNotebook *notebook;
  GnomeCanvas *canvas;
  GtkWidget *scrolled_window;
  GList *list;
  GtkWidget *fs;
  GcwPLdev* dev;
  guint n;
  
  /* Get the current canvas */
  notebook = GTK_NOTEBOOK(data);
  n = gtk_notebook_get_current_page(notebook);
  scrolled_window = gtk_notebook_get_nth_page(notebook,n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Hide the file selection widget */
  fs = GTK_WIDGET(g_object_get_data(G_OBJECT(notebook),"fs"));
  gtk_widget_hide(GTK_WIDGET(fs));

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <file_cancel_sel>: Device not found");

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(notebook));
}

/* Callback to create file selection dialog */
void filesel(GtkWidget *widget, gpointer data ) {

  GtkNotebook *notebook;

  /* Create a new file dialog if it doesn't already exist */
  if(filew == NULL) {

    /* Create a new file selection widget */
    filew = gtk_file_selection_new ("File selection");
    
    /* Connect the ok_button to file_ok_sel function */
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
		     "clicked", G_CALLBACK(file_ok_sel), (gpointer)data);
    
    /* Connect the cancel_button to destroy the widget */
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
		     "clicked", G_CALLBACK(file_cancel_sel), (gpointer)data);
    
    /* Lets set the filename, as if this were a save dialog, and we are giving
       a default filename */
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew), 
				    "plot.psc");

    /* Get the notebook, and attach the fileselector to it */
    notebook = GTK_NOTEBOOK(data);
    g_object_set_data(G_OBJECT(notebook),"fs",(gpointer)filew);

  }
    
  gtk_widget_show(filew);
}

/**************************
 * Key release callbacks. *
 **************************/

void key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data ) {
  if(event->keyval == '+')  zoom(data,2);
  if(event->keyval == '=')  zoom(data,1);
  if(event->keyval == '-')  zoom(data,0);
  if(event->keyval == 'q')  destroy(widget,data);
}


/*--------------------------------------------------------------------------*\
 * install_canvas()
 *
 * Creates and installs a canvas into a window on the fly.  This function
 * is called if a canvas hasn't been supplied to the driver.
\*--------------------------------------------------------------------------*/

void install_canvas(PLStream *pls)
{
  GcwPLdev* dev = pls->dev;
  GtkWidget *window,*vbox,*hbox,*button,*image,*scrolled_window;
  GnomeCanvas *canvas;

  gboolean flag = FALSE;

#ifdef DEBUG_GCW
  debug("<install_canvas>\n");
#endif

  if(dev->window==NULL) {

    flag = TRUE;

    /* Create a new window and prepare a notebook for it */

    /* Create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dev->window = window;
    gtk_window_set_title(GTK_WINDOW(window),"PLplot");

    /* Connect the signal handlers to the window decorations */
    g_signal_connect(G_OBJECT(window),"delete_event",
		     G_CALLBACK(delete_event),NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);


    /* Create a vbox and put it into the window */
    vbox = gtk_vbox_new(FALSE,2);
    gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(vbox));

    /* Create a hbox and put it into the vbox */
    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(hbox),TRUE,TRUE,0);

    /* Create a statusbar and put it into the vbox */
    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(statusbar),FALSE,FALSE,0);

    /* Add an vbox to the hbox */
    vbox = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
    gtk_box_pack_start(GTK_BOX(hbox),GTK_WIDGET(vbox),FALSE,FALSE,0);

    /* Create the new notebook and add it to the hbox*/
    dev->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(dev->notebook),TRUE);
    gtk_box_pack_start(GTK_BOX(hbox),GTK_WIDGET(dev->notebook),TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(dev->notebook), "key_release_event",
                         G_CALLBACK(key_release), G_OBJECT(dev->notebook));

    /* Use a few labels as spacers */
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(gtk_label_new(" ")),
		       FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(gtk_label_new(" ")),
		       FALSE,FALSE,0);

    /* Add buttons to the vbox */

    /* Add zoom in button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_IN,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), GTK_WIDGET(image));
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(button),FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_in), G_OBJECT(dev->notebook));
    g_signal_connect(G_OBJECT(button), "key_release_event",
                         G_CALLBACK(key_release), G_OBJECT(dev->notebook));

    /* Add zoom100 button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_100,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), GTK_WIDGET(image));
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(button),FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_100), G_OBJECT(dev->notebook));

    /* Add zoom out button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_OUT,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), GTK_WIDGET(image));
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(button),FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_out), G_OBJECT(dev->notebook));
    g_signal_connect(G_OBJECT(button), "key_release_event",
                         G_CALLBACK(key_release), G_OBJECT(dev->notebook));

    /* Add save button and create callbacks */
    if(pls->plbuf_write) {
      image = gtk_image_new_from_stock(GTK_STOCK_SAVE,
				       GTK_ICON_SIZE_SMALL_TOOLBAR);
      button = gtk_button_new();
      gtk_container_add(GTK_CONTAINER(button), GTK_WIDGET(image));
      gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(button),FALSE,FALSE,0);
      g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(filesel), G_OBJECT(dev->notebook));
    }
  }

  /* Create a new canvas */
  if(aa) {
    if( !GNOME_IS_CANVAS(canvas = GNOME_CANVAS(gnome_canvas_new_aa())) )
      plexit("GCW driver <install_canvas>: Could not create aa canvas");
    dev->aa = TRUE;
  }
  else {
    if( !GNOME_IS_CANVAS(canvas = GNOME_CANVAS(gnome_canvas_new())) )
      plexit("GCW driver <install_canvas>: Could not create canvas");
    dev->aa = FALSE;
  }

  gcw_set_canvas(pls,canvas);
  gcw_set_canvas_zoom(canvas,ZOOM100);

  /* Put the canvas in a scrolled window */
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), 
					GTK_WIDGET(canvas));

  /* Install the scrolled window in the notebook */
  gtk_notebook_append_page(GTK_NOTEBOOK(dev->notebook),
			   GTK_WIDGET(scrolled_window), NULL);

  if(flag) {

    /* Set the focus on the notebook */
    gtk_window_set_focus(GTK_WINDOW(window),GTK_WIDGET(dev->notebook));

    /* Size the window */
    gtk_window_resize(GTK_WINDOW(window),dev->width*ZOOM100+65,
 		      dev->height*ZOOM100+75);
  }

  /* Display everything */
  gtk_widget_show_all(dev->window);
}

void clear_pixmap(PLStream* pls)
{
  GcwPLdev* dev=pls->dev;
  GdkGC* gc;
  GdkColor color;
  PLColor plcolor = pls->cmap0[0];

  /* Allocate the background color*/
  color.red=(guint16)(plcolor.r/255.*65535); 
  color.green=(guint16)(plcolor.g/255.*65535); 
  color.blue=(guint16)(plcolor.b/255.*65535);
  gdk_colormap_alloc_color(dev->colormap,&color,FALSE,TRUE);

  /* Clear the pixmap with the background color */
  gc = gdk_gc_new(dev->pixmap);
  gdk_gc_set_foreground(gc,&color);
  gdk_draw_rectangle(dev->pixmap,gc,TRUE,0,0,dev->width,dev->height);
  gdk_gc_unref(gc);
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

#ifdef DEBUG_GCW
  debug("<plD_dispatch_init_gcw>\n");
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

  PLINT width, height;

#ifdef DEBUG_GCW
  debug("<plD_init_gcw>\n");
#endif

  /* Parse the driver options */
  plParseDrvOpts(gcw_options);

  /* Set up the stream */
  pls->termin = 1;      /* Is an interactive terminal */
  pls->dev_flush = 1;   /* Handle our own flushes */
  pls->plbuf_write = replot; /* Use plot buffer to replot to another device */
  pls->width = 1;
  pls->dev_clear = 1;   /* Handle plclear() */
  pls->dev_fill0 = 1;	/* Handle solid fills */

  /* Create the device */
  if((dev = g_malloc(sizeof(GcwPLdev))) == NULL) 
    plexit("GCW driver <plD_init_gcw>: Cannot create device");
  pls->dev = dev;

  /* Initialize gtk */
  gtk_init(0,NULL);

  /* Set up the physical device in the following command block.  It is very 
   * important to do this properly, because many of the calculations are
   * based on physical coordinates.  If this is not done correctly, then
   * dashed lines and hatched areas will not scale correctly, and the 
   * replot mechanism will produce incorrect results.
   */

  /* Determine the page width and height, measured in pixels */
  if (pls->xlength > 0 && pls->ylength > 0) {
    width = pls->xlength;
    height = pls->ylength;
  }
  else {    
    width = (PLINT)(CANVAS_WIDTH*PIXELS_PER_IN);
    height = (PLINT)(CANVAS_HEIGHT*PIXELS_PER_IN);
  }

  /* Set the number of pixels per mm */
  plP_setpxl((PLFLT)PIXELS_PER_MM,(PLFLT)PIXELS_PER_MM);

  /* Set up physical limits of plotting device (in drawing units) */
  plP_setphy((PLINT)0,width,(PLINT)0,height);

  /* Set the width and height for the device */
  dev->width = width;
  dev->height = height;

  /* Set text handling */
#ifdef HAVE_FREETYPE
  if(aa && text) {
    pls->dev_text = TRUE;
    pls->dev_unicode = TRUE;
    dev->use_text = TRUE;
    if(hrshsym) pls->dev_hrshsym = 1;

    /* Initialize freetype */
    plD_FreeType_init(pls);
  }
  else {
    pls->dev_text = FALSE;
    pls->dev_unicode = FALSE;
    dev->use_text = FALSE;
  }
#else
  pls->dev_text = FALSE;
  pls->dev_unicode = FALSE;
  dev->use_text = FALSE;
#endif

  /* Set fast rendering for polylines */
  dev->use_fast_rendering = (gboolean)fast;

  /* Set up the pixmap support */
  dev->use_pixmap = (gboolean)pixmap;
  dev->pixmap = NULL;
  dev->pixmap_has_data = FALSE;

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

  PLINT i;

  gdouble width;
  guint32 color;

#ifdef DEBUG_GCW
  debug("<plD_polyline_gcw>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);

#ifdef ASSERT_GCW
  if(!GNOME_IS_CANVAS_ITEM(
    group = dev->group_current
    )) {
    plexit("GCW driver <plD_polyline_gcw>: Canvas group is NULL");
  }
#else
  group = dev->group_current;
#endif

  /* Put the data in a points structure */
  if( (points = gnome_canvas_points_new(npts)) == NULL )
    plabort("GCW driver <plD_polyline_gcw>: Cannot create points");
  for ( i = 0; i < npts; i++ ) {
    points->coords[2*i] = ((gdouble) x[i]);
    points->coords[2*i + 1] = ((gdouble) -y[i]);
  }

  /* Get the width and color */
  width = pls->width;
  color = dev->color;

  if(dev->use_fast_rendering) {
    if(!GNOME_IS_CANVAS_ITEM(
      item=gnome_canvas_item_new(group,
				 GNOME_TYPE_CANVAS_LINE,
				 "cap_style", GDK_CAP_ROUND,
				 "join-style", GDK_JOIN_ROUND,
				 "points", points,
				 "fill-color-rgba", color,
				 "width-units",width,
				 NULL)
      )) {
      plwarn("GCW driver <plD_polyline_gcw>: Canvas item not created.");
    }

    /* Free the points structure */
    gnome_canvas_points_free(points);
  }
  else {

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

  GdkPixbuf* pixbuf;
  GnomeCanvasItem* item;

  gdouble dx, dy;

  FILE *f;
  gint count=1,n;
  U_CHAR tmp;

  gint *icol,*ncol;
  PLColor *cmap;

  guint i;

#ifdef DEBUG_GCW
  debug("<plD_eop_gcw>\n");
#endif

  canvas = dev->canvas;
  if(canvas == NULL) return;

  /* 
   * Render the pixmap to a pixbuf on the canvas and move it to the back
   * of the current group 
   */
  
  if(dev->pixmap_has_data) {
    
    if((pixbuf=gdk_pixbuf_get_from_drawable(NULL,
					    dev->pixmap,
					    dev->colormap,
					    0,0,
					    0,0,
					    dev->width,dev->height))==NULL) {
      plwarn("GCW driver <plD_eop_gcw>: Can't draw pixmap into pixbuf.");
    }
    else {

      /* Different offsets depending on the type of canvas */
      if(dev->aa) { dx=0.; dy=0.; }
      else { dx=1.; dy=1.; }
      
      if(!GNOME_IS_CANVAS_ITEM(
        item = gnome_canvas_item_new(dev->group_current,
				     GNOME_TYPE_CANVAS_PIXBUF,
				     "pixbuf",pixbuf,
				     "x", dx,
				     "y", -dev->height+dy,
				     "width", dev->width,
				     "height", dev->height,
				     NULL)
	)) {
	plwarn("GCW driver <plD_eop_gcw>: Canvas item not created.");
      }
      else {
	gnome_canvas_item_lower_to_bottom(item);
	gnome_canvas_item_lower_to_bottom (dev->background);
      }
      
      /* Free the pixbuf */
      gdk_pixbuf_unref(pixbuf);
    }
    
    /* Set the pixmap as unused */
    dev->pixmap_has_data = FALSE;
  }
  
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
  
  /*
   * Copy the plot buffer for future reference, otherwise it is 
   * thrown out.  We will also need to store the colormaps.
   */
  if(pls->plbuf_write) {
    
    pls->plbuf_write = FALSE;
    pls->plbuf_read = TRUE;
    
    /* Remove the old tempfile, if it exists */
    if( (f=g_object_get_data(G_OBJECT(canvas),"plotbuf")) != NULL ) {
      fclose(f);
      g_object_set_data(G_OBJECT(canvas),"plotbuf",NULL);
    }

    /* Copy the plot buffer to a tempfile */
    if((f=tmpfile())==NULL) {
      plwarn("GCW driver <plD_eop_gcw>: Could not create tempfile.");
    }
    else {
      rewind(pls->plbufFile);
      while(count = fread(&tmp, sizeof(U_CHAR), 1, pls->plbufFile)) {
	if(fwrite(&tmp, sizeof(U_CHAR), 1, f)!=count) {
	  plwarn("GCW driver <plD_eop_gcw>: Could not write to tempfile.");
	  fclose(f);
	  f=NULL;
	}
      }
      
      /* Attach the tempfile to the canvas */
      g_object_set_data(G_OBJECT(canvas),"plotbuf",(gpointer)f);
      
      pls->plbuf_write = TRUE;
      pls->plbuf_read = FALSE;      
      
      /* cmap 0 */
      if((icol=(gint*)malloc(sizeof(gint)))==NULL)
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      else *icol = pls->icol0;
      g_object_set_data(G_OBJECT(canvas),"icol0",(gpointer)icol);
      if((ncol=(gint*)malloc(sizeof(gint)))==NULL)
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      else *ncol = pls->ncol0;
      g_object_set_data(G_OBJECT(canvas),"ncol0",(gpointer)ncol);
      if((cmap=(PLColor *) calloc(1, pls->ncol0 * sizeof(PLColor)))==NULL) {
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      }
      else {
	for (i = 0; i < pls->ncol0; i++)
	  pl_cpcolor(&cmap[i], &pls->cmap0[i]);
      }
      g_object_set_data(G_OBJECT(canvas),"cmap0",(gpointer)cmap);
      
      /* cmap 1 */
      if((icol=(gint*)malloc(sizeof(gint)))==NULL)
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      else *icol = pls->icol1;
      g_object_set_data(G_OBJECT(canvas),"icol1",(gpointer)icol);
      if((ncol=(gint*)malloc(sizeof(gint)))==NULL)
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      else *ncol = pls->ncol1;
      g_object_set_data(G_OBJECT(canvas),"ncol1",(gpointer)ncol);
      if((cmap=(PLColor *) calloc(1, pls->ncol1 * sizeof(PLColor)))==NULL) {
	plwarn("GCW driver <plD_eop_gcw>: Insufficient memory.");
      }
      else {
	for (i = 0; i < pls->ncol1; i++)
	  pl_cpcolor(&cmap[i], &pls->cmap1[i]);
      }
      g_object_set_data(G_OBJECT(canvas),"cmap1",(gpointer)cmap);
    }
  }
  
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

#ifdef DEBUG_GCW
  debug("<plD_bop_gcw>\n");
#endif

  canvas = dev->canvas;
  if(canvas == NULL) return;
  
  /* Make sure the zoom is initialized */
  if(!dev->zoom_is_initialized) gcw_set_canvas_zoom(canvas,1.);
  
  /* Creat a new hidden group; all new drawing will be to this group */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_hidden = (GnomeCanvasGroup*)gnome_canvas_item_new(
                                             gnome_canvas_root(canvas),
					     gnome_canvas_clipgroup_get_type(),
					     "x",0.,
					     "y",0.,
					     NULL)
    )) {
    plexit("GCW driver <plD_bop_gcw>: Canvas group cannot be created");
  }
  
  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_hidden),"path",NULL,NULL);
  
  /* Hide this group until drawing is done */
  gnome_canvas_item_hide((GnomeCanvasItem*)(dev->group_hidden));

  /* Set the hidden group as current unless it is fore or background */
  if( (dev->group_current != dev->group_foreground) &&
      (dev->group_current != dev->group_background) )
    dev->group_current = dev->group_hidden;
  
  /* Make sure the foreground group is at the front, and the background
   * group is at the back 
   */
  gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(dev->group_foreground));
  gnome_canvas_item_lower_to_bottom(GNOME_CANVAS_ITEM(dev->group_background));
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
}


/*--------------------------------------------------------------------------*\
 * plD_state_gcw()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
 *
\*--------------------------------------------------------------------------*/

guint32 plcolor_to_rgba(PLColor color, guchar alpha)
{
  return
    ((int)(color.r) << 24)
    + ((int)(color.g) << 16)
    + ((int)(color.b) << 8)
    + alpha;
}

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
/*       if(dev->canvas==NULL) install_canvas(pls); */
      break;
    case (6): /* PLSTATE_CMAP1 */
/*      if(dev->canvas==NULL) install_canvas(pls); */
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
\*--------------------------------------------------------------------------*/

static void fill_polygon (PLStream* pls)
{
  GnomeCanvasPoints* points;
  GnomeCanvasGroup* group;
  GnomeCanvasItem* item;
  GdkGC* gc;
  GcwPLdev* dev = pls->dev;
  GnomeCanvas* canvas;
  
  PLINT i;

  GdkColor color;
  GdkPoint* gdkpoints;

  PLINT tmp;

#ifdef DEBUG_GCW
  debug("<fill_polygon>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);

#ifdef ASSERT_GCW
  if(!GNOME_IS_CANVAS_ITEM(
    group = dev->group_current
    )) {
    plexit("GCW driver <fill_polygon>: Canvas group is NULL");
  }
#else
  group = dev->group_current;
#endif

  if(dev->use_pixmap) { /* Write to a pixmap */

    /* Allocate a new pixmap if needed */
    if(dev->pixmap == NULL) gcw_use_pixmap(canvas,TRUE);

    /* Clear the pixmap if required */
    if(! dev->pixmap_has_data) clear_pixmap(pls);

    gc = gdk_gc_new(dev->pixmap);

    color.red=(guint16)(pls->curcolor.r/255.*65535); 
    color.green=(guint16)(pls->curcolor.g/255.*65535); 
    color.blue=(guint16)(pls->curcolor.b/255.*65535);

    gdk_colormap_alloc_color(gtk_widget_get_colormap(GTK_WIDGET(dev->canvas)),
			     &color,FALSE,TRUE);
    gdk_gc_set_foreground(gc,&color);

    if((gdkpoints = (GdkPoint*)malloc(pls->dev_npts*sizeof(GdkPoint)))==NULL)
      plabort("GCW driver <fill_polygon>: Could not create gdkpoints");

    for(i=0;i<pls->dev_npts;i++) {
      gdkpoints[i].x = pls->dev_x[i];
      gdkpoints[i].y = dev->height-pls->dev_y[i];
    }

    gdk_draw_polygon(dev->pixmap,gc,TRUE,gdkpoints,pls->dev_npts);
    
    dev->pixmap_has_data = TRUE;

    free(gdkpoints);
    gdk_gc_unref(gc);
  }
  else { /* Use Gnome Canvas polygons */

    if( (points = gnome_canvas_points_new (pls->dev_npts)) == NULL )
      plabort("GCW driver <fill_polygon>: Could not create points");

    for (i=0; i<pls->dev_npts; i++) {
      points->coords[2*i] = ((gdouble) pls->dev_x[i]);
      points->coords[2*i + 1] = ((gdouble) -pls->dev_y[i]);
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


    /* Draw a thin outline for each polygon */
#ifdef OUTLINE_POLYGON_GCW
    tmp = pls->width;
    pls->width=1;
    plD_polyline_gcw(pls,pls->dev_x,pls->dev_y,pls->dev_npts);
    pls->width = tmp;
#endif
  }
}



/*--------------------------------------------------------------------------*\
 * clear()
 *
 * Handles call to clear the canvas.
\*--------------------------------------------------------------------------*/

static void clear (PLStream* pls)
{
  GcwPLdev* dev = pls->dev;

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
 * proc_str()
 *
 * Handles call to draw text on the canvas when the HAS_TEXT escape funtion
 * case is invoked.
 *
 * This routine is unicode enabled, and requires freetype.
\*--------------------------------------------------------------------------*/

void proc_str(PLStream *pls, EscText *args)
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
  gdouble affine_plplot[6] = {0.,0.,0.,0.,0.,0.};

  PLINT clxmin, clxmax, clymin, clymax; /* Clip limits */
  ArtBpath* clip;
  GnomeCanvasPathDef* path;

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


#ifdef DEBUG_GCW
  debug("<proc_str>\n");
#endif

  if(dev->canvas==NULL) install_canvas(pls);
  canvas = dev->canvas;

  if(dev->group_hidden==NULL) plD_bop_gcw(pls);

#ifdef ASSERT_GCW
  if(!GNOME_IS_CANVAS_ITEM(
    group = dev->group_current
    )) {
    plexit("GCW driver <proc_str>: Canvas group is NULL");
  }
#else
  group = dev->group_current;
#endif

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

  /* Apply plplot difilt transformations; i.e., determine the string
   * position and clip limits.
   */
  difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);

  /* Set the clip in this clipgroup */
  path = gnome_canvas_path_def_new();
  gnome_canvas_path_def_ensure_space(path,6);
  gnome_canvas_path_def_moveto(path,clxmin,-clymin);
  gnome_canvas_path_def_lineto(path,clxmin,-clymax);
  gnome_canvas_path_def_lineto(path,clxmax,-clymax);
  gnome_canvas_path_def_lineto(path,clxmax,-clymin);
  gnome_canvas_path_def_closepath(path);
  g_object_set(G_OBJECT(group),"path",path,NULL);

  /* Font size: size is in pixels but chrht is in mm.  Why the extra factor? */
  font_size = (gint)(pls->chrht*PIXELS_PER_MM*1.5);

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
    art_affine_translate(affine_baseline,
			 -total_width*args->just + sum_width,
			 height[0]/2.5-up_list[i]);
    art_affine_translate(affine_translate,
			 args->x,-args->y);
    gnome_canvas_item_affine_relative(item[i],affine_translate);
    gnome_canvas_item_affine_relative(item[i],affine_plplot);
    gnome_canvas_item_affine_relative(item[i],affine_baseline);

    /* Keep track of the position in the string */
    sum_width += width[i];
    if(i!=N-1) sum_width += 2; /* Add a little extra space */
  }
}


/*--------------------------------------------------------------------------*\
 * plD_esc_gcw()
 *
 * Escape functions.
 *
\*--------------------------------------------------------------------------*/

void plD_esc_gcw(PLStream *pls, PLINT op, void *ptr)
{

#ifdef DEBUG_GCW
  char msg[100];

  sprintf(msg,"<plD_esc_gcw>: %d\n",op);
  debug(msg);
#endif

  switch(op) {

  case PLESC_DEVINIT:
    gcw_set_canvas(pls,GNOME_CANVAS(ptr));
    break;

  case PLESC_CLEAR:
    /*    clear(pls); */
    break;

  case PLESC_FILL:
#ifdef POLYGON_GCW
    fill_polygon(pls);
#endif
    break;

  case PLESC_HAS_TEXT:
    if(ptr!=NULL) {
      proc_str(pls, ptr); /* Draw the text */
    }
    else { 

      /* Assume this was a request to change the text handling,
       * which is a special hack for this driver.  This is done 
       * through the escape function so that we can get easy access
       * to pls.
       */
      if(((GcwPLdev*)(pls->dev))->use_text) 
	pls->dev_text = 1; /* Allow text handling */
      else pls->dev_text = 0; /* Disallow text handling */
    }
    break;

  case PLESC_GRAPH:
    break;

  default:
    break;
  }
}
