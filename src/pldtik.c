/* $Id$
   $Log$
   Revision 1.3  1993/01/23 05:53:31  mjl
   Formatting changes only (everything got run through indent).

 * Revision 1.2  1992/09/29  04:45:52  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:21  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	pldtik.c

	Determines tick spacing and mode (fixed or floating) of
	numeric axis labels.
*/

#include "plplot.h"
#include <math.h>

#define MIN_FLTDIG	3	/* disregarded if fractional part is 0 */
#define MAX_FIXDIG_POS	6
#define MAX_FIXDIG_NEG	4
#define DIGMAX_DEF	5

/*----------------------------------------------------------------------*\
* void pldtik()
*
* If tick == 0, this works out a "nice" interval, so that there are between
* 3 and 7.5 major tick intervals in the input range "vmin" to "vmax". Using
* this value for the tick interval or supplied value, it also computes
* "prec" which specifies the number of places that should be written after
* the decimal point. The recommended number of subticks is returned in
* "nsubt" unless the routine is entered with a non-zero value of "nsubt".
* The output variable "mode" is set to 0 if labels are to be written in
* floating-point format, or to 1 if they are to be written in scientific
* format.    For mode = 1, the exponent will be placed at:
*
* 	top left	for vertical axis on left
* 	top right	for vertical axis on right
* 	bottom right	for horizontal axis
*
* The digmax flag can be set by the user, and represents the maximum number
* of digits a label may occupy.  If digmax<0, it is disregarded, and if
* digmax=0 the default value is used.  For digmax>0, mode=1 is chosen if
* there is insufficient room for the label within the specified # of digits.
*
* In the case of mode=0, the actual # of digits will become too large when
* the magnitude of the labels become too large.  The mode=1 case offers the
* greatest precision for the smallest field length.
*
* The determination of maximum length for fixed point quantities is
* complicated by the fact that very long fixed point representations look
* much worse than the same sized floating point representation.  Further, a
* fixed point number with a large negative exponent will actually gain in
* precision when written as floating point.  Thus we use certain fuzz
* factors to get 'digfix' from 'digmax', however it will always be true
* that digfix<=digmax.
*
* Finally, if 'digmax' is set, 'prec' is reduced in size if necessary so
* that the labels fit the requested field length.
\*----------------------------------------------------------------------*/

void
pldtik(PLFLT vmin, PLFLT vmax, PLFLT *tick, PLINT *nsubt,
       PLINT *mode, PLINT *prec, PLINT digmax, PLINT *scale)
{
    PLFLT t0, t1, t2, vmod;
    PLINT msd, np, ns, digmin, digfix, autoscale = 0;

    *mode = 0;
    *scale = 0;
    if (*tick == 0.0)
	autoscale = 1;
    if (digmax == 0)
	digmax = DIGMAX_DEF;

/* Magnitute of min/max to get number of significant digits */

    vmod = MAX(ABS(vmin), ABS(vmax));
    t0 = (PLFLT) log10(vmod);
    msd = (PLINT) floor(t0);

/* Autoselect the mode flag */
/* 'digmin' is the minimum number of places taken up by the label */

    if (msd >= 0) {
	digmin = msd + 1;
	digfix = MAX_FIXDIG_POS;
	if (digmax > 0)
	    digfix = MIN(digmax, MAX_FIXDIG_POS);
    }
    else {
	digmin = -msd + 2;
	digfix = MAX_FIXDIG_NEG;
	if (digmax > 0)
	    digfix = MIN(digmax, MAX_FIXDIG_NEG);
    }

    if (digmin > digfix) {
	*mode = 1;
	*scale = msd;
    }

/* Magnitude of min/max difference to get tick spacing */

    t1 = (PLFLT) log10(ABS(vmax - vmin));
    np = (PLINT) floor(t1);
    t1 = t1 - np;

/* Get tick spacing. */

    if (t1 > 0.7781512503) {
	t2 = 2.0;
	ns = 4;
    }
    else if (t1 > 0.4771212549) {
	t2 = 1.0;
	ns = 5;
    }
    else if (t1 > 0.1760912591) {
	t2 = 5.0;
	ns = 5;
	np = np - 1;
    }
    else {
	t2 = 2.0;
	ns = 4;
	np = np - 1;
    }

/* Now compute tick spacing */

    if (autoscale) {
	*tick = t2 * pow(10.0, (double) np);
	if (vmin > vmax)
	    *tick = -*tick;
    }
    else {
	t1 = log10(ABS(*tick));
	np = (PLINT) floor(t1);
    }
    if (*nsubt == 0)
	*nsubt = ns;

/* Establish precision.  */

    if (*mode != 0)
	*prec = msd - np;
    else
	*prec = MAX(-np, 0);

/* One last hack required here: if exponent < 0, i.e. number has leading
   '0.', it is better to change to floating point form if the number of
    digits is insufficient to represent the tick spacing. */

    if (autoscale) {
	if (*mode == 0 && digmax > 0) {
	    if (t0 < 0.0) {
		if (digmax - 2 - *prec < 0) {
		    *mode = 1;
		    *scale = msd;
		}
	    }
	    else
		*prec = MAX(MIN(*prec, digmax - msd - 1), 0);
	}
	if (*mode != 0) {
	    *prec = msd - np;
	    *prec = MAX(MIN(*prec, MAX(digmax, MIN_FLTDIG)), 0);
	}
    }
}
