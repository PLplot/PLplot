//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 16 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x16 {

   // number of shade levels, x data points, y data points, and perimeter points.
   final int NSHADES = 20;
   final int XPTS = 35;
   final int YPTS = 46;
   final int PERIMETERPTS = 100;

   // calculated constants and array that depends on them
   final double XSPA =  2./(XPTS-1);
   final double YSPA =  2./(YPTS-1);
   final double tr[] = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};

   PLStream pls;

   double fmin, fmax;

   // Compute min and max value of a 2-d array.

   void f2mnmx( double[][] f, int nx, int ny )
     {
        fmax = f[0][0];
        fmin = fmax;
	
        for( int i=0; i < nx; i++ )
	  for( int j=0; j < ny; j++ ) {
	     if (f[i][j] < fmin) fmin = f[i][j];
	     if (f[i][j] > fmax) fmax = f[i][j];
	  }
     }

   // Does a variety of shade plots with continuous (cmap1) colours

   public static void main( String[] args ) 
     {
        x16 x = new x16( args );
     }

   public x16( String[] args )
     {
        pls = new PLStream();
	int i, j;
	double x, y, argx, argy, distort, r, t, zmin, zmax;
	double[] px = new double[PERIMETERPTS];
	double[] py = new double[PERIMETERPTS];
	double[] xg0 = new double[XPTS];
	double[] yg0 = new double[YPTS];
	double[] xg1 = new double[XPTS];
	double[] yg1 = new double[YPTS];
	double[][] z = new double[XPTS][YPTS];
	double[][] w = new double[XPTS][YPTS];
	double[][] xg2 = new double[XPTS][YPTS];
	double[][] yg2 = new double[XPTS][YPTS];
	double[] clevel = new double[NSHADES];
	double[] shedge = new double[NSHADES+1];
	final int fill_width = 2, cont_color = 0, cont_width = 0;

	// Parse and process command line arguments.
	pls.ParseOpts( args, pls.PL_PARSE_FULL );

	// Reduce colors in cmap 0 so that cmap 1 is useful on a 
	//16-color display
	pls.scmap0n(3);

	//Initialize plplot
         
          pls.init();
	// Set up data array

	for (i = 0; i < XPTS; i++) {
	   x = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
	   for (j = 0; j < YPTS; j++) {
	      y = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;

	      z[i][j] = - Math.sin(7.*x) * Math.cos(7.*y) + x*x - y*y;
	      w[i][j] = - Math.cos(7.*x) * Math.sin(7.*y) + 2 * x * y;
	   }
	}

	f2mnmx( z, XPTS, YPTS );
	zmin = fmin;
	zmax = fmax;
	
	for (i = 0; i < NSHADES; i++)
	  clevel[i] = zmin + (zmax - zmin) * (i + 0.5) / (double) NSHADES;

	for (i = 0; i < NSHADES+1; i++)
	  shedge[i] = zmin + (zmax - zmin) * (double) i / (double) NSHADES;

	for (i = 0; i < XPTS; i++) {
	   for (j = 0; j < YPTS; j++) {
	      // Replacement for mypltr of x16c.c
	      x = tr[0] * i + tr[1] * j + tr[2];
	      y = tr[3] * i + tr[4] * j + tr[5];

	      argx = x * Math.PI/2;
	      argy = y * Math.PI/2;
	      distort = 0.4;

	      // Note these are one-dimensional because of arrangement of
	      //zeros in the final tr definition above.
	      xg0[i] = x;
	      yg0[j] = y;

	      xg1[i] = x + distort * Math.cos(argx);
	      yg1[j] = y - distort * Math.cos(argy);

	      xg2[i][j] = x + distort * Math.cos(argx) * Math.cos(argy);
	      yg2[i][j] = y - distort * Math.cos(argx) * Math.cos(argy);
	   }
	}

	// Plot using identity transform

	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);

	pls.psty(0);

