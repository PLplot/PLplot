/* plplotcanvas - a GCW API and associated GnomeCanvas widget

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


DESCRIPTION

  This module provides the PlplotCanvas widget which is derived from
  GnomeCanvas (see 
  http://developer.gnome.org/doc/API/2.0/libgnomecanvas/index.html) .

*/

#include "plplotcanvas.h"


/*==========================================================================*/
/* PlplotCanvas API */

static GObjectClass *parent_class = NULL;

void plplot_canvas_dispose(PlplotCanvas *self) {
  parent_class->dispose(G_OBJECT(self));
}

static void plplot_canvas_finalize(PlplotCanvas *self) {

  /* Flush the plplot buffers and close the stream */
  plsstrm(self->Nstream); /* Select stream before plplot call */
  plend1();
  
  /* Call parent's finalize impletmentation */
  parent_class->finalize(G_OBJECT(self));
}

static void plplot_canvas_init(PlplotCanvas *self) {
  char text_buffer[1000];
  PLINT Nstream;

  plP_getinitdriverlist(text_buffer);
  if(text_buffer[0]!='\0') {
    plmkstrm(&Nstream);
    self->Nstream = (gint)Nstream;
  }
  else self->Nstream=0;
}

static void plplot_canvas_class_init(PlplotCanvasClass *klass) {

  parent_class = g_type_class_peek(G_TYPE_OBJECT);
  GObjectClass* gobject_class = G_OBJECT_CLASS(klass);
  gobject_class->dispose = (void*)plplot_canvas_dispose;
  gobject_class->finalize = (void*)plplot_canvas_finalize;

}

void plplot_canvas_devinit(PlplotCanvas *self) {

  plscol0(0,255,255,255); /* Change the plplot background color to white */
  plscol0(15,0,0,0);

  /* Use the hack variable to tell the driver to expect a PLESC_DEVINIT
   * escape call to finish the driver initialization
   */
  plsc->hack = 1;

  plsdev("gcw"); /* Set the device */
  plinit(); /* Initialize the device */
  plP_esc(PLESC_DEVINIT,(void*)self); /* Install into the driver */
}

GType plplot_canvas_get_type() {

  static GType this_type = 0;

  if (!this_type) {

    static const GTypeInfo this_info = {
      sizeof (PlplotCanvasClass),
      NULL,
      NULL,
      (GClassInitFunc) plplot_canvas_class_init,
      NULL,
      NULL,
      sizeof(PlplotCanvas),
      0,
      (GInstanceInitFunc) plplot_canvas_init,
    };

    this_type = g_type_register_static(GNOME_TYPE_CANVAS, "PlplotCanvas", 
				       &this_info, 0);    
  }
  return this_type;
}

PlplotCanvas* plplot_canvas_new() {
  
  PlplotCanvas *canvas;

  canvas = PLPLOT_CANVAS(g_object_new(PLPLOT_TYPE_CANVAS,"aa",TRUE,NULL));

  plplot_canvas_devinit(canvas);

  return canvas;
}

gint plplot_canvas_get_stream_number(PlplotCanvas* self)
{
  return self->Nstream;
}

void plplot_canvas_set_size(PlplotCanvas* self,gint width,gint height)
{
  plsstrm(self->Nstream); /* Select stream before plplot call */
  gcw_set_canvas_size(GNOME_CANVAS(self),(PLINT)width,(PLINT)height);
}

void plplot_canvas_set_zoom(PlplotCanvas* self,gdouble zoom)
{
  plsstrm(self->Nstream); /* Select stream before plplot call */
  gcw_set_canvas_zoom(GNOME_CANVAS(self),(PLFLT)zoom);
}

void plplot_canvas_use_text(PlplotCanvas* self,gboolean use_text)
{
  plsstrm(self->Nstream); /* Select stream before plplot call */
  gcw_use_text((PLINT)use_text);
}

void plplot_canvas_use_pixmap(PlplotCanvas* self,gboolean use_pixmap)
{
  plsstrm(self->Nstream); /* Select stream before plplot call */
  gcw_use_pixmap((PLINT)use_pixmap);
}

void plplot_canvas_use_persistence(PlplotCanvas* self,gboolean use_persistence)
{
  plsstrm(self->Nstream); /* Select stream before plplot call */
  gcw_use_persistence((PLINT)use_persistence);
}


/*--------------------------------------------------------------------------*
 * Plplot wrappers - last updated 29 January 2005
 *
 *  This should be kept up-to-date with plplot.h
 *
/*--------------------------------------------------------------------------*/

/* set the format of the contour labels */

void plplot_canvas_setcontlabelformat(PlplotCanvas* self, PLINT lexp, PLINT sigdig) {
  plsstrm(self->Nstream); 
  pl_setcontlabelformat(lexp, sigdig);
}

