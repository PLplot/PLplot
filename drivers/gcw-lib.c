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


void gcw_debug(char* msg)
{
  fprintf(stderr,msg);
  fflush(stderr);
}


/*--------------------------------------------------------------------------*\
 * gcw_init_canvas()
 *
 * Initializes a canvas, and prepares it to be drawn to.
 *
 * The associated device is attached to the canvas as the property "dev".
\*--------------------------------------------------------------------------*/

void gcw_init_canvas(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_init_canvas>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) plexit("GCW driver: Canvas not found");

  /* Get the device */
  dev = plsc->dev;

  /* Add the canvas to the device */
  dev->canvas=canvas;

  /* Attach the device and associated stream pointer to the canvas widget */
  g_object_set_data(G_OBJECT(canvas),"dev",(gpointer)dev);

  /* Determine if the canvas is antialiased */
  g_object_get(G_OBJECT(canvas),"aa",&(dev->aa),NULL);

  /* Size the canvas */
  gcw_set_canvas_size(canvas,dev->width,dev->height);

  /* Add the background to the canvas and move it to the back */
  if(!GNOME_IS_CANVAS_ITEM(
    dev->group_background = (GnomeCanvasGroup*)gnome_canvas_item_new(
					  gnome_canvas_root(canvas),
					  gnome_canvas_clipgroup_get_type(),
					  "x",0.,
					  "y",0.,
					  NULL)
    )) {
    plabort("GCW driver <gcw_init_canvas>: Canvas item not created");
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
			"fill-color-rgba", dev->bgcolor,
			"width-units", 0.0,
			NULL)
    )) {
    plabort("GCW driver <gcw_init_canvas>: Canvas item not created");
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
    plabort("GCW driver <gcw_init_canvas>: Canvas item not created");
  }

  /* Set the clip to NULL */
  g_object_set(G_OBJECT(dev->group_foreground),"path",NULL,NULL);

  gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(dev->group_foreground));

  /* Get the colormap */
  dev->colormap = gtk_widget_get_colormap(GTK_WIDGET(dev->canvas));

#ifdef DEBUG_GCW1
  gcw_debug("</gcw_init_canvas>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_install_canvas()
 *
 * Installs a canvas into the standalone driver window.  The canvas is
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
    /* Get current magnification */
    if(dev->zoom_is_initialized) gnome_canvas_c2w(canvas,1,0,&curmag,&dum); 
    curmag = 1./curmag;
    gcw_set_canvas_zoom(canvas,(PLFLT)(ZOOM100/curmag));
  }

  /* Set the focus on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(notebook));
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
  FILE *f;
  guint n;
  char devname[10],str[100];

  PLINT cur_strm, new_strm;

  GtkWidget *fs;

  gdouble curmag,dum;

  PLINT icol0,icol1,ncol0,ncol1;
  PLColor *cmap0,*cmap1;

  gboolean errflag = FALSE;

  struct stat buf;

  GtkDialog *dialog;
  GtkWidget *hbox,*message,*icon;
  gint result;

  guint context;
  
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
  gtk_widget_hide(GTK_WIDGET(dev->filew));

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(dev->notebook));

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

  /* Get the current canvas */
  n = gtk_notebook_get_current_page(GTK_NOTEBOOK(dev->notebook));
  scrolled_window = gtk_notebook_get_nth_page(GTK_NOTEBOOK(dev->notebook),n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

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


  /* Restore */

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
  GcwPLdev* dev = data;
  
  /* Hide the file selection widget */
  gtk_widget_hide(GTK_WIDGET(dev->filew));

  /* Put the focus back on the notebook */
  gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(dev->notebook));
}

