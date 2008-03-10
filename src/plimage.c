/* plimage()
 *
 * Author: Alessandro Mirone, Nov 2001
 * Adapted: Joao Cardoso
 *
 * Copyright (C) 2004  Alan W. Irwin
 *
 * This file is part of PLplot.
 *
 * PLplot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Library Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PLplot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with PLplot; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "plplotP.h"

/* Get better names, those are too criptic!
 *
 * ZEROW2B: zero writing to buffer ?
 * ZEROW2D: zero writing to display ?
 * ONEW2B: set writing to buffer ?
 * ONEW2D: set writing to display ?
 */

void
NoBufferNoPixmap()
{
  PLINT op = ZEROW2B;

  plsc->plbuf_write = 0; /* TODO: store previous state */
  plP_esc(PLESC_EXPOSE, NULL);
  plP_esc(PLESC_IMAGEOPS, &op);
}

void
RestoreWrite2BufferPixmap()
{
  PLINT op = ONEW2B;

  plsc->plbuf_write = 1; /* TODO: revert from previous state */
  plP_esc(PLESC_IMAGEOPS, &op);
}

void
disabledisplay()
{
  PLINT op = ZEROW2D;

  plP_esc(PLESC_IMAGEOPS, &op);
}

void
enabledisplay()
{
  PLINT op = ONEW2D;

  plP_esc(PLESC_IMAGEOPS, &op);
  plP_esc(PLESC_EXPOSE, NULL);
}



void
plimageslow(short *x, short *y, unsigned short *data, PLINT nx, PLINT ny,
	    PLFLT xmin, PLFLT ymin, PLFLT dx, PLFLT dy,
	    unsigned short zmin, unsigned short zmax)
{
  PLINT ix, iy, i;
  PLFLT xf[4], yf[4];
  short xs[5], ys[5];
  int corners[4];
  unsigned short col;

  for (ix = 0; ix < nx ; ix++) {
    for (iy = 0; iy < ny ; iy++) {

      col = data[ix*ny+iy];
      /* only plot points within zmin/zmax range */
      if (col < zmin || col > zmax)
	continue;

      plcol1(col/(float)USHRT_MAX);

      if (plsc->plbuf_read == 1) {
	/* buffer read, is a replot to a slow device. */

	corners[0] = ix*(ny+1)+iy;       /* [ix][iy] */
	corners[1] = (ix+1)*(ny+1)+iy;   /* [ix+1][iy] */
	corners[2] = (ix+1)*(ny+1)+iy+1; /* [ix+1][iy+1] */
	corners[3] = ix*(ny+1)+iy+1;     /* [ix][iy+1] */

	for (i = 0; i < 4; i++) {
	  xs[i] = x[corners[i]];
	  ys[i] = y[corners[i]];
	}
	xs[4] = xs[0]; ys[4] = ys[0];
	plP_fill(xs, ys, 5);

      } else {

	xf[0] = xf[1] = ix;
	xf[2] = xf[3] = ix+1;
	yf[0] = yf[3] = iy;
	yf[1] = yf[2] = iy+1;

	for (i = 0; i < 4; i++) {
	  xf[i] = xmin + xf[i]*dx;
	  yf[i] = ymin + yf[i]*dy;
	}
	plfill(4, xf, yf);
      }
    }
  }
}

void
grimage(short *x, short *y, unsigned short *z, PLINT nx, PLINT ny)
{
    plsc->dev_ix = x;
    plsc->dev_iy = y;
    plsc->dev_z = z;
    plsc->dev_nptsX = nx;
    plsc->dev_nptsY = ny;

    plP_esc(PLESC_IMAGE, NULL);
}

/*-------------------------------------------------------------------------*\
 * plimagefr
 *
 * arguments are
 *   idata: array containing image data
 *   nx: dimension of the array in the X axis.
 *   ny: dimension of the  array in the Y axis
 *   The array data is indexed like data[ix][iy]
 *
 *   xmin, xmax, ymin, ymax:
 *       data[0][0] corresponds to (xmin,ymin)
 *       data[nx-1][ny-1] to (xmax,ymax)
 *
 *   zmin, zmax:
 *       only data within bounds zmin <= data <= zmax will be
 *       plotted. If zmin == zmax, all data will be ploted.
 *
 *   Dxmin, Dxmax, Dymin, Dymax:
 *       plots only the window of points whose(x,y)'s fall
 *       inside the [Dxmin->Dxmax]X[Dymin->Dymax] window
 *
 *   valuemin, valuemax:
 *       The minimum and maximum values to use for value -> color
 *       mappings.  A value in idata of valuemin or less will have
 *       color 0.0 and a value in idata of valuemax or greater will
 *       have color 1.0.  Values between valuemin and valuemax will
 *       map linearly to to the colors between 0.0 and 1.0.
 *       If you do not want to display values outside of the
 *       (valuemin -> valuemax) range, then set zmin = valuemin and
 *       zmax = valuemax.
 *       This allows for multiple plots to use the same color scale
 *       with a consistent value -> color mapping, regardless of the
 *       image content.
 *
\*-------------------------------------------------------------------------*/

