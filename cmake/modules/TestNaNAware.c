/* Test for NaN awareness. */
#include <stdio.h>
#if defined(_AIX)
#include "math.h"
#else
#include "nan.h"
#endif

int
main()
{
#if defined(_AIX)
   double x = NAN;
#else
   double x = NaN;
#endif
   /* N.B. CMake run-time tests need return of true (1) or false (0) not
    * the usual status code of 0 for success. */
#if defined(_MSC_VER) | defined(__BORLANDC__)
   if (isnan(x)) return 1;
#endif

   if (x != x) return 1;
   else return 0;
}
