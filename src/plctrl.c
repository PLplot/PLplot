/* $Id$
 * $Log$
 * Revision 1.12  1994/03/23 08:11:11  mjl
 * Many functions moved into this file (from plcore.c):
 *
 * 	plwarn plexit plcol0 plcol1 plrgb plrgb1 plscolbg plscol0 plgcol0
 * 	plscmap1 plscmap1f1 plscolor
 *
 * Significant changes made to operation of old routines that manipulate the
 * color map.  plscmap1l added to set color map 1 colors using a piece-wise
 * linear relationship between intensity [0,1] (cmap 1 index) and position in
 * HLS or RGB color space.  plscmap0n added to determine the number of colors
 * to use in cmap 0 (must be called before plinit, or cmap 0 allocation).
 * This helps free up unused colors that can be used in cmap 1.
 *
 * Color 0 (in cmap 0) now defaults to black.  This is done for consistency
 * with 16 color tek displays, where color 0 is the background.  You can
 * always change it back if you want the old way (in fact, you are encouraged
 * to not rely on the default color map now that there's a supported way of
 * changing it).
 *
 * plabort() added to be almost identical to plwarn(), except for message
 * content.  Now used before a return on most error conditions.
 *
 * Revision 1.11  1993/12/08  06:22:23  mjl
 * Fix for dos386/djgpp.
 *
 * Revision 1.10  1993/11/15  08:39:05  mjl
 * Removed plexit().
 *
 * Revision 1.9  1993/11/07  09:08:14  mjl
 * Added user-settable exit handler (call plsexit to set).
*/

/*	plctrl.c

	Misc. control routines, like begin, end, exit, change graphics/text
	mode, change color.  Includes some spillage from plcore.c.  If you
	don't know where it should go, put it here.  
*/

#include "plplotP.h"
#include "plstream.h"
#include <string.h>

#ifdef __GO32__			/* dos386/djgpp */
#ifdef __unix
#undef __unix
#endif
#endif

#ifdef __unix
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif

/* Static functions */

static void	strcat_delim	(char *);
static void	(*exit_handler) (void);
static void	plHLS_RGB	(PLFLT h, PLFLT l, PLFLT s,
				 PLFLT *p_r, PLFLT *p_g, PLFLT *p_b);

/* Stream pointer.  */
/* Fetched by some routines to directly set plplot state data */

static PLStream	*plsc;

/*----------------------------------------------------------------------*\
*  Routines that deal with colors & color maps.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* plcol0()
*
* Set color, map 0.  Argument is integer between 0 and 15.
\*----------------------------------------------------------------------*/

void
c_plcol0(PLINT icol0)
{
    plgpls(&plsc);

    if (plsc->level < 1) {
	plabort("plcol0: Please call plinit first");
	return;
    }
    if (icol0 < 0 || icol0 > 15) {
	plabort("plcol0: Invalid color.");
	return;
    }
    if (plsc->cmap0setcol[icol0] == 0) {
	plabort("plcol0: Requested color not allocated.");
	return;
    }

    plsc->icol0 = icol0;
    plsc->curcolor.r = plsc->cmap0[icol0].r;
    plsc->curcolor.g = plsc->cmap0[icol0].g;
    plsc->curcolor.b = plsc->cmap0[icol0].b;

    plsc->curcmap = 0;
    plP_state(PLSTATE_COLOR0);
}

/*----------------------------------------------------------------------*\
* plcol1()
*
* Set color, map 1.  Argument is a float between 0. and 1.
\*----------------------------------------------------------------------*/

void
c_plcol1(PLFLT col1)
{
    PLINT icol1;

    plgpls(&plsc);

    if (plsc->level < 1) {
	plabort("plcol1: Please call plinit first");
	return;
    }
    if (col1 < 0 || col1 > 1) {
	plabort("plcol1: Invalid color.");
	return;
    }

    icol1 = col1 * plsc->ncol1;
    icol1 = MIN(icol1, plsc->ncol1-1);

    plsc->icol1 = icol1;
    plsc->curcolor.r = plsc->cmap1[plsc->icol1].r;
    plsc->curcolor.g = plsc->cmap1[plsc->icol1].g;
    plsc->curcolor.b = plsc->cmap1[plsc->icol1].b;

    plsc->curcmap = 1;
    plP_state(PLSTATE_COLOR1);
}