/* set offset and spacing of contour labels */

void plplot_canvas_setcontlabelparam(PlplotCanvas* self, PLFLT offset, PLFLT size, PLFLT spacing, PLINT active) {
  plsstrm(self->Nstream); 
  pl_setcontlabelparam(offset, size, spacing, active);
}

/* Advance to subpage "page", or to the next one if "page" = 0. */

void plplot_canvas_adv(PlplotCanvas* self, PLINT page) {
  plsstrm(self->Nstream); 
  pladv(page);
}

/* simple arrow plotter. */

void plplot_canvas_arrows(PlplotCanvas* self, PLFLT *u, PLFLT *v, PLFLT *x, PLFLT *y, PLINT n, PLFLT scale, PLFLT dx, PLFLT dy) {
  plsstrm(self->Nstream); 
  plarrows(u, v, x, y, n, scale, dx, dy) ;
}

void plplot_canvas_vect(PlplotCanvas* self, PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plvect(u, v, nx, ny, scale, pltr, pltr_data);
}

void plplot_canvas_svect(PlplotCanvas* self, PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLINT fill) {
  plsstrm(self->Nstream); 
  plsvect(arrowx, arrowy, npts, fill);
}

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */

void plplot_canvas_axes(PlplotCanvas* self, PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub, const char *yopt, PLFLT ytick, PLINT nysub) {
  plsstrm(self->Nstream); 
  plaxes(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub);
}

/* Plot a histogram using x to store data values and y to store frequencies */

void plplot_canvas_bin(PlplotCanvas* self, PLINT nbin, PLFLT *x, PLFLT *y, PLINT center) {
  plsstrm(self->Nstream); 
  plbin(nbin, x, y, center);
}

/* Start new page.  Should only be used with pleop(). */

void plplot_canvas_bop(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  plbop();
}

/* This draws a box around the current viewport. */

void plplot_canvas_box(PlplotCanvas* self, const char *xopt, PLFLT xtick, PLINT nxsub, const char *yopt, PLFLT ytick, PLINT nysub) {
  plsstrm(self->Nstream); 
  plbox(xopt, xtick, nxsub, yopt, ytick, nysub);
}

/* This is the 3-d analogue of plbox(). */

void plplot_canvas_box3(PlplotCanvas* self, const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx, const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby, const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz) {
  plsstrm(self->Nstream); 
  plbox3(xopt, xlabel, xtick, nsubx, yopt, ylabel, ytick, nsuby, zopt, zlabel, ztick, nsubz);
}

/* Calculate world coordinates and subpage from relative device coordinates. */

void plplot_canvas_calc_world(PlplotCanvas* self, PLFLT rx, PLFLT ry, PLFLT *wx, PLFLT *wy, PLINT *window) {
  plsstrm(self->Nstream); 
  plcalc_world(rx, ry, wx, wy, window);
}

/* Clear current subpage. */

void plplot_canvas_clear(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  plclear();
}

/* Set color, map 0.  Argument is integer between 0 and 15. */

void plplot_canvas_col0(PlplotCanvas* self, PLINT icol0) {
  plsstrm(self->Nstream); 
  plcol0(icol0);
}

/* Set color, map 1.  Argument is a float between 0. and 1. */

void plplot_canvas_col1(PlplotCanvas* self, PLFLT col1) {
  plsstrm(self->Nstream); 
  plcol1(col1);
}

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data.
 */

void plplot_canvas_cont(PlplotCanvas* self, PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plcont(f, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr, pltr_data);
}

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 */

void plplot_canvas_fcont(PlplotCanvas* self, PLFLT (*f2eval) (PLINT, PLINT, PLPointer), PLPointer f2eval_data, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plfcont(f2eval, f2eval_data, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr, pltr_data);
}

/* /\* Copies state parameters from the reference stream to the current stream. *\/ */

/* void plplot_canvas_cpstrm(PlplotCanvas* self, PLINT iplsr, PLINT flags) { */
/*   plsstrm(self->Nstream);  */
/*   plcpstrm(iplsr, flags); */
/* } */

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

void plplot_canvas_did2pc(PlplotCanvas* self, PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax) {
  plsstrm(self->Nstream); 
  pldid2pc(xmin, ymin, xmax, ymax);
}

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

void plplot_canvas_dip2dc(PlplotCanvas* self, PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax) {
  plsstrm(self->Nstream); 
  pldip2dc(xmin, ymin, xmax, ymax);
}

/* /\* End a plotting session for all open streams. *\/ */

/* void plplot_canvas_end(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plend(); */
/* } */