void
c_plimagefr(PLFLT **idata, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax,
         PLFLT valuemin, PLFLT valuemax)
{
  PLINT nnx, nny, ix, iy, ixx, iyy, xm, ym;
  PLFLT dx, dy;
  // Zf holds transformed image pixel values
  // szmin and szmax are zmin and zmax scaled to unsigned short values
  unsigned short *Zf, szmin, szmax;
  short *Xf, *Yf;
  // This is used when looping through the image array, checking to
  // make sure the values are within an acceptable range.
  PLFLT datum;

  if (plsc->level < 3) {
    plabort("plimage: window must be set up first");
    return;
  }

  if (nx <= 0 || ny <= 0) {
    plabort("plimage: nx and ny must be positive");
    return;
  }

  if (Dxmin < xmin || Dxmax > xmax || Dymin < ymin || Dymax > ymax) {
    plabort("plimage: Dxmin or Dxmax or Dymin or Dymax not compatible with xmin or xmax or ymin or ymax.");
    return;
  }

  dx = (xmax - xmin) / (nx - 1);
  dy = (ymax - ymin) / (ny - 1);
  nnx = (Dxmax-Dxmin)/dx + 1;
  nny = (Dymax-Dymin)/dy + 1;

  if ((Zf = (unsigned short *) malloc(nny*nnx*sizeof(unsigned short)))==NULL)
    {
      plexit("plimage: Insufficient memory");
    }

  xm = floor((Dxmin-xmin)/dx); ym = floor((Dymin-ymin)/dy);

  // Go through the image values and scale them to fit in an
  // unsigned short range.
  // Any values greater than valuemax are set to valuemax,
  // and values less than valuemin are set to valuemin.
  ixx=-1;
  for (ix=xm; ix<xm+nnx; ix++) {
    ixx++; iyy=0;
    for (iy=ym; iy<ym+nny; iy++) {
      datum = idata[ix][iy];
      if (datum < valuemin) {
        datum = valuemin;
      }
      else if (datum > valuemax) {
        datum = valuemax;
      }
      Zf[ixx*nny+iyy++] =
        (datum - valuemin) / (valuemax - valuemin) * USHRT_MAX;
    }
  }

  if (zmin == zmax) {
    zmin = valuemin;
    zmax = valuemax;
  }
  else {
    if (zmin < valuemin)
      zmin = valuemin;
    if (zmax > valuemax)
      zmax = valuemax;
  }

  // The value range to plot, scaled to unsigned short values
  szmin = (zmin - valuemin) / (valuemax - valuemin) * USHRT_MAX;
  szmax = (zmax - valuemin) / (valuemax - valuemin) * USHRT_MAX;

  xmin = Dxmin;  xmax = Dxmax;
  ymin = Dymin;  ymax = Dymax;

  /* The X and Y arrays has size nnx*nny */
  nnx++; nny++;

  if (((Xf = (short *) malloc(nny*nnx*sizeof(short)))==NULL)||
      ((Yf = (short *) malloc(nny*nnx*sizeof(short)))==NULL))
      {
        plexit("plimage: Insufficient memory");
      }

  /* adjust the step for the X/Y arrays */
  dx = dx*(nx-1)/nx;
  dy = dy*(ny-1)/ny;

  for (ix = 0; ix < nnx; ix++) {
    for (iy = 0; iy < nny; iy++) {
      Xf[ix*nny+iy] =  plP_wcpcx(xmin + ix*dx);
      Yf[ix*nny+iy] =  plP_wcpcy(ymin + iy*dy);
    }
  }

  plP_image(Xf, Yf, Zf, nnx, nny, xmin, ymin, dx, dy, szmin, szmax);

  free(Xf);
  free(Yf);
  free(Zf);
}

/*-------------------------------------------------------------------------*\
 * plimage
 *           (***** SUBJECT TO CHANGE ******)
 *
 * arguments are
 *   idata: array containing image data
 *   nx: dimension of the array in the X axis.
 *   ny: dimension of the  array in the Y axis
 *   The array data is indexed like data[ix][iy]
 *
 *   xmin, xmax, ymin, ymax:
 *       data[0][0] corresponds to (xmin,ymin)
 *       data[nx-1][ny-1] to (xmax,ymax)
 *
 *   zmin, zmax:
 *       only data within bounds zmin <= data <= zmax will be
 *       plotted. If zmin == zmax, all data will be ploted.
 *
 *   Dxmin, Dxmax, Dymin, Dymax:
 *       plots only the window of points whose(x,y)'s fall
 *       inside the [Dxmin->Dxmax]X[Dymin->Dymax] window
 *
\*-------------------------------------------------------------------------*/

void
c_plimage(PLFLT **idata, PLINT nx, PLINT ny,
	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
	PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax)
{
  PLINT ix, iy;
  PLFLT data_min, data_max, iz;

  // Find the minimum and maximum values in the image, and automatically
  // scale the colors scale over this range.
  data_min = data_max = idata[0][0];

  for (ix = 0; ix < nx; ix++) {
    for (iy = 0; iy < ny; iy++) {
      iz = idata[ix][iy];
      if (data_max < iz)
	data_max = iz;
      if (data_min > iz)
	data_min = iz;
    }
  }

  // Call plimagefr with the value -> color range mapped to the minimum
  // and maximum values in idata.
  plimagefr(idata, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax,
           Dxmin, Dxmax, Dymin, Dymax, data_min, data_max);
}
