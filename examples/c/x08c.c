/* $Id$

	3-d plot demo.
*/

#include "plplot/plcdemos.h"

#define XPTS    35		/* Data points in x */
#define YPTS    46		/* Datat points in y */

static int opt[] = {1, 2, 3, 3};
static PLFLT alt[] = {60.0, 20.0, 60.0, 60.0};
static PLFLT az[] = {30.0, 60.0, 120.0, 160.0};
static void cmap1_init(int);

static char *title[4] =
{
    "#frPLplot Example 8 - Alt=60, Az=30",
    "#frPLplot Example 8 - Alt=20, Az=60",
    "#frPLplot Example 8 - Alt=60, Az=120",
    "#frPLplot Example 8 - Alt=60, Az=160"
};

/*--------------------------------------------------------------------------*\
 * cmap1_init1
 *
 * Initializes color map 1 in HLS space.
 * Basic grayscale variation from half-dark (which makes more interesting
 * looking plot compared to dark) to light.
 * An interesting variation on this:
 *	s[1] = 1.0
\*--------------------------------------------------------------------------*/

static void
cmap1_init(int gray)
{
  PLFLT i[2], h[2], l[2], s[2];

  i[0] = 0.0;		/* left boundary */
  i[1] = 1.0;		/* right boundary */

  if (gray) {
    h[0] = 0.0;		/* hue -- low: red (arbitrary if s=0) */
    h[1] = 0.0;		/* hue -- high: red (arbitrary if s=0) */

    l[0] = 0.5;		/* lightness -- low: half-dark */
    l[1] = 1.0;		/* lightness -- high: light */

    s[0] = 0.0;		/* minimum saturation */
    s[1] = 0.0;		/* minimum saturation */
  } else {
    h[0] = 240; /* blue -> green -> yellow -> */
    h[1] = 0;   /* -> red */

    l[0] = 0.6;
    l[1] = 0.6;

    s[0] = 0.8;
    s[1] = 0.8;
  }

  plscmap1n(256);
  c_plscmap1l(0, 2, i, h, l, s, NULL);
}

/*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of 3-d plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*/

int
main(int argc, char *argv[])
{
  int i, j, k;
  PLFLT *x, *y, **z;
  PLFLT xx, yy, r;
  PLINT ifshade;

  /* Parse and process command line arguments */

  (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

  /* Initialize plplot */

  plinit();

  x = (PLFLT *) malloc(XPTS * sizeof(PLFLT));
  y = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
  z = (PLFLT **) malloc(XPTS * sizeof(PLFLT *));

  for (i = 0; i < XPTS; i++) {
    z[i] = (PLFLT *) malloc(YPTS * sizeof(PLFLT));
    x[i] = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
  }

  for (i = 0; i < YPTS; i++)
    y[i] = (double) (i - (YPTS / 2)) / (double) (YPTS / 2);

  for (i = 0; i < XPTS; i++) {
    xx = x[i];
    for (j = 0; j < YPTS; j++) {
      yy = y[j];
      r = sqrt(xx * xx + yy * yy);
      z[i][j] = exp(-r * r) * cos(2.0 * PI * r);
    }
  }

  pllightsource(1.,1.,1.);
    	
  for (k = 0; k < 4; k++) {
    for (ifshade = 0; ifshade < 4; ifshade++) {
      pladv(0);
      plvpor(0.0, 1.0, 0.0, 0.9);
      plwind(-1.0, 1.0, -0.9, 1.1);
      plcol0(1);
      plmtex("t", 1.0, 0.5, 0.5, title[k]);
      plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
      plbox3("bnstu", "x axis", 0.0, 0,
	     "bnstu", "y axis", 0.0, 0,
	     "bcdmnstuv", "z axis", 0.0, 0);
      plcol0(2);

      if (ifshade == 0)        /* wireframe plot */
	plot3d(x, y, z, XPTS, YPTS, opt[k], 1);
      else if (ifshade == 1) { /* light difused shaded plot */
	cmap1_init(1);
	plotsh3d(x, y, z, XPTS, YPTS, 0);
      } else {                 /* false color plot */
	cmap1_init(0);
	plotfc3d(x, y, z, XPTS, YPTS, 0);
	if (ifshade == 3) {    /* add wireframe to false color plot */
	  plcol0(0);
	  plot3d(x, y, z, XPTS, YPTS, opt[k], 0);
	}
      }
    }   
  }

  plend();
  exit(0);
}
