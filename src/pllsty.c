/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:37  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plfill.c

	Sets fill pattern.
*/

#include "plplot.h"

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

void 
c_pllsty(lin)
PLINT lin;
{
    PLINT level;

    glev(&level);
    if (level < 1)
	plexit("Please call plstar before calling pllsty.");
    if (lin < 1 || lin > 8)
	plexit("Invalid line in pllsty.");
    plstyl(line[lin - 1].nels, &line[lin - 1].mark[0], &line[lin - 1].space[0]);

}
