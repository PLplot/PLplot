//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 15 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x15 {

   final int XPTS = 35;
   final int YPTS = 46;

   PLStream pls;

// Does a variety of shade plots with discrete colours and patterns.

   public static void main( String[] args ) 
     {
        x15 x = new x15( args );
     }

   public x15( String[] args )
     {
        pls = new PLStream();

        int i, j;
	
        double xx;
        double yy;
        double[][] z = new double[XPTS][YPTS];
	double zmin, zmax;
	
	// Parse and process command line arguments.
	// 
        pls.ParseOpts( args, pls.PL_PARSE_FULL );
	
	// Initialize plplot.
	// 
        pls.init();
	// no need to fiddle with color maps since example uses default cmap0
	// and the cmap1 fiddling that x15c.c does is completely irrelevant
	// (although interesting).
	// 
        for( i = 0; i < XPTS; i++ )
	  {
	     xx = (double) (i - (XPTS/2)) / (double) (XPTS/2);
	     for( j = 0; j < YPTS; j++ )
	       {
		  yy = ((double) (j - (YPTS/2)) / (double) (YPTS/2)) - 1.;
		  z[i][j] = xx*xx - yy*yy + (xx - yy)/(xx*xx+yy*yy + 0.1);
	       }
	  }
	
// Copy from x09.java once this function is implemented there.
//	f2mnmx(&z[0][0], XPTS, YPTS, &zmin, &zmax);
//	temporary
	zmin = 0.;
	zmax = 31.;
//      end of temporary
	
//AWI, Please fix calls and argument lists.  I don't know how to 
//pass arrays to java functions.
	plot1(/*z,*/ zmin, zmax);
	plot2(/*z,*/ zmin, zmax);
	
        pls.end();
     }
   void plot1(/*z,*/ double zmin, double zmax)
// Illustrates a single shaded region.
     {
	double shade_min, shade_max, sh_color;
	int sh_cmap = 0, sh_width;
	int min_color = 0, min_width = 0, max_color = 0, max_width = 0;
	
	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);
	
	/* Plot using identity transform */
	

	shade_min = zmin + (zmax-zmin)*0.4;
	shade_max = zmin + (zmax-zmin)*0.6;
	sh_color = 7;
	sh_width = 2;
	min_color = 9;
	max_color = 2;
	min_width = 2;
	max_width = 2;
     
	pls.psty(8);
/*API	like python?  See xw15.py and plmodules.c
	pls.shade(z, XPTS, YPTS, -1., 1., -1., 1., 
		 shade_min, shade_max, 
		 sh_cmap, sh_color, sh_width,
		 min_color, min_width, max_color, max_width, 1) */

	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);
	pls.lab("distance", "altitude", "Bogon flux");
     }

   void plot2(/*z,*/ double zmin, double zmax)
// Illustrates multiple adjacent shaded regions, using different fill
// patterns for each region.
     {
	double shade_min, shade_max, sh_color;
	int sh_cmap = 0, sh_width;
	int min_color = 0, min_width = 0, max_color = 0, max_width = 0;
	int i;
	sh_width = 2;
	
	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);
	
	/* Plot using identity transform */

	for (i = 0; i < 10; i++) {
	   shade_min = zmin + (zmax - zmin) * i / 10.0;
	   shade_max = zmin + (zmax - zmin) * (i +1) / 10.0;
	   sh_color = i+6;
	   pls.psty((i + 2) % 8 + 1);
       
/*API	like python?  See xw15.py and plmodules.c
	pls.shade(z, XPTS, YPTS, -1., 1., -1., 1., 
		 shade_min, shade_max, 
		 sh_cmap, sh_color, sh_width,
		 min_color, min_width, max_color, max_width, 1) */
	}

	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);
	pls.lab("distance", "altitude", "Bogon flux");
     }

}

//---------------------------------------------------------------------------//
//                              End of x15.java
//---------------------------------------------------------------------------//