/* Callback to create file selection dialog */
void filesel(GtkWidget *widget, gpointer data ) {

  GtkNotebook *notebook;
  GtkWidget *scrolled_window;
  GnomeCanvas *canvas;

  GcwPLdev* dev;

  guint n;

  /* Get the current canvas */
  notebook = GTK_NOTEBOOK(data);
  n = gtk_notebook_get_current_page(notebook);
  scrolled_window = gtk_notebook_get_nth_page(notebook,n);
  canvas = GNOME_CANVAS(gtk_container_get_children(
	   GTK_CONTAINER(gtk_container_get_children(
           GTK_CONTAINER(scrolled_window))->data))->data);

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <file_cancel_sel>: Device not found");

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
  GcwPLdev* dev;
  GtkWidget *vbox,*hbox,*button,*image,*scrolled_window;

  gboolean flag = FALSE;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_install_canvas>\n");
#endif

  if(!GNOME_IS_CANVAS(canvas)) {   /* Create a new canvas */
    if(((GcwPLdev*)(plsc->dev))->aa) {
      if( !GNOME_IS_CANVAS(canvas = GNOME_CANVAS(gnome_canvas_new_aa())) )
	plexit("GCW driver <gcw_install_canvas>: Could not create aa Canvas");
    }
    else {
      if( !GNOME_IS_CANVAS(canvas = GNOME_CANVAS(gnome_canvas_new())) )
	plexit("GCW driver <gcw_install_canvas>: Could not create gdk Canvas");
    }
  }

  /* Initialize canvas */
  gcw_init_canvas(canvas);

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plexit("GCW driver <zoom>: Device not found");

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
    gtk_container_add(GTK_CONTAINER(dev->window),GTK_WIDGET(vbox));

    /* Create a hbox and put it into the vbox */
    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(hbox),TRUE,TRUE,0);

    /* Create a statusbar and put it into the vbox */
    dev->statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(dev->statusbar),FALSE,FALSE,0);

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
    if(dev->use_plot_buffer) {
      image = gtk_image_new_from_stock(GTK_STOCK_SAVE,
				       GTK_ICON_SIZE_SMALL_TOOLBAR);
      button = gtk_button_new();
      gtk_container_add(GTK_CONTAINER(button), GTK_WIDGET(image));
      gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(button),FALSE,FALSE,0);
      g_signal_connect (G_OBJECT(button), "clicked",
			G_CALLBACK(filesel), G_OBJECT(dev->notebook));
    }
  } /* if(dev->window==NULL) */

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
    gtk_window_set_focus(GTK_WINDOW(dev->window),GTK_WIDGET(dev->notebook));

    /* Size the window */
    gtk_window_resize(GTK_WINDOW(dev->window),dev->width*ZOOM100+65,
 		      dev->height*ZOOM100+75);
  }

  /* Display everything */
  gtk_widget_show_all(dev->window);

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_install_canvas>\n");
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
  GcwPLdev* dev;

  gdouble curmag=1.,dum;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_set_canvas_zoom>\n");
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

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_set_canvas_zoom>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_set_canvas_size()
 *
 * Sets the canvas size.
 *
 * Width and height are both measured in device coordinate units.
 *
 * Notice that coordinates in the driver are measured in device units,
 * which correspond to the pixel size on a typical screen.  The coordinates
 * reported and received from the PLplot core, however, are in virtual
 * coordinates, which is just a scaled version of the device coordinates.
 * This strategy is used so that the calculations performed in the PLplot
 * core are performed at reasonably high resolution.
 *
\*--------------------------------------------------------------------------*/

void gcw_set_canvas_size(GnomeCanvas* canvas,PLFLT width,PLFLT height)
{
  GcwPLdev* dev;
  PLINT w, h;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_set_canvas_size>\n");
#endif

  /* Retrieve the device */
  if(GNOME_IS_CANVAS(canvas)) {
    if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
      plabort("GCW driver <gcw_set_canvas_size>: Device not found");
  }
  else dev = plsc->dev;

  /* Set the number of virtual coordinate units per mm */
  plP_setpxl((PLFLT)VIRTUAL_PIXELS_PER_MM,(PLFLT)VIRTUAL_PIXELS_PER_MM);

  /* Set up physical limits of plotting device, in virtual coordinate units */
  w = (PLINT)(width*VSCALE);
  h = (PLINT)(height*VSCALE);
  plP_setphy((PLINT)0,w,(PLINT)0,h);

  /* Save the width and height for the device, in device units */
  dev->width = width;
  dev->height = height;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_set_canvas_size>\n");
#endif
}


/*--------------------------------------------------------------------------*\
 * gcw_use_text()
 *
 * Used to turn text usage on and off.
\*--------------------------------------------------------------------------*/

void gcw_use_text(GnomeCanvas* canvas,PLINT use_text)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_text>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_text>: Device not found");

#ifdef HAVE_FREETYPE
  dev->use_text = (gboolean)use_text;

  /* Use a hack to the plplot escape mechanism to update text handling */
  plP_esc(PLESC_HAS_TEXT,NULL);
#endif

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_text>\n");
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

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_fast_rendering>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_fast_rendering>: Device not found");

  dev->use_fast_rendering = (gboolean)use_fast_rendering;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_fast_rendering>\n");
#endif
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

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_pixmap>\n");
#endif

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

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_pixmap>\n");
#endif
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

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_background_group>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_background_group>: Device not found");

  dev->group_current = dev->group_background;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_background_group>\n");
#endif
}

void gcw_use_foreground_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_foreground_group>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_foreground_group>: Device not found");

  dev->group_current = dev->group_foreground;

#ifdef DEBUG_GCW_1
  gcw_debug("</gcw_use_foreground_group>\n");
#endif
}

void gcw_use_default_group(GnomeCanvas* canvas)
{
  GcwPLdev* dev;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_default_group>\n");
#endif

  /* Retrieve the device */
  if( (dev = g_object_get_data(G_OBJECT(canvas),"dev")) == NULL)
    plabort("GCW driver <gcw_use_default_group>: Device not found");

  dev->group_current = dev->group_hidden;

#ifdef DEBUG_GCW_1
  gcw_debug("<gcw_use_default_group>\n");
#endif
}
