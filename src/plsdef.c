/* $Id$
   $Log$
   Revision 1.7  1993/07/16 22:36:33  mjl
   Minor change to comments.

 * Revision 1.6  1993/07/01  22:13:42  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible internal
 * plplot functions now start with "plP_".
 *
 * Revision 1.5  1993/02/23  05:19:29  mjl
 * Changed references in error messages from plstar to plinit.  Also changed
 * behavior of size-setting routines (should not affect user code).  Fixed
 * data structure initializers to be ANSI-conformant.
 *
 * Revision 1.4  1993/01/23  05:58:48  mjl
 * Holds functions that modify plplot defaults.  These do not need direct
 * access to the stream data.
 *
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

#include "plplotP.h"

/* Line pattern defaults */

static struct line {
    PLINT nels;
    PLINT mark[4];
    PLINT space[4];
} line[] = {

    {
	0,			/* Continuous line */
	{
	    0, 0, 0, 0
	},
	{
	    0, 0, 0, 0
	}
    },
    {
	1,
	{
	    1000, 0, 0, 0
	},
	{
	    1000, 0, 0, 0
	}
    },
    {
	1,
	{
	    2000, 0, 0, 0
	},
	{
	    2000, 0, 0, 0
	}
    },
    {
	1,
	{
	    2000, 0, 0, 0
	},
	{
	    1000, 0, 0, 0
	}
    },
    {
	2,
	{
	    2500, 1000, 0, 0
	},
	{
	    1000, 1000, 0, 0
	}
    },
    {
	2,
	{
	    2000, 1000, 0, 0
	},
	{
	    2000, 1000, 0, 0
	}
    },
    {
	3,
	{
	    1000, 1500, 2000, 0
	},
	{
	    1000, 1500, 2000, 0
	}
    },
    {
	3,
	{
	    1000, 1500, 2000, 0
	},
	{
	    1000, 1000, 1000, 0
	}
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
	{
	    0, 0
	},
	{
	    2000, 0
	}
    },
    {
	1,
	{
	    900, 0
	},
	{
	    2000, 0
	}
    },
    {
	1,
	{
	    450, 0
	},
	{
	    2000, 0
	}
    },
    {
	1,
	{
	    -450, 0
	},
	{
	    2000, 0
	}
    },
    {
	1,
	{
	    300, 0
	},
	{
	    2000, 0
	}
    },
    {
	1,
	{
	    -300, 0
	},
	{
	    2000, 0
	}
    },
    {
	2,
	{
	    0, 900
	},
	{
	    2000, 2000
	}
    },
    {
	2,
	{
	    450, -450
	},
	{
	    2000, 2000
	}
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
    PLFLT defalt, ht;

    if (def != 0.0)
	plP_schr((PLFLT) (def), (PLFLT) (scale * def));
    else {
	plgchr(&defalt, &ht);
	plP_schr(defalt, (PLFLT) (scale * defalt));
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
    PLFLT defalt, ht;

    if (def != 0.0)
	plP_smin(def, (PLFLT) (scale * def));
    else {
	plP_gmin(&defalt, &ht);
	plP_smin(defalt, (PLFLT) (scale * defalt));
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
    PLFLT defalt, ht;

    if (def != 0.0)
	plP_smaj(def, (PLFLT) (scale * def));
    else {
	plP_gmaj(&defalt, &ht);
	plP_smaj(defalt, (PLFLT) (scale * defalt));
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
    PLFLT defalt, ht;

    if (def != 0.0)
	plP_ssym(def, (PLFLT) (scale * def));
    else {
	plP_gsym(&defalt, &ht);
	plP_ssym(defalt, (PLFLT) (scale * defalt));
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

    plP_glev(&level);
    if (level < 1)
	plexit("pllsty: Please call plinit first.");

    if (lin < 1 || lin > 8)
	plexit("pllsty: Invalid line style.");

    plstyl(line[lin - 1].nels, &line[lin - 1].mark[0], &line[lin - 1].space[0]);

}

/*----------------------------------------------------------------------*\
* void plpat()
*
* Set fill pattern directly.
\*----------------------------------------------------------------------*/

void
c_plpat(PLINT nlin, PLINT *inc, PLINT *del)
{
    PLINT i, level;

    plP_glev(&level);
    if (level < 1)
	plexit("plpat: Please call plinit first.");

    if (nlin < 1 || nlin > 2)
	plexit("plpat: Only 1 or 2 line styles allowed.");

    for (i = 0; i < nlin; i++) {
	if (del[i] < 0)
	    plexit("plpat: Line spacing must be greater than 0.");
    }
    plP_spat(inc, del, nlin);
}

/*----------------------------------------------------------------------*\
* void plpsty()
*
* Set fill pattern, using one of the predefined patterns.
\*----------------------------------------------------------------------*/

void
c_plpsty(PLINT patt)
{
    PLINT level;

    plP_glev(&level);
    if (level < 1)
	plexit("plpsty: Please call plinit first.");

    if (patt < 1 || patt > 8)
	plexit("plpsty: Invalid pattern.");

    plpat(pattern[patt - 1].nlines, &pattern[patt - 1].inc[0],
	  &pattern[patt - 1].del[0]);

}
