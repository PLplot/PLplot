/* $Id$
   $Log$
   Revision 1.4  1993/02/23 05:23:11  mjl
   Changed references in error messages from plstar to plinit.

 * Revision 1.3  1993/01/23  06:02:29  mjl
 * Now holds all routines dealing with viewport generation.
 *
 * Revision 1.2  1992/09/29  04:46:26  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:56  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plvpor.c

	Functions dealing with viewports.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plenv()
*
* Simple interface for defining viewport and window. If "just"=1,
* X and Y scales will be the same, otherwise they are scaled
* independently. The "axis" parameter is interpreted as follows:
*
*	axis=-2 : draw no box, axis or labels
*	axis=-1 : draw box only
*	axis= 0 : Draw box and label with coordinates
*	axis= 1 : Also draw the coordinate axes
*	axis= 2 : Draw a grid at major tick positions
*	axis=10 : Logarithmic X axis, Linear Y axis, No X=0 axis
*	axis=11 : Logarithmic X axis, Linear Y axis, X=0 axis
*	axis=20 : Linear X axis, Logarithmic Y axis, No Y=0 axis
*	axis=21 : Linear X axis, Logarithmic Y axis, Y=0 axis
*	axis=30 : Logarithmic X and Y axes
\*----------------------------------------------------------------------*/

void
c_plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis)
{
    PLINT level;
    PLFLT chrdef, chrht;
    PLFLT lb, rb, tb, bb, dx, dy;
    PLFLT xsize, ysize, xscale, yscale, scale;
    PLFLT spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;

    glev(&level);
    if (level < 1)
	plexit("plenv: Please call plinit first.");

    if (xmin == xmax)
	plexit("plenv: Invalid xmin and xmax arguments");

    if (ymin == ymax)
	plexit("plenv: Invalid ymin and ymax arguments");

    if ((just != 0) && (just != 1))
	plexit("plenv: Invalid just option");

    pladv(0);
    if (just == 0)
	plvsta();
    else {
	gchr(&chrdef, &chrht);
	lb = 8.0 * chrht;
	rb = 5.0 * chrht;
	tb = 5.0 * chrht;
	bb = 5.0 * chrht;
	dx = ABS(xmax - xmin);
	dy = ABS(ymax - ymin);
	plgspa(&spxmin, &spxmax, &spymin, &spymax);
	xsize = spxmax - spxmin;
	ysize = spymax - spymin;
	xscale = dx / (xsize - lb - rb);
	yscale = dy / (ysize - tb - bb);
	scale = MAX(xscale, yscale);
	vpxmin = MAX(lb, 0.5 * (xsize - dx / scale));
	vpxmax = vpxmin + (dx / scale);
	vpymin = MAX(bb, 0.5 * (ysize - dy / scale));
	vpymax = vpymin + (dy / scale);

	plsvpa(vpxmin, vpxmax, vpymin, vpymax);
    }
    plwind(xmin, xmax, ymin, ymax);
    if (axis == -2);
    else if (axis == -1)
	plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
    else if (axis == 0)
	plbox("bcnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
    else if (axis == 1)
	plbox("abcnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
    else if (axis == 2)
	plbox("abcgnst", (PLFLT) 0.0, 0, "abcgnstv", (PLFLT) 0.0, 0);
    else if (axis == 10)
	plbox("bclnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
    else if (axis == 11)
	plbox("bclnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
    else if (axis == 20)
	plbox("bcnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
    else if (axis == 21)
	plbox("bcnst", (PLFLT) 0.0, 0, "abclnstv", (PLFLT) 0.0, 0);
    else if (axis == 30)
	plbox("bclnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
    else
	plwarn("plenv: Invalid axis argument");
}

/*----------------------------------------------------------------------*\
* void plvsta()
*
* Defines a "standard" viewport with seven character heights for
* the left margin and four character heights everywhere else.
\*----------------------------------------------------------------------*/

void
c_plvsta()
{
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT chrdef, chrht, spdxmi, spdxma, spdymi, spdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plvsta: Please call plinit first.");

    gchr(&chrdef, &chrht);
    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);

/*  Find out position of subpage boundaries in millimetres, reduce by */
/*  the desired border, and convert back into normalized subpage */
/*  coordinates */

    xmin = dcscx(mmdcx((PLFLT) (dcmmx(spdxmi) + 8 * chrht)));
    xmax = dcscx(mmdcx((PLFLT) (dcmmx(spdxma) - 5 * chrht)));
    ymin = dcscy(mmdcy((PLFLT) (dcmmy(spdymi) + 5 * chrht)));
    ymax = dcscy(mmdcy((PLFLT) (dcmmy(spdyma) - 5 * chrht)));

    plvpor(xmin, xmax, ymin, ymax);
}

/*----------------------------------------------------------------------*\
* void plvpor()
*
* Sets the edges of the viewport to the specified normalized subpage
* coordinates.
\*----------------------------------------------------------------------*/

void
c_plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT nx, ny, cs;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plvpor: Please call plinit first.");

    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) ||
	(ymin < 0.) || (xmax > 1.) || (ymax > 1.))
	plexit("plvpor: Invalid limits.");

    gsub(&nx, &ny, &cs);
    if ((cs <= 0) || (cs > (nx * ny)))
	plexit("plvpor: Please call pladv or plenv to go to a subpage.");

    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    vpdxmi = spdxmi + (spdxma - spdxmi) * xmin;
    vpdxma = spdxmi + (spdxma - spdxmi) * xmax;
    vpdymi = spdymi + (spdyma - spdymi) * ymin;
    vpdyma = spdymi + (spdyma - spdymi) * ymax;
    svpd(vpdxmi, vpdxma, vpdymi, vpdyma);

    vppxmi = dcpcx(vpdxmi);
    vppxma = dcpcx(vpdxma);
    vppymi = dcpcy(vpdymi);
    vppyma = dcpcy(vpdyma);
    svpp(vppxmi, vppxma, vppymi, vppyma);
    sclp(vppxmi, vppxma, vppymi, vppyma);
    slev(2);
}

/*----------------------------------------------------------------------*\
* void plvpas()
*
* Sets the edges of the viewport to that of the largest window at the
* given aspect ratio that fits within the specified normalized subpage
* coordinates.
\*----------------------------------------------------------------------*/

void
c_plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect)
{
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLINT level;
    PLFLT vpxmid, vpymid, vpxlen, vpylen, w_aspect, ratio;

    glev(&level);
    if (level < 1)
	plexit("plvpas: Please call plinit first.");

    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) ||
	(ymin < 0.) || (xmax > 1.) || (ymax > 1.))
	plexit("plvpas: Invalid limits.");

    if (aspect <= 0.0) {
	c_plvpor(xmin, xmax, ymin, ymax);
	return;
    }

    vpxmi = dcmmx(xmin);
    vpxma = dcmmx(xmax);
    vpymi = dcmmy(ymin);
    vpyma = dcmmy(ymax);

    vpxmid = (vpxmi + vpxma) / 2.;
    vpymid = (vpymi + vpyma) / 2.;

    vpxlen = vpxma - vpxmi;
    vpylen = vpyma - vpymi;

    w_aspect = vpylen / vpxlen;
    ratio = aspect / w_aspect;

