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
	    PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	    PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax)
{

  PLINT  ix, iy;
  int    i;
  PLFLT  dx, dy;
  PLFLT x[8], y[8];
  PLFLT zmin, zmax;
  PLFLT xm, ym;

  zmin = zmax = data[0];
  for (ix = 0; ix < nx - 1; ix++) {
    for (iy = 0; iy < ny - 1; iy++) {
      if( data[ix*ny+iy] > zmax ) zmax = data[ix*ny+iy];
      if( data[ix*ny+iy] < zmin ) zmin = data[ix*ny+iy];
    }
  }
  
  dx = (xmax - xmin) / (nx - 1);
  dy = (ymax - ymin) / (ny - 1);

  /* disabledisplay(); */
  /* plsc->plbuf_write=0; */
  for (ix = 0; ix < nx - 1; ix++) {
    for (iy = 0; iy < ny - 1; iy++) {

      plcol1((data[ix*ny+iy]-zmin) / (zmax-zmin));

      xm = xmin + ix*dx;
      ym = ymin + iy*dy;

      if( Dxmin <= xm && Dxmax >= xm && Dymin <= ym && Dymax >= ym) {

	x[0] = x[1] = ix;
	x[2] = x[3] = ix+1;
	y[0] = y[3] = iy;
	y[1] = y[2] = iy+1;
	for (i = 0; i < 4; i++) {
	  x[i] = xmin + x[i]*dx - dx*0.5;
	  y[i] = ymin + y[i]*dy - dy*0.5;
	}
	plfill(4, x, y);
      }
    }
  }
  /* enabledisplay(); */
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
 *           (subject to change in the future)
 *
 * arguments are
 *   data: array containing image data
 *   nx: dimension on the X axis.        
 *   ny: dimension of the Y axis 
 *   The array data is indexed like data[ix*ny+iy]
 *
 *   xmin, xmax, ymin, ymax:
 *       data[0][0] corresponds to (xmin,ymin)
 *       data[0][ny-1] to (xmin,ymax)
 *       data[nx-1][ny-1] to (xmax,ymax)
 *
 *   Dxmin, Dxmax, Dymin, Dymax:
 *       plots only the window of points whose(x,y)'s fall
 *       inside the [Dxmin->Dxmax]X[Dymin->Dymax] window
 *
\*-------------------------------------------------------------------------*/

void
plimage(PLFLT *data, PLINT nx, PLINT ny, 
	PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax)
{

  PLINT  ix, iy;
  PLFLT  dx, dy;
  PLFLT zmin, zmax;
  PLINT *Xf, *Yf;
  PLFLT *Zf;
  
  if (plsc->level < 3) {
    plabort("plimage: window must be set up first");
    return;
  }

  if (nx <= 0 || ny <= 0) {
    plabort("plimage: nx and ny must be positive");
    return;
  }
  
  if( plsc->is_a_fast_image_device == 0) {
    plimageslow( data, nx,  ny, 
		 xmin, xmax, ymin, ymax,
		 Dxmin, Dxmax, Dymin, Dymax);
    return ;
  }

  /* compose the array to pass to plP_image */

  Xf = (PLINT *) malloc(ny*nx*sizeof(PLINT));
  Yf = (PLINT *) malloc(ny*nx*sizeof(PLINT));
  Zf = (PLFLT *) malloc(ny*nx*sizeof(PLFLT));
  
  zmin = zmax = data[0];
  for (ix = 0; ix < nx ; ix++) {
    for (iy = 0; iy < ny ; iy++) {
      if( data[ix*ny+iy] > zmax ) zmax = data[ix*ny+iy];
      if( data[ix*ny+iy] < zmin ) zmin = data[ix*ny+iy];
    }
  }
  
  dx = (xmax - xmin) / (nx - 1);
  dy = (ymax - ymin) / (ny - 1);
  
  for (ix = 0; ix < nx; ix++) {
    for (iy = 0; iy < ny; iy++) {      
      Zf[ix*ny+iy] =  (data[ix*ny+iy]-zmin) / (zmax-zmin) ; 
      Xf[ix*ny+iy] =  plP_wcpcx(xmin + ix*dx);
      Yf[ix*ny+iy] =  plP_wcpcy(ymin + iy*dy);
    }
  }

  /* plsc->offX=0;*/
  /* plsc->offY=0;*/

  plsc->Dxmin = plP_wcpcx(Dxmin);
  plsc->Dxmax = plP_wcpcx(Dxmax);
  plsc->Dymin = plP_wcpcy(Dymin);
  plsc->Dymax = plP_wcpcy(Dymax);
 
  /* plsc->offXu  = 20;*/
  /* plsc->offYu  = -30;*/

  /* disabledisplay(); */
  plP_image(Xf, Yf, Zf, nx, ny);
  /* enabledisplay(); */

  free(Xf);
  free(Yf);
  free(Zf);
}
