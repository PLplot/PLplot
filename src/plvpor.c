/* $Id$
 * $Log$
 * Revision 1.11  1998/12/01 20:51:13  furnish
 * Various fixups contributed by Joao Cardoso <jcardoso@inescn.pt>.
 *
 * Revision 1.10  1995/03/17  00:15:33  mjl
 * Eliminated unnecessary accessor variables and other cleaning up.
 *
 * Revision 1.9  1994/06/30  18:22:23  mjl
 * All core source files: made another pass to eliminate warnings when using
 * gcc -Wall.  Lots of cleaning up: got rid of includes of math.h or string.h
 * (now included by plplot.h), and other minor changes.  Now each file has
 * global access to the plstream pointer via extern; many accessor functions
 * eliminated as a result.
*/

/*	plvpor.c

	Functions dealing with viewports.
*/

#include "plplotP.h"

/*--------------------------------------------------------------------------*\
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
\*--------------------------------------------------------------------------*/

void
c_plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis)
{
    PLFLT lb, rb, tb, bb, dx, dy;
    PLFLT xsize, ysize, xscale, yscale, scale;
    PLFLT spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;

    if (plsc->level < 1) {
	plabort("plenv: Please call plinit first");
	return;
    }
    if (xmin == xmax) {
	plabort("plenv: Invalid xmin and xmax arguments");
	return;
    }
    if (ymin == ymax) {
	plabort("plenv: Invalid ymin and ymax arguments");
	return;
    }
    if ((just != 0) && (just != 1)) {
	plabort("plenv: Invalid just option");
	return;
    }

    pladv(0);

    if (just == 0)
	plvsta();
    else {
	lb = 8.0 * plsc->chrht;
	rb = 5.0 * plsc->chrht;
	tb = 5.0 * plsc->chrht;
	bb = 5.0 * plsc->chrht;
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

    switch (axis) {
    case -2:
	break;
    case -1:
	plbox("bc", (PLFLT) 0.0, 0, "bc", (PLFLT) 0.0, 0);
	break;
    case 0:
	plbox("bcnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
	break;
    case 1:
	plbox("abcnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
	break;
    case 2:
	plbox("abcgnst", (PLFLT) 0.0, 0, "abcgnstv", (PLFLT) 0.0, 0);
	break;
    case 10:
	plbox("bclnst", (PLFLT) 0.0, 0, "bcnstv", (PLFLT) 0.0, 0);
	break;
    case 11:
	plbox("bclnst", (PLFLT) 0.0, 0, "abcnstv", (PLFLT) 0.0, 0);
	break;
    case 12:/* jc: make it regular */
	plbox("bclgnst", (PLFLT) 0.0, 0, "abcgnstv", (PLFLT) 0.0, 0);
	break;
    case 20:
	plbox("bcnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
	break;
    case 21:
	plbox("bcnst", (PLFLT) 0.0, 0, "abclnstv", (PLFLT) 0.0, 0);
	break;
    case 22:/* jc: make it regular */
	plbox("bcngst", (PLFLT) 0.0, 0, "abcglnstv", (PLFLT) 0.0, 0);
	break;	
    case 30:
	plbox("bclnst", (PLFLT) 0.0, 0, "bclnstv", (PLFLT) 0.0, 0);
	break;
    case 31:/* jc: make it regular */
	plbox("bclnst", (PLFLT) 0.0, 0, "abclnstv", (PLFLT) 0.0, 0);
	break;
    case 32:/* jc: make it regular */
	plbox("bclngst", (PLFLT) 0.0, 0, "abcglnstv", (PLFLT) 0.0, 0);
	break;	
    default:
	plwarn("plenv: Invalid axis argument");
    }
}

/*--------------------------------------------------------------------------*\
 * void plvsta()
 *
 * Defines a "standard" viewport with seven character heights for
 * the left margin and four character heights everywhere else.
\*--------------------------------------------------------------------------*/

void
c_plvsta(void)
{
    PLFLT xmin, xmax, ymin, ymax;
    PLFLT lb, rb, tb, bb;

    if (plsc->level < 1) {
	plabort("plvsta: Please call plinit first");
	return;
    }

/*  Find out position of subpage boundaries in millimetres, reduce by */
/*  the desired border, and convert back into normalized subpage */
/*  coordinates */

    lb = 8.0 * plsc->chrht;
    rb = 5.0 * plsc->chrht;
    tb = 5.0 * plsc->chrht;
    bb = 5.0 * plsc->chrht;

    xmin = plP_dcscx(plP_mmdcx((PLFLT) (plP_dcmmx(plsc->spdxmi) + lb)));
    xmax = plP_dcscx(plP_mmdcx((PLFLT) (plP_dcmmx(plsc->spdxma) - rb)));
    ymin = plP_dcscy(plP_mmdcy((PLFLT) (plP_dcmmy(plsc->spdymi) + tb)));
    ymax = plP_dcscy(plP_mmdcy((PLFLT) (plP_dcmmy(plsc->spdyma) - bb)));

    plvpor(xmin, xmax, ymin, ymax);
}

/*--------------------------------------------------------------------------*\
 * void plvpor()
 *
 * Creates a viewport with the specified normalized subpage coordinates.
\*--------------------------------------------------------------------------*/

void
c_plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    if (plsc->level < 1) {
	plabort("plvpor: Please call plinit first");
	return;
    }
    if ((xmin >= xmax) || (ymin >= ymax)) {
	plabort("plvpor: Invalid limits");
	return;
    }
    if ((plsc->cursub <= 0) || (plsc->cursub > (plsc->nsubx * plsc->nsuby))) {
	plabort("plvpor: Please call pladv or plenv to go to a subpage");
	return;
    }

    plsc->vpdxmi = plsc->spdxmi + (plsc->spdxma - plsc->spdxmi) * xmin;
    plsc->vpdxma = plsc->spdxmi + (plsc->spdxma - plsc->spdxmi) * xmax;
    plsc->vpdymi = plsc->spdymi + (plsc->spdyma - plsc->spdymi) * ymin;
    plsc->vpdyma = plsc->spdymi + (plsc->spdyma - plsc->spdymi) * ymax;

    plsc->vppxmi = plP_dcpcx(plsc->vpdxmi);
    plsc->vppxma = plP_dcpcx(plsc->vpdxma);
    plsc->vppymi = plP_dcpcy(plsc->vpdymi);
    plsc->vppyma = plP_dcpcy(plsc->vpdyma);

    plsc->clpxmi = MAX(plsc->vppxmi, plsc->phyxmi);
    plsc->clpxma = MIN(plsc->vppxma, plsc->phyxma);
    plsc->clpymi = MAX(plsc->vppymi, plsc->phyymi);
    plsc->clpyma = MIN(plsc->vppyma, plsc->phyyma);

    plsc->level = 2;
}

/*--------------------------------------------------------------------------*\
 * void plvpas()
 *
 * Creates the largest viewport of the specified aspect ratio that fits
 * within the specified normalized subpage coordinates.
\*--------------------------------------------------------------------------*/

void
c_plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect)
{
    PLFLT vpxmi, vpxma, vpymi, vpyma;
    PLFLT vpxmid, vpymid, vpxlen, vpylen, w_aspect, ratio;

    if (plsc->level < 1) {
	plabort("plvpas: Please call plinit first");
	return;
    }
    if ((xmin >= xmax) || (ymin >= ymax)) {
	plabort("plvpas: Invalid limits");
	return;
    }

    if (aspect <= 0.0) {
	c_plvpor(xmin, xmax, ymin, ymax);
	return;
    }

    vpxmi = plP_dcmmx(xmin);
    vpxma = plP_dcmmx(xmax);
    vpymi = plP_dcmmy(ymin);
    vpyma = plP_dcmmy(ymax);

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

    if (ratio <= 0.) {
	plabort("plvpas: Error in aspect ratio setting");
	return;
    }
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

/*--------------------------------------------------------------------------*\
 * void plvasp()
 *
 * Sets the edges of the viewport with the given aspect ratio, leaving
 * room for labels.
\*--------------------------------------------------------------------------*/

void
c_plvasp(PLFLT aspect)
{
    PLFLT spxmin, spxmax, spymin, spymax;
    PLFLT vpxmin, vpxmax, vpymin, vpymax;
    PLFLT xsize, ysize, nxsize, nysize;
    PLFLT lb, rb, tb, bb;

    if (plsc->level < 1) {
	plabort("plvasp: Please call plinit first");
	return;
    }

    lb = 8.0 * plsc->chrht;
    rb = 5.0 * plsc->chrht;
    tb = 5.0 * plsc->chrht;
    bb = 5.0 * plsc->chrht;

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

/*--------------------------------------------------------------------------*\
 * void plsvpa()
 *
 * Sets the edges of the viewport to the specified absolute coordinates
 * (mm), measured with respect to the current subpage boundaries.
\*--------------------------------------------------------------------------*/

void
c_plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    PLFLT sxmin, symin;

    if (plsc->level < 1) {
	plabort("plsvpa: Please call plinit first");
	return;
    }
    if ((xmin >= xmax) || (ymin >= ymax)) {
	plabort("plsvpa: Invalid limits");
	return;
    }
    if ((plsc->cursub <= 0) || (plsc->cursub > (plsc->nsubx * plsc->nsuby))) {
	plabort("plsvpa: Please call pladv or plenv to go to a subpage");
	return;
    }

    sxmin = plP_dcmmx(plsc->spdxmi);
    symin = plP_dcmmy(plsc->spdymi);

    plsc->vpdxmi = plP_mmdcx((PLFLT) (sxmin + xmin));
    plsc->vpdxma = plP_mmdcx((PLFLT) (sxmin + xmax));
    plsc->vpdymi = plP_mmdcy((PLFLT) (symin + ymin));
    plsc->vpdyma = plP_mmdcy((PLFLT) (symin + ymax));

    plsc->vppxmi = plP_dcpcx(plsc->vpdxmi);
    plsc->vppxma = plP_dcpcx(plsc->vpdxma);
    plsc->vppymi = plP_dcpcy(plsc->vpdymi);
    plsc->vppyma = plP_dcpcy(plsc->vpdyma);

    plsc->clpxmi = plP_dcpcx(plsc->vpdxmi);
    plsc->clpxma = plP_dcpcx(plsc->vpdxma);
    plsc->clpymi = plP_dcpcy(plsc->vpdymi);
    plsc->clpyma = plP_dcpcy(plsc->vpdyma);

    plsc->level = 2;
}
