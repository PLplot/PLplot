/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:49  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:18  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plcol.c

	Functions for setting and allocating colors.
*/

#include "plplot.h"

/* Sets line color */

void 
c_plcol(PLINT color)
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plcol.");
    if (color < 0)
	plexit("Invalid color in plcol.");

    scol(color);
    grcol();
}

/*  Set line color by red, green blue from  0. to 1. */

void 
c_plrgb(PLFLT red, PLFLT green, PLFLT blue)
{
    pleRGB cols;
    char *ptr = (char *) &cols;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plrgb.");

    cols.red   = red;
    cols.green = green;
    cols.blue  = blue;

    gresc((PLINT) PL_SET_RGB, ptr);
}

/* Allocate given named color and associated number for use with plcol(). */

void 
c_plancol( PLINT icolor, char *name )
{
    pleNcol col;
    char *ptr = (char *) &col;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plancol.");

    col.icolor = icolor;
    col.name   = name;

    gresc((PLINT) PL_ALLOC_NCOL, ptr);
}

/* auxiliary function used by hlsrgb */

static float 
value (double n1, double n2, double hue)
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

    return(val);
}

/*
*  Sets line color by hue, lightness, and saturation.
*  Converts hls color coordinates to rgb, then calls plrgb.
*  Bounds on each coordinate (I think):
*	hue		[0., 360.]	degrees
*	lightness	[0., 1.]	magnitude
*	saturation	[0., 1.]	magnitude
*/

void 
c_plhls(PLFLT hue, PLFLT lightness, PLFLT saturation)
{
    float m1, m2;
    PLFLT red, green, blue;

    if (lightness <= .5)
	m2 = lightness * (saturation + 1.);
    else
	m2 = lightness + saturation - lightness * saturation;

    m1 = 2 * lightness - m2;

    red = value(m1, m2, hue + 120.);
    green = value(m1, m2, hue);
    blue = value(m1, m2, hue - 120.);

    plrgb((PLFLT) red, (PLFLT) green, (PLFLT) blue);
}