/* /\* End a plotting session for the current stream only. *\/ */

/* void plplot_canvas_end1(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plend1(); */
/* } */

/* /\* Simple interface for defining viewport and window. *\/ */

/* void plplot_canvas_env(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis) { */
/*   plsstrm(self->Nstream);  */
/*   plenv(xmin, xmax, ymin, ymax, just, axis); */
/* } */

/* /\* similar to plenv() above, but in multiplot mode does not advance the subpage, */
/*  instead the current subpage is cleared *\/ */

/* void plplot_canvas_env0(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis) { */
/*   plsstrm(self->Nstream);  */
/*   plenv0(xmin, xmax, ymin, ymax, just, axis); */
/* } */

/* End current page.  Should only be used with plbop(). */

void plplot_canvas_eop(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  pleop();
}

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

void plplot_canvas_errx(PlplotCanvas* self, PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y) {
  plsstrm(self->Nstream); 
  plerrx(n, xmin, xmax, y);
}

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

void plplot_canvas_erry(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax) {
  plsstrm(self->Nstream); 
  plerry(n, x, ymin, ymax);
}

/* /\* Advance to the next family file on the next new page *\/ */

/* void plplot_canvas_famadv(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plfamadv(); */
/* } */

/* Pattern fills the polygon bounded by the input points. */

void plplot_canvas_fill(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y) {
  plsstrm(self->Nstream); 
  plfill(n, x, y);
}

/* Pattern fills the 3d polygon bounded by the input points. */

void plplot_canvas_fill3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z) {
  plsstrm(self->Nstream); 
  plfill3(n, x, y, z);
}

/* /\* Flushes the output stream.  Use sparingly, if at all. *\/ */

/* void plplot_canvas_flush(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plflush(); */
/* } */

/* Sets the global font flag to 'ifont'. */

void plplot_canvas_font(PlplotCanvas* self, PLINT ifont) {
  plsstrm(self->Nstream); 
  plfont(ifont);
}

/* Load specified font set. */

void plplot_canvas_fontld(PlplotCanvas* self, PLINT fnt) {
  plsstrm(self->Nstream); 
  plfontld(fnt);
}

/* Get character default height and current (scaled) height */

void plplot_canvas_gchr(PlplotCanvas* self, PLFLT *p_def, PLFLT *p_ht) {
  plsstrm(self->Nstream); 
  plgchr(p_def, p_ht);
}

/* Returns 8 bit RGB values for given color from color map 0 */

void plplot_canvas_gcol0(PlplotCanvas* self, PLINT icol0, PLINT *r, PLINT *g, PLINT *b) {
  plsstrm(self->Nstream); 
  plgcol0(icol0, r, g, b);
}

/* Returns the background color by 8 bit RGB value */

void plplot_canvas_gcolbg(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b) {
  plsstrm(self->Nstream); 
  plgcolbg(r, g, b);
}

/* Returns the current compression setting */

void plplot_canvas_gcompression(PlplotCanvas* self, PLINT *compression) {
  plsstrm(self->Nstream); 
  plgcompression(compression);
}

/* Get the current device (keyword) name */

void plplot_canvas_gdev(PlplotCanvas* self, char *p_dev) {
  plsstrm(self->Nstream); 
  plgdev(p_dev);
}

/* Retrieve current window into device space */

void plplot_canvas_gdidev(PlplotCanvas* self, PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy) {
  plsstrm(self->Nstream); 
  plgdidev(p_mar, p_aspect, p_jx, p_jy);
}

/* Get plot orientation */

void plplot_canvas_gdiori(PlplotCanvas* self, PLFLT *p_rot) {
  plsstrm(self->Nstream); 
  plgdiori(p_rot);
}

/* Retrieve current window into plot space */

void plplot_canvas_gdiplt(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax) {
  plsstrm(self->Nstream); 
  plgdiplt(p_xmin, p_ymin, p_xmax, p_ymax);
}

/* Get FCI (font characterization integer) */

void plplot_canvas_gfci(PlplotCanvas* self, PLUNICODE *pfci) {
  plsstrm(self->Nstream); 
  plgfci(pfci);
}

/* /\* Get family file parameters *\/ */

/* void plplot_canvas_gfam(PlplotCanvas* self, PLINT *p_fam, PLINT *p_num, PLINT *p_bmax) { */
/*   plsstrm(self->Nstream);  */
/*   plgfam(p_fam, p_num, p_bmax); */
/* } */

/* Get the (current) output file name.  Must be preallocated to >80 bytes */

void plplot_canvas_gfnam(PlplotCanvas* self, char *fnam) {
  plsstrm(self->Nstream); 
  plgfnam(fnam);
}

/* Get the (current) run level.  */

