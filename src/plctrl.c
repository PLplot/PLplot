/* $Id$
   $Log$
   Revision 1.1  1993/01/23 05:51:56  mjl
   Added for the high level routines that don't result in something being
   plotted ("control" routines) that don't need direct access to the stream
   or dispatch table data.

*/

/*	plctrl.c

	Misc. control routines, like begin, end, exit, change graphics/text
	mode, change color.
*/

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*----------------------------------------------------------------------*\
* void plbeg()
*
* Set up the device "dev" for plotting, dividing the page into
* "nx" by "ny" subpages.
\*----------------------------------------------------------------------*/

void
c_plbeg(PLINT dev, PLINT nx, PLINT ny)
{
    PLFLT scale, def, ht;

    if ((nx <= 0) || (ny <= 0))
	plexit("plbeg: Cannot have negative number of subpages.");

    scale = 1.0 / sqrt((double) ny);
    grbeg(dev);
    ssub(nx, ny, 0);

    /* Set up character, symbol and tick sizes for requested number of */
    /* subpages */

    gchr(&def, &ht);
    schr((PLFLT) (def * scale), (PLFLT) (def * scale));
    gsym(&def, &ht);
    ssym((PLFLT) (def * scale), (PLFLT) (def * scale));
    gmaj(&def, &ht);
    smaj((PLFLT) (def * scale), (PLFLT) (def * scale));
    gmin(&def, &ht);
    smin((PLFLT) (def * scale), (PLFLT) (def * scale));
}

/*----------------------------------------------------------------------*\
* void plend()
*
* End a plotting session for all open streams.
\*----------------------------------------------------------------------*/

void
c_plend()
{
    PLINT i;

    for (i = 0; i < PL_NSTREAMS; i++) {
	plsstrm(i);
	c_plend1();
    }
    plsstrm(0);
    plfontrel();
}

/*----------------------------------------------------------------------*\
* void plend1()
*
* End a plotting session for the current stream only.
\*----------------------------------------------------------------------*/

void
c_plend1()
{
    PLINT level;

    glev(&level);
    if (level == 0)
	return;
    grtidy();
    slev(0);
}

/*----------------------------------------------------------------------*\
* void plexit()
*
* In case of an abort this routine is called.  It just prints out an
* error message and trys to clean up as much as possible.
\*----------------------------------------------------------------------*/

void
plexit(char *errormsg)
{
    plend();
    if (*errormsg != '\0') {
	fprintf(stderr, "\n*** PLPLOT ERROR ***\n");
	fprintf(stderr, "%s\n", errormsg);
    }
    fprintf(stderr, "Program aborted\n");
    pl_exit();
    exit(1);
}

/*----------------------------------------------------------------------*\
* void pl_exit()
*
* A stub.  The user should write his/her own pl_exit() routine, if cleanup
* needs to be done in the user program.
\*----------------------------------------------------------------------*/

void
pl_exit(void)
{
}

/*----------------------------------------------------------------------*\
* void plgra()
*
* Switches to graphics mode.
\*----------------------------------------------------------------------*/

void
c_plgra()
{
    PLINT level;
    glev(&level);
    if (level < 1)
	plexit("plgra: Please call plstar first.");
    grgra();
}

/*----------------------------------------------------------------------*\
* void pltext()
*
* Switches back to text mode.
\*----------------------------------------------------------------------*/

void
c_pltext()
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("pltext: Please call plstar first.");

    grtext();
}

/*----------------------------------------------------------------------*\
* void plhls()
*
* Set line color by hue, lightness, and saturation.
* Convert hls color coordinates to rgb, then call plrgb.
\*----------------------------------------------------------------------*/

void
c_plhls(PLFLT h, PLFLT l, PLFLT s)
{
    PLFLT r, g, b;

    plHLS_RGB(h, l, s, &r, &g, &b);
    plrgb(r, g, b);
}

/*----------------------------------------------------------------------*\
* void value()
*
* Auxiliary function used by plHLS_RGB().
\*----------------------------------------------------------------------*/

static float
value(double n1, double n2, double hue)
{
    float val;

    while (hue >= 360.)
	hue -= 360.;
    while (hue < 0.)
	hue += 360.;

    if (hue < 60.)
	val = n1 + (n2 - n1) * hue / 60.;
    else if (hue < 180.)
	val = n2;
    else if (hue < 240.)
	val = n1 + (n2 - n1) * (240. - hue) / 60.;
    else
	val = n1;

    return (val);
}

/*----------------------------------------------------------------------*\
* void plHLS_RGB()
*
* Convert HLS color to RGB color.
* Bounds on each coordinate:
*	hue		[0., 360.]	degrees
*	lightness	[0., 1.]	magnitude
*	saturation	[0., 1.]	magnitude
*
*  Hue is always mapped onto the interval [0., 360.] regardless of input.
\*----------------------------------------------------------------------*/

void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b)
{
    float m1, m2;

    if (l <= .5)
	m2 = l * (s + 1.);
    else
	m2 = l + s - l * s;

    m1 = 2 * l - m2;

    *p_r = value(m1, m2, h + 120.);
    *p_g = value(m1, m2, h);
    *p_b = value(m1, m2, h - 120.);
}