/*----------------------------------------------------------------------*\
* plscolbg()
*
* Set the background color by 8 bit RGB value
* Note: for some drivers this corresponds to a cmap 0 color
\*----------------------------------------------------------------------*/

void
c_plscolbg(PLINT r, PLINT g, PLINT b)
{
    plgpls(&plsc);

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plabort("plscolbg: Invalid color");
	return;
    }

    plsc->bgcolor.r = r;
    plsc->bgcolor.g = g;
    plsc->bgcolor.b = b;
    plsc->bgcolorset = 1;
}

/*----------------------------------------------------------------------*\
* plgcolbg()
*
* Returns the background color by 8 bit RGB value
* Note: for some drivers this corresponds to a cmap 0 color
\*----------------------------------------------------------------------*/

void
c_plgcolbg(PLINT *r, PLINT *g, PLINT *b)
{
    plgpls(&plsc);

    *r = plsc->bgcolor.r;
    *g = plsc->bgcolor.g;
    *b = plsc->bgcolor.b;
}

/*----------------------------------------------------------------------*\
* plscol0()
*
* Set a given color from color map 0 by 8 bit RGB value
* Does not result in any additional cells to be allocated.
\*----------------------------------------------------------------------*/

void
c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b)
{
    plgpls(&plsc);

    if (icol0 < 0 || icol0 > 15) {
	plabort("plscol0: Illegal color table value");
	return;
    }
    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plabort("plscol0: Invalid color");
	return;
    }

    plsc->cmap0[icol0].r = r;
    plsc->cmap0[icol0].g = g;
    plsc->cmap0[icol0].b = b;
    plsc->cmap0setcol[icol0] = 1;

    if (plsc->level > 0)
	plP_state(PLSTATE_CMAP0);
}

/*----------------------------------------------------------------------*\
* plgcol0()
*
* Returns 8 bit RGB values for given color from color map 0
* Values are negative if an invalid color id is given
\*----------------------------------------------------------------------*/

void
c_plgcol0(PLINT icol0, PLINT *r, PLINT *g, PLINT *b)
{
    plgpls(&plsc);

    *r = -1;
    *g = -1;
    *b = -1;

    if (icol0 < 0 || icol0 > 15) {
	plabort("plgcol0: Invalid color index");
	return;
    }
    if (plsc->cmap0setcol[icol0] == 0) {
	plabort("plgcol0: Requested color not allocated");
	return;
    }

    *r = plsc->cmap0[icol0].r;
    *g = plsc->cmap0[icol0].g;
    *b = plsc->cmap0[icol0].b;
    return;
}

/*----------------------------------------------------------------------*\
* plscmap0n()
*
* Set number of colors in cmap 0
* Can only be called once, prior to cmap 0 initialization.
\*----------------------------------------------------------------------*/

void
c_plscmap0n(PLINT ncol0)
{
    plgpls(&plsc);

    if (plsc->ncol0 > 0) {
	plabort("plscmap0n: Must be called before cmap 0 allocation");
	return;
    }
    if (ncol0 > 16 || ncol0 < 1) {
	plabort("plscmap0n: Number of colors must be between 1 and 16");
	return;
    }

    plsc->ncol0 = ncol0;
}

/*----------------------------------------------------------------------*\
* plscmap1n()
*
* Set number of colors in cmap 1
* Can only be called once, prior to cmap 1 initialization.
\*----------------------------------------------------------------------*/

void
c_plscmap1n(PLINT ncol1)
{
    plgpls(&plsc);

    if (plsc->ncol1 > 0) {
	plabort("plscmap1n: Must be called before cmap 1 allocation");
	return;
    }
    if (ncol1 > 256 || ncol1 < 1) {
	plabort("plscmap1n: Number of colors must be between 1 and 256");
	return;
    }

    plsc->ncol1 = ncol1;
}

/*----------------------------------------------------------------------*\
* plscmap0()
*
* Set color map 0 colors by 8 bit RGB values
* Note -- this sets the entire map.  ncol0 is set ONLY if not already set.
\*----------------------------------------------------------------------*/