void plplot_canvas_glevel(PlplotCanvas* self, PLINT *p_level) {
  plsstrm(self->Nstream); 
  plglevel(p_level);
}

/* Get output device parameters. */

void plplot_canvas_gpage(PlplotCanvas* self, PLFLT *p_xp, PLFLT *p_yp, PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff) {
  plsstrm(self->Nstream); 
  plgpage(p_xp, p_yp, p_xleng, p_yleng, p_xoff, p_yoff);
}

/* /\* Switches to graphics screen. *\/ */

/* void plplot_canvas_gra(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plgra(); */
/* } */

/* grid irregularly sampled data */

void plplot_canvas_griddata(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT *z, PLINT npts, PLFLT *xg, PLINT nptsx, PLFLT *yg, PLINT nptsy, PLFLT **zg, PLINT type, PLFLT data) {
  plsstrm(self->Nstream); 
  plgriddata(x, y, z, npts, xg, nptsx, yg, nptsy, zg, type, data);
}

/* Get subpage boundaries in absolute coordinates */

void plplot_canvas_gspa(PlplotCanvas* self, PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax) {
  plsstrm(self->Nstream); 
  plgspa(xmin, xmax, ymin, ymax);
}

/* /\* Get current stream number. *\/ */

/* void plplot_canvas_gstrm(PlplotCanvas* self, PLINT *p_strm) { */
/*   plsstrm(self->Nstream);  */
/*   plgstrm(p_strm); */
/* } */

/* Get the current library version number */

void plplot_canvas_gver(PlplotCanvas* self, char *p_ver) {
  plsstrm(self->Nstream); 
  plgver(p_ver);
}

/* Get viewport boundaries in normalized device coordinates */

void plplot_canvas_gvpd(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax) {
  plsstrm(self->Nstream); 
  plgvpd(p_xmin, p_xmax, p_ymin, p_ymax);
}

/* Get viewport boundaries in world coordinates */

void plplot_canvas_gvpw(PlplotCanvas* self, PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax) {
  plsstrm(self->Nstream); 
  plgvpw(p_xmin, p_xmax, p_ymin, p_ymax);
}

/* Get x axis labeling parameters */

void plplot_canvas_gxax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits) {
  plsstrm(self->Nstream); 
  plgxax(p_digmax, p_digits);
}

/* Get y axis labeling parameters */

void plplot_canvas_gyax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits) {
  plsstrm(self->Nstream); 
  plgyax(p_digmax, p_digits);
}

/* Get z axis labeling parameters */

void plplot_canvas_gzax(PlplotCanvas* self, PLINT *p_digmax, PLINT *p_digits) {
  plsstrm(self->Nstream); 
  plgzax(p_digmax, p_digits);
}

/* Draws a histogram of n values of a variable in array data[0..n-1] */

void plplot_canvas_hist(PlplotCanvas* self, PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax, PLINT nbin, PLINT oldwin) {
  plsstrm(self->Nstream); 
  plhist(n, data, datmin, datmax, nbin, oldwin);
}

/* Set current color (map 0) by hue, lightness, and saturation. */

void plplot_canvas_hls(PlplotCanvas* self, PLFLT h, PLFLT l, PLFLT s) {
  plsstrm(self->Nstream); 
  plhls(h, l, s);
}

/* /\* Initializes PLplot, using preset or default options *\/ */

/* void plplot_canvas_plinit(PlplotCanvas* self) { */
/*   plsstrm(self->Nstream);  */
/*   plinit(); */
/* } */

/* Draws a line segment from (x1, y1) to (x2, y2). */

void plplot_canvas_join(PlplotCanvas* self, PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2) {
  plsstrm(self->Nstream); 
  pljoin(x1, y1, x2, y2);
}

/* Simple routine for labelling graphs. */

void plplot_canvas_lab(PlplotCanvas* self, const char *xlabel, const char *ylabel, const char *tlabel) {
  plsstrm(self->Nstream); 
  pllab(xlabel, ylabel, tlabel);
}

/* Sets position of the light source */

void plplot_canvas_lightsource(PlplotCanvas* self, PLFLT x, PLFLT y, PLFLT z) {
  plsstrm(self->Nstream); 
  pllightsource(x, y, z);
}

/* Draws line segments connecting a series of points. */

void plplot_canvas_line(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y) {
  plsstrm(self->Nstream); 
  plline(n, x, y);
}

/* Draws a line in 3 space.  */

void plplot_canvas_line3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z) {
  plsstrm(self->Nstream); 
  plline3(n, x, y, z);
}

/* Set line style. */

void plplot_canvas_lsty(PlplotCanvas* self, PLINT lin) {
  plsstrm(self->Nstream); 
  pllsty(lin);
}

