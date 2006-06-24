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

extern PLStream *pls[];

void gcw_debug(char* msg)
{
  fprintf(stderr,msg);
  fflush(stderr);
}


/*--------------------------------------------------------------------------*\
 * gcw_set_gdk_color()
 *
 * Sets the gdk foreground color for drawing in the current device 
 * according to the current drawing color.
 *
\*--------------------------------------------------------------------------*/

void gcw_set_gdk_color()
{
#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_set_gdk_color>\n");
#endif

  GcwPLdev* dev = plsc->dev;
  GdkColor color;

  color.red=(guint16)(plsc->curcolor.r/255.*65535); 
  color.green=(guint16)(plsc->curcolor.g/255.*65535); 
  color.blue=(guint16)(plsc->curcolor.b/255.*65535);
  
  if(!gdk_colormap_alloc_color(dev->colormap,&color,FALSE,TRUE))
    plwarn("GCW driver <set_gdk_color>: Could not allocate color.");
  
  gdk_gc_set_foreground(dev->gc,&color);

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_set_gdk_color>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_clear_background()
 *
 * Clears the background pixmap for the current gcw device with the 
 * background color.
 *
\*--------------------------------------------------------------------------*/

void gcw_clear_background()
{
  GcwPLdev* dev = plsc->dev;

  PLINT width,height;

#ifdef DEBUG_GCW_1
  gcw_debug("<clear_background>\n");
#endif

  /* Retrieve the device width and height */
  width = *(PLINT*)g_object_get_data(G_OBJECT(dev->canvas),"canvas-width");
  height = *(PLINT*)g_object_get_data(G_OBJECT(dev->canvas),"canvas-height");

  /* Allocate the background color*/
  gdk_colormap_alloc_color(dev->colormap,&(dev->bgcolor),FALSE,TRUE);

  /* Clear the background pixmap with the background color.  Note that
   * we are going to reset the current gdk drawing color below, so we
   * can reuse the gc. */
  gdk_gc_set_foreground(dev->gc,&(dev->bgcolor));
  gdk_draw_rectangle(dev->background,dev->gc,TRUE,0,0,width,height);

  /* Note that our pixmap is currently clear */
  dev->pixmap_has_data = FALSE;

  /* Reset the current gdk drawing color */
  gcw_set_gdk_color();

#ifdef DEBUG_GCW_1
  gcw_debug("</clear_background>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_init_canvas()
 *
 * Initializes a canvas for use with this driver, and installs it into
 * the current gcw device.
 *
\*--------------------------------------------------------------------------*/

void gcw_init_canvas(GnomeCanvas* canvas)
{
  GcwPLdev* dev = plsc->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_init_canvas>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) plexit("GCW driver: Canvas not found");

  /* Add the canvas to the device */
  dev->canvas=canvas;

  /* Get the colormap */
  dev->colormap = gtk_widget_get_colormap(GTK_WIDGET(dev->canvas));

  /* Size the canvas */
  gcw_set_canvas_size(canvas,dev->width,dev->height);

  /* Create the persistent group, which is always visible */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_persistent = GNOME_CANVAS_GROUP(gnome_canvas_item_new(
			      gnome_canvas_root(canvas),
			      gnome_canvas_clipgroup_get_type(),
			      "x",0.,
			      "y",0.,
			      NULL))
    )) {
    plexit("GCW driver <gcw_init_canvas>: Canvas group cannot be created");
  }

  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_persistent),"path",NULL,NULL);