void
c_plscmap0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0)
{
    int i;

    plgpls(&plsc);

    if (plsc->level == 0) {
	plabort("plscmap0: Must be called before plinit");
	return;
    }

    if (plsc->ncol0 == 0)
	plscmap0n(ncol0);

    else {
	if (ncol0 != plsc->ncol0) {
	    plabort("plscmap0: Must set entire color map");
	    return;
	}
    }

    for (i = 0; i < plsc->ncol0; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    fprintf(stderr, "plscmap0: Invalid RGB color: %d, %d, %d\n",
		    r[i], g[i], b[i]);

	    plabort("plscmap0: Invalid color");
	    return;
	}

	plsc->cmap0[i].r = r[i];
	plsc->cmap0[i].g = g[i];
	plsc->cmap0[i].b = b[i];
	plsc->cmap0setcol[i] = 1;
    }

    if (plsc->level > 0)
	plP_state(PLSTATE_CMAP0);
}

/*----------------------------------------------------------------------*\
* plscmap1()
*
* Set color map 1 colors by 8 bit RGB values
* Note -- this sets the entire map, including ncol1.
* May be called at any time.
\*----------------------------------------------------------------------*/

void
c_plscmap1(PLINT *r, PLINT *g, PLINT *b, PLINT ncol1)
{
    int i;

    plgpls(&plsc);

    if (plsc->ncol1 == 0)
	plscmap1n(ncol1);

    else {
	if (ncol1 != plsc->ncol1) {
	    plabort("plscmap1: Must set entire color map");
	    return;
	}
    }

    for (i = 0; i < plsc->ncol1; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    fprintf(stderr, "plscmap1: Invalid RGB color: %d, %d, %d\n",
		    r[i], g[i], b[i]);

	    plabort("plscmap1: Invalid color");
	    return;
	}
	plsc->cmap1[i].r = r[i];
	plsc->cmap1[i].g = g[i];
	plsc->cmap1[i].b = b[i];
    }

    plsc->cmap1set = 1;
    if (plsc->level > 0)
	plP_state(PLSTATE_CMAP1);
}

/*----------------------------------------------------------------------*\
* plscmap1l()
*
* Set color map 1 colors using a piece-wise linear relationship between
* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space.
* May be called at any time.
*
* The idea here is to specify a number of control points that specify the
* mapping between HLS (or RGB) and palette 1 value.  Between these points,
* linear interpolation is used.  This gives a smooth variation of color
* with intensity.  Any number of control points may be specified, located
* at arbitrary intensities, although typically 2 - 4 are enough.  Another
* way of stating this is that we are traversing a given number of lines
* through HLS (or RGB) space as we move through cmap 1 entries.  The
* control points at the minimum and maximum intensity (0 and 1) must
* always be specified.  By adding more control points you can get more
* variation.  One good technique for plotting functions that vary about
* some expected average is to use an additional 2 control points in the
* center (intensity ~= 0.5) that are the same color as the background
* (typically white for paper output, black for crt), and same hue as the
* boundary control points.  This allows the highs and lows to be very
* easily distinguished.
*
* Each control point must specify the position in cmap 1 as well as
* three coordinates in HLS or RGB space.  The first point MUST correspond
* to intensity = 0, and the last to intensity = 1.
*
* Bounds on RGB coordinates:
*	R,G,B		[0, 1]		magnitude
*
* Bounds on HLS coordinates:
*	hue		[0, 360]	degrees
*	lightness	[0, 1]		magnitude
*	saturation	[0, 1]		magnitude
*
* The inputs are:
*	itype		0: RGB, 1: HLS
*	npts		number of control points
*	intensity[]	intensity for each control point
*	coord1[]	first coordinate for each control point
*	coord2[]	second coordinate for each control point
*	coord3[]	third coordinate for each control point 
\*----------------------------------------------------------------------*/

