/* $Id$

	Mesh plot demo.
*/

#include "plcdemos.h"

#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Datat points in y */
#define LEVELS 10 

static int opt[] = { DRAW_LINEXY, DRAW_LINEXY};

static PLFLT alt[] = {33.0, 17.0};
static PLFLT az[] = {24.0, 115.0};

static char *title[4] =
{
    "#frPLplot Example 11 - Alt=33, Az=17, Opt=3",
    "#frPLplot Example 11 - Alt=24, Az=115, Opt=3",
};

static void
cmap1_init()
{
  PLFLT i[2], h[2], l[2], s[2];

  i[0] = 0.0;		/* left boundary */
  i[1] = 1.0;		/* right boundary */

  h[0] = 240; /* blue -> green -> yellow -> */
  h[1] = 0;   /* -> red */

  l[0] = 0.6;
  l[1] = 0.6;

  s[0] = 0.8;
  s[1] = 0.8;

  plscmap1n(256);
  c_plscmap1l(0, 2, i, h, l, s, NULL);
}

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of mesh plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
  int i, j, k;
  PLFLT *x, *y, **z;
  PLFLT xx, yy;
  int nlevel = LEVELS;
  PLFLT clevel[LEVELS];
  PLFLT zmin, zmax, step;

  /* Parse and process command line arguments */

  (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

  /* Initialize plplot */

  plinit();

  x = (PLFLT *) malloc(XPTS * sizeof(PLFLT));
  y = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
  z = (PLFLT **) malloc(XPTS * sizeof(PLFLT *));
  for (i = 0; i < XPTS; i++) {
    z[i] = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
    x[i] = 3. * (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
  }

  for (i = 0; i < YPTS; i++)
    y[i] = 3.* (double) (i - (YPTS / 2)) / (double) (YPTS / 2);

  for (i = 0; i < XPTS; i++) {
    xx = x[i];
    for (j = 0; j < YPTS; j++) {
      yy = y[j];
      z[i][j] = 3. * (1.-xx)*(1.-xx) * exp(-(xx*xx) - (yy+1.)*(yy+1.)) -
	10. * (xx/5. - pow(xx,3.) - pow(yy,5.)) * exp(-xx*xx-yy*yy) -
	1./3. * exp(-(xx+1)*(xx+1) - (yy*yy));
		 
      if(0) { /* Jungfraujoch/Interlaken */
	if (z[i][j] < -1.)
	  z[i][j] = -1.;
      }
    }
  }

  plMinMax2dGrid(z, XPTS, YPTS, &zmax, &zmin);  
  step = (zmax - zmin)/(nlevel+1);
  for (i=0; i<nlevel; i++)
    clevel[i] = zmin + step + step*i;

  cmap1_init();
  for (k = 0; k < 2; k++) {
    for (i=0; i<4; i++) {
      pladv(0);
      plcol0(1);
      plvpor(0.0, 1.0, 0.0, 0.9);
      plwind(-1.0, 1.0, -1.0, 1.5);
      plw3d(1.0, 1.0, 1.2, -3.0, 3.0, -3.0, 3.0, -7., 8., alt[k], az[k]);
      plbox3("bnstu", "x axis", 0.0, 0,
	     "bnstu", "y axis", 0.0, 0,
	     "bcdmnstuv", "z axis", 0.0, 4);

      plcol0(2);

      /* wireframe plot */
      if (i==0)
	plmesh(x, y, z, XPTS, YPTS, opt[k]);

      /* magnitude colored wireframe plot */
      else if (i==1)
	plmesh(x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR);

      /* magnitude colored wireframe plot with sides */
      else if (i==2)
	plot3d(x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR, 1);

      /* magnitude colored wireframe plot with base contour */
      else if (i==3)
	plmeshc(x, y, z, XPTS, YPTS, opt[k] | MAG_COLOR | BASE_CONT,
		clevel, nlevel);

      plcol0(3);
      plmtex("t", 1.0, 0.5, 0.5, title[k]);
    }
  }

  plend();
  exit(0);
}
