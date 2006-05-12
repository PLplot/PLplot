/* Auxiliary program: write the include file for determining
   PLplot's floating-point type
*/

#include <stdio.h>
#include <stdlib.h>
#include "plConfig.h"

main(int argc, char *argv[] )
{
   FILE *outfile;
   char *kind;

   outfile = fopen( "plflt.inc", "w" ) ;
#ifdef PL_DOUBLE
   kind = "1.0d0";
#else
   kind = "1.0";
#endif

   fprintf( outfile, "!     NOTE: Generated code\n");
   fprintf( outfile, "!\n");
   fprintf( outfile, "!     Type of floating-point numbers in PLplot\n");
   fprintf( outfile, "!\n");
   fprintf( outfile, "      integer, parameter :: plf   = kind(%s)\n", kind);
   fprintf( outfile, "      integer, parameter :: plflt = plf\n");
   fclose( outfile);
}
