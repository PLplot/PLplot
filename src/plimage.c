/* plimage() 
 *
 * Author: Alessandro Mirone, Nov 2001
 * Adapted: Joao Cardoso
 */

#include "plplot/plplotP.h"

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
plimageslow(PLFLT *data, PLINT nx, PLINT ny, 
	    PLFLT xmin, PLFLT ymin, PLFLT dx, PLFLT dy,
	    PLFLT zmin, PLFLT zmax)
{

  PLINT ix, iy, i;
  PLFLT x[4], y[4];
  PLFLT xm, ym;

  for (ix = 0; ix < nx ; ix++) {
    for (iy = 0; iy < ny ; iy++) {

      plcol1((data[ix*ny+iy]-zmin) / (zmax-zmin));

      xm = xmin + ix*dx;
      ym = ymin + iy*dy;

      x[0] = x[1] = ix;
      x[2] = x[3] = ix+1;
      y[0] = y[3] = iy;
      y[1] = y[2] = iy+1;
      for (i = 0; i < 4; i++) {
	x[i] = xmin + x[i]*dx;
	y[i] = ymin + y[i]*dy;
      }
      plfill(4, x, y);
    }
  }
}

void
grimage(PLINT *x, PLINT *y, PLFLT *z, PLINT nx, PLINT ny)
{
    plsc->dev_ix = x;
    plsc->dev_iy = y;
    plsc->dev_z = z;
    plsc->dev_nptsX = nx;
    plsc->dev_nptsY = ny;

    plP_esc(PLESC_IMAGE, NULL);
}

/*-------------------------------------------------------------------------*\
 * plimage
 *           (***** SUBJECT TO CHANGE ******)
 *
 * arguments are
 *   data: array containing image data
 *   nx: dimension of the array in the X axis.        
 *   ny: dimension of the  array in the Y axis 
 *   The array data is indexed like data[ix][iy]
 *
 *   xmin, xmax, ymin, ymax:
 *       data[0][0] corresponds to (xmin,ymin)
 *       data[nx-1][ny-1] to (xmax,ymax)
 *
 *   Dxmin, Dxmax, Dymin, Dymax:
 *       plots only the window of points whose(x,y)'s fall
 *       inside the [Dxmin->Dxmax]X[Dymin->Dymax] window
 *
\*-------------------------------------------------------------------------*/

void
plimage(PLFLT **idata, PLINT nx, PLINT ny, 
	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax)
{
  PLINT nnx, nny, ix, iy, j;
  PLFLT dx, dy, xm, ym;
  PLINT *Xf, *Yf;
  PLFLT *Zf, *data, zmin, zmax;
  
  if (plsc->level < 3) {
    plabort("plimage: window must be set up first");
    return;
  }

  if (nx <= 0 || ny <= 0) {
    plabort("plimage: nx and ny must be positive");
    return;
  }

  plMinMax2dGrid(idata, nx, ny, &zmax, &zmin);
  dx = (xmax - xmin) / (nx - 1);
  dy = (ymax - ymin) / (ny - 1);
  nnx = (Dxmax-Dxmin)/dx + 1;
  nny = (Dymax-Dymin)/dy + 1;

  data = (PLFLT *) malloc(nny*nnx*sizeof(PLFLT));

  j=0;
  for (ix=0; ix<nx; ix++)
    for (iy=0; iy<ny; iy++) {
      xm = xmin + ix*dx;
      ym = ymin + iy*dy;
      if( Dxmin <= xm && Dxmax >= xm && Dymin <= ym && Dymax >= ym)
	data[j++] = idata[ix][iy];
    }

  xmin = Dxmin;  xmax = Dxmax;
  ymin = Dymin;  ymax = Dymax;

  /* adjust the step for the X/Y arrays */
  dx = dx*(nx-1)/nx;
  dy = dy*(ny-1)/ny;

  if( plsc->dev_fastimg == 0) {
    plimageslow( data, nnx,  nny, 
		 xmin, ymin, dx, dy,
		 zmin, zmax);
    free(data);
    return ;
  }

  /* compose the arrays to pass to plP_image */

  /* the Z array has size (nnx-1)*(nny-1) */

  Zf = (PLFLT *) malloc(nny*nnx*sizeof(PLFLT));  
  for (ix = 0; ix < nnx; ix++)
    for (iy = 0; iy < nny; iy++)
      Zf[ix*nny+iy] =  (data[ix*nny+iy]-zmin) / (zmax-zmin) ; 

  /* while the X and Y arrays has size nnx*nny */
  nnx++; nny++;
  Xf = (PLINT *) malloc(nny*nnx*sizeof(PLINT));
  Yf = (PLINT *) malloc(nny*nnx*sizeof(PLINT));

  for (ix = 0; ix < nnx; ix++)
    for (iy = 0; iy < nny; iy++) {      
      Xf[ix*nny+iy] =  plP_wcpcx(xmin + ix*dx);
      Yf[ix*nny+iy] =  plP_wcpcy(ymin + iy*dy);
    }

  plP_image(Xf, Yf, Zf, nnx, nny);

  free(Xf);
  free(Yf);
  free(Zf);
  free(data);
}
