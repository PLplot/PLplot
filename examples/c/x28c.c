/* $Id$

	plmtex3, plptex3 demo.
*/

#include "plcdemos.h"
/* Choose these values to correspond to tick marks. */
#define XPTS 7
#define YPTS 5

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates plotting text in 3D.
\*--------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
   PLFLT *x, *y, **z, xmin=-1.5, xmax=1.5, ymin=-0.5, 
     ymax=1.5, zmin=0., zmax = 2.;
   int i,j;
   /* Parse and process command line arguments */
   
   (void) plparseopts(&argc, argv, PL_PARSE_FULL);
   
   plinit();
   x = (PLFLT *) calloc(XPTS, sizeof(PLFLT));
   y = (PLFLT *) calloc(YPTS, sizeof(PLFLT));

   plAlloc2dGrid(&z, XPTS, YPTS);
   for (i = 0; i < XPTS; i++) {
      x[i] = xmin + (double) i * (xmax-xmin)/ (double) (XPTS-1);
   }

   for (j = 0; j < YPTS; j++)
     y[j] = ymin + (double) j * (ymax-ymin)/ (double) (YPTS-1);

   for (i = 0; i < XPTS; i++) {
      for (j = 0; j < YPTS; j++) {
	 z[i][j] = 0.;
      }
   }

   pladv(0);
   plvpor(0.0, 1.0, 0.0, 0.9);
   plwind(-1.2, 1.2, -0.8, 1.5);
   plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 
	 40.0, 60.0 + 270.0);
   
   plcol0(2);
   plbox3("bnt", "", 0.5, 0,
	  "bnt", "", 0.0, 0,
	  "bcdmntv", "", 0.0, 0);
   plmtex3("xpv", 2.5, 0.5, 1.0, "one");
   plmtex3("xs", -1.0, 0.5, 0.5, "two");
   plmtex3("ypv", -0.5, 0.6, 0.0, "three");
   plmtex3("ys", 3.0, 0.5, 0.5, "------- four ------");
   plmtex3("zpv", 2.5, 0.5, 1.0, "five");
   plmtex3("zs", 3.0, 0.5, 0.5, "six");
   plptex3(xmin, 0.0, 0.0,  1.0, 0.0, 0.0,  0.0, 0.2, 0.0,  0.0,
	   "seven(a)");
   plptex3(xmin, 0.0, 0.5,  1.0, 0.0, 0.0,  -0.1, 0.2, 0.0,  0.0,
	   "eight(a)");
   plptex3(xmin, 0.0, 1.0,  1.0, 0.0, 0.0,  -0.2, 0.2, 0.0,  0.0,
	   "nine(a)");
   plptex3(xmin, 0.0, 1.5,  1.0, 0.0, 0.0,  -0.3, 0.2, 0.0,  0.0,
	   "ten(a)");
   plptex3(xmin, 0.0, 2.0,  1.0, 0.0, 0.0,  -0.4, 0.2, 0.0,  0.0,
	   "eleven(a)");
   plptex3(xmin, 1.0, 0.0,  1.0, 0.0, 0.0,  0.0, 0.0, 0.2,  0.0,
	   "seven(b)");
   plptex3(xmin, 1.0, 0.5,  1.0, 0.0, 0.0,  0.0, 0.1, 0.2,  0.0,
	   "eight(b)");
   plptex3(xmin, 1.0, 1.0,  1.0, 0.0, 0.0,  0.0, 0.2, 0.2,  0.0,
	   "nine(b)");
   plptex3(xmin, 1.0, 1.5,  1.0, 0.0, 0.0,  0.0, 0.3, 0.2,  0.0,
	   "ten(b)");
   plptex3(xmin, 1.0, 2.0,  1.0, 0.0, 0.0,  0.0, 0.4, 0.2,  0.0,
	   "eleven(b)");
   /* Draw 3D grid (where the grid lines correspond to tick marks because
    * of XPTS, YPTS choice) at Z = 0. */
   plmesh(x, y, z, XPTS, YPTS, DRAW_LINEXY);
   
   plend();
}