#ifdef DEBUG_GCW1
  gcw_debug("</gcw_init_canvas>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_install_canvas()
 *
 * Installs a canvas into the current gcw device's window.  The canvas is
 * created if necessary.  A variety of callbacks are defined for actions
 * in the window.
 *
\*--------------------------------------------------------------------------*/

/*******************************
 * Main window event callbacks *
 *******************************/

/* Delete event callback */
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data ) {
  return FALSE; /* FALSE follows with the destroy signal */
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

  GnomeCanvas *canvas;
  GtkWidget *scrolled_window;
  GList *list;

  GcwPLdev* dev = data;

  gdouble curmag,dum;

  /* Get the current canvas */
  n = gtk_notebook_get_current_page(GTK_NOTEBOOK(dev->notebook));
  scrolled_window = gtk_notebook_get_nth_page(GTK_NOTEBOOK(dev->notebook),n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Determine the new magnification */
  if(flag==2) /* Zoom in */
    gcw_set_canvas_zoom(canvas,ZOOMSTEP);
  else if(flag==0) { /* Zoom out */

    /* Don't zoom smaller than the original size: this causes GDK pixmap
     * errors.
     */
    gnome_canvas_c2w(canvas,1,0,&curmag,&dum); 
    curmag = 1./curmag;
    if(curmag/ZOOMSTEP>1.) {
      gcw_set_canvas_zoom(canvas,1./ZOOMSTEP);
    }
    else {
      gcw_set_canvas_zoom(canvas,(PLFLT)(ZOOM100/curmag));
    }
  }
  else { /* Zoom 100 */
    /* Get current magnification */
    gnome_canvas_c2w(canvas,1,0,&curmag,&dum); 
    curmag = 1./curmag;
    gcw_set_canvas_zoom(canvas,(PLFLT)(ZOOM100/curmag));
  }

  /* Set the focus on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),dev->notebook);
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
  GcwPLdev* dev = data;

  GtkWidget *scrolled_window;
  GnomeCanvas *canvas;

  gchar *fname;
  guint n;
  char devname[10],str[100];

  void *save_state;
  void *new_state;

  PLINT cur_strm, new_strm;
  PLStream *plsr;

  GtkWidget *fs;

  gdouble curmag,dum;
  
  gboolean errflag = FALSE;

  struct stat buf;

  GtkDialog *dialog;
  GtkWidget *hbox,*message,*icon;
  gint result;

  guint context;
  
  FILE *f;
  
  /* Get the file name */
  if( (fname = 
       strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dev->filew))))
      ==NULL )
    plabort("GCW driver <file_ok_sel>: Cannot obtain filename");

  /* Check to see if the file already exists, and respond appropriately */
  if(stat(fname,&buf)==0) {

    /* Confirm the user wants to overwrite the existing file */

    dialog = GTK_DIALOG(gtk_dialog_new_with_buttons("",
		          GTK_WINDOW(dev->filew),
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
  gtk_widget_hide(dev->filew);

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),dev->notebook);

  /* Test that we can open and write to the file */
  if((f=fopen(fname,"w"))==NULL)
    plabort("GCW driver <file_ok_sel>: Cannot open output file");
  fclose(f);
  remove(fname); /* Otherwise, we may leave behind a zero-length file */

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
    if(dev->statusbar!=NULL) {
      context=gtk_statusbar_get_context_id(GTK_STATUSBAR(dev->statusbar),
					   "PLplot");
      gtk_statusbar_push(GTK_STATUSBAR(dev->statusbar), context,
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
      if(dev->statusbar!=NULL) {
	context=gtk_statusbar_get_context_id(GTK_STATUSBAR(dev->statusbar),
					     "PLplot");

	gtk_statusbar_push(GTK_STATUSBAR(dev->statusbar),context,
			   " NOTE: Use '-drvopt hrshsym' on command-line "
			   "for complete symbol set in PostScript files."
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

  /* Get the current canvas */
  n = gtk_notebook_get_current_page(GTK_NOTEBOOK(dev->notebook));
  scrolled_window = gtk_notebook_get_nth_page(GTK_NOTEBOOK(dev->notebook),n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Switch in the previously saved plot state */
  new_state = g_object_get_data(G_OBJECT(canvas),"plotbuf");

  if(new_state != NULL) {
    save_state = (void *)plbuf_switch(plsc, new_state);

    plsetopt ("drvopt","text"); /* Blank out the drvopts (not needed here) */

    /* Get the current stream and make a new one */
    plgstrm(&cur_strm);
    plsr = plsc; /* Save a pointer to the current stream */

    plmkstrm(&new_strm);
    plsfnam(fname); /* Set the file name */
    plsdev(devname); /* Set the device */

    /* Copy the current stream to the new stream. */
    plcpstrm(cur_strm, 0); 

    plreplot();            /* do the save by replaying the plot buffer */

    plend1();              /* finish the device */

    plsstrm(cur_strm);     /* return to previous stream */

    plbuf_restore(plsc, save_state);
    free(save_state);
  } else {
    plwarn("GCW driver <file_ok_sel>: Cannot set up output stream.");
  }
}

/* Callback to cancel file selection */
void file_cancel_sel(GtkWidget *w, gpointer data)
{
  GcwPLdev* dev = data;
  
  /* Hide the file selection widget */
  gtk_widget_hide(dev->filew);

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),dev->notebook);
}

/* Callback to create file selection dialog */
void filesel(GtkWidget *widget, gpointer data ) {

  GtkWidget *scrolled_window;
  GnomeCanvas *canvas;

  GcwPLdev* dev = data;

  guint n;

  /* Get the current canvas */
  n = gtk_notebook_get_current_page(GTK_NOTEBOOK(dev->notebook));
  scrolled_window = gtk_notebook_get_nth_page(GTK_NOTEBOOK(dev->notebook),n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Create a new file dialog if it doesn't already exist */
  if(dev->filew == NULL) {

    /* Create a new file selection widget */
    dev->filew = gtk_file_selection_new ("File selection");
    
    /* Connect the ok_button to file_ok_sel function */
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(dev->filew)->ok_button),
		     "clicked", G_CALLBACK(file_ok_sel), (gpointer)dev);
    
    /* Connect the cancel_button to destroy the widget */
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(dev->filew)->cancel_button),
		     "clicked", G_CALLBACK(file_cancel_sel), (gpointer)dev);
    
    /* Lets set the filename, as if this were a save dialog, and we are giving
       a default filename */
    gtk_file_selection_set_filename(GTK_FILE_SELECTION(dev->filew), 
				    "plot.psc");
  }
    
  gtk_widget_show(dev->filew);
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

/**********************
 * gcw_install_canvas *
 **********************/

void gcw_install_canvas(GnomeCanvas *canvas)
{
  GcwPLdev* dev = plsc->dev;
  GtkWidget *vbox,*hbox,*button,*image,*scrolled_window;

  gboolean flag = FALSE;

  PLINT width,height;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_install_canvas>\n");
#endif

  /* Create a new canvas if needed */
  if(!GNOME_IS_CANVAS(canvas)) {   
      if( !GNOME_IS_CANVAS(canvas = GNOME_CANVAS(gnome_canvas_new_aa())) )
	plexit("GCW driver <gcw_install_canvas>: Could not create Canvas");
  }

  /* Initialize canvas */
  gcw_init_canvas(canvas);

  if(dev->window==NULL) { /* Create a new window and install a notebook */

    flag = TRUE;
    
    /* Create a new window */
    dev->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dev->window),"PLplot");

    /* Connect the signal handlers to the window decorations */
    g_signal_connect(G_OBJECT(dev->window),"delete_event",
		     G_CALLBACK(delete_event),NULL);
    g_signal_connect(G_OBJECT(dev->window),"destroy",G_CALLBACK(destroy),NULL);


    /* Create a vbox and put it into the window */
    vbox = gtk_vbox_new(FALSE,2);
    gtk_container_add(GTK_CONTAINER(dev->window),vbox);

    /* Create a hbox and put it into the vbox */
    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,0);

    /* Create a statusbar and put it into the vbox */
    dev->statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox),dev->statusbar,FALSE,FALSE,0);

    /* Add an vbox to the hbox */
    vbox = gtk_vbox_new(FALSE,5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
    gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);

    /* Create the new notebook and add it to the hbox*/
    dev->notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(dev->notebook),TRUE);
    gtk_box_pack_start(GTK_BOX(hbox),dev->notebook,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(dev->notebook), "key_release_event",
                         G_CALLBACK(key_release), G_OBJECT(dev->notebook));

    /* Use a few labels as spacers */
    gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new(" "),FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),gtk_label_new(" "),FALSE,FALSE,0);

    /* Add buttons to the vbox */

    /* Add zoom in button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_IN,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button),image);
    gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_in), (gpointer)dev);
    g_signal_connect(G_OBJECT(button), "key_release_event",
                         G_CALLBACK(key_release), (gpointer)dev);

    /* Add zoom100 button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_100,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button),image);
    gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_100), (gpointer)dev);

    /* Add zoom out button and create callbacks */
    image = gtk_image_new_from_stock(GTK_STOCK_ZOOM_OUT,
				     GTK_ICON_SIZE_SMALL_TOOLBAR);
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button),image);
    gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
    g_signal_connect (G_OBJECT(button), "clicked",
		      G_CALLBACK(zoom_out), (gpointer)dev);
    g_signal_connect(G_OBJECT(button), "key_release_event",
                         G_CALLBACK(key_release), (gpointer)dev);

    /* Add save button and create callbacks */
    if(plsc->plbuf_write) {
      image = gtk_image_new_from_stock(GTK_STOCK_SAVE,
				       GTK_ICON_SIZE_SMALL_TOOLBAR);
      button = gtk_button_new();
      gtk_container_add(GTK_CONTAINER(button),image);
      gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
      g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(filesel), (gpointer)dev);
    }
  } /* if(dev->window==NULL) */

  /* Put the canvas in a scrolled window */
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), 
					GTK_WIDGET(canvas));

  /* Install the scrolled window in the notebook */
  gtk_notebook_append_page(GTK_NOTEBOOK(dev->notebook),scrolled_window, NULL);

  if(flag) {

    /* Set the focus on the notebook */
    gtk_window_set_focus(GTK_WINDOW(dev->window),dev->notebook);

    /* Retrieve the canvas width and height */
    width = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-width");
    height = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-height");

    /* Size the window */
    gtk_window_resize(GTK_WINDOW(dev->window),width*ZOOM100+65,
 		      height*ZOOM100+75);
  }

  /* Display everything */
  gtk_widget_show_all(dev->window);

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_install_canvas>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_set_device_size()
 *
 * Sets the page size for this device.
 *
 * Width and height are both measured in device coordinate units.
 *
 * Note that coordinates in driver-space are measured in device units,
 * which correspond to the pixel size on a typical screen.  The coordinates
 * reported and received from the PLplot core, however, are in virtual
 * coordinates, which is just a scaled version of the device coordinates.
 * This strategy is used so that the calculations in the PLplot
 * core are performed at reasonably high resolution.
 *