/* plot continental outline in world coordinates */

void plplot_canvas_map(PlplotCanvas* self,  void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat) {
  plsstrm(self->Nstream); 
  plmap(mapform, type, minlong, maxlong, minlat, maxlat);
}

/* Plot the latitudes and longitudes on the background. */

void plplot_canvas_meridians(PlplotCanvas* self, void (*mapform)(PLINT, PLFLT *, PLFLT *), PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat) {
  plsstrm(self->Nstream); 
  plmeridians(mapform, dlong, dlat, minlong, maxlong, minlat, maxlat );
}

/* Plots a mesh representation of the function z[x][y]. */

void plplot_canvas_mesh(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt) {
  plsstrm(self->Nstream); 
  plmesh(x, y, z, nx, ny, opt);
}

/* Plots a mesh representation of the function z[x][y] with contour */

void plplot_canvas_meshc(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel) {
  plsstrm(self->Nstream); 
  plmeshc(x, y, z, nx, ny, opt, clevel, nlevel);
}

/* /\* Creates a new stream and makes it the default.  *\/ */

/* void plplot_canvas_plmkstrm(PlplotCanvas* self, PLINT *p_strm) { */
/*   plsstrm(self->Nstream);  */
/*   plmkstrm(p_strm); */
/* } */

/* Prints out "text" at specified position relative to viewport */

void plplot_canvas_mtex(PlplotCanvas* self, const char *side, PLFLT disp, PLFLT pos, PLFLT just, const char *text) {
  plsstrm(self->Nstream); 
  plmtex(side, disp, pos, just, text);
}

/* Plots a 3-d representation of the function z[x][y]. */

void plplot_canvas_plot3d(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLINT side) {
  plsstrm(self->Nstream); 
  plot3d(x, y, z, nx, ny, opt, side);
}

/* Plots a 3-d representation of the function z[x][y] with contour. */

void plplot_canvas_plot3dc(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel) {
  plsstrm(self->Nstream); 
  plot3dc(x, y, z, nx, ny, opt, clevel, nlevel);
}

/* Plots a 3-d representation of the function z[x][y] with contour and
 * y index limits. */

void plplot_canvas_plot3dcl(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT*indexymax) {
  plsstrm(self->Nstream); 
  plot3dcl(x, y, z, nx, ny, opt, clevel, nlevel, ixstart, ixn, indexymin, indexymax);
}

/* Set fill pattern directly. */

void plplot_canvas_pat(PlplotCanvas* self, PLINT nlin, PLINT *inc, PLINT *del) {
  plsstrm(self->Nstream); 
  plpat(nlin, inc, del);
}

/* Plots array y against x for n points using ASCII code "code".*/

void plplot_canvas_poin(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLINT code) {
  plsstrm(self->Nstream); 
  plpoin(n, x, y, code);
}

/* Draws a series of points in 3 space. */

void plplot_canvas_poin3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code) {
  plsstrm(self->Nstream); 
  plpoin3(n, x, y, z, code);
}

/* Draws a polygon in 3 space.  */

void plplot_canvas_poly3(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *draw, PLINT ifcc) {
  plsstrm(self->Nstream); 
  plpoly3(n, x, y, z, draw, ifcc);
}

/* Set the floating point precision (in number of places) in numeric labels. */

void plplot_canvas_prec(PlplotCanvas* self, PLINT setp, PLINT prec) {
  plsstrm(self->Nstream); 
  plprec(setp, prec);
}

/* Set fill pattern, using one of the predefined patterns.*/

void plplot_canvas_psty(PlplotCanvas* self, PLINT patt) {
  plsstrm(self->Nstream); 
  plpsty(patt);
}

/* Prints out "text" at world cooordinate (x,y). */

void plplot_canvas_ptex(PlplotCanvas* self, PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text) {
  plsstrm(self->Nstream); 
  plptex(x, y, dx, dy, just, text);
}

/* Replays contents of plot buffer to current device/file. */

void plplot_canvas_replot(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  plreplot();
}

/* Set line color by red, green, blue from  0. to 1. */

void plplot_canvas_rgb(PlplotCanvas* self, PLFLT r, PLFLT g, PLFLT b) {
  plsstrm(self->Nstream); 
  plrgb(r, g, b);
}

/* Set line color by 8 bit RGB values. */

void plplot_canvas_rgb1(PlplotCanvas* self, PLINT r, PLINT g, PLINT b) {
  plsstrm(self->Nstream); 
  plrgb1(r, g, b);
}

/* Set character height. */

void plplot_canvas_schr(PlplotCanvas* self, PLFLT def, PLFLT scale) {
  plsstrm(self->Nstream); 
  plschr(def, scale);
}

