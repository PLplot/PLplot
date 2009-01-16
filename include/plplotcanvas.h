/* PlplotCanvas - a GnomeCanvas widget for use with plplot's gcw driver

  Copyright (C) 2004, 2005 Thomas J. Duck
  All rights reserved.

  Thomas J. Duck <tom.duck@dal.ca>
  Department of Physics and Atmospheric Science,
  Dalhousie University, Halifax, Nova Scotia, Canada, B3H 3J5


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

#ifndef __PLPLOTCANVAS_H__
#define __PLPLOTCANVAS_H__

#include "plplot.h"
#include "gcw.h"

#include <glib-object.h>
#include <libgnomecanvas/libgnomecanvas.h>


G_BEGIN_DECLS

#define PLPLOT_TYPE_CANVAS plplot_canvas_get_type()

#define PLPLOT_CANVAS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),PLPLOT_TYPE_CANVAS,PlplotCanvas))

#define PLPLOT_CANVAS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),PLPLOT_TYPE_CANVAS,PlplotCanvasClass))

#define PLPLOT_IS_CANVAS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj),PLPLOT_TYPE_CANVAS))

#define PLPLOT_IS_CANVAS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),PLPLOT_TYPE_CANVAS))

#define PLPLOT_CANVAS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o),PLPLOT_TYPE_CANVAS,PlplotCanvasClass))

typedef struct _PlplotCanvas PlplotCanvas;
typedef struct _PlplotCanvasClass PlplotCanvasClass;


struct _PlplotCanvas {
  GnomeCanvas parent;

  gint Nstream; /* The stream number */
};

struct _PlplotCanvasClass {
  GnomeCanvasClass parent;
};

PLDLLIMPEXP_GNOME2 GType plplot_canvas_get_type();

PLDLLIMPEXP_GNOME2 PlplotCanvas* plplot_canvas_new();
PLDLLIMPEXP_GNOME2 void plplot_canvas_devinit(PlplotCanvas* self);
PLDLLIMPEXP_GNOME2 void plplot_canvas_dispose(PlplotCanvas *self);

PLDLLIMPEXP_GNOME2 gint plplot_canvas_get_stream_number(PlplotCanvas* self);
PLDLLIMPEXP_GNOME2 void plplot_canvas_set_size(PlplotCanvas* self,gint width,gint height);
PLDLLIMPEXP_GNOME2 void plplot_canvas_set_zoom(PlplotCanvas* self,gdouble magnification);
PLDLLIMPEXP_GNOME2 void plplot_canvas_use_text(PlplotCanvas* self,gboolean use_text);
PLDLLIMPEXP_GNOME2 void plplot_canvas_use_pixmap(PlplotCanvas* self,gboolean use_pixmap);
PLDLLIMPEXP_GNOME2 void plplot_canvas_use_persistence(PlplotCanvas* self,gboolean use_persistence);


/*--------------------------------------------------------------------------*
 * Plplot wrappers - last updated 29 January 2005
 *
 *  This should be kept up-to-date with plplot.h
 *
/*--------------------------------------------------------------------------*/


/* set the format of the contour labels */

PLDLLIMPEXP_GNOME2 void plplot_canvas_setcontlabelformat(PlplotCanvas* self, PLINT lexp, PLINT sigdig);

/* set offset and spacing of contour labels */

PLDLLIMPEXP_GNOME2 void plplot_canvas_setcontlabelparam(PlplotCanvas* self, PLFLT offset, PLFLT size, PLFLT spacing, PLINT active);

/* Advance to subpage "page", or to the next one if "page" = 0. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_adv(PlplotCanvas* self, PLINT page);

/* simple arrow plotter. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_arrows(PlplotCanvas* self, PLFLT *u, PLFLT *v, PLFLT *x, PLFLT *y, PLINT n, PLFLT scale, PLFLT dx, PLFLT dy);

PLDLLIMPEXP_GNOME2 void plplot_canvas_vect(PlplotCanvas* self, PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

PLDLLIMPEXP_GNOME2 void plplot_canvas_svect(PlplotCanvas* self, PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLINT fill);

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_axes(PlplotCanvas* self, PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub, const char *yopt, PLFLT ytick, PLINT nysub);

/* Plot a histogram using x to store data values and y to store frequencies */