void
c_plscmap1l(PLINT itype, PLINT npts, PLFLT *intensity,
	    PLFLT *coord1, PLFLT *coord2, PLFLT *coord3)
{
    int i, n;
    PLFLT icmap1, delta;
    PLFLT h, l, s, r, g, b;

    plgpls(&plsc);

    if (npts < 2) {
	plabort("plscmap1l: Must specify at least two control points");
	return;
    }

    if ( (intensity[0] != 0) || (intensity[npts-1] != 1)) {
	plabort("plscmap1l: First and last control points must lie on boundary");
	return;
    }

    if (plsc->ncol1 == 0)
	plsc->ncol1 = 256;

    for (n = 0; n < npts-1; n++) {
	for (i = 0; i < plsc->ncol1; i++) {
	    icmap1 = (double) i / (plsc->ncol1 - 1.0);
	    if ( ! (intensity[n] <= icmap1 && icmap1 <= intensity[n+1]) )
		continue;

	    delta = (icmap1 - intensity[n]) / (intensity[n+1] - intensity[n]);
	    r = coord1[n] + (coord1[n+1] - coord1[n]) * delta;
	    g = coord2[n] + (coord2[n+1] - coord2[n]) * delta;
	    b = coord3[n] + (coord3[n+1] - coord3[n]) * delta;

	    if (itype == 0) {
		h = r;
		l = g;
		s = b;
		plHLS_RGB(h, l, s, &r, &g, &b);
	    }

	    if ((r < 0 || r > 1) || (g < 0 || g > 1) || (b < 0 || b > 1)) {
		fprintf(stderr, "plscmap1l: Invalid RGB color: %f, %f, %f\n",
			r, g, b);
		plabort("plscmap1l: Invalid color");
		return;
	    }
	    plsc->cmap1[i].r = 255.99999 * r;
	    plsc->cmap1[i].g = 255.99999 * g;
	    plsc->cmap1[i].b = 255.99999 * b;
	}
    }

    plsc->cmap1set = 1;
    if (plsc->level > 0)
	plP_state(PLSTATE_CMAP1);
}

/*----------------------------------------------------------------------*\
* color_def()
*
* Initializes color table entries by RGB values.
* Does nothing if color already set.
\*----------------------------------------------------------------------*/

static void
color_def(PLINT i, U_CHAR r, U_CHAR g, U_CHAR b)
{
    if ( ! plsc->cmap0setcol[i] && i < plsc->ncol0) {
	plsc->cmap0[i].r = r;
	plsc->cmap0[i].g = g;
	plsc->cmap0[i].b = b;
	plsc->cmap0setcol[i] = 1;
    }
}

/*----------------------------------------------------------------------*\
* plCmap0_init()
*
* Initializes color map 0.
* Do not initialize if already done.
*
* Initial RGB values for color map 0 taken from HPUX 8.07 X-windows 
* rgb.txt file, and may not accurately represent the described colors on 
* all systems.
*
* Note the background color is not set, since the device driver may be
* able to detect if a monochrome output device is being used, in which
* case I want to choose the default background color there.
\*----------------------------------------------------------------------*/

void
plCmap0_init(void)
{
    plgpls(&plsc);
    if (plsc->ncol0 == 0)
	plsc->ncol0 = 16;

/* Color map 0 */
/* Any entries already filled by user or unallocated are not touched */

    color_def(0,    0,   0,   0);	/* black */
    color_def(1,  255,   0,   0);	/* red */
    color_def(2,  255, 255,   0);	/* yellow */
    color_def(3,    0, 255,   0);	/* green */
    color_def(4,   50, 191, 193);	/* aquamarine */
    color_def(5,  255, 181, 197);	/* pink */
    color_def(6,  245, 222, 179);	/* wheat */
    color_def(7,  126, 126, 126);	/* grey */
    color_def(8,  165,  42,  42);	/* brown */
    color_def(9,    0,   0, 255);	/* blue */
    color_def(10, 138,  43, 226);	/* Blue Violet */
    color_def(11,   0, 255, 255);	/* cyan */
    color_def(12,  25, 204, 223);	/* turquoise */
    color_def(13, 255,   0, 255);	/* magenta */
    color_def(14, 233, 150, 122);	/* salmon */
    color_def(15, 255, 255, 255);	/* white */
}

/*----------------------------------------------------------------------*\
* plCmap1_init()
*
* Initializes color map 1.
* Do not initialize if already done.
*
* The default initialization uses 4 control points in HLS space, the two
* inner ones being very close to one of the vertices of the HLS double
* cone.  The vertex used (black or white) is chosen to be the closer to
* the background color.  If you don't like these settings you can always
* initialize it yourself.
\*----------------------------------------------------------------------*/