/*
* If ratio < 1, you are requesting an aspect ratio (y/x) less than the natural
* aspect ratio of the specified window, and you will need to reduce the length
* in y correspondingly.  Similarly, for ratio > 1, x length must be reduced.
*/

    if (ratio <= 0.)
	plexit("plvpas: Error in aspect ratio setting");
    else if (ratio < 1.)
	vpylen = vpylen * ratio;
    else
	vpxlen = vpxlen / ratio;

    vpxmi = vpxmid - vpxlen / 2.;
    vpxma = vpxmid + vpxlen / 2.;
    vpymi = vpymid - vpylen / 2.;
    vpyma = vpymid + vpylen / 2.;

    plsvpa(vpxmi, vpxma, vpymi, vpyma);
}

/*----------------------------------------------------------------------*\
* void plvasp()
*
* Sets the edges of the viewport with the given aspect ratio, leaving
* room for labels.
\*----------------------------------------------------------------------*/

void
c_plvasp(PLFLT aspect)
{
    PLINT level;
    PLFLT chrdef, chrht, spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;
    PLFLT xsize, ysize, nxsize, nysize;
    PLFLT lb, rb, tb, bb;

    glev(&level);
    if (level < 1)
	plexit("plvasp: Please call plinit first.");

    gchr(&chrdef, &chrht);
    lb = 8.0 * chrht;
    rb = 5.0 * chrht;
    tb = 5.0 * chrht;
    bb = 5.0 * chrht;
    plgspa(&spxmin, &spxmax, &spymin, &spymax);
    xsize = spxmax - spxmin;
    ysize = spymax - spymin;
    xsize -= lb + rb;		/* adjust for labels */
    ysize -= bb + tb;
    if (aspect * xsize > ysize) {
	nxsize = ysize / aspect;
	nysize = ysize;
    }
    else {
	nxsize = xsize;
	nysize = xsize * aspect;
    }

/* center plot within page */

    vpxmin = .5 * (xsize - nxsize) + lb;
    vpxmax = vpxmin + nxsize;
    vpymin = .5 * (ysize - nysize) + bb;
    vpymax = vpymin + nysize;
    plsvpa(vpxmin, vpxmax, vpymin, vpymax);
}

/*----------------------------------------------------------------------*\
* void plsvpa()
*
* Sets the edges of the viewport to the specified absolute
* coordinates (mm), measured with respect to the current subpage
* boundaries.
\*----------------------------------------------------------------------*/

void
c_plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLINT nx, ny, cs;
    PLFLT sxmin, symin;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plsvpa: Please call plinit first.");

    if ((xmin >= xmax) || (ymin >= ymax) || (xmin < 0.) || (ymin < 0.))
	plexit("plsvpa: Invalid limits.");

    gsub(&nx, &ny, &cs);
    if ((cs <= 0) || (cs > (nx * ny)))
	plexit("plsvpa: Please call pladv or plenv to go to a subpage.");

    gspd(&spdxmi, &spdxma, &spdymi, &spdyma);
    sxmin = dcmmx(spdxmi);
    symin = dcmmy(spdymi);

    vpdxmi = mmdcx((PLFLT) (sxmin + xmin));
    vpdxma = mmdcx((PLFLT) (sxmin + xmax));
    vpdymi = mmdcy((PLFLT) (symin + ymin));
    vpdyma = mmdcy((PLFLT) (symin + ymax));

    svpd(vpdxmi, vpdxma, vpdymi, vpdyma);
    svpp(dcpcx(vpdxmi), dcpcx(vpdxma), dcpcy(vpdymi), dcpcy(vpdyma));
    sclp(dcpcx(vpdxmi), dcpcx(vpdxma), dcpcy(vpdymi), dcpcy(vpdyma));
    slev(2);
}
