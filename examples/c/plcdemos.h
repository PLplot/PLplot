/* $Id$

	Everything needed by the C demo programs.
	Created to avoid junking up plplot.h with this stuff.
*/

#ifndef __PLCDEMOS_H__
#define __PLCDEMOS_H__

/* Next line added by Rafael Laboissiere <rafael@icp.inpg.fr>
   on Sat Nov  6 17:35:14 CET 1999  */
#define PL_DOUBLE

#include "plplot/plConfig.h"
#include "plplot/plplot.h"
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