/* Set color map 0 colors by 8 bit RGB values */

void plplot_canvas_scmap0(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b, PLINT ncol0) {
  plsstrm(self->Nstream); 
  plscmap0(r, g, b, ncol0);
}

/* Set number of colors in cmap 0 */

void plplot_canvas_scmap0n(PlplotCanvas* self, PLINT ncol0) {
  plsstrm(self->Nstream); 
  plscmap0n(ncol0);
}

/* Set color map 1 colors by 8 bit RGB values */

void plplot_canvas_scmap1(PlplotCanvas* self, PLINT *r, PLINT *g, PLINT *b, PLINT ncol1) {
  plsstrm(self->Nstream); 
  plscmap1(r, g, b, ncol1);
}

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

void plplot_canvas_scmap1l(PlplotCanvas* self, PLINT itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLINT *rev) {
  plsstrm(self->Nstream); 
  plscmap1l(itype, npts, intensity, coord1, coord2, coord3, rev);
}

/* Set number of colors in cmap 1 */

void plplot_canvas_scmap1n(PlplotCanvas* self, PLINT ncol1) {
  plsstrm(self->Nstream); 
  plscmap1n(ncol1);
}

/* Set a given color from color map 0 by 8 bit RGB value */

void plplot_canvas_scol0(PlplotCanvas* self, PLINT icol0, PLINT r, PLINT g, PLINT b) {
  plsstrm(self->Nstream); 
  plscol0(icol0, r, g, b);
}

/* Set the background color by 8 bit RGB value */

void plplot_canvas_scolbg(PlplotCanvas* self, PLINT r, PLINT g, PLINT b) {
  plsstrm(self->Nstream); 
  plscolbg(r, g, b);
}

/* Used to globally turn color output on/off */

void plplot_canvas_scolor(PlplotCanvas* self, PLINT color) {
  plsstrm(self->Nstream); 
  plscolor(color);
}

/* Set the compression level */

void plplot_canvas_scompression(PlplotCanvas* self, PLINT compression) {
  plsstrm(self->Nstream); 
  plscompression(compression);
}

/* /\* Set the device (keyword) name *\/ */

/* void plplot_canvas_plsdev(PlplotCanvas* self, const char *devname) { */
/*   plsstrm(self->Nstream);  */
/*   plsdev(devname); */
/* } */

/* Set window into device space using margin, aspect ratio, and */
/* justification */

void plplot_canvas_sdidev(PlplotCanvas* self, PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy) {
  plsstrm(self->Nstream); 
  plsdidev(mar, aspect, jx, jy);
}

/* Set up transformation from metafile coordinates. */

void plplot_canvas_sdimap(PlplotCanvas* self, PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax, PLFLT dimxpmm, PLFLT dimypmm) {
  plsstrm(self->Nstream); 
  plsdimap(dimxmin, dimxmax, dimymin, dimymax, dimxpmm, dimypmm);
}

/* Set plot orientation, specifying rotation in units of pi/2. */

void plplot_canvas_sdiori(PlplotCanvas* self, PLFLT rot) {
  plsstrm(self->Nstream); 
  plsdiori(rot);
}

/* Set window into plot space */

void plplot_canvas_sdiplt(PlplotCanvas* self, PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax) {
  plsstrm(self->Nstream); 
  plsdiplt(xmin, ymin, xmax, ymax);
}

/* Set window into plot space incrementally (zoom) */

void plplot_canvas_sdiplz(PlplotCanvas* self, PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax) {
  plsstrm(self->Nstream); 
  plsdiplz(xmin, ymin, xmax, ymax);
}

/* Set the escape character for text strings. */

void plplot_canvas_sesc(PlplotCanvas* self, char esc) {
  plsstrm(self->Nstream); 
  plsesc(esc);
}

/* /\* Set family file parameters *\/ */

/* void plplot_canvas_sfam(PlplotCanvas* self, PLINT fam, PLINT num, PLINT bmax) { */
/*   plsstrm(self->Nstream);  */
/*   plsfam(fam, num, bmax); */
/* } */

/* Set FCI (font characterization integer) */

void plplot_canvas_sfci(PlplotCanvas* self, PLUNICODE fci) {
  plsstrm(self->Nstream); 
  plsfci(fci);
}

/* Set the output file name. */

void plplot_canvas_sfnam(PlplotCanvas* self, const char *fnam) {
  plsstrm(self->Nstream); 
  plsfnam(fnam);
}

/* Shade region. */

void plplot_canvas_shade(PlplotCanvas* self, PLFLT **a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plshade(a, nx, ny, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, fill, rectangular, pltr, pltr_data);
}

