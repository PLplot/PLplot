/* $Id$
   $Log$
   Revision 1.3  1992/11/07 08:04:28  mjl
   Fixed a problem encountered when a user tried to change the default
   character/symbol scale heights.

 * Revision 1.2  1992/09/29  04:46:16  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
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
    PLFLT defalt, ht, base_scale;

    gscale(&base_scale);
    if (def != 0.0)
	schr((PLFLT) def * base_scale, (PLFLT) (scale * def));
    else {
	gchr(&defalt, &ht);
	schr(defalt, (PLFLT) (scale * defalt));
    }
}

/* Sets up lengths of minor tick marks. */

void
c_plsmin (PLFLT def, PLFLT scale)
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

/* Sets up lengths of major tick marks. */

void
c_plsmaj (PLFLT def, PLFLT scale)
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

/* Set symbol height. */

void
c_plssym (PLFLT def, PLFLT scale)
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