\*--------------------------------------------------------------------------*/

void gcw_set_device_size(PLINT width,PLINT height) {
  GcwPLdev* dev = plsc->dev;
  PLINT w, h;

  /* Set the number of virtual coordinate units per mm */
  plP_setpxl((PLFLT)VIRTUAL_PIXELS_PER_MM,(PLFLT)VIRTUAL_PIXELS_PER_MM);
  
  /* Set up physical limits of plotting device, in virtual coord units */
  w = (PLINT)(width*VSCALE);
  h = (PLINT)(height*VSCALE);
  plP_setphy((PLINT)0,w,(PLINT)0,h);
  
  /* Save the width and height for the device, in device units */
  dev->width = width;
  dev->height = height;
}

/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_size()
 *
 * Sets the canvas size.  If resizing is not allowed, this function
 * ensures that the canvas size matches the physical page size.
 *
\*--------------------------------------------------------------------------*/

gint count = 0;

void gcw_set_canvas_size(GnomeCanvas* canvas,PLINT width,PLINT height)
{
  GcwPLdev* dev = plsc->dev;
  PLINT tmp;

  PLINT *w,*h;

  GdkGC *gc_new;
  GdkGCValues values;

  PLINT strm;

  GdkPixmap *background;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_set_canvas_size>\n");
#endif

  /* Set the device size, if resizing is allowed. */
  if(dev->allow_resize) gcw_set_device_size(width,height);

  width = dev->width;
  height = dev->height;
  if(plsc->portrait) { /* Swap width and height of display */
    tmp=width;
    width=height;
    height=tmp;
  }

  /* The width and height need to be enlarged by 1 pixel for the canvas */
  width += 1;
  height += 1;

  /* Attach the width and height to the canvas */
  if((w=(PLINT*)malloc(sizeof(gint)))==NULL)
    plwarn("GCW driver <gcw_set_canvas_size>: Insufficient memory.");
  if((h=(PLINT*)malloc(sizeof(gint)))==NULL)
    plwarn("GCW driver <gcw_set_canvas_size>: Insufficient memory.");
  *w = width;
  *h = height;
  g_object_set_data(G_OBJECT(canvas),"canvas-width",(gpointer)w);
  g_object_set_data(G_OBJECT(canvas),"canvas-height",(gpointer)h);

  /* Size the canvas appropriately */
  gtk_widget_set_size_request(GTK_WIDGET(canvas),(gint)(width),
			      (gint)(height));

  /* Position the canvas appropriately */
  gnome_canvas_set_scroll_region(canvas,0.,(gdouble)(-height),
				 (gdouble)(width),1.);

  /* Set up the background pixmap */
  if(dev->background==NULL || dev->allow_resize) { 

    if(GDK_IS_PIXMAP(dev->background)) g_object_unref(dev->background);

    /* Why does this next *useless* command speed up the animation demo?
     * If we unref the allocated pixmaps, the benefit goes away!! */
/*     if(count<2) { */
/*       gdk_pixmap_new(NULL,width,height,gdk_visual_get_best_depth()); */
/*       printf("Count %d\n",count); */
/*       count++; */
/*     } */
/*     else { printf("Count %d\n",count); count ++; } */

    dev->background = gdk_pixmap_new(NULL,width,height,
		            gtk_widget_get_visual(GTK_WIDGET(canvas))->depth);
  }

  /* Set up the drawing context for the background pixmap */
  if(dev->gc==NULL || dev->allow_resize) {

    /* Maintain the old values for pen width, color, etc */
    if(GDK_IS_GC(dev->gc)) {
      gdk_gc_get_values(dev->gc,&values);
      gdk_gc_unref(dev->gc);
      dev->gc = gdk_gc_new_with_values(dev->background,&values,
				       GDK_GC_FOREGROUND | GDK_GC_LINE_WIDTH |
				       GDK_GC_LINE_STYLE | GDK_GC_CAP_STYLE |
				       GDK_GC_JOIN_STYLE);
    }
    else dev->gc = gdk_gc_new(dev->background);
  }

  /* Clear the background pixmap */
  gcw_clear_background(dev);

  /* Advance the page if we are allowing resizing.  This ensures that
   * the physical coordinate system is set up correctly.
   */
  if(dev->allow_resize) pladv(0);

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_set_canvas_size>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_zoom()
 *
 * Sets the zoom magnification on the canvas and resizes the widget
 * appropriately.
\*--------------------------------------------------------------------------*/

void gcw_set_canvas_zoom(GnomeCanvas* canvas,PLFLT magnification)
{
  gdouble curmag=1.,dum;

  PLINT width,height;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_set_canvas_zoom>\n");
#endif

  /* Retrieve the device width and height */
  width = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-width");
  height = *(PLINT*)g_object_get_data(G_OBJECT(canvas),"canvas-height");

  /* Get the current magnification */
  gnome_canvas_c2w(canvas,1,0,&curmag,&dum);
  curmag = 1./curmag;

  /* Apply the new magnification */
  gnome_canvas_set_pixels_per_unit(canvas,magnification*curmag);

  /* Size the canvas appropriately */
  gtk_widget_set_size_request(GTK_WIDGET(canvas),
			      (gint)((width)*magnification*curmag),
			      (gint)((height)*magnification*curmag));

  /* Position the canvas appropriately */
  gnome_canvas_set_scroll_region(canvas,0.,(gdouble)(-height),
				 (gdouble)(width),1.);

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_set_canvas_zoom>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_use_text()
 *
 * Used to turn text usage on and off for the current device.
\*--------------------------------------------------------------------------*/

void gcw_use_text(PLINT use_text)
{
  GcwPLdev* dev = plsc->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_text>\n");
#endif

#ifdef HAVE_FREETYPE

  if(use_text) plsc->dev_text = 1; /* Allow text handling */
  else plsc->dev_text = 0; /* Disallow text handling */

#endif

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_text>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_use_pixmap()
 *
 * Used to turn pixmap usage on and off for the current device.
 * This is relevant for polygon fills (used during shading calls).
\*--------------------------------------------------------------------------*/

void gcw_use_pixmap(PLINT use_pixmap)
{
  GcwPLdev* dev = plsc->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_pixmap>\n");
#endif

  dev->use_pixmap = (gboolean)use_pixmap;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_pixmap>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_use_hrshsym()
 *
 * Used to turn hershey symbol usage on and off for the current device.
\*--------------------------------------------------------------------------*/

void gcw_use_hrshsym(PLINT use_hrshsym)
{
  GcwPLdev* dev = plsc->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_hrshsym>\n");
#endif

  plsc->dev_hrshsym = use_hrshsym;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_hrshsym>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_use_persistence
 *
 * Directs the GCW driver whether or not the subsequent drawing operations
 * should be persistent.
\*--------------------------------------------------------------------------*/

void gcw_use_persistence(PLINT use_persistence)
{
  GcwPLdev* dev = plsc->dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_persistence>\n");
#endif

  dev->use_persistence = (gboolean)use_persistence;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_persistence>\n");
#endif
}
