/* Test for NaN awareness. */
#include <stdio.h>
#if defined(__GNUC__)
static const double NaN = 0.0 / 0.0;
#elif defined(BIG_ENDIAN) || defined(_BIG_ENDIAN)
static const long long lNaN = 0x7fffffffffffffff;
# define NaN (*(double*)&lNaN)
#else
#if !defined(_MSC_VER)
static const long long lNaN = 0xfff8000000000000;
# define NaN (*(double*)&lNaN)
#else
#include <float.h>
#include <ymath.h>
#undef NaN
#define NaN _Nan._D
#define isnan _isnan
#endif
#endif

int
main()
{
   double x = NaN;
   /* N.B. CMake run-time tests need return of true (1) or false (0) not
    * the usual status code of 0 for success. */
#if defined(_MSC_VER)
   if (isnan(x)) return 1;
#endif

   if (x != x) return 1;
   else return 0;
}
