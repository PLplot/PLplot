/* $Id$

	3d plot routines.
*/

#include "plplot/plplotP.h"

/* Internal constants */

#define  BINC      50		/* Block size for memory allocation */

static PLINT pl3mode = 0;	/* 0 3d solid; 1 mesh plot */
static PLINT pl3upv = 1;	/* 1 update view; 0 no update */

static PLINT zbflg = 0, zbcol;
static PLFLT zbtck;

static PLINT *oldhiview = NULL;
static PLINT *oldloview = NULL;
static PLINT *newhiview = NULL;
static PLINT *newloview = NULL;
static PLINT *utmp = NULL;
static PLINT *vtmp = NULL;
static PLFLT *ctmp =0;

static PLINT mhi, xxhi, newhisize;
static PLINT mlo, xxlo, newlosize;

/* Light source for shading */
static PLFLT xlight, ylight, zlight;

/* define WANT_MISSING_TRIANGLES if you want to enable the old buggy way in plotsh3 */
/* #define WANT_MISSING_TRIANGLES 1 */

#if WANT_MISSING_TRIANGLES
static PLINT threedshading;
#endif

/* Prototypes for static functions */

static void plgrid3	(PLFLT);
static void plnxtv (PLINT *, PLINT *, PLFLT*, PLINT, PLINT);
static void plside3	(PLFLT *, PLFLT *, PLFLT **, PLINT, PLINT, PLINT);
static void plt3zz	(PLINT, PLINT, PLINT, PLINT, 
			 PLINT, PLINT *, PLFLT *, PLFLT *, PLFLT **, 
			   PLINT, PLINT, PLINT *, PLINT *, PLFLT*);
static void plnxtvhi (PLINT *, PLINT *, PLFLT*, PLINT, PLINT);
static void plnxtvlo (PLINT *, PLINT *, PLFLT*, PLINT, PLINT);
static void plnxtvhi_draw(PLINT *u, PLINT *v, PLFLT* c, PLINT n);

static void savehipoint	(PLINT, PLINT);
static void savelopoint	(PLINT, PLINT);
static void swaphiview	(void);
static void swaploview	(void);
static void myexit	(char *);
static void myabort	(char *);
static void freework	(void);
static int  plabv	(PLINT, PLINT, PLINT, PLINT, PLINT, PLINT);
static void pl3cut	(PLINT, PLINT, PLINT, PLINT, PLINT, 
				PLINT, PLINT, PLINT, PLINT *, PLINT *);
static PLFLT plGetAngleToLight(PLFLT* x, PLFLT* y, PLFLT* z);
static void plP_draw3d(PLINT x, PLINT y, PLINT j, PLINT move);

/* #define MJL_HACK 1 */
#if MJL_HACK
static void plP_fill3(PLINT x0, PLINT y0, PLINT x1, PLINT y1,
		      PLINT x2, PLINT y2, PLINT j);
static void plP_fill4(PLINT x0, PLINT y0, PLINT x1, PLINT y1,
		      PLINT x2, PLINT y2, PLINT x3, PLINT y3, PLINT j);
#endif

/*--------------------------------------------------------------------------*\
 * void plsetlightsource(x, y, z)
 *
 * Sets the position of the light source.
\*--------------------------------------------------------------------------*/

void
c_pllightsource(PLFLT x, PLFLT y, PLFLT z)
{
    xlight = x;
    ylight = y;
    zlight = z;
}

/*--------------------------------------------------------------------------*\
 * void plmesh(x, y, z, nx, ny, opt)
 *
 * Plots a mesh representation of the function z[x][y]. The x values
 * are stored as x[0..nx-1], the y values as y[0..ny-1], and the
 * z values are in the 2-d array z[][]. The integer "opt" specifies:
 *
 *  opt = 1:  Draw lines parallel to x-axis
 *  opt = 2:  Draw lines parallel to y-axis
 *  opt = 3:  Draw lines parallel to both axes
\*--------------------------------------------------------------------------*/

void
c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt)
{
    pl3mode = 1;
    plot3d(x, y, z, nx, ny, opt, 0);

    pl3mode = 0;
}

/*--------------------------------------------------------------------------*\
 * void plotsh3d(x, y, z, nx, ny, side)
 *
 * Plots a 3-d representation of the function z[x][y]. The x values
 * are stored as x[0..nx-1], the y values as y[0..ny-1], and the z
 * values are in the 2-d array z[][]. 
\*--------------------------------------------------------------------------*/

