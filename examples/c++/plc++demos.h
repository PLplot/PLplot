/* $Id$

	All the common includes and constants needed by the C++ demo programs.
*/

#ifndef __PLCXXDEMOS_H__
#define __PLCXXDEMOS_H__

/* plplot actually includes the C versions of these 2, but
 * add here for completeness */
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <cstring>
#include <cmath>

/* common includes */
#include "plstream.h"

/* common defines */
#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

/* various utility macros */
#ifndef ROUND
#define ROUND(a) (PLINT)((a)<0. ? ((a)-0.5) : ((a)+0.5))
#endif

/* Add in missing isnan definition if required */
#if defined(PL__HAVE_ISNAN)
#  define isnan _isnan
#  if defined(_MSC_VER)
#    include <float.h>
#  endif
#endif

#if !defined(PL_HAVE_ISNAN)
#  define isnan(x) ((x) != (x))
#endif

#endif	/* __PLCXXDEMOS_H__ */
