/* $Id$
   $Log$
   Revision 1.9  1994/03/23 08:24:33  mjl
   Added support for hardware fill patterns (negative).

   All external API source files: replaced call to plexit() on simple
   (recoverable) errors with simply printing the error message (via
   plabort()) and returning.  Should help avoid loss of computer time in some
   critical circumstances (during a long batch run, for example).

 * Revision 1.8  1993/11/15  08:40:21  mjl
 * Comment fix.
 *
 * Revision 1.7  1993/07/16  22:36:33  mjl
 * Minor change to comments.
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
* Set symbol height.
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
    if (level < 1) {
	plabort("pllsty: Please call plinit first");
	return;
    }
    if (lin < 1 || lin > 8) {
	plabort("pllsty: Invalid line style");
	return;
    }

    plstyl(line[lin - 1].nels,
	   &line[lin - 1].mark[0], &line[lin - 1].space[0]);
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
    if (level < 1) {
	plabort("plpat: Please call plinit first");
	return;
    }
    if (nlin < 1 || nlin > 2) {
	plabort("plpat: Only 1 or 2 line styles allowed");
	return;
    }
    for (i = 0; i < nlin; i++) {
	if (del[i] < 0) {
	    plabort("plpat: Line spacing must be greater than 0");
	    return;
	}
    }
    plP_spat(inc, del, nlin);
}

/*----------------------------------------------------------------------*\
* void plpsty()
*
* Set fill pattern, using one of the predefined patterns.
* A fill pattern <= 0 indicates hardware fill.
\*----------------------------------------------------------------------*/

void
c_plpsty(PLINT patt)
{
    PLINT level;

    plP_glev(&level);
    if (level < 1) {
	plabort("plpsty: Please call plinit first");
	return;
    }
    if (patt > 8) {
	plabort("plpsty: Invalid pattern");
	return;
    }
    plP_spsty(patt);
    if (patt > 0) {
	plP_spat(&pattern[patt - 1].inc[0], &pattern[patt - 1].del[0],
		 pattern[patt - 1].nlines);
    }
}