void
plCmap1_init(void)
{
    int vertex;
    PLFLT i[4], h[4], l[4], s[4];

    plgpls(&plsc);

/* Return if the user has already filled color map */

    if (plsc->cmap1set)
	return;

/* Locations of control points */

    i[0] = 0;		/* left boundary */
    i[1] = 0.45;	/* just before center */
    i[2] = 0.55;	/* just after center */
    i[3] = 1;		/* right boundary */

/* Pick default vertex of HLS double cone to match bg color if possible */
/* If not set, bg is assumed to be black by default */

    vertex = ((float) plsc->bgcolor.r + (float) plsc->bgcolor.g +
	      (float) plsc->bgcolor.b) / 3. / 256. + 0.5;

/* Set hue */

    h[0] = 260;		/* low: blue-violet */
    h[1] = 260;		/* only change as we go over vertex */
    h[2] = 20;		/* high: red */
    h[3] = 20;		/* keep fixed */

/* Set lightness */

    l[0] = 0.5;		/* low */
    l[1] = vertex;	/* bg */
    l[2] = vertex;	/* bg */
    l[3] = 0.5;		/* high */

/* Set saturation -- keep at maximum */

    s[0] = 1;
    s[1] = 1;
    s[2] = 1;
    s[3] = 1;

    c_plscmap1l(0, 4, i, h, l, s);
}

/*----------------------------------------------------------------------*\
* plscolor()
*
* Used to globally turn color output on/off
\*----------------------------------------------------------------------*/

void
c_plscolor(PLINT color)
{
    plgpls(&plsc);
    plsc->colorset = 1;
    plsc->color = color;
}

/*----------------------------------------------------------------------*\
* plrgb()
*
* Set line color by red, green, blue from  0. to 1.
* Do NOT use this.  Only retained for backward compatibility
\*----------------------------------------------------------------------*/

void
c_plrgb(PLFLT r, PLFLT g, PLFLT b)
{
    plgpls(&plsc);

    if (plsc->level < 1) {
	plabort("plrgb: Please call plinit first");
	return;
    }
    if ((r < 0. || r > 1.) || (g < 0. || g > 1.) || (b < 0. || b > 1.)) {
	plabort("plrgb: Invalid RGB color value");
	return;
    }

    plsc->icol0 = PL_RGB_COLOR;
    plsc->curcolor.r = MIN(255, (int) (256. * r));
    plsc->curcolor.g = MIN(255, (int) (256. * g));
    plsc->curcolor.b = MIN(255, (int) (256. * b));

    plsc->curcmap = 0;
    plP_state(PLSTATE_COLOR0);
}

/*----------------------------------------------------------------------*\
* plrgb1()
*
* Set line color by 8 bit RGB values.
* See note to plrgb()
\*----------------------------------------------------------------------*/

void
c_plrgb1(PLINT r, PLINT g, PLINT b)
{
    plgpls(&plsc);

    if (plsc->level < 1) {
	plabort("plrgb1: Please call plinit first");
	return;
    }
    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plabort("plrgb1: Invalid color");
	return;
    }

    plsc->icol0 = PL_RGB_COLOR;
    plsc->curcolor.r = r;
    plsc->curcolor.g = g;
    plsc->curcolor.b = b;

    plsc->curcmap = 0;
    plP_state(PLSTATE_COLOR0);
}

/*----------------------------------------------------------------------*\
* void plhls()
*
* Set current color by hue, lightness, and saturation.
* Convert hls color coordinates to rgb, then call plrgb.
* See note to plrgb()
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

static void
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

/*----------------------------------------------------------------------*\
* A grab-bag of various control routines.
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* void plwarn()
*
* A handy way to issue warnings, if need be.
\*----------------------------------------------------------------------*/

void
plwarn(char *errormsg)
{
    int was_gfx = 0;

    plgpls(&plsc);
    if (plsc->level > 0) {
	if (plsc->graphx == 1) {
	    was_gfx = 1;
	    pltext();
	}
    }

    fprintf(stderr, "\n*** PLPLOT WARNING ***\n");
    if (*errormsg != '\0')
	fprintf(stderr, "%s\n", errormsg);

    if (was_gfx == 1) {
	plgra();
    }
}

/*----------------------------------------------------------------------*\
* void plabort()
*
* Exactly the same as plwarn(), but appends ", aborting operation" to the
* error message.  Helps to keep source code uncluttered and provides a
* convention for error aborts.
\*----------------------------------------------------------------------*/