#if WANT_MISSING_TRIANGLES
void
c_plotsh3d(PLFLT *x, PLFLT *y, PLFLT **z,
		 PLINT nx, PLINT ny, PLINT side)
{
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT init, i, ix, iy, color;

    if (plsc->level < 3) {
	myabort("plot3d: Please set up window first");
	return;
    }
    
    if (nx <= 0 || ny <= 0) {
	myabort("plot3d: Bad array dimensions.");
	return;
    }
    
/* Check that points in x and in y are strictly increasing */

    for (i = 0; i < nx - 1; i++) {
	if (x[i] >= x[i + 1]) {
	    myabort("plot3d: X array must be strictly increasing");
	    return;
	}
    }
    for (i = 0; i < ny - 1; i++) {
	if (y[i] >= y[i + 1]) {
	    myabort("plot3d: Y array must be strictly increasing");
	    return;
	}
    }

/* Allocate work arrays */

    utmp = (PLINT *) malloc((size_t) (2 * MAX(nx, ny) * sizeof(PLINT)));
    vtmp = (PLINT *) malloc((size_t) (2 * MAX(nx, ny) * sizeof(PLINT)));
    ctmp = (PLFLT *) malloc((size_t) (2 * MAX(nx, ny) * sizeof(PLFLT)));
    if ( ! utmp || ! vtmp || ! ctmp)
	myexit("plotsh3d: Out of memory.");

    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    init = 1;

    /* Extract shading info */
    threedshading = 1;
	
/* Call 3d shline plotter.  Each viewing quadrant 
   (perpendicular to x-y plane) must be handled separately. */ 

    if (cxx >= 0.0 && cxy <= 0.0) {
	for (iy = 2; iy <= ny; iy++)
	    plt3zz(1, iy, 1, -1, -3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
	for (ix = 1; ix <= nx - 1; ix++)
	    plt3zz(ix, ny, 1, -1, 3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
    } else if (cxx <= 0.0 && cxy <= 0.0) {
	for (ix = 2; ix <= nx; ix++) 
	    plt3zz(ix, ny, -1, -1, 3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
	for (iy = ny; iy >= 2; iy--)
	    plt3zz(nx, iy, -1, -1, -3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
    } else if (cxx <= 0.0 && cxy >= 0.0) {
	for (iy = ny - 1; iy >= 1; iy--) 
	    plt3zz(nx, iy, -1, 1, -3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
	for (ix = nx; ix >= 2; ix--)
	    plt3zz(ix, 1, -1, 1, 3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
    } else if (cxx >= 0.0 && cxy >= 0.0) {
	for (ix = nx - 1; ix >= 1; ix--) 
	    plt3zz(ix, 1, 1, 1, 3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
	for (iy = 1; iy <= ny - 1; iy++)
	    plt3zz(1, iy, 1, 1, -3, &init, x, y, z, nx, ny, utmp, vtmp,ctmp);
    }
    
/* Finish up by drawing sides, background grid (both are optional) */
    threedshading = 0;
    if (side)
	plside3(x, y, z, nx, ny, 3);

    if (zbflg) {
	color = plsc->icol0;
	plcol(zbcol);
	plgrid3(zbtck);
	plcol(color);
    }

    freework();
}
#else

/* clipping helper for 3d polygons */

int plP_clip_poly(int Ni, PLFLT *Vi[3], int axis, PLFLT dir, PLFLT offset)
{
  int anyout = 0;
  PLFLT in[PL_MAXPOLY], T[3][PL_MAXPOLY];
  int No = 0;
  int i, j, k;

  for(i=0; i<Ni; i++) {
    in[i] = Vi[axis][i] * dir + offset;
    anyout += in[i] < 0;
  }

  /* none out */
  if(anyout == 0)
    return Ni;

  /* all out */
  if(anyout == Ni) {
    return 0;
  }

  /* some out */
  /* copy over to a temp vector */
  for(i=0; i<3; i++) {
    for(j=0; j<Ni; j++) {
      T[i][j] = Vi[i][j];
    }
  }
  /* copy back selectively */
  for(i=0; i<Ni; i++) {
    j = (i+1) % Ni;

    if(in[i]>=0 && in[j]>=0) {
      for(k=0; k<3; k++)
	Vi[k][No] = T[k][j];
      No++;
    } else if(in[i]>=0 && in[j]<0) {
      PLFLT u = in[i] / (in[i] - in[j]);
      for(k = 0; k<3; k++)
	Vi[k][No] = T[k][i]*(1-u) + T[k][j]*u;
      No++;
    } else if(in[i]<0 && in[j]>=0) {
      PLFLT u = in[i] / (in[i] - in[j]);
      for(k = 0; k<3; k++)
	Vi[k][No] = T[k][i]*(1-u) + T[k][j]*u;
      No++;
      for(k=0; k<3; k++)
	Vi[k][No] = T[k][j];
      No++;
    }
  }
  return No;
}

/* helper for plotsh3d below */
static void shade_triangle(PLFLT x0, PLFLT y0, PLFLT z0,
			   PLFLT x1, PLFLT y1, PLFLT z1,
			   PLFLT x2, PLFLT y2, PLFLT z2)
{
  int i;
  /* arrays for interface to core functions */
  PLINT u[6], v[6];
  PLFLT x[6], y[6], z[6], c;
  int n;
  PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;
  PLFLT *V[3];

  plP_gdom(&xmin, &xmax, &ymin, &ymax);
  plP_grange(&zscale, &zmin, &zmax);

  x[0] = x0; x[1] = x1; x[2] = x2;
  y[0] = y0; y[1] = y1; y[2] = y2;
  z[0] = z0; z[1] = z1; z[2] = z2;
  n = 3;
  c = plGetAngleToLight(x, y, z);
  V[0] = x; V[1] = y; V[2] = z;

  n = plP_clip_poly(n, V, 0,  1, -xmin);
  n = plP_clip_poly(n, V, 0, -1,  xmax);
  n = plP_clip_poly(n, V, 1,  1, -ymin);
  n = plP_clip_poly(n, V, 1, -1,  ymax);
  n = plP_clip_poly(n, V, 2,  1, -zmin);
  n = plP_clip_poly(n, V, 2, -1,  zmax);

  for(i=0; i<n; i++) {
    u[i] = plP_wcpcx(plP_w3wcx(x[i], y[i], z[i]));
    v[i] = plP_wcpcy(plP_w3wcy(x[i], y[i], z[i]));
  }
  u[n] = u[0];
  v[n] = v[0];
  plcol1(c);
  plP_plfclp(u, v, n+1, plsc->clpxmi, plsc->clpxma, plsc->clpymi, plsc->clpyma, plP_fill);
}

/*--------------------------------------------------------------------------*\
 * void plotsh3d(x, y, z, nx, ny, side)
 *
 * Plots a 3-d representation of the function z[x][y]. The x values
 * are stored as x[0..nx-1], the y values as y[0..ny-1], and the z
 * values are in the 2-d array z[][]. 
 *
 * This code is a complete departure from the approach taken in the old version
 * of this routine. Formerly to code attempted to use the logic for the hidden
 * line algorithm to draw the hidden surface. This was really hard. This code
 * below uses a simple back to front (painters) algorithm. All the the
 * triangles are drawn.
 *
 * There are multitude of ways this code could be optimized. Given the
 * problems with the old code, I tried to focus on clarity here. 
\*--------------------------------------------------------------------------*/

void
c_plotsh3d(PLFLT *x, PLFLT *y, PLFLT **z,
		 PLINT nx, PLINT ny, PLINT side)
{
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT i, j;
    PLINT ixDir, ixOrigin, iyDir, iyOrigin, nFast, nSlow;
    PLINT ixFast, ixSlow, iyFast, iySlow;
    PLINT iFast, iSlow;
    PLINT iX[2][2], iY[2][2];
    PLFLT xbox[3], ybox[3], zbox[3];
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;
    PLINT ixmin=0, ixmax=nx, iymin=0, iymax=ny;

    if (plsc->level < 3) {
	myabort("plotsh3d: Please set up window first");
	return;
    }
    
    if (nx <= 0 || ny <= 0) {
	myabort("plotsh3d: Bad array dimensions.");
	return;
    }
    
    plP_gdom(&xmin, &xmax, &ymin, &ymax);
    plP_grange(&zscale, &zmin, &zmax);
    if(zmin > zmax) {
      PLFLT t = zmin;
      zmin = zmax;
      zmax = t;
    }

    /* Check that points in x and in y are strictly increasing  and in range */

    for (i = 0; i < nx - 1; i++) {
	if (x[i] >= x[i + 1]) {
	    myabort("plotsh3d: X array must be strictly increasing");
	    return;
	}
	if (x[i] < xmin && x[i+1] >= xmin)
	  ixmin = i;
	if (x[i+1] > xmax && x[i] <= xmax)
	  ixmax = i+2;
    }
    for (i = 0; i < ny - 1; i++) {
	if (y[i] >= y[i + 1]) {
	    myabort("plotsh3d: Y array must be strictly increasing");
	    return;
	}
	if (y[i] < ymin && y[i+1] >= ymin)
	  iymin = i;
	if (y[i+1] > ymax && y[i] <= ymax)
	  iymax = i+2;
    }

    /* get the viewing parameters */
    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);

    /* we're going to draw from back to front */

    /* iFast will index the dominant (fastest changing) dimension
       iSlow will index the slower changing dimension

       iX indexes the X dimension
       iY indexes the Y dimension */

    /* get direction for X */
    if(cxy >= 0) {
      ixDir = 1;	/* direction in X */
      ixOrigin = ixmin;	/* starting point */
    } else {
      ixDir = -1;
      ixOrigin = ixmax - 1;
    }
    /* get direction for Y */
    if(cxx >= 0) {
      iyDir = -1;
      iyOrigin = iymax - 1;
    } else {
      iyDir = 1;
      iyOrigin = iymin;
    }

    /* figure out which dimension is dominant */
    if (fabs(cxx) > fabs(cxy)) {
      /* X is dominant */
      nFast = ixmax - ixmin;	/* samples in the Fast direction */
      nSlow = iymax - iymin;	/* samples in the Slow direction */
      
      ixFast = ixDir; ixSlow = 0;
      iyFast = 0;     iySlow = iyDir;
    } else {
      nFast = iymax - iymin;
      nSlow = ixmax - ixmin;

      ixFast = 0;     ixSlow = ixDir;
      iyFast = iyDir; iySlow = 0;
    }

    /* we've got to draw the background grid first, hidden line code has to draw it last */
    if (zbflg) {
      PLINT color = plsc->icol0;
      PLFLT bx[3], by[3], bz[3];
      PLFLT tick=0, tp;
      PLINT nsub;

      /* get the tick spacing */
      pldtik(zmin, zmax, &tick, &nsub);

      /* determine the vertices for the background grid line */
      bx[0] = (ixOrigin!=ixmin && ixFast==0) || ixFast > 0 ? xmax : xmin;
      by[0] = (iyOrigin!=iymin && iyFast==0) || iyFast > 0 ? ymax : ymin;
      bx[1] = ixOrigin!=ixmin ? xmax : xmin;
      by[1] = iyOrigin!=iymin ? ymax : ymin;
      bx[2] = (ixOrigin!=ixmin && ixSlow==0) || ixSlow > 0 ? xmax : xmin;
      by[2] = (iyOrigin!=iymin && iySlow==0) || iySlow > 0 ? ymax : ymin;

      plcol(zbcol);
      for(tp = tick * floor(zmin / tick) + tick; tp <= zmax; tp += tick) {
	bz[0] = bz[1] = bz[2] = tp;
	plline3(3, bx, by, bz);	
      }
      /* draw the vertical line at the back corner */
      bx[0] = bx[1];
      by[0] = by[1];
      bz[0] = zmin;
      plline3(2, bx, by, bz);
      plcol(color);
    }

    /* Now we can iterate over the grid drawing the quads */
    for(iSlow=0; iSlow < nSlow-1; iSlow++) {
      for(iFast=0; iFast < nFast-1; iFast++) {
	/* get the 4 corners of the Quad */
	for(i=0; i<2; i++) {
	  for(j=0; j<2; j++) {
	    /* we're transforming from Fast/Slow coordinates to x/y coordinates */
	    /* note, these are the indices, not the values */
	    iX[i][j] = ixFast * (iFast+i) + ixSlow * (iSlow+j) + ixOrigin;
	    iY[i][j] = iyFast * (iFast+i) + iySlow * (iSlow+j) + iyOrigin;
	  }
	}

	/* now draw the quad as two triangles (4 might be better) */
	shade_triangle(x[iX[0][0]], y[iY[0][0]], z[iX[0][0]][iY[0][0]],
		       x[iX[1][0]], y[iY[1][0]], z[iX[1][0]][iY[1][0]],
		       x[iX[0][1]], y[iY[0][1]], z[iX[0][1]][iY[0][1]]);
	shade_triangle(x[iX[0][1]], y[iY[0][1]], z[iX[0][1]][iY[0][1]],
		       x[iX[1][1]], y[iY[1][1]], z[iX[1][1]][iY[1][1]],
		       x[iX[1][0]], y[iY[1][0]], z[iX[1][0]][iY[1][0]]);
      }
    }

    if (side) {
      /* draw one more row with all the Z's set to zmin */
      PLFLT zscale, zmin, zmax;

      plP_grange(&zscale, &zmin, &zmax);
      iSlow = nSlow-1;
      for(iFast=0; iFast < nFast-1; iFast++) {
	for(i=0; i<2; i++) {
	  iX[i][0] = ixFast * (iFast+i) + ixSlow * iSlow + ixOrigin;
	  iY[i][0] = iyFast * (iFast+i) + iySlow * iSlow + iyOrigin;
	}
	/* now draw the quad as two triangles (4 might be better) */
	shade_triangle(x[iX[0][0]], y[iY[0][0]], z[iX[0][0]][iY[0][0]],
		       x[iX[1][0]], y[iY[1][0]], z[iX[1][0]][iY[1][0]],
		       x[iX[0][0]], y[iY[0][0]], zmin);
	shade_triangle(x[iX[1][0]], y[iY[1][0]], z[iX[1][0]][iY[1][0]],
		       x[iX[1][0]], y[iY[1][0]], zmin,
		       x[iX[0][0]], y[iY[0][0]], zmin);
      }
    }

}
#endif

/*--------------------------------------------------------------------------*\
 * void plot3d(x, y, z, nx, ny, opt, side)
 *
 * Plots a 3-d representation of the function z[x][y]. The x values
 * are stored as x[0..nx-1], the y values as y[0..ny-1], and the z
 * values are in the 2-d array z[][]. The integer "opt" specifies:
 *
 *  opt = 1:  Draw lines parallel to x-axis
 *  opt = 2:  Draw lines parallel to y-axis
 *  opt = 3:  Draw lines parallel to both axes
\*--------------------------------------------------------------------------*/

void
c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side)
{
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLINT init, i, ix, iy, color;
    PLFLT xmin, xmax, ymin, ymax, zmin, zmax, zscale;
    PLINT ixmin=0, ixmax=nx-1, iymin=0, iymax=ny-1;
    PLINT clipped = 0;

    if (plsc->level < 3) {
	myabort("plot3d: Please set up window first");
	return;
    }

    if (opt < 1 || opt > 3) {
	myabort("plot3d: Bad option");
	return;
    }

    if (nx <= 0 || ny <= 0) {
	myabort("plot3d: Bad array dimensions.");
	return;
    }

    plP_gdom(&xmin, &xmax, &ymin, &ymax);
    plP_grange(&zscale, &zmin, &zmax);
    if(zmin > zmax) {
      PLFLT t = zmin;
      zmin = zmax;
      zmax = t;
    }
    if(xmin < x[0]) xmin = x[0];
    if(xmax > x[nx-1]) xmax = x[nx-1];
    if(ymin < y[0]) ymin = y[0];
    if(ymax > y[ny-1]) ymax = y[ny-1];

/* Check that points in x and in y are strictly increasing */

    for (i = 0; i < nx - 1; i++) {
	if (x[i] >= x[i + 1]) {
	    myabort("plot3d: X array must be strictly increasing");
	    return;
	}
	if (x[i] < xmin)
	  ixmin = i;
	if (x[i] < xmax)
	  ixmax = i+1;
    }
    for (i = 0; i < ny - 1; i++) {
	if (y[i] >= y[i + 1]) {
	    myabort("plot3d: Y array must be strictly increasing");
	    return;
	}
	if (y[i] < ymin)
	  iymin = i;
	if (y[i] < ymax)
	  iymax = i+1;
    }
    /* do we need to clip? */
    if(ixmin > 0 || ixmax < nx-1 || iymin > 0 || iymax < ny-1) {
      /* adjust the input so it stays within bounds */
      int _nx = ixmax - ixmin + 1;
      int _ny = iymax - iymin + 1;
      PLFLT *_x = (PLFLT*)malloc(_nx * sizeof(PLFLT));
      PLFLT *_y = (PLFLT*)malloc(_ny * sizeof(PLFLT));
      PLFLT **_z = (PLFLT**)malloc(_nx * sizeof(PLFLT*));
      PLFLT ty0, ty1, tx0, tx1;
      int i, j;

      clipped = 1;

      _x[0] = xmin;
      _x[_nx-1] = xmax;
      for(i=1; i<_nx-1; i++)
	_x[i] = x[ixmin+i];
      _y[0] = ymin;
      _y[_ny-1] = ymax;
      for(i=1; i<_ny-1; i++)
	_y[i] = y[iymin+i];

      for(i=0; i<_nx; i++)
	_z[i] = (PLFLT*)malloc(_ny * sizeof(PLFLT));

      ty0 = (ymin - y[iymin]) / (y[iymin+1] - y[iymin]);
      ty1 = (ymax - y[iymax-1]) / (y[iymax] - y[iymax-1]);
      tx0 = (xmin - x[ixmin]) / (x[ixmin+1] - x[ixmin]);
      tx1 = (xmax - x[ixmax-1]) / (x[ixmax] - x[ixmax-1]);
      for(i=0; i<_nx; i++) {
	if(i==0) {
	  _z[i][0] = z[ixmin][iymin]*(1-ty0)*(1-tx0) + z[ixmin][iymin+1]*(1-tx0)*ty0
	    + z[ixmin+1][iymin]*tx0*(1-ty0) + z[ixmin+1][iymin+1]*tx0*ty0;
	  for(j=1; j<_ny-1; j++)
	    _z[i][j] = z[ixmin][iymin+j]*(1-tx0) + z[ixmin+1][iymin+j]*tx0;
	  _z[i][_ny-1] = z[ixmin][iymax-1]*(1-tx0)*(1-ty1) + z[ixmin][iymax]*(1-tx0)*ty1
	    + z[ixmin+1][iymax-1]*tx0*(1-ty1) + z[ixmin+1][iymax]*tx0*ty1;
	} else if(i==_nx-1) {
	  _z[i][0] = z[ixmax-1][iymin]*(1-tx1)*(1-ty0) + z[ixmax-1][iymin+1]*(1-tx1)*ty0
	    + z[ixmax][iymin]*tx1*(1-ty0) + z[ixmax][iymin+1]*tx1*ty0;
	  for(j=1; j<_ny-1; j++)
	    _z[i][j] = z[ixmax-1][iymin+j]*(1-tx1) + z[ixmax][iymin+j]*tx1;
	  _z[i][_ny-1] = z[ixmax-1][iymax-1]*(1-tx1)*(1-ty1) + z[ixmax][iymax]*(1-tx1)*ty1
	    + z[ixmax][iymax-1]*tx1*(1-ty1) + z[ixmax][iymax]*tx1*ty1;
	} else {
	  _z[i][0] = z[ixmin+i][iymin]*(1-ty0) + z[ixmin+i][iymin+1]*ty0;
	  for(j=1; j<_ny-1; j++)
	    _z[i][j] = z[ixmin+i][iymin+j];
	  _z[i][_ny-1] = z[ixmin+i][iymax-1]*(1-ty1) + z[ixmin+i][iymax]*ty1;
	}
	for(j=0; j<_ny; j++) {
	  if(_z[i][j] < zmin)
	    _z[i][j] = zmin;
	  else if(_z[i][j] > zmax)
	    _z[i][j] = zmax;
	}
      }
      x = _x;
      y = _y;
      z = _z;
      nx = _nx;
      ny = _ny;	  
    }
/* Allocate work arrays */

    utmp = (PLINT *) malloc((size_t) (2 * MAX(nx, ny) * sizeof(PLINT)));
    vtmp = (PLINT *) malloc((size_t) (2 * MAX(nx, ny) * sizeof(PLINT)));
    ctmp = 0;
    
    if ( ! utmp || ! vtmp)
	myexit("plot3d: Out of memory.");

    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    /* fprintf(stderr, "cxx=%g cxy=%g cyx=%g cyy=%g cyz=%g\n", cxx, cxy, cyx, cyy, cyz); */
    init = 1;
#if WANT_MISSING_TRIANGLES
    threedshading = 0;
#endif
/* Call 3d line plotter.  Each viewing quadrant 
   (perpendicular to x-y plane) must be handled separately. */ 

    if (cxx >= 0.0 && cxy <= 0.0) {
	if (opt == 2) 
	    plt3zz(1, ny, 1, -1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (iy = 2; iy <= ny; iy++)
		plt3zz(1, iy, 1, -1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
	if (opt == 1)
	    plt3zz(1, ny, 1, -1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (ix = 1; ix <= nx - 1; ix++)
		plt3zz(ix, ny, 1, -1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
    }

    else if (cxx <= 0.0 && cxy <= 0.0) {
	if (opt == 1) 
	    plt3zz(nx, ny, -1, -1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (ix = 2; ix <= nx; ix++) 
		plt3zz(ix, ny, -1, -1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
	if (opt == 2)
	    plt3zz(nx, ny, -1, -1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (iy = ny; iy >= 2; iy--)
		plt3zz(nx, iy, -1, -1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
    }

    else if (cxx <= 0.0 && cxy >= 0.0) {
	if (opt == 2) 
	    plt3zz(nx, 1, -1, 1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (iy = ny - 1; iy >= 1; iy--) 
		plt3zz(nx, iy, -1, 1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
	if (opt == 1)
	    plt3zz(nx, 1, -1, 1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (ix = nx; ix >= 2; ix--)
		plt3zz(ix, 1, -1, 1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
    }

    else if (cxx >= 0.0 && cxy >= 0.0) {
	if (opt == 1) 
	    plt3zz(1, 1, 1, 1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (ix = nx - 1; ix >= 1; ix--) 
		plt3zz(ix, 1, 1, 1, opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
	if (opt == 2)
	    plt3zz(1, 1, 1, 1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	else {
	    for (iy = 1; iy <= ny - 1; iy++)
		plt3zz(1, iy, 1, 1, -opt, &init, x, y, z, nx, ny, utmp, vtmp,0);
	}
    }

/* Finish up by drawing sides, background grid (both are optional) */

    if (side)
	plside3(x, y, z, nx, ny, opt);

    if (zbflg) {
	color = plsc->icol0;
	plcol(zbcol);
	plgrid3(zbtck);
	plcol(color);
    }

    freework();

    if(clipped) {
      free(x);
      free(y);
      for(i=0; i<nx; i++)
	free(z[i]);
      free(z);
    }
}

/*--------------------------------------------------------------------------*\
 * void plP_gzback()
 *
 * Get background parameters for 3d plot.
\*--------------------------------------------------------------------------*/

void
plP_gzback(PLINT **zbf, PLINT **zbc, PLFLT **zbt)
{
    *zbf = &zbflg;
    *zbc = &zbcol;
    *zbt = &zbtck;
}

/*--------------------------------------------------------------------------*\
 * PLFLT plGetAngleToLight()
 *
 * Gets cos of angle between normal to a polygon and a light source.
 * Requires at least 3 elements, forming non-parallel lines 
 * in the arrays.
\*--------------------------------------------------------------------------*/

static PLFLT
plGetAngleToLight(PLFLT* x, PLFLT* y, PLFLT* z)
{
    PLFLT vx1, vx2, vy1, vy2, vz1, vz2;
    PLFLT px, py, pz;
    PLFLT vlx, vly, vlz;
    PLFLT mag1, mag2;
    PLFLT cosangle;

    vx1 = x[1] - x[0];
    vx2 = x[2] - x[1];
    vy1 = y[1] - y[0];
    vy2 = y[2] - y[1];
    vz1 = z[1] - z[0];
    vz2 = z[2] - z[1];

/* Find vector perpendicular to the face */
    px = vy1*vz2 - vz1*vy2;
    py = vz1*vx2 - vx1*vz2;
    pz = vx1*vy2 - vy1*vx2;
    mag1 = px*px + py*py + pz*pz;

/* Vectors were parallel! */
    if (mag1 == 0) 
	return 1;

    vlx = xlight - x[0];
    vly = ylight - y[0];
    vlz = zlight - z[0];
    mag2 = vlx*vlx + vly*vly + vlz*vlz;
    if (mag2 ==0) 
	return 1;

/* Now have 3 vectors going through the first point on the given surface */
    cosangle = fabs( (vlx*px + vly*py + vlz*pz) / sqrt(mag1*mag2) );

/* In case of numerical rounding */
    if (cosangle > 1) cosangle = 1;
    return cosangle;
}

/*--------------------------------------------------------------------------*\
 * void plt3zz()
 *
 * Draws the next zig-zag line for a 3-d plot.  The data is stored in array
 * z[][] as a function of x[] and y[], and is plotted out starting at index
 * (x0,y0).
 *
 * Depending on the state of "flag", the sequence of data points sent to
 * plnxtv is altered so as to allow cross-hatch plotting, or plotting
 * parallel to either the x-axis or the y-axis.
\*--------------------------------------------------------------------------*/

static void
plt3zz(PLINT x0, PLINT y0, PLINT dx, PLINT dy, PLINT flag, PLINT *init,
       PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, 
       PLINT *u, PLINT *v, PLFLT* c)
{
    PLINT n = 0;
    PLFLT x2d, y2d;
    PLFLT fill3x[3],fill3y[3],fill3z[3];
    PLINT threedcount = 0;

    while (1 <= x0 && x0 <= nx && 1 <= y0 && y0 <= ny) {
	x2d = plP_w3wcx(x[x0 - 1], y[y0 - 1], z[x0 - 1][y0 - 1]);
	y2d = plP_w3wcy(x[x0 - 1], y[y0 - 1], z[x0 - 1][y0 - 1]);
#if WANT_MISSING_TRIANGLES
	if (threedshading) {
	    fill3x[threedcount] = x[x0-1];
	    fill3y[threedcount] = y[y0-1];
	    fill3z[threedcount] = z[x0-1][y0-1];
	    threedcount++;
	    if (threedcount > 2) threedcount = 0;
	    if (n > 1) {
		c[n] = plGetAngleToLight(fill3x,fill3y,fill3z);
	    }
	}
#endif
	u[n] = plP_wcpcx(x2d);
	v[n] = plP_wcpcy(y2d);

	switch (flag) {
	case -3:
	    y0 += dy;
	    flag = -flag;
	    break;
	case -2:
	    y0 += dy;
	    break;
	case -1:
	    y0 += dy;
	    flag = -flag;
	    break;
	case 1:
	    x0 += dx;
	    break;
	case 2:
	    x0 += dx;
	    flag = -flag;
	    break;
	case 3:
	    x0 += dx;
	    flag = -flag;
	    break;
	}
	n++;
    }

/* Get initial color value by linear interpolation. */
#if WANT_MISSING_TRIANGLES
    if (threedshading) {
	c[1] = 2*c[2] - c[3];
	if (c[1] < 0) c[1] = 0;
	if (c[1] > 1) c[1] = 1;
    }
#endif

    if (flag == 1 || flag == -2) {
	if (flag == 1) {
	    x0 -= dx;
	    y0 += dy;
	}
	else if (flag == -2) {
	    y0 -= dy;
	    x0 += dx;
	}
	if (1 <= x0 && x0 <= nx && 1 <= y0 && y0 <= ny) {
	    x2d = plP_w3wcx( x[x0 - 1], y[y0 - 1], z[x0 - 1][y0 - 1]);
	    y2d = plP_w3wcy( x[x0 - 1], y[y0 - 1], z[x0 - 1][y0 - 1]);
#if WANT_MISSING_TRIANGLES
	/* Although this is a rare boundary case, it's good to be safe */
	    if (threedshading) {
		fill3x[threedcount] = x[x0-1];
		fill3y[threedcount] = y[y0-1];
		fill3z[threedcount] = z[x0-1][y0-1];
		threedcount++;
		if (threedcount > 2) threedcount = 0;
		if (n > 1) {
		    c[n] = plGetAngleToLight(fill3x,fill3y,fill3z);
		}
	    }
#endif
	    u[n] = plP_wcpcx(x2d);
	    v[n] = plP_wcpcy(y2d);
	    n++;
	}
    }

/* All the setup is done.  Time to do the work. */

    plnxtv(u, v, c, n, *init);
    *init = 0;
}

/*--------------------------------------------------------------------------*\
 * void plside3()
 *
 * This routine draws sides around the front of the 3d plot so that
 * it does not appear to float.
\*--------------------------------------------------------------------------*/

static void
plside3(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt)
{
    PLINT i;
    PLFLT cxx, cxy, cyx, cyy, cyz;
    PLFLT xmin, ymin, zmin, xmax, ymax, zmax, zscale;
    PLFLT tx, ty, ux, uy;

    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gdom(&xmin, &xmax, &ymin, &ymax);
    plP_grange(&zscale, &zmin, &zmax);

/* Get x, y coordinates of legs and plot */

    if (cxx >= 0.0 && cxy <= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = plP_w3wcx(x[i], y[0], zmin);
		ty = plP_w3wcy(x[i], y[0], zmin);
		ux = plP_w3wcx(x[i], y[0], z[i][0]);
		uy = plP_w3wcy(x[i], y[0], z[i][0]);
		pljoin(tx, ty, ux, uy);
	    }
	}
	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = plP_w3wcx(x[0], y[i], zmin);
		ty = plP_w3wcy(x[0], y[i], zmin);
		ux = plP_w3wcx(x[0], y[i], z[0][i]);
		uy = plP_w3wcy(x[0], y[i], z[0][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = plP_w3wcx(x[i], y[ny - 1], zmin);
		ty = plP_w3wcy(x[i], y[ny - 1], zmin);
		ux = plP_w3wcx(x[i], y[ny - 1], z[i][ny - 1]);
		uy = plP_w3wcy(x[i], y[ny - 1], z[i][ny - 1]);
		pljoin(tx, ty, ux, uy);
	    }
	}
	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = plP_w3wcx(x[0], y[i], zmin);
		ty = plP_w3wcy(x[0], y[i], zmin);
		ux = plP_w3wcx(x[0], y[i], z[0][i]);
		uy = plP_w3wcy(x[0], y[i], z[0][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = plP_w3wcx(x[i], y[ny - 1], zmin);
		ty = plP_w3wcy(x[i], y[ny - 1], zmin);
		ux = plP_w3wcx(x[i], y[ny - 1], z[i][ny - 1]);
		uy = plP_w3wcy(x[i], y[ny - 1], z[i][ny - 1]);
		pljoin(tx, ty, ux, uy);
	    }
	}
	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = plP_w3wcx(x[nx - 1], y[i], zmin);
		ty = plP_w3wcy(x[nx - 1], y[i], zmin);
		ux = plP_w3wcx(x[nx - 1], y[i], z[nx - 1][i]);
		uy = plP_w3wcy(x[nx - 1], y[i], z[nx - 1][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	if (opt != 1) {
	    for (i = 0; i < nx; i++) {
		tx = plP_w3wcx(x[i], y[0], zmin);
		ty = plP_w3wcy(x[i], y[0], zmin);
		ux = plP_w3wcx(x[i], y[0], z[i][0]);
		uy = plP_w3wcy(x[i], y[0], z[i][0]);
		pljoin(tx, ty, ux, uy);
	    }
	}
	if (opt != 2) {
	    for (i = 0; i < ny; i++) {
		tx = plP_w3wcx(x[nx - 1], y[i], zmin);
		ty = plP_w3wcy(x[nx - 1], y[i], zmin);
		ux = plP_w3wcx(x[nx - 1], y[i], z[nx - 1][i]);
		uy = plP_w3wcy(x[nx - 1], y[i], z[nx - 1][i]);
		pljoin(tx, ty, ux, uy);
	    }
	}
    }
}

/*--------------------------------------------------------------------------*\
 * void plgrid3()
 *
 * This routine draws a grid around the back side of the 3d plot with
 * hidden line removal.
\*--------------------------------------------------------------------------*/

static void
plgrid3(PLFLT tick)
{
    PLFLT xmin, ymin, zmin, xmax, ymax, zmax, zscale;
    PLFLT cxx, cxy, cyx, cyy, cyz, zmin_in, zmax_in;
    PLINT u[3], v[3];
    PLINT nsub = 0;
    PLFLT tp;

    plP_gw3wc(&cxx, &cxy, &cyx, &cyy, &cyz);
    plP_gdom(&xmin, &xmax, &ymin, &ymax);
    plP_grange(&zscale, &zmin_in, &zmax_in);
    zmin = (zmax_in > zmin_in) ? zmin_in: zmax_in;
    zmax = (zmax_in > zmin_in) ? zmax_in: zmin_in;

    pldtik(zmin, zmax, &tick, &nsub);
    tp = tick * floor(zmin / tick) + tick;
    pl3upv = 0;

    if (cxx >= 0.0 && cxy <= 0.0) {
	while (tp <= zmax) {
	    u[0] = plP_wcpcx(plP_w3wcx(xmin, ymax, tp));
	    v[0] = plP_wcpcy(plP_w3wcy(xmin, ymax, tp));
	    u[1] = plP_wcpcx(plP_w3wcx(xmax, ymax, tp));
	    v[1] = plP_wcpcy(plP_w3wcy(xmax, ymax, tp));
	    u[2] = plP_wcpcx(plP_w3wcx(xmax, ymin, tp));
	    v[2] = plP_wcpcy(plP_w3wcy(xmax, ymin, tp));
	    plnxtv(u, v, 0, 3, 0);

	    tp += tick;
	}
	u[0] = plP_wcpcx(plP_w3wcx(xmax, ymax, zmin));
	v[0] = plP_wcpcy(plP_w3wcy(xmax, ymax, zmin));
	u[1] = plP_wcpcx(plP_w3wcx(xmax, ymax, zmax));
	v[1] = plP_wcpcy(plP_w3wcy(xmax, ymax, zmax));
	plnxtv(u, v, 0, 2, 0);
    }
    else if (cxx <= 0.0 && cxy <= 0.0) {
	while (tp <= zmax) {
	    u[0] = plP_wcpcx(plP_w3wcx(xmax, ymax, tp));
	    v[0] = plP_wcpcy(plP_w3wcy(xmax, ymax, tp));
	    u[1] = plP_wcpcx(plP_w3wcx(xmax, ymin, tp));
	    v[1] = plP_wcpcy(plP_w3wcy(xmax, ymin, tp));
	    u[2] = plP_wcpcx(plP_w3wcx(xmin, ymin, tp));
	    v[2] = plP_wcpcy(plP_w3wcy(xmin, ymin, tp));
	    plnxtv(u, v, 0,3, 0);

	    tp += tick;
	}
	u[0] = plP_wcpcx(plP_w3wcx(xmax, ymin, zmin));
	v[0] = plP_wcpcy(plP_w3wcy(xmax, ymin, zmin));
	u[1] = plP_wcpcx(plP_w3wcx(xmax, ymin, zmax));
	v[1] = plP_wcpcy(plP_w3wcy(xmax, ymin, zmax));
	plnxtv(u, v, 0,2, 0);
    }
    else if (cxx <= 0.0 && cxy >= 0.0) {
	while (tp <= zmax) {
	    u[0] = plP_wcpcx(plP_w3wcx(xmax, ymin, tp));
	    v[0] = plP_wcpcy(plP_w3wcy(xmax, ymin, tp));
	    u[1] = plP_wcpcx(plP_w3wcx(xmin, ymin, tp));
	    v[1] = plP_wcpcy(plP_w3wcy(xmin, ymin, tp));
	    u[2] = plP_wcpcx(plP_w3wcx(xmin, ymax, tp));
	    v[2] = plP_wcpcy(plP_w3wcy(xmin, ymax, tp));
	    plnxtv(u, v, 0,3, 0);

	    tp += tick;
	}
	u[0] = plP_wcpcx(plP_w3wcx(xmin, ymin, zmin));
	v[0] = plP_wcpcy(plP_w3wcy(xmin, ymin, zmin));
	u[1] = plP_wcpcx(plP_w3wcx(xmin, ymin, zmax));
	v[1] = plP_wcpcy(plP_w3wcy(xmin, ymin, zmax));
	plnxtv(u, v, 0,2, 0);
    }
    else if (cxx >= 0.0 && cxy >= 0.0) {
	while (tp <= zmax) {
	    u[0] = plP_wcpcx(plP_w3wcx(xmin, ymin, tp));
	    v[0] = plP_wcpcy(plP_w3wcy(xmin, ymin, tp));
	    u[1] = plP_wcpcx(plP_w3wcx(xmin, ymax, tp));
	    v[1] = plP_wcpcy(plP_w3wcy(xmin, ymax, tp));
	    u[2] = plP_wcpcx(plP_w3wcx(xmax, ymax, tp));
	    v[2] = plP_wcpcy(plP_w3wcy(xmax, ymax, tp));
	    plnxtv(u, v, 0,3, 0);

	    tp += tick;
	}
	u[0] = plP_wcpcx(plP_w3wcx(xmin, ymax, zmin));
	v[0] = plP_wcpcy(plP_w3wcy(xmin, ymax, zmin));
	u[1] = plP_wcpcx(plP_w3wcx(xmin, ymax, zmax));
	v[1] = plP_wcpcy(plP_w3wcy(xmin, ymax, zmax));
	plnxtv(u, v, 0,2, 0);
    }
    pl3upv = 1;
}

/*--------------------------------------------------------------------------*\
 * void plnxtv()
 *
 * Draw the next view of a 3-d plot. The physical coordinates of the
 * points for the next view are placed in the n points of arrays u and
 * v. The silhouette found so far is stored in the heap as a set of m peak
 * points.
 *
 * These routines dynamically allocate memory for hidden line removal.
 * Memory is allocated in blocks of 2*BINC*sizeof(PLINT) bytes.  Large
 * values of BINC give better performance but also allocate more memory
 * than is needed. If your 3D plots are very "spiky" or you are working
 * with very large matrices then you will probably want to increase BINC.
\*--------------------------------------------------------------------------*/

static void
plnxtv(PLINT *u, PLINT *v, PLFLT* c, PLINT n, PLINT init)
{
    plnxtvhi(u, v, c, n, init);

    if (pl3mode)
	plnxtvlo(u, v, c, n, init);
}

/*--------------------------------------------------------------------------*\
 * void plnxtvhi()
 *
 * Draw the top side of the 3-d plot.
\*--------------------------------------------------------------------------*/

static void
plnxtvhi(PLINT *u, PLINT *v, PLFLT* c, PLINT n, PLINT init)
{
/*
 * For the initial set of points, just display them and store them as the
 * peak points.
 */
    if (init == 1) {
	int i;
	oldhiview = (PLINT *) malloc((size_t) (2 * n * sizeof(PLINT)));
	if ( ! oldhiview)
	    myexit("plnxtvhi: Out of memory.");

	oldhiview[0] = u[0];
	oldhiview[1] = v[0];
	plP_draw3d(u[0],v[0],0,1);
	for (i = 1; i < n; i++) {
	    oldhiview[2 * i] = u[i];
	    oldhiview[2 * i + 1] = v[i];
	    plP_draw3d(u[i],v[i],i,0);
	}
	mhi = n;
	return;
    }

/*
 * Otherwise, we need to consider hidden-line removal problem. We scan
 * over the points in both the old (i.e. oldhiview[]) and new (i.e. u[]
 * and v[]) arrays in order of increasing x coordinate.  At each stage, we
 * find the line segment in the other array (if one exists) that straddles
 * the x coordinate of the point. We have to determine if the point lies
 * above or below the line segment, and to check if the below/above status
 * has changed since the last point.
 *
 * If pl3upv = 0 we do not update the view, this is useful for drawing
 * lines on the graph after we are done plotting points.  Hidden line
 * removal is still done, but the view is not updated.
 */
    xxhi = 0;
    if (pl3upv != 0) {
	newhisize = 2 * (mhi + BINC);
	if (newhiview != NULL) {
	    newhiview = 
		(PLINT *) realloc((void *) newhiview,
				  (size_t) (newhisize * sizeof(PLINT)));
	}
	else {
	    newhiview = 
		(PLINT *) malloc((size_t) (newhisize * sizeof(PLINT)));
	}
	if ( ! newhiview)
	    myexit("plnxtvhi: Out of memory.");
    }

/* Do the draw or shading with hidden line removal */

    plnxtvhi_draw(u, v, c, n);

/* Set oldhiview */

    swaphiview();
}

/*--------------------------------------------------------------------------*\
 * void plnxtvhi_draw()
 *
 * Draw the top side of the 3-d plot.
\*--------------------------------------------------------------------------*/

static void
plnxtvhi_draw(PLINT *u, PLINT *v, PLFLT* c, PLINT n)
{
    PLINT i = 0, j = 0, first = 1;
    PLINT sx1 = 0, sx2 = 0, sy1 = 0, sy2 = 0;
    PLINT su1, su2, sv1, sv2;
    PLINT cx, cy, px, py;
    PLINT seg, ptold, lstold = 0, pthi, pnewhi = 0, newhi, change, ochange = 0;

/*
 * (oldhiview[2*i], oldhiview[2*i]) is the i'th point in the old array
 * (u[j], v[j]) is the j'th point in the new array
 */

/* 
 * First attempt at 3d shading.  It works ok for simple plots, but
 * will just not draw faces, or draw them overlapping for very
 * jagged plots
 */
    while (i < mhi || j < n) {

    /*
     * The coordinates of the point under consideration are (px,py).  The
     * line segment joins (sx1,sy1) to (sx2,sy2).  "ptold" is true if the
     * point lies in the old array. We set it by comparing the x coordinates
     * of the i'th old point and the j'th new point, being careful if we
     * have fallen past the edges. Having found the point, load up the point
     * and segment coordinates appropriately.
     */

	ptold = ((oldhiview[2 * i] < u[j] && i < mhi) || j >= n);
	if (ptold) {
	    px = oldhiview[2 * i];
	    py = oldhiview[2 * i + 1];
	    seg = j > 0 && j < n;
	    if (seg) {
		sx1 = u[j - 1];
		sy1 = v[j - 1];
		sx2 = u[j];
		sy2 = v[j];
	    }
	} else {
	    px = u[j];
	    py = v[j];
	    seg = i > 0 && i < mhi;
	    if (seg) {
		sx1 = oldhiview[2 * (i - 1)];
		sy1 = oldhiview[2 * (i - 1) + 1];
		sx2 = oldhiview[2 * i];
		sy2 = oldhiview[2 * i + 1];
	    }
	}

    /*
     * Now determine if the point is higher than the segment, using the
     * logical function "above". We also need to know if it is the old view
     * or the new view that is higher. "newhi" is set true if the new view
     * is higher than the old.
     */
	if (seg)
	    pthi = plabv(px, py, sx1, sy1, sx2, sy2);
	else
	    pthi = 1;

	newhi = (ptold && !pthi) || (!ptold && pthi);
    /*
     * The last point and this point lie on different sides of
     * the current silouette
     */
	change = (newhi && !pnewhi) || (!newhi && pnewhi);

    /*
     * There is a new intersection point to put in the peak array if the
     * state of "newhi" changes.
     */
	if (first) {
	    plP_draw3d(px, py, j, 1);
	    first = 0;
	    lstold = ptold;
	    savehipoint(px, py);
	    pthi = 0;
	    ochange = 0;
	} else if (change) {

	/*
	 * Take care of special cases at end of arrays.  If pl3upv is 0 the
	 * endpoints are not connected to the old view.
	 */
	    if (pl3upv == 0 && ((!ptold && j == 0) || (ptold && i == 0))) {
		plP_draw3d(px, py, j, 1);
		lstold = ptold;
		pthi = 0;
		ochange = 0;
	    } else if (pl3upv == 0 &&
		       (( ! ptold && i >= mhi) || (ptold && j >= n))) {
		plP_draw3d(px, py, j, 1);
		lstold = ptold;
		pthi = 0;
		ochange = 0;
	    } else {

	    /*
	     * If pl3upv is not 0 then we do want to connect the current line
	     * with the previous view at the endpoints.  Also find intersection
	     * point with old view.
	     */
		if (i == 0) {
		    sx1 = oldhiview[0];
		    sy1 = -1;
		    sx2 = oldhiview[0];
		    sy2 = oldhiview[1];
		} else if (i >= mhi) {
		    sx1 = oldhiview[2 * (mhi - 1)];
		    sy1 = oldhiview[2 * (mhi - 1) + 1];
		    sx2 = oldhiview[2 * (mhi - 1)];
		    sy2 = -1;
		} else {
		    sx1 = oldhiview[2 * (i - 1)];
		    sy1 = oldhiview[2 * (i - 1) + 1];
		    sx2 = oldhiview[2 * i];
		    sy2 = oldhiview[2 * i + 1];
		}

		if (j == 0) {
		    su1 = u[0];
		    sv1 = -1;
		    su2 = u[0];
		    sv2 = v[0];
		} else if (j >= n) {
		    su1 = u[n - 1];
		    sv1 = v[n - 1];
		    su2 = u[n];
		    sv2 = -1;
		} else {
		    su1 = u[j - 1];
		    sv1 = v[j - 1];
		    su2 = u[j];
		    sv2 = v[j];
		}

	    /* Determine the intersection */

		pl3cut(sx1, sy1, sx2, sy2, su1, sv1, su2, sv2, &cx, &cy);
		if (cx == px && cy == py) {
		    if (lstold && !ochange)
			plP_draw3d(px, py, j, 1);
		    else
			plP_draw3d(px, py, j, 0);

		    savehipoint(px, py);
		    lstold = 1;
		    pthi = 0;
		} else {
		    if (lstold && !ochange)
			plP_draw3d(cx, cy, j, 1);
		    else
			plP_draw3d(cx, cy, j, 0);

		    lstold = 1;
		    savehipoint(cx, cy);
		}
		ochange = 1;

#if MJL_HACK
	    /*
	     * I tried experimenting with some of the different boundary cases
	     * here.   x08c shows some improvement, but there are errors.  SIGH.
	     * I'm beginning to thing it will take a thorough rewrite to fix
	     * this. 
	     */
#if WANT_MISSING_TRIANGLES
		if (threedshading && (j > 1)) {
		    PLINT cx0, cy0, cx1 = cx, cy1 = cy, cx2, cy2;
		    PLINT su0 = u[j-2], sv0 = v[j-2];
		    int hi0 = plabv(su0, sv0, sx1, sy1, sx2, sy2);
		    int hi1 = plabv(su1, sv1, sx1, sy1, sx2, sy2);
		    int hi2 = plabv(su2, sv2, sx1, sy1, sx2, sy2);
		    pl3cut(sx1, sy1, sx2, sy2, su0, sv0, su1, sv1, &cx0, &cy0);
		    pl3cut(sx1, sy1, sx2, sy2, su2, sv2, su0, sv0, &cx2, &cy2);

		    if ( !(cx0 == sx2 && cy0 == sy2) && 
			 !(cx0 == su1 && cy0 == sv1) &&
			 !(cx1 == sx2 && cy1 == sy2) && 
			 !(cx1 == su2 && cy1 == sv2) &&
			 !(cx2 == sx2 && cy2 == sy2) && 
			 !(cx2 == su0 && cy2 == sv0) && 1) {

			if (0 && !hi0 && !hi1 && hi2)
			    plP_fill3(cx2, cy2, cx1, cy1, su2, sv2, j);

			if (0 && hi0 && !hi1 && hi2)
			    plP_fill4(cx0, cy0, cx1, cy1, su2, sv2, su0, sv0, j);

			if (0 && hi0 && hi1 && !hi2)
			    plP_fill4(cx2, cy2, cx1, cy1, su1, sv1, su0, sv0, j);

			if (0 && !hi0 && hi1 && !hi2)
			    plP_fill3(cx0, cy0, su1, sv1, cx1, cy1, j);
		    }
		}
#endif
#endif
	    }
	}

    /* If point is high then draw plot to point and update view. */

	if (pthi) {
	    if (lstold && ptold)
		plP_draw3d(px, py, j, 1);
	    else
		plP_draw3d(px, py, j, 0);

	    savehipoint(px, py);
	    lstold = ptold;
	    ochange = 0;
	}
	pnewhi = newhi;

	if (ptold)
	    i++;
	else
	    j++;
    }
}

/*--------------------------------------------------------------------------*\
 * void  plP_draw3d()
 *
 * If threedshading is set, moves and then fills a series of triangles using
 * colours previously assigned.  Otherwise does a simple move and line draw.
\*--------------------------------------------------------------------------*/

#define MAX_POLY 3

static void
plP_draw3d(PLINT x, PLINT y, PLINT j, PLINT move)
{
    static int count = 0;
    static int vcount = 0;
    static short px[MAX_POLY], py[MAX_POLY];
#if WANT_MISSING_TRIANGLES
    if (threedshading) {
    /*
     * Use two circular buffers for coordinates.
     */
	if (move) {
	    count = vcount = 0;
	    px[count] = x;
	    py[count] = y;
	} else {
	/* 
	 * Scan for identical points already in the set.  For some reason the
	 * drawing algorithm will include the same point or segment multiple
	 * times.  Although this is harmless when covering the surface by
	 * simple lines, when shading it causes triangles to be missed. 
	 */
	    int i, numpts = MIN(count+1, MAX_POLY);
	    for (i = 0; i < numpts; i++)
		if ((px[i] == x) && (py[i] == y)) return;

	    count++; vcount++;
	    if (vcount==MAX_POLY) vcount = 0;
	    px[vcount] = x;
	    py[vcount] = y;
	    if (count+1 >= MAX_POLY) {
		plcol1(ctmp[j]);
		plP_fill(px, py, MAX_POLY);
	    }
	}
    } else {
#else
    {
#endif
	if (move)
	    plP_movphy(x, y);
	else
	    plP_draphy(x, y);
    }
}

/*--------------------------------------------------------------------------*\
 * void plP_fill3()
 *
 * Fills the polygon specified.  Just for experimentation.
\*--------------------------------------------------------------------------*/

#if MJL_HACK
static void
plP_fill3(PLINT x0, PLINT y0, PLINT x1, PLINT y1, PLINT x2, PLINT y2, PLINT j)
{
    short px[3], py[3];
    int m, n;

    px[0] = x0; py[0] = y0;
    px[1] = x1; py[1] = y1;
    px[2] = x2; py[2] = y2;
    for (m = 1; m < 3; m++)
	for (n = 0; n < m; n++)
	    if ((px[m] == px[n]) && (py[m] == py[n])) return;

    plcol1(ctmp[j]);
    plP_fill(px, py, 3);
}

/*--------------------------------------------------------------------------*\
 * void plP_fill4()
 *
 * Fills the polygon specified.  Just for experimentation.
\*--------------------------------------------------------------------------*/

static void
plP_fill4(PLINT x0, PLINT y0, PLINT x1, PLINT y1, PLINT x2, PLINT y2, PLINT x3, PLINT y3, PLINT j)
{
    short px[4], py[4];
    int m, n;

    px[0] = x0; py[0] = y0;
    px[1] = x1; py[1] = y1;
    px[2] = x2; py[2] = y2;
    px[3] = x3; py[3] = y3;
    for (m = 1; m < 4; m++)
	for (n = 0; n < m; n++)
	    if ((px[m] == px[n]) && (py[m] == py[n])) return;

    plcol1(ctmp[j]);
    plP_fill(px, py, 4);
}
#endif	/* MJL_HACK */

/*--------------------------------------------------------------------------*\
 * void plnxtvlo()
 *
 * Draw the bottom side of the 3-d plot.
\*--------------------------------------------------------------------------*/

static void
plnxtvlo(PLINT *u, PLINT *v, PLFLT*c, PLINT n, PLINT init)
{
    PLINT i, j, first;
    PLINT sx1 = 0, sx2 = 0, sy1 = 0, sy2 = 0;
    PLINT su1, su2, sv1, sv2;
    PLINT cx, cy, px, py;
    PLINT seg, ptold, lstold = 0, ptlo, pnewlo, newlo, change, ochange = 0;

    first = 1;
    pnewlo = 0;

/*
 * For the initial set of points, just display them and store them as the
 * peak points.
 */
    if (init == 1) {

	oldloview = (PLINT *) malloc((size_t) (2 * n * sizeof(PLINT)));
	if ( ! oldloview)
	    myexit("plnxtvlo: Out of memory.");

	plP_movphy(u[0], v[0]);
	oldloview[0] = u[0];
	oldloview[1] = v[0];
	for (i = 1; i < n; i++) {
	    plP_draphy(u[i], v[i]);
	    oldloview[2 * i] = u[i];
	    oldloview[2 * i + 1] = v[i];
	}
	mlo = n;
	return;
    }

/*
 * Otherwise, we need to consider hidden-line removal problem. We scan
 * over the points in both the old (i.e. oldloview[]) and new (i.e. u[]
 * and v[]) arrays in order of increasing x coordinate.  At each stage, we
 * find the line segment in the other array (if one exists) that straddles
 * the x coordinate of the point. We have to determine if the point lies
 * above or below the line segment, and to check if the below/above status
 * has changed since the last point.
 *
 * If pl3upv = 0 we do not update the view, this is useful for drawing
 * lines on the graph after we are done plotting points.  Hidden line
 * removal is still done, but the view is not updated.
 */
    xxlo = 0;
    i = 0;
    j = 0;
    if (pl3upv != 0) {
	newlosize = 2 * (mlo + BINC);
	if (newloview != NULL) {
	    newloview = 
		(PLINT *) realloc((void *) newloview,
				  (size_t) (newlosize * sizeof(PLINT)));
	}
	else {
	    newloview = 
		(PLINT *) malloc((size_t) (newlosize * sizeof(PLINT)));
	}
	if ( ! newloview)
	    myexit("plnxtvlo: Out of memory.");
    }

/*
 * (oldloview[2*i], oldloview[2*i]) is the i'th point in the old array
 * (u[j], v[j]) is the j'th point in the new array.
 */
    while (i < mlo || j < n) {

    /*
     * The coordinates of the point under consideration are (px,py).  The
     * line segment joins (sx1,sy1) to (sx2,sy2).  "ptold" is true if the
     * point lies in the old array. We set it by comparing the x coordinates
     * of the i'th old point and the j'th new point, being careful if we
     * have fallen past the edges. Having found the point, load up the point
     * and segment coordinates appropriately.
     */
	ptold = ((oldloview[2 * i] < u[j] && i < mlo) || j >= n);
	if (ptold) {
	    px = oldloview[2 * i];
	    py = oldloview[2 * i + 1];
	    seg = j > 0 && j < n;
	    if (seg) {
		sx1 = u[j - 1];
		sy1 = v[j - 1];
		sx2 = u[j];
		sy2 = v[j];
	    }
	}
	else {
	    px = u[j];
	    py = v[j];
	    seg = i > 0 && i < mlo;
	    if (seg) {
		sx1 = oldloview[2 * (i - 1)];
		sy1 = oldloview[2 * (i - 1) + 1];
		sx2 = oldloview[2 * i];
		sy2 = oldloview[2 * i + 1];
	    }
	}

    /*
     * Now determine if the point is lower than the segment, using the
     * logical function "above". We also need to know if it is the old view
     * or the new view that is lower. "newlo" is set true if the new view is
     * lower than the old.
     */
	if (seg)
	    ptlo = !plabv(px, py, sx1, sy1, sx2, sy2);
	else
	    ptlo = 1;

	newlo = (ptold && !ptlo) || (!ptold && ptlo);
	change = (newlo && !pnewlo) || (!newlo && pnewlo);

    /*
     * There is a new intersection point to put in the peak array if the
     * state of "newlo" changes.
     */
	if (first) {
	    plP_movphy(px, py);
	    first = 0;
	    lstold = ptold;
	    savelopoint(px, py);
	    ptlo = 0;
	    ochange = 0;
	}
	else if (change) {

	/*
	 * Take care of special cases at end of arrays.  If pl3upv is 0 the
	 * endpoints are not connected to the old view.
	 */
	    if (pl3upv == 0 && ((!ptold && j == 0) || (ptold && i == 0))) {
		plP_movphy(px, py);
		lstold = ptold;
		ptlo = 0;
		ochange = 0;
	    }
	    else if (pl3upv == 0 &&
		     (( ! ptold && i >= mlo) || (ptold && j >= n))) {
		plP_movphy(px, py);
		lstold = ptold;
		ptlo = 0;
		ochange = 0;
	    }

	/*
	 * If pl3upv is not 0 then we do want to connect the current line
	 * with the previous view at the endpoints.  Also find intersection
	 * point with old view.
	 */
	    else {
		if (i == 0) {
		    sx1 = oldloview[0];
		    sy1 = 100000;
		    sx2 = oldloview[0];
		    sy2 = oldloview[1];
		}
		else if (i >= mlo) {
		    sx1 = oldloview[2 * (mlo - 1)];
		    sy1 = oldloview[2 * (mlo - 1) + 1];
		    sx2 = oldloview[2 * (mlo - 1)];
		    sy2 = 100000;
		}
		else {
		    sx1 = oldloview[2 * (i - 1)];
		    sy1 = oldloview[2 * (i - 1) + 1];
		    sx2 = oldloview[2 * i];
		    sy2 = oldloview[2 * i + 1];
		}

		if (j == 0) {
		    su1 = u[0];
		    sv1 = 100000;
		    su2 = u[0];
		    sv2 = v[0];
		}
		else if (j >= n) {
		    su1 = u[n - 1];
		    sv1 = v[n - 1];
		    su2 = u[n];
		    sv2 = 100000;
		}
		else {
		    su1 = u[j - 1];
		    sv1 = v[j - 1];
		    su2 = u[j];
		    sv2 = v[j];
		}

	    /* Determine the intersection */

		pl3cut(sx1, sy1, sx2, sy2, su1, sv1, su2, sv2, &cx, &cy);
		if (cx == px && cy == py) {
		    if (lstold && !ochange)
			plP_movphy(px, py);
		    else
			plP_draphy(px, py);

		    savelopoint(px, py);
		    lstold = 1;
		    ptlo = 0;
		}
		else {
		    if (lstold && !ochange)
			plP_movphy(cx, cy);
		    else
			plP_draphy(cx, cy);

		    lstold = 1;
		    savelopoint(cx, cy);
		}
		ochange = 1;
	    }
	}

    /* If point is low then draw plot to point and update view. */

	if (ptlo) {
	    if (lstold && ptold)
		plP_movphy(px, py);
	    else
		plP_draphy(px, py);

	    savelopoint(px, py);
	    lstold = ptold;
	    ochange = 0;
	}

	pnewlo = newlo;

	if (ptold)
	    i = i + 1;
	else
	    j = j + 1;
    }

/* Set oldloview */

    swaploview();
}

/*--------------------------------------------------------------------------*\
 * savehipoint
 * savelopoint
 *
 * Add a point to the list of currently visible peaks/valleys, when
 * drawing the top/bottom surface, respectively.
\*--------------------------------------------------------------------------*/

static void
savehipoint(PLINT px, PLINT py)
{
    if (pl3upv == 0)
	return;

    if (xxhi >= newhisize) {	/* allocate additional space */
	newhisize += 2 * BINC;
	newhiview = (PLINT *) realloc((void *) newhiview,
				      (size_t) (newhisize * sizeof(PLINT)));
	if ( ! newhiview) 
	    myexit("savehipoint: Out of memory.");
    }

    newhiview[xxhi] = px;
    xxhi++;
    newhiview[xxhi] = py;
    xxhi++;
}

static void
savelopoint(PLINT px, PLINT py)
{
    if (pl3upv == 0)
	return;

    if (xxlo >= newlosize) {	/* allocate additional space */
	newlosize += 2 * BINC;
	newloview = (PLINT *) realloc((void *) newloview,
				      (size_t) (newlosize * sizeof(PLINT)));
	if ( ! newloview)
	    myexit("savelopoint: Out of memory.");
    }

    newloview[xxlo] = px;
    xxlo++;
    newloview[xxlo] = py;
    xxlo++;
}

/*--------------------------------------------------------------------------*\
 * swaphiview
 * swaploview
 *
 * Swaps the top/bottom views.  Need to do a real swap so that the 
 * memory cleanup routine really frees everything (and only once).
\*--------------------------------------------------------------------------*/

static void
swaphiview(void)
{
    PLINT *tmp;

    if (pl3upv != 0) {
	mhi = xxhi / 2;
	tmp = oldhiview;
	oldhiview = newhiview;
	newhiview = tmp;
    }
}

static void
swaploview(void)
{
    PLINT *tmp;

    if (pl3upv != 0) {
	mlo = xxlo / 2;
	tmp = oldloview;
	oldloview = newloview;
	newloview = tmp;
    }
}

/*--------------------------------------------------------------------------*\
 * freework
 *
 * Frees memory associated with work arrays
\*--------------------------------------------------------------------------*/

static void
freework(void)
{
    free_mem(oldhiview);
    oldhiview = NULL;
    free_mem(oldloview);
    oldloview  = NULL;
    free_mem(newhiview);
    newhiview = NULL;
    free_mem(newloview);
    newloview = NULL;
    free_mem(vtmp);
    vtmp = NULL;
    free_mem(utmp);
    utmp = NULL;
    if(ctmp) {
	free_mem(ctmp);
    }
    ctmp = 0;
}

/*--------------------------------------------------------------------------*\
 * myexit
 *
 * Calls plexit, cleaning up first.
\*--------------------------------------------------------------------------*/

static void
myexit(char *msg)
{
    freework();
    plexit(msg);
}

/*--------------------------------------------------------------------------*\
 * myabort
 *
 * Calls plabort, cleaning up first.
 * Caller should return to the user program.
\*--------------------------------------------------------------------------*/

static void
myabort(char *msg)
{
    freework();
    plabort(msg);
}

/*--------------------------------------------------------------------------*\
 * int plabv()
 *
 * Determines if point (px,py) lies above the line joining (sx1,sy1) to
 * (sx2,sy2). It only works correctly if sx1 <= px <= sx2.
\*--------------------------------------------------------------------------*/

static int
plabv(PLINT px, PLINT py, PLINT sx1, PLINT sy1, PLINT sx2, PLINT sy2)
{
    int above;

    if (py >= sy1 && py >= sy2)
	above = 1;
    else if (py < sy1 && py < sy2)
	above = 0;
    else if ((double) (sx2 - sx1) * (py - sy1) >=
	     (double) (px - sx1) * (sy2 - sy1))
	above = 1;
    else
	above = 0;

    return above;
}

/*--------------------------------------------------------------------------*\
 * void pl3cut()
 *
 * Determines the point of intersection (cx,cy) between the line joining
 * (sx1,sy1) to (sx2,sy2) and the line joining (su1,sv1) to (su2,sv2).
\*--------------------------------------------------------------------------*/

static void
pl3cut(PLINT sx1, PLINT sy1, PLINT sx2, PLINT sy2,
       PLINT su1, PLINT sv1, PLINT su2, PLINT sv2, PLINT *cx, PLINT *cy)
{
    PLINT x21, y21, u21, v21, yv1, xu1, a, b;
    double fa, fb;

    x21 = sx2 - sx1;
    y21 = sy2 - sy1;
    u21 = su2 - su1;
    v21 = sv2 - sv1;
    yv1 = sy1 - sv1;
    xu1 = sx1 - su1;

    a = x21 * v21 - y21 * u21;
    fa = (double) a;
    if (a == 0) {
	if (sx2 < su2) {
	    *cx = sx2;
	    *cy = sy2;
	}
	else {
	    *cx = su2;
	    *cy = sv2;
	}
	return;
    }
    else {
	b = yv1 * u21 - xu1 * v21;
	fb = (double) b;
	*cx = (PLINT) (sx1 + (fb * x21) / fa + .5);
	*cy = (PLINT) (sy1 + (fb * y21) / fa + .5);
    }
}
