/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:13  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:44  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plpsty.c

	Sets fill pattern.
*/

#include "plplot.h"

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

void 
c_plpsty( PLINT patt )
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling plpsty.");
    if (patt < 1 || patt > 8)
	plexit("Invalid pattern in plpsty.");
    plpat(pattern[patt - 1].nlines, &pattern[patt - 1].inc[0],
	  &pattern[patt - 1].del[0]);

}