void
plabort(char *errormsg)
{
    int was_gfx = 0;

    plgpls(&plsc);
    if (plsc->level > 0) {
	if (plsc->graphx == 1) {
	    was_gfx = 1;
	    pltext();
	}
    }

    fprintf(stderr, "\n*** PLPLOT WARNING ***\n");
    if (*errormsg != '\0')
	fprintf(stderr, "%s, aborting operation\n", errormsg);

    if (was_gfx == 1) {
	plgra();
    }
}

/*----------------------------------------------------------------------*\
* void plexit()
*
* In case of an abort this routine is called.  It just prints out an error
* message and tries to clean up as much as possible.  It's best to turn
* off pause and then restore previous setting before returning.
\*----------------------------------------------------------------------*/

void
plexit(char *errormsg)
{
    PLINT nopause;

    plgpls(&plsc);
    nopause = plsc->nopause;
    plsc->nopause = 1;

    plend();
    if (*errormsg != '\0') {
	fprintf(stderr, "\n*** PLPLOT ERROR ***\n");
	fprintf(stderr, "%s\n", errormsg);
    }
    plsc->nopause = nopause;
    pl_exit();
}

/*----------------------------------------------------------------------*\
* void pl_exit()
*
* Just a front-end to exit().  If cleanup needs to be done in the main
* program, the user should write his/her own exit handler and pass it in
* via plsexit().
\*----------------------------------------------------------------------*/

void
pl_exit(void)
{
    if (exit_handler != NULL)
	(*exit_handler)();
    else {
	fprintf(stderr, "Program aborted\n");
	exit(1);
    }
}

/*----------------------------------------------------------------------*\
* void plsexit()
*
* Sets the exit handler to use instead of exit().
\*----------------------------------------------------------------------*/

void
plsexit(void (*handler) (void))
{
    exit_handler = handler;
}

/*----------------------------------------------------------------------*\
* void plgra()
*
* Switches to graphics screen.
\*----------------------------------------------------------------------*/

void
c_plgra(void)
{
    PLINT level;
    plP_glev(&level);
    if (level < 1) {
	plabort("plgra: Please call plinit first");
	return;
    }
    plP_esc(PLESC_GRAPH, NULL);
}

/*----------------------------------------------------------------------*\
* void pltext()
*
* Switches to text screen.
\*----------------------------------------------------------------------*/

void
c_pltext(void)
{
    PLINT level;

    plP_glev(&level);
    if (level < 1) {
	plabort("pltext: Please call plinit first");
	return;
    }
    plP_esc(PLESC_TEXT, NULL);
}

/*----------------------------------------------------------------------*\
* void pl_cmd()
*
* Front-end to driver escape function.
* In principle this can be used to pass just about anything directly
* to the driver.
\*----------------------------------------------------------------------*/

void
pl_cmd(PLINT op, void *ptr)
{
    plP_esc(op, ptr);
}

/*----------------------------------------------------------------------*\
* char *plFindCommand
*
* Looks for the specified executable file.  Search path:
*	current directory
*	$(PLPLOT_DIR)
*	INSTALL_DIR
*
* The caller must free the returned pointer (points to malloc'ed memory)
* when finished with it.
\*----------------------------------------------------------------------*/

char *
plFindCommand(char *fn)
{
    char *fs = NULL, *dn;

/* Current directory */

    plGetName(".", "", fn, &fs);
    if ( ! plFindName(fs))
	return fs;

/* $(PLPLOT_DIR) */

    if ((dn = getenv("PLPLOT_DIR")) != NULL) {
	plGetName(dn, "", fn, &fs);
	if ( ! plFindName(fs))
	    return fs;
    }

/* INSTALL_DIR */

#ifdef INSTALL_DIR
    plGetName(INSTALL_DIR, "", fn, &fs);
    if ( ! plFindName(fs))
	return fs;
#endif

/* Crapped out */

    free_mem(fs);
    fprintf(stderr, "plFindCommand: cannot locate command: %s\n", fn);
    return NULL;
}

/*----------------------------------------------------------------------*\
* int plFindName
*
* Authors: Paul Dubois (LLNL), others?
* This function is in the public domain.
*
* Given a pathname, determine if it is a symbolic link.  If so, continue
* searching to the ultimate terminus - there may be more than one link.
* Use the error value to determine when the terminus is reached, and to
* determine if the pathname really exists.  Then stat it to determine
* whether it's executable.  Return 0 for an executable, errno otherwise.
* Note that 'p' _must_ have at least one '/' character - it does by
* construction in this program.  The contents of the array pointed to by
* 'p' are changed to the actual pathname if findname is successful.
*
* This function is only defined under Unix for now.
\*----------------------------------------------------------------------*/