void plplot_canvas_shade1(PlplotCanvas* self, PLFLT *a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plshade1(a, nx, ny, defined, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, fill, rectangular, pltr, pltr_data);
}

void plplot_canvas_shades(PlplotCanvas* self, PLFLT **a, PLINT nx, PLINT ny, PLINT (*defined) (PLFLT, PLFLT), PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT *clevel, PLINT nlevel, PLINT fill_width, PLINT cont_color, PLINT cont_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plshades(a, nx, ny, defined, xmin, xmax, ymin, ymax, clevel, nlevel, fill_width, cont_color, cont_width, fill, rectangular, pltr, pltr_data);
}

void plplot_canvas_fshade(PlplotCanvas* self, PLFLT (*f2eval) (PLINT, PLINT, PLPointer), PLPointer f2eval_data, PLFLT (*c2eval) (PLINT, PLINT, PLPointer), PLPointer c2eval_data, PLINT nx, PLINT ny, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular, void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer), PLPointer pltr_data) {
  plsstrm(self->Nstream); 
  plfshade(f2eval, f2eval_data, c2eval, c2eval_data, nx, ny, left, right, bottom, top, shade_min, shade_max, sh_cmap, sh_color, sh_width, min_color, min_width, max_color, max_width, fill, rectangular, pltr, pltr_data);
}

/* Set up lengths of major tick marks. */

void plplot_canvas_smaj(PlplotCanvas* self, PLFLT def, PLFLT scale) {
  plsstrm(self->Nstream); 
  plsmaj(def, scale);
}

/* /\* Set the memory area to be plotted (with the 'mem' driver) *\/ */

/* void plplot_canvas_smem(PlplotCanvas* self, PLINT maxx, PLINT maxy, void *plotmem) { */
/*   plsstrm(self->Nstream);  */
/*   plsmem(maxx, maxy, plotmem); */
/* } */

/* Set up lengths of minor tick marks. */

void plplot_canvas_smin(PlplotCanvas* self, PLFLT def, PLFLT scale) {
  plsstrm(self->Nstream); 
  plsmin(def, scale);
}

/* /\* Set orientation.  Must be done before calling plinit. *\/ */

/* void plplot_canvas_sori(PlplotCanvas* self, PLINT ori) { */
/*   plsstrm(self->Nstream);  */
/*   plsori(ori); */
/* } */

/* Set output device parameters.  Usually ignored by the driver. */

void plplot_canvas_spage(PlplotCanvas* self, PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff) {
  plsstrm(self->Nstream); 
  plspage(xp, yp, xleng, yleng, xoff, yoff);
}

/* /\* Set the pause (on end-of-page) status *\/ */

/* void plplot_canvas_spause(PlplotCanvas* self, PLINT pause) { */
/*   plsstrm(self->Nstream);  */
/*   plspause(pause); */
/* } */

/* /\* Set stream number.  *\/ */

/* void plplot_canvas_sstrm(PlplotCanvas* self, PLINT strm) { */
/*   plsstrm(self->Nstream);  */
/*   plsstrm(strm); */
/* } */

/* Set the number of subwindows in x and y */

void plplot_canvas_ssub(PlplotCanvas* self, PLINT nx, PLINT ny) {
  plsstrm(self->Nstream); 
  plssub(nx, ny);
}

/* Set symbol height. */

void plplot_canvas_ssym(PlplotCanvas* self, PLFLT def, PLFLT scale) {
  plsstrm(self->Nstream); 
  plssym(def, scale);
}

/* /\* Initialize PLplot, passing in the windows/page settings. *\/ */

/* void plplot_canvas_star(PlplotCanvas* self, PLINT nx, PLINT ny) { */
/*   plsstrm(self->Nstream);  */
/*   plstar(nx, ny); */
/* } */

/* /\* Initialize PLplot, passing the device name and windows/page settings. *\/ */

/* void plplot_canvas_start(PlplotCanvas* self, const char *devname, PLINT nx, PLINT ny) { */
/*   plsstrm(self->Nstream);  */
/*   plstart(devname, nx, ny); */
/* } */

/* /\* Add a point to a stripchart.  *\/ */

/* void plplot_canvas_stripa(PlplotCanvas* self, PLINT id, PLINT pen, PLFLT x, PLFLT y) { */
/*   plsstrm(self->Nstream);  */
/*   plstripa(id, pen, x, y); */
/* } */

/* /\* Create 1d stripchart *\/ */

/* void plplot_canvas_stripc(PlplotCanvas* self, PLINT *id, char *xspec, char *yspec, PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax, PLFLT xlpos, PLFLT ylpos, PLINT y_ascl, PLINT acc, PLINT colbox, PLINT collab, PLINT colline[], PLINT styline[], char *legline[], char *labx, char *laby, char *labtop) { */
/*   plsstrm(self->Nstream);  */
/*   plstripc(id, xspec, yspec, xmin, xmax, xjump, ymin, ymax, xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline, labx, laby, labtop); */
/* } */

