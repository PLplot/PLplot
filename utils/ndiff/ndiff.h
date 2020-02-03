#if !defined(NDIFF_H_)

#include <confix.h>

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_STDIO_H
#include <stdio.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#define NDIFF_H_

#if HAVE_GMP
    #include <gmp.h>
    typedef mpf_t fp_t;
    #define ARITHMETIC     "multiple-precision arithmetic"
#elif HAVE_LONG_DOUBLE
    typedef long double fp_t;
    #define ARITHMETIC     "extended-precision arithmetic"
#else
    typedef double fp_t;
    #define ARITHMETIC     "double-precision arithmetic"
#endif

extern void store(fp_t *x);

#endif  /* !defined(NDIFF_H_) */
