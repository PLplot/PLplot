/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:16  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:46  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plsdef.c

	Routines to set default lengths (char, symbol, tick).
*/

#include "plplot.h"

/* Sets character height. */

void
c_plschr (PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht;

    if (def != 0.0)
	schr(def, (PLFLT) (scale * def));
    else {
	gchr(&defalt, &ht);
	schr(defalt, (PLFLT) (scale * defalt));
    }
}

/* Sets up lengths of minor tick marks. */

void
c_plsmin (PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht;

    if (def != 0.0)
	smin(def, (PLFLT) (scale * def));
    else {
	gmin(&defalt, &ht);
	smin(defalt, (PLFLT) (scale * defalt));
    }
}

/* Sets up lengths of major tick marks. */

void
c_plsmaj (PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht;

    if (def != 0.0)
	smaj(def, (PLFLT) (scale * def));
    else {
	gmaj(&defalt, &ht);
	smaj(defalt, (PLFLT) (scale * defalt));
    }
}

/* Set symbol height. */

void
c_plssym (PLFLT def, PLFLT scale)
{
    PLFLT defalt, ht;

    if (def != 0.0)
	ssym(def, (PLFLT) (scale * def));
    else {
	gsym(&defalt, &ht);
	ssym(defalt, (PLFLT) (scale * defalt));
    }
}
