/* $Id$
   $Log$
   Revision 1.4  1993/01/23 05:58:48  mjl
   Holds functions that modify plplot defaults.  These do not need direct
   access to the stream data.

 * Revision 1.3  1992/11/07  08:04:28  mjl
 * Fixed a problem encountered when a user tried to change the default
 * character/symbol scale heights.
 *
 * Revision 1.2  1992/09/29  04:46:16  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:46  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plsdef.c

	Routines to set various plplot parameters, such as char height,
	symbol size, tick length, line and fill patterns, etc.
*/

#include "plplot.h"

/* Line pattern defaults */

static struct line {
    PLINT nels;
    PLINT mark[4];
    PLINT space[4];
} line[] = {

    {
	0,			/* Continuous line */
	0, 0, 0, 0,
	0, 0, 0, 0
    },
    {
	1,
	1000, 0, 0, 0,
	1000, 0, 0, 0
    },
    {
	1,
	2000, 0, 0, 0,
	2000, 0, 0, 0
    },
    {
	1,
	2000, 0, 0, 0,
	1000, 0, 0, 0
    },
    {
	2,
	2500, 1000, 0, 0,
	1000, 1000, 0, 0
    },
    {
	2,
	2000, 1000, 0, 0,
	2000, 1000, 0, 0
    },
    {
	3,
	1000, 1500, 2000, 0,
	1000, 1500, 2000, 0
    },
    {
	3,
	1000, 1500, 2000, 0,
	1000, 1000, 1000, 0
    }
};

/* Fill pattern defaults */

static struct pattern {
    PLINT nlines;		/* Number of lines in pattern (1 or 2) */
    PLINT inc[2];		/* Inclination 10 ths of degrees */
    PLINT del[2];		/* Spacing for each line */
} pattern[] = {

    {
	1,
	0, 0,
	2000, 0
    },
    {
	1,
	900, 0,
	2000, 0
    },
    {
	1,
	450, 0,
	2000, 0
    },
    {
	1,
	-450, 0,
	2000, 0
    },
    {
	1,
	300, 0,
	2000, 0
    },
    {
	1,
	-300, 0,
	2000, 0
    },
    {
	2,
	0, 900,
	2000, 2000
    },
    {
	2,
	450, -450,
	2000, 2000
    }
};

/*----------------------------------------------------------------------*\
* void plschr()
*
* Set character height.
\*----------------------------------------------------------------------*/

void
c_plschr(PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht, base_scale;

    gscale(&base_scale);
    if (def != 0.0)
	schr((PLFLT) (def * base_scale), (PLFLT) (scale * def));
    else {
	gchr(&defalt, &ht);
	schr(defalt, (PLFLT) (scale * defalt));
    }
}

/*----------------------------------------------------------------------*\
* void plsmin()
*
* Set up lengths of minor tick marks.
\*----------------------------------------------------------------------*/

void
c_plsmin(PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht, base_scale;

    gscale(&base_scale);
    if (def != 0.0)
	smin(def, (PLFLT) (scale * def * base_scale));
    else {
	gmin(&defalt, &ht);
	smin(defalt, (PLFLT) (scale * defalt * base_scale));
    }
}

/*----------------------------------------------------------------------*\
* void plsmaj()
*
* Set up lengths of major tick marks.
\*----------------------------------------------------------------------*/

void
c_plsmaj(PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht, base_scale;

    gscale(&base_scale);
    if (def != 0.0)
	smaj(def, (PLFLT) (scale * def * base_scale));
    else {
	gmaj(&defalt, &ht);
	smaj(defalt, (PLFLT) (scale * defalt * base_scale));
    }
}

/*----------------------------------------------------------------------*\
* void plssym()
*
* Se symbol height.
\*----------------------------------------------------------------------*/

void
c_plssym(PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht, base_scale;

    gscale(&base_scale);
    if (def != 0.0)
	ssym(def, (PLFLT) (scale * def * base_scale));
    else {
	gsym(&defalt, &ht);
	ssym(defalt, (PLFLT) (scale * defalt * base_scale));
    }
}

/*----------------------------------------------------------------------*\
* void pllsty()
*
* Set line style.
\*----------------------------------------------------------------------*/

void
c_pllsty(PLINT lin)
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("pllsty: Please call plstar first.");

    if (lin < 1 || lin > 8)
	plexit("pllsty: Invalid line style.");

    plstyl(line[lin - 1].nels, &line[lin - 1].mark[0], &line[lin - 1].space[0]);

}

/*----------------------------------------------------------------------*\
* void plpat()
*
* Set line width.
\*----------------------------------------------------------------------*/

void
c_plpat(PLINT nlin, PLINT *inc, PLINT *del)
{
    PLINT i, level;

    glev(&level);
    if (level < 1)
	plexit("plpat: Please call plstar first.");

    if (nlin < 1 || nlin > 2)
	plexit("plpat: Only 1 or 2 line styles allowed.");

    for (i = 0; i < nlin; i++) {
	if (del[i] < 0)
	    plexit("plpat: Line spacing must be greater than 0.");
    }
    spat(inc, del, nlin);
}

/*----------------------------------------------------------------------*\
* void plpsty()
*
* Set fill pattern.
\*----------------------------------------------------------------------*/

void
c_plpsty(PLINT patt)
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("plpsty: Please call plstar first.");

    if (patt < 1 || patt > 8)
	plexit("plpsty: Invalid pattern.");

    plpat(pattern[patt - 1].nlines, &pattern[patt - 1].inc[0],
	  &pattern[patt - 1].del[0]);

}
