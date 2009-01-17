/*
 * demo.c - Demonstrates the simplest use of the plplot canvas widget with gtk.
 * 
 *  Copyright (C) 2004, 2005 Thomas J. Duck
 *  All rights reserved.
 *
 *  Thomas J. Duck <tom.duck@dal.ca>
 *  Department of Physics and Atmospheric Science,
 *  Dalhousie University, Halifax, Nova Scotia, Canada, B3H 3J5
 *
 *  $Author$
 *  $Revision$
 *  $Date$
 *  $Name$
 *
 *
 * NOTICE
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include <plplotcanvas.h>
#include <gtk/gtk.h>

/* The width and height of the plplot canvas widget */
#define WIDTH 1000 /* 500 */
#define HEIGHT 600 /* 300 */

/* Delete event callback */
gint delete_event_local( GtkWidget *widget,GdkEvent *event,gpointer data ) {
  return FALSE;
}

/* Destroy event calback */
void destroy_local(GtkWidget *widget,gpointer data) {
  gtk_main_quit ();
}


int main(int argc,char *argv[] )
{

  PlplotCanvas* canvas;
  GtkWidget *window;

  /* Parse the options */
  plparseopts(&argc, (const char **) argv, PL_PARSE_FULL);

  /* The data to plot */
  double x[11] = {0,1,2,3,4,5,6,7,8,9,10};
  double y[11] = {0,0.1,0.4,0.9,1.6,2.6,3.6,4.9,6.4,8.1,10};

  /* Initialize gtk and the glib type system */
  gtk_init(&argc, &argv);
  g_type_init();

  /* Create the canvas and set its size; during the creation process,
   * the gcw driver is loaded into plplot, and plinit() is invoked.
   */
  canvas=plplot_canvas_new(TRUE);
  plplot_canvas_set_size(canvas,WIDTH,HEIGHT);

  /* Create a new window and stuff the canvas into it */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width(GTK_CONTAINER(window),10);
  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(canvas));

  /* Connect the signal handlers to the window decorations */
  g_signal_connect(G_OBJECT(window),"delete_event",
		   G_CALLBACK(delete_event_local),NULL);
  g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy_local),NULL);

  /* Display everything */
  gtk_widget_show_all(window);

  /* Draw on the canvas with Plplot */
  plplot_canvas_adv(canvas,0); /* Advance to first page */
  plplot_canvas_col0(canvas,15); /* Set color to black */
  plplot_canvas_wid(canvas,2); /* Set the pen width */
  plplot_canvas_vsta(canvas); /* Set the viewport */
  plplot_canvas_wind(canvas,0.,10.,0.,10.); /* Set the window */
  plplot_canvas_box(canvas,"bcnst",0.,0,"bcnstv",0.,0); /* Set the box */
  plplot_canvas_lab(canvas,"x-axis","y-axis","A Simple Plot"); /* Draw some labels */

  /* Draw the line */
  plplot_canvas_col0(canvas,1); /* Set the pen color */
  plplot_canvas_line(canvas,11,x,y);

  /* Advancing the page finalizes this plot */
  plplot_canvas_adv(canvas,0);

  /* Start the gtk main loop */
  gtk_main();
  return 0;
}
