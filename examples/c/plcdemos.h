/* $Id$
 * $Log$
 * Revision 1.4  2000/05/10 20:35:58  furnish
 * Include plConfig.h to get PLDOUBLE definition.  Rob Managan.
 *
 * Revision 1.3  1995/06/01 21:40:04  mjl
 * All C demo files: changed file inclusion to use quotes instead of angle
 * brackets so that dependencies are retained during development.
 *
 * Revision 1.2  1995/05/07  02:22:20  mjl
 * Added include of ctype.h for the demos that need it.
 *
 * Revision 1.1  1995/04/12  08:21:11  mjl
 * Created to keep all the junk common to the C demos.  Helps to keep me
 * from cluttering up plplot.h.
*/

/*	plcdemos.h

	Everything needed by the C demo programs.
	Created to avoid junking up plplot.h with this stuff.
*/

#ifndef __PLCDEMOS_H__
#define __PLCDEMOS_H__

#include "plConfig.h"
#include "plplot.h"
#include <math.h>
#include <string.h>
#include <ctype.h>

/* define PI if not defined by math.h */

#ifndef PI
#define PI 3.1415926535897932384
#endif

/* various utility macros */

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#ifndef ROUND
#define ROUND(a)    (PLINT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif

#endif	/* __PLCDEMOS_H__ */
