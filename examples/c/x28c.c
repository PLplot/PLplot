/* $Id$

	plmtex3 demo.
*/

#include "plcdemos.h"

/*--------------------------------------------------------------------------*\
 * main
 *
 * Demonstrates plotting text in 3D.
\*--------------------------------------------------------------------------*/


int main(int argc, char *argv[])
{
/* Parse and process command line arguments */
   
       (void) plparseopts(&argc, argv, PL_PARSE_FULL);
   
	plinit();
	pladv(0);
	plvpor(0.0, 1.0, 0.0, 0.9);
	plwind(-1.0, 1.0, -0.9, 1.1);
	plw3d(1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, 0.0, 2.0, 40.0, 60.0+270.0);

    	plcol0(1);
	plbox3(	"bnst", "", 0.0, 0,
			"bnst", "", 0.0, 0,
	     	"bcdmnstv", "", 0.0, 0);
	plmtex3("xpv", 4.5, 0.5, 0.5, "one");
	plmtex3("xs", 4.5, 0.5, 0.5, "two");
	plmtex3("ypv", 4.5, 0.5, 0.5, "three");
	plmtex3("ys", 4.5, 0.5, 0.5, "------- four ------");
	plmtex3("zpv", 4.5, 0.5, 0.5, "five");
	plmtex3("zs", 4.5, 0.5, 0.5, "six");

	plend();
}