//API	pls.shades(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
//		 shedge, NSHADES+1, fill_width,
//		 cont_color, cont_width,
//		 plfill, 1, NULL, NULL);

//Temporary replacement for API
	pls.cont(z, shedge, xg0, yg0, 0);
	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);
	pls.lab("distance", "altitude", "Bogon density");

	// Plot using 1d coordinate transform
    
	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);

	pls.psty(0);

//    plshades(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
//	     shedge, NSHADES+1, fill_width,
//	     cont_color, cont_width,
//	     plfill, 1, pltr1, (void *) &cgrid1);

//Temporary replacement for API
        pls.cont(z, shedge, xg1, yg1, 0);
	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);

	pls.lab("distance", "altitude", "Bogon density");

	// Plot using 2d coordinate transform

	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);

	pls.psty(0);

//    plshades(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
//	     shedge, NSHADES+1, fill_width,
//	     cont_color, cont_width,
//	     plfill, 0, pltr2, (void *) &cgrid2);

//Temporary replacement for API
        pls.cont(z, shedge, xg2, yg2, 0);
	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);
	pls.cont(w, clevel, xg2, yg2, 0);

	pls.lab("distance", "altitude", "Bogon density, with streamlines");

	// Plot using 2d coordinate transform

	pls.adv(0);
	pls.vpor(0.1, 0.9, 0.1, 0.9);
	pls.wind(-1.0, 1.0, -1.0, 1.0);

	pls.psty(0);

//    plshades(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
//	     shedge, NSHADES+1, fill_width,
//	     2, 3,
//	     plfill, 0, pltr2, (void *) &cgrid2);

//Temporary replacement for API
        pls.cont(z, shedge, xg2, yg2, 0);

	pls.col0(1);
	pls.box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.col0(2);

	pls.lab("distance", "altitude", "Bogon density");

	// Note this exclusion API will probably change so don't bother
	// with it for x16.java example.


	//Example with polar coordinates.

	pls.adv(0);
	pls.vpor( .1, .9, .1, .9 );
	pls.wind( -1., 1., -1., 1. );

	pls.psty(0);

	// Build new coordinate matrices.
    
	for (i = 0; i < XPTS; i++) {
	   r = ((double) i)/ (XPTS-1);
	   for (j = 0; j < YPTS; j++) {
	      t = (2.*Math.PI/(YPTS-1.))*j;
	      xg2[i][j] = r*Math.cos(t);
	      yg2[i][j] = r*Math.sin(t);
	      z[i][j] = Math.exp(-r*r)*Math.cos(5.*Math.PI*r)*Math.cos(5.*t);
	   }
	}

	//Need a new shedge to go along with the new data set.

	f2mnmx( z, XPTS, YPTS );
	zmin = fmin;
	zmax = fmax;

	for (i = 0; i < NSHADES+1; i++)
	  shedge[i] = zmin + (zmax - zmin) * (double) i / (double) NSHADES;

	// Now we can shade the interior region.
//    plshades(z, XPTS, YPTS, NULL, -1., 1., -1., 1., 
//	     shedge, NSHADES+1, fill_width,
//	     cont_color, cont_width,
//	     plfill, 0, pltr2, (void *) &cgrid2);

//Temporary replacement for API
        pls.cont(z, shedge, xg2, yg2, 0);

// Now we can draw the perimeter.  (If do before, shade stuff may overlap.)
	for (i = 0; i < PERIMETERPTS; i++) {
	   t = (2.*Math.PI/(PERIMETERPTS-1))*(double)i;
	   px[i] = Math.cos(t);
	   py[i] = Math.sin(t);
	}
	pls.col0(1);
	pls.line(PERIMETERPTS, px, py);
                  
	// And label the plot.

	pls.col0(2);
	pls.lab( "", "",  "Tokamak Bogon Instability" );

	// Clean up 

	pls.end();

     }
}

//---------------------------------------------------------------------------//
//                              End of x16.java
//---------------------------------------------------------------------------//

