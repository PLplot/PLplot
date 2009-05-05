/* Auxiliary program: write the include file for determining
   PLplot's floating-point type
*/

#include <stdio.h>
#include <stdlib.h>
#include "plConfig.h"

int main(int argc, char *argv[] )
{
   FILE *outfile ;
   char *kind ;

   outfile = fopen( "plflt.inc", "w" ) ;
#ifdef PL_DOUBLE
   kind = "1.0d0";
#else
   kind = "1.0";
#endif

   fprintf( outfile, "\
! NOTE: Generated code\n\
!\n\
! Type of floating-point numbers in PLplot\n\
!\n\
      integer, parameter :: plf   = kind(%s)\n\
      integer, parameter :: plflt = plf\n", kind ) ;
  fclose( outfile ) ;
  return 0 ;
}
