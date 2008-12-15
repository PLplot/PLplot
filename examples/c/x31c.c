/* $Id$

   set/get tester
*/

#include "plcdemos.h"

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates absolute positioning of graphs on a page.
 \*--------------------------------------------------------------------------*/

int
main(int argc, const char *argv[])
{
  PLFLT xmin, xmax, ymin, ymax;
  /* Parse and process command line arguments */

  (void) plparseopts(&argc, argv, PL_PARSE_FULL);

  /* Initialize plplot */

  plinit();

  pladv(0);
  plvpor(0.0, 1.0, 0.0, 1.0);

  plwind(0.2, 0.3, 0.4, 0.5);
  plgvpw(&xmin, &xmax, &ymin, &ymax);
  if (xmin != 0.2 || xmax != 0.3 || ymin != 0.4 || ymax != 0.5) {
    fprintf(stderr, "%s\n", "plgvpw test failed");
    plend();
    exit(1);
  }
  plend();
  exit(0);
}