PLDLLIMPEXP_GNOME2 void plplot_canvas_bin(PlplotCanvas* self, PLINT nbin, PLFLT *x, PLFLT *y, PLINT center);

/* Start new page.  Should only be used with pleop(). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_bop(PlplotCanvas* self);

/* This draws a box around the current viewport. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_box(PlplotCanvas* self, const char *xopt, PLFLT xtick, PLINT nxsub, const char *yopt, PLFLT ytick, PLINT nysub);

/* This is the 3-d analogue of plbox(). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_box3(PlplotCanvas* self, const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx, const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby, const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz);

/* Calculate world coordinates and subpage from relative device coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_calc_world(PlplotCanvas* self, PLFLT rx, PLFLT ry, PLFLT *wx, PLFLT *wy, PLINT *window);

/* Clear current subpage. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_clear(PlplotCanvas* self);

/* Set color, map 0.  Argument is integer between 0 and 15. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_col0(PlplotCanvas* self, PLINT icol0);

/* Set color, map 1.  Argument is a float between 0. and 1. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_col1(PlplotCanvas* self, PLFLT col1);

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data.
 */

PLDLLIMPEXP_GNOME2 void plplot_canvas_cont(PlplotCanvas* self, PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 */

PLDLLIMPEXP_GNOME2 void plplot_canvas_fcont(PlplotCanvas* self, PLFLT (*f2eval) (PLINT, PLINT, PLPointer), PLPointer f2eval_data, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

/* /\* Copies state parameters from the reference stream to the current stream. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_cpstrm(PlplotCanvas* self, PLINT iplsr, PLINT flags); */

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_did2pc(PlplotCanvas* self, PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_dip2dc(PlplotCanvas* self, PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* /\* End a plotting session for all open streams. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_plend(PlplotCanvas* self); */

/* /\* End a plotting session for the current stream only. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_plend1(PlplotCanvas* self); */

/* /\* Simple interface for defining viewport and window. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_plenv(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis); */

/* /\* similar to plenv() above, but in multiplot mode does not advance the subpage, */
/*  instead the current subpage is cleared *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_plenv0(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis); */

/* End current page.  Should only be used with plbop(). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_eop(PlplotCanvas* self);

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_errx(PlplotCanvas* self, PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y);

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_erry(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax);

/* /\* Advance to the next family file on the next new page *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_famadv(PlplotCanvas* self); */

/* Pattern fills the polygon bounded by the input points. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_fill(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y);

/* Pattern fills the 3d polygon bounded by the input points. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_fill3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* /\* Flushes the output stream.  Use sparingly, if at all. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_flush(PlplotCanvas* self); */

/* Sets the global font flag to 'ifont'. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_font(PlplotCanvas* self, PLINT ifont);

/* Load specified font set. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_fontld(PlplotCanvas* self, PLINT fnt);

/* Get character default height and current (scaled) height */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gchr(PlplotCanvas* self, PLFLT *p_def, PLFLT *p_ht);

/* Returns 8 bit RGB values for given color from color map 0 */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gcol0(PlplotCanvas* self, PLINT icol0, PLINT *r, PLINT *g, PLINT *b);

/* Returns the background color by 8 bit RGB value */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gcolbg(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b);

/* Returns the current compression setting */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gcompression(PlplotCanvas* self, PLINT *compression);

/* Get the current device (keyword) name */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gdev(PlplotCanvas* self, char *p_dev);

/* Retrieve current window into device space */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gdidev(PlplotCanvas* self, PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy);

/* Get plot orientation */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gdiori(PlplotCanvas* self, PLFLT *p_rot);

/* Retrieve current window into plot space */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gdiplt(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax);

/* Get FCI (font characterization integer) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gfci(PlplotCanvas* self, PLUNICODE *pfci);

/* /\* Get family file parameters *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_gfam(PlplotCanvas* self, PLINT *p_fam, PLINT *p_num, PLINT *p_bmax); */

/* Get the (current) output file name.  Must be preallocated to >80 bytes */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gfnam(PlplotCanvas* self, char *fnam);

/* Get the (current) run level.  */

PLDLLIMPEXP_GNOME2 void plplot_canvas_glevel(PlplotCanvas* self, PLINT *p_level);

/* Get output device parameters. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gpage(PlplotCanvas* self, PLFLT *p_xp, PLFLT *p_yp, PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff);

/* /\* Switches to graphics screen. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_gra(PlplotCanvas* self); */

/* grid irregularly sampled data */

PLDLLIMPEXP_GNOME2 void plplot_canvas_griddata(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT *z, PLINT npts, PLFLT *xg, PLINT nptsx, PLFLT *yg, PLINT nptsy, PLFLT **zg, PLINT type, PLFLT data);

/* Get subpage boundaries in absolute coordinates */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gspa(PlplotCanvas* self, PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax);

/* /\* Get current stream number. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_gstrm(PlplotCanvas* self, PLINT *p_strm); */

/* Get the current library version number */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gver(PlplotCanvas* self, char *p_ver);

/* Get viewport boundaries in normalized device coordinates */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gvpd(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get viewport boundaries in world coordinates */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gvpw(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get x axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gxax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits);

/* Get y axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gyax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits);

/* Get z axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_gzax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits);

/* Draws a histogram of n values of a variable in array data[0..n-1] */

PLDLLIMPEXP_GNOME2 void plplot_canvas_hist(PlplotCanvas* self, PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax, PLINT nbin, PLINT oldwin);

/* Set current color (map 0) by hue, lightness, and saturation. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_hls(PlplotCanvas* self, PLFLT h, PLFLT l, PLFLT s);

/* /\* Initializes PLplot, using preset or default options *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_plinit(PlplotCanvas* self); */

/* Draws a line segment from (x1, y1) to (x2, y2). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_join(PlplotCanvas* self, PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);

/* Simple routine for labelling graphs. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_lab(PlplotCanvas* self, const char *xlabel, const char *ylabel, const char *tlabel);

/* Sets position of the light source */

PLDLLIMPEXP_GNOME2 void plplot_canvas_lightsource(PlplotCanvas* self, PLFLT x, PLFLT y, PLFLT z);

/* Draws line segments connecting a series of points. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_line(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y);

/* Draws a line in 3 space.  */

PLDLLIMPEXP_GNOME2 void plplot_canvas_line3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* Set line style. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_lsty(PlplotCanvas* self, PLINT lin);

/* plot continental outline in world coordinates */

PLDLLIMPEXP_GNOME2 void plplot_canvas_map(PlplotCanvas* self, void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plot the latitudes and longitudes on the background. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_meridians(PlplotCanvas* self, void (*mapform)(PLINT, PLFLT *, PLFLT *), PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plots a mesh representation of the function z[x][y]. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_mesh(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt);

/* Plots a mesh representation of the function z[x][y] with contour */

PLDLLIMPEXP_GNOME2 void plplot_canvas_meshc(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* /\* Creates a new stream and makes it the default.  *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_mkstrm(PlplotCanvas* self, PLINT *p_strm); */

/* Prints out "text" at specified position relative to viewport */

PLDLLIMPEXP_GNOME2 void plplot_canvas_mtex(PlplotCanvas* self, const char *side, PLFLT disp, PLFLT pos, PLFLT just, const char *text);

/* Plots a 3-d representation of the function z[x][y]. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_plot3d(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLINT side);

/* Plots a 3-d representation of the function z[x][y] with contour. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_plot3dc(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots a 3-d representation of the function z[x][y] with contour and
 * y index limits. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_plot3dcl(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT*indexymax);

/* Set fill pattern directly. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_pat(PlplotCanvas* self, PLINT nlin, PLINT *inc, PLINT *del);

/* Plots array y against x for n points using ASCII code "code".*/

PLDLLIMPEXP_GNOME2 void plplot_canvas_poin(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Draws a series of points in 3 space. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_poin3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code);

/* Draws a polygon in 3 space.  */

PLDLLIMPEXP_GNOME2 void plplot_canvas_poly3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *draw, PLINT ifcc);

/* Set the floating point precision (in number of places) in numeric labels. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_prec(PlplotCanvas* self, PLINT setp, PLINT prec);

/* Set fill pattern, using one of the predefined patterns.*/

PLDLLIMPEXP_GNOME2 void plplot_canvas_psty(PlplotCanvas* self, PLINT patt);

/* Prints out "text" at world cooordinate (x,y). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_ptex(PlplotCanvas* self, PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text);

/* Replays contents of plot buffer to current device/file. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_replot(PlplotCanvas* self);

/* Set line color by red, green, blue from  0. to 1. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_rgb(PlplotCanvas* self, PLFLT r, PLFLT g, PLFLT b);

/* Set line color by 8 bit RGB values. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_rgb1(PlplotCanvas* self, PLINT r, PLINT g, PLINT b);

/* Set character height. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_schr(PlplotCanvas* self, PLFLT def, PLFLT scale);

/* Set color map 0 colors by 8 bit RGB values */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scmap0(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b, PLINT ncol0);

/* Set number of colors in cmap 0 */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scmap0n(PlplotCanvas* self, PLINT ncol0);

/* Set color map 1 colors by 8 bit RGB values */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scmap1(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b, PLINT ncol1);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scmap1l(PlplotCanvas* self, PLINT itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLINT *rev);

/* Set number of colors in cmap 1 */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scmap1n(PlplotCanvas* self, PLINT ncol1);

/* Set a given color from color map 0 by 8 bit RGB value */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scol0(PlplotCanvas* self, PLINT icol0, PLINT r, PLINT g, PLINT b);

/* Set the background color by 8 bit RGB value */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scolbg(PlplotCanvas* self, PLINT r, PLINT g, PLINT b);

/* Used to globally turn color output on/off */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scolor(PlplotCanvas* self, PLINT color);

/* Set the compression level */

PLDLLIMPEXP_GNOME2 void plplot_canvas_scompression(PlplotCanvas* self, PLINT compression);

/* /\* Set the device (keyword) name *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_sdev(PlplotCanvas* self, const char *devname); */

/* Set window into device space using margin, aspect ratio, and */
/* justification */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sdidev(PlplotCanvas* self, PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

/* Set up transformation from metafile coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sdimap(PlplotCanvas* self, PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax, PLFLT dimxpmm, PLFLT dimypmm);

/* Set plot orientation, specifying rotation in units of pi/2. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sdiori(PlplotCanvas* self, PLFLT rot);

/* Set window into plot space */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sdiplt(PlplotCanvas* self, PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set window into plot space incrementally (zoom) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sdiplz(PlplotCanvas* self, PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set the escape character for text strings. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sesc(PlplotCanvas* self, char esc);

/* /\* Set family file parameters *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_sfam(PlplotCanvas* self, PLINT fam, PLINT num, PLINT bmax); */

/* Set FCI (font characterization integer) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sfci(PlplotCanvas* self, PLUNICODE fci);

/* Set the output file name. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sfnam(PlplotCanvas* self, const char *fnam);

/* Shade region. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_shade(PlplotCanvas* self, PLFLT **a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

PLDLLIMPEXP_GNOME2 void plplot_canvas_shade1(PlplotCanvas* self, PLFLT *a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

PLDLLIMPEXP_GNOME2 void plplot_canvas_shades(PlplotCanvas* self, PLFLT **a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT *clevel, PLINT nlevel, PLINT fill_width, PLINT cont_color, PLINT cont_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

PLDLLIMPEXP_GNOME2 void plplot_canvas_fshade(PlplotCanvas* self, PLFLT (*f2eval) (PLINT, PLINT, PLPointer), PLPointer f2eval_data, PLFLT (*c2eval) (PLINT, PLINT, PLPointer), PLPointer c2eval_data, PLINT nx, PLINT ny, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data);

/* Set up lengths of major tick marks. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_smaj(PlplotCanvas* self, PLFLT def, PLFLT scale);

/* /\* Set the memory area to be plotted (with the 'mem' driver) *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_smem(PlplotCanvas* self, PLINT maxx, PLINT maxy, void *plotmem); */

/* Set up lengths of minor tick marks. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_smin(PlplotCanvas* self, PLFLT def, PLFLT scale);

/* /\* Set orientation.  Must be done before calling plinit. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_sori(PlplotCanvas* self, PLINT ori); */

/* Set output device parameters.  Usually ignored by the driver. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_spage(PlplotCanvas* self, PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff);

/* /\* Set the pause (on end-of-page) status *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_spause(PlplotCanvas* self, PLINT pause); */

/* /\* Set stream number.  *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_sstrm(PlplotCanvas* self, PLINT strm); */

/* Set the number of subwindows in x and y */

PLDLLIMPEXP_GNOME2 void plplot_canvas_ssub(PlplotCanvas* self, PLINT nx, PLINT ny);

/* Set symbol height. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_ssym(PlplotCanvas* self, PLFLT def, PLFLT scale);

/* /\* Initialize PLplot, passing in the windows/page settings. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_star(PlplotCanvas* self, PLINT nx, PLINT ny); */

/* /\* Initialize PLplot, passing the device name and windows/page settings. *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_start(PlplotCanvas* self, const char *devname, PLINT nx, PLINT ny); */

/* /\* Add a point to a stripchart.  *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_stripa(PlplotCanvas* self, PLINT id, PLINT pen, PLFLT x, PLFLT y); */

/* /\* Create 1d stripchart *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_stripc(PlplotCanvas* self, PLINT *id, char *xspec, char *yspec, PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax, PLFLT xlpos, PLFLT ylpos, PLINT y_ascl, PLINT acc, PLINT colbox, PLINT collab, PLINT colline[], PLINT styline[], char *legline[], char *labx, char *laby, char *labtop); */

/* /\* Deletes and releases memory used by a stripchart.  *\/ */

/* PLDLLIMPEXP_GNOME2 void plplot_canvas_stripd(PlplotCanvas* self, PLINT id); */

/* plots a 2d image (or a matrix too large for plshade() ) */

PLDLLIMPEXP_GNOME2 void plplot_canvas_image(PlplotCanvas* self,  PLFLT **data, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

/* Set up a new line style */

PLDLLIMPEXP_GNOME2 void plplot_canvas_styl(PlplotCanvas* self, PLINT nms, PLINT *mark, PLINT *space);

/* Plots the 3d surface representation of the function z[x][y]. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_surf3d(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots the 3d surface representation of the function z[x][y] with y
 * index limits. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_surf3dl(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT*indexymax);

/* Sets the edges of the viewport to the specified absolute coordinates */

PLDLLIMPEXP_GNOME2 void plplot_canvas_svpa(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Set x axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sxax(PlplotCanvas* self, PLINT digmax, PLINT digits);

/* Set inferior X window */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sxwin(PlplotCanvas* self, PLINT window_id);

/* Set y axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_syax(PlplotCanvas* self, PLINT digmax, PLINT digits);

/* Plots array y against x for n points using Hershey symbol "code" */

PLDLLIMPEXP_GNOME2 void plplot_canvas_sym(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Set z axis labeling parameters */

PLDLLIMPEXP_GNOME2 void plplot_canvas_szax(PlplotCanvas* self, PLINT digmax, PLINT digits);

/* Switches to text screen. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_text(PlplotCanvas* self);

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_vasp(PlplotCanvas* self, PLFLT aspect);

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_vpas(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);

/* Creates a viewport with the specified normalized subpage coordinates. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_vpor(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_vsta(PlplotCanvas* self);

/* Set up a window for three-dimensional plotting. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_w3d(PlplotCanvas* self, PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0, PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0, PLFLT zmax0, PLFLT alt, PLFLT az);

/* Set pen width. */

PLDLLIMPEXP_GNOME2 void plplot_canvas_wid(PlplotCanvas* self, PLINT width);

/* Set up world coordinates of the viewport boundaries (2d plots). */

PLDLLIMPEXP_GNOME2 void plplot_canvas_wind(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/*  set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device  */

PLDLLIMPEXP_GNOME2 void plplot_canvas_xormod(PlplotCanvas* self, PLINT mode, PLINT *status);


G_END_DECLS

#endif /* __PLPLOTCANVAS_H__ */
