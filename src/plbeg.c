/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:12  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	plbeg.c

	Sets up the device "dev" for plotting, dividing the page into 
	"nx" by "ny" subpages.
*/

#include "plplot.h"
#include <math.h>

void 
c_plbeg(dev, nx, ny)
PLINT dev, nx, ny;
{
    PLFLT scale, def, ht;

    if ((nx <= 0) || (ny <= 0))
	plexit("Cannot have negative number of subpages in plstar.");

    scale = 1.0 / sqrt((double) ny);
    slev(1);
    grbeg(dev);
    ssub(nx, ny, 0);

    /* Set up character, symbol and tick sizes for requested number of */
    /* subpages */

    gchr(&def, &ht);
    schr((PLFLT) (def * scale), (PLFLT) (def * scale));
    gsym(&def, &ht);
    ssym((PLFLT) (def * scale), (PLFLT) (def * scale));
    gmaj(&def, &ht);
    smaj((PLFLT) (def * scale), (PLFLT) (def * scale));
    gmin(&def, &ht);
    smin((PLFLT) (def * scale), (PLFLT) (def * scale));
}