#ifdef __unix
int 
plFindName(char *p)
{
    int n;
    char buf[1024], *cp;
    extern int errno;
    struct stat sbuf;

    while ((n = readlink(p, buf, 1024)) > 0) {
#ifdef DEBUG
	fprintf(stderr, "Readlink read %d chars at: %s\n", n, p);
#endif
	if (buf[0] == '/') {	/* Link is an absolute path */
	    strncpy(p, buf, n);
	    p[n] = '\0';
#ifdef DEBUG
	    fprintf(stderr, "Link is absolute: %s\n", p);
#endif
	}
	else {			/* Link is relative to its directory; make it
				   absolute */
	    cp = 1 + strrchr(p, '/');
	    strncpy(cp, buf, n);
	    cp[n] = '\0';
#ifdef DEBUG
	    fprintf(stderr, "Link is relative: %s\n\tTotal path: %s\n", cp, p);
#endif
	}
    }

/* SGI machines return ENXIO instead of EINVAL Dubois 11/92 */

    if (errno == EINVAL || errno == ENXIO) {
#ifdef DEBUG
	fprintf(stderr, "%s may be the one ...", p);
#endif
#ifdef SX
#define S_ISREG(mode)   (mode & S_IFREG)
#endif
	if ((stat(p, &sbuf) == 0) && S_ISREG(sbuf.st_mode)) {
#ifdef DEBUG
	    fprintf(stderr, "regular file\n");
#endif
	    return (access(p, X_OK));
	}
    }
#ifdef DEBUG
    fprintf(stderr, "not executable\n");
#endif
    return (errno ? errno : -1);
}

#else
int 
plFindName(char *p)
{
    return 1;
}
#endif

/*----------------------------------------------------------------------*\
* void plGetName()
*
* Gets search name for file by concatenating the dir, subdir, and file
* name, allocating memory as needed.  The appropriate delimiter is added
* after the dir specification as necessary.  The caller is responsible
* for freeing the malloc'ed memory.
\*----------------------------------------------------------------------*/

void
plGetName(char *dir, char *subdir, char *filename, char **filespec)
{
    int lfilespec;

/* Malloc space for filespec */

    free_mem(*filespec);
    lfilespec = 10;
    lfilespec = strlen(dir) + strlen(subdir) + strlen(filename) + 10;
    *filespec = (char *) malloc(lfilespec);

    strcpy(*filespec, dir);

    if (*subdir != '\0') {
	strcat_delim(*filespec);
	strcat(*filespec, subdir);
    }
    if (*filename != '\0') {
	strcat_delim(*filespec);
	strcat(*filespec, filename);
    }
}

/*----------------------------------------------------------------------*\
* void strcat_delim()
*
* Append path name deliminator if necessary (does not add one if one's
* there already, or if dealing with a colon-terminated device name as
* used on the Amiga).
\*----------------------------------------------------------------------*/

static void
strcat_delim(char *dirspec)
{
    int ldirspec = strlen(dirspec);
#ifdef MSDOS
    if (dirspec[ldirspec-1] != '\\')
	strcat(dirspec, "\\");
#endif
#ifdef __unix
    if (dirspec[ldirspec-1] != '/')
	strcat(dirspec, "/");
#endif
#ifdef AMIGA
    if (dirspec[ldirspec-1] != '/' && dirspec[ldirspec-1] != ':')
	strcat(dirspec, "/");
#endif
}

/*----------------------------------------------------------------------*\
* plGetInt()
*
* Prompts human to input an integer in response to given message.
\*----------------------------------------------------------------------*/

PLINT
plGetInt(char *s)
{
    PLINT m;
    int i = 0;
    char line[256];

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atoi(line);
	return (m);
#else
	if (sscanf(line, "%d", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
    return (0);
}

/*----------------------------------------------------------------------*\
* plGetFlt()
*
* Prompts human to input a float in response to given message.
\*----------------------------------------------------------------------*/

PLFLT
plGetFlt(char *s)
{
    PLFLT m;
    int i = 0;
    char line[256];

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atof(line);
	return (m);
#else
	if (sscanf(line, "%f", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
    return (0.);
}
