/* Test for NaN awareness. */
#include <stdio.h>
#include "nan.h"

int
main()
{
   double x = NaN;
   /* N.B. CMake run-time tests need return of true (1) or false (0) not
    * the usual status code of 0 for success. */
#if defined(_MSC_VER) | defined(__BORLANDC__)
   if (isnan(x)) return 1;
#endif

   if (x != x) return 1;
   else return 0;
}
