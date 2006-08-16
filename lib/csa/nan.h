/******************************************************************************
 *
 * File:           nan.h
 *
 * Created:        18/10/2001
 *
 * Author:         Pavel Sakov
 *                 CSIRO Marine Research
 *
 * Purpose:        NaN definition
 *
 * Description:    Should cover machines with 64 bit doubles or other machines
 *                 with GCC
 *
 * Revisions:      None
 *
 *****************************************************************************/

#if !defined(_NAN_H)
#define _NAN_H

#if defined(__GNUC__) && !defined(__ICC)

static const double NaN = 0.0 / 0.0;

#elif defined(_WIN32)

#if !defined(_MSC_VER) || _MSC_VER != 600

static unsigned _int64 lNaN = ((unsigned _int64) 1 << 63) - 1;

#define NaN (*(double*)&lNaN)

#else

#include <float.h>
#include <ymath.h>
#undef NaN
#define NaN _Nan._D
#define isnan _isnan
#define copysign _copysign

#endif

#else

static const long long lNaN = ((unsigned long long) 1 << 63) - 1;

#define NaN (*(double*)&lNaN)

#endif

#endif