/* /\* Deletes and releases memory used by a stripchart.  *\/ */

/* void plplot_canvas_stripd(PlplotCanvas* self, PLINT id) { */
/*   plsstrm(self->Nstream);  */
/*   plstripd(id); */
/* } */

/* plots a 2d image (or a matrix too large for plshade() ) */

void plplot_canvas_image(PlplotCanvas* self,  PLFLT **data, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax) {
  plsstrm(self->Nstream); 
  plimage(data, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, Dxmin, Dxmax, Dymin, Dymax);
}

/* Set up a new line style */

void plplot_canvas_styl(PlplotCanvas* self, PLINT nms, PLINT *mark, PLINT *space) {
  plsstrm(self->Nstream); 
  plstyl(nms, mark, space);
}

/* Plots the 3d surface representation of the function z[x][y]. */

void plplot_canvas_surf3d(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel) {
  plsstrm(self->Nstream); 
  plsurf3d(x, y, z, nx, ny, opt, clevel, nlevel);
}

/* Plots the 3d surface representation of the function z[x][y] with y
 * index limits. */

void plplot_canvas_surf3dl(PlplotCanvas* self, PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT*indexymax) {
  plsstrm(self->Nstream); 
  plsurf3dl(x, y, z, nx, ny, opt, clevel, nlevel, ixstart, ixn, indexymin, indexymax);
}

/* Sets the edges of the viewport to the specified absolute coordinates */

void plplot_canvas_svpa(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax) {
  plsstrm(self->Nstream); 
  plsvpa(xmin, xmax, ymin, ymax);
}

/* Set x axis labeling parameters */

void plplot_canvas_sxax(PlplotCanvas* self, PLINT digmax, PLINT digits) {
  plsstrm(self->Nstream); 
  plsxax(digmax, digits);
}

/* Set inferior X window */

void plplot_canvas_sxwin(PlplotCanvas* self, PLINT window_id) {
  plsstrm(self->Nstream); 
  plsxwin(window_id);
}

/* Set y axis labeling parameters */

void plplot_canvas_syax(PlplotCanvas* self, PLINT digmax, PLINT digits) {
  plsstrm(self->Nstream); 
  plsyax(digmax, digits);
}

/* Plots array y against x for n points using Hershey symbol "code" */

void plplot_canvas_sym(PlplotCanvas* self, PLINT n, PLFLT *x, PLFLT *y, PLINT code) {
  plsstrm(self->Nstream); 
  plsym(n, x, y, code);
}

/* Set z axis labeling parameters */

void plplot_canvas_szax(PlplotCanvas* self, PLINT digmax, PLINT digits) {
  plsstrm(self->Nstream); 
  plszax(digmax, digits);
}

/* Switches to text screen. */

void plplot_canvas_text(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  pltext();
}

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

void plplot_canvas_vasp(PlplotCanvas* self, PLFLT aspect) {
  plsstrm(self->Nstream); 
  plvasp(aspect);
}

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

void plplot_canvas_vpas(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect) {
  plsstrm(self->Nstream); 
  plvpas(xmin, xmax, ymin, ymax, aspect);
}

/* Creates a viewport with the specified normalized subpage coordinates. */

void plplot_canvas_vpor(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax) {
  plsstrm(self->Nstream); 
  plvpor(xmin, xmax, ymin, ymax);
}

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

void plplot_canvas_vsta(PlplotCanvas* self) {
  plsstrm(self->Nstream); 
  plvsta();
}

/* Set up a window for three-dimensional plotting. */

void plplot_canvas_w3d(PlplotCanvas* self, PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0, PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0, PLFLT zmax0, PLFLT alt, PLFLT az) {
  plsstrm(self->Nstream); 
  plw3d(basex, basey, height, xmin0, xmax0, ymin0, ymax0, zmin0, zmax0, alt, az);
}

/* Set pen width. */

void plplot_canvas_wid(PlplotCanvas* self, PLINT width) {
  plsstrm(self->Nstream); 
  plwid(width);
}

/* Set up world coordinates of the viewport boundaries (2d plots). */

void plplot_canvas_wind(PlplotCanvas* self, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax) {
  plsstrm(self->Nstream); 
  plwind(xmin, xmax, ymin, ymax);
}

/*  set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device  */

void plplot_canvas_xormod(PlplotCanvas* self, PLINT mode, PLINT *status) {
  plsstrm(self->Nstream); 
  plxormod(mode, status);
}

