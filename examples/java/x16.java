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

   PLStreamc plsdummy = new PLStreamc();
   plplotjavac pls = new plplotjavac();

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

/*----------------------------------------------------------------------*\
 * Temporary API for plshades() until it is defined properly in javabind.c
 *
 * Shade regions via a series of calls to plshade.
 * All arguments are the same as plshade except the following:
 * clevel is a pointer to an array of values representing 
 * the shade edge values, nlevel-1 is
 * the number of different shades, (nlevel is the number of shade edges),
 * fill_width is the pattern fill width, and cont_color and cont_width
 * are the color and width of the contour drawn at each shade edge.
 * (if cont_color <= 0 or cont_width <=0, no such contours are drawn).
\*----------------------------------------------------------------------*/

   void plshades_local1(double [][] a,
		  double xmin, double xmax, double ymin, double ymax,
		  double [] clevel, int nlevel, int fill_width,
		  int cont_color, int cont_width, int rectangular,
		  double [] x, double [] y,
		  int wrap)
     //API note, x, and y and wrap are ignored for now because plshade is
     //very incompletely implemented in javabind.c.  Also, in this local
     //version of plshades, clevel array is handled explicitly with int nlevel, but
     //nlevel will disappear for final javabind.c version.
     //
     {
	double shade_min, shade_max, shade_color;
	int i, init_color, init_width;
	
	for (i = 0; i < nlevel-1; i++) {
	   shade_min = clevel[i];
	   shade_max = clevel[i+1];
	   shade_color = i / (double) (nlevel-2);
      /* The constants in order mean 
       * (1) color map1,
       * (0, 0, 0, 0) all edge effects will be done with plcont rather
       * than the normal plshade drawing which gets partially blocked
       * when sequential shading is done as in the present case */
      
	   pls.plshade(a, xmin, xmax, ymin, ymax,
		   shade_min, shade_max,
		   1, shade_color, fill_width,
		   0, 0, 0, 0,
		   rectangular);
	}
	if(cont_color > 0 && cont_width > 0) {
	   // Note, don't bother with cont_color and cont_width setting.
	   // since this local plshades will disappear soon in any case.
//	   pls.plcol0(cont_color);
//	   pls.plwid(cont_width);
	   pls.plcont(a, clevel, x, y, wrap);
	}
}
   void plshades_local2(double [][] a,
		  double xmin, double xmax, double ymin, double ymax,
		  double [] clevel, int nlevel, int fill_width,
		  int cont_color, int cont_width, int rectangular,
		  double [][] x, double [][] y,
		  int wrap)
     //API note, x, and y and wrap are ignored for now because plshade is
     //very incompletely implemented in javabind.c.  Also, in this local
     //version of plshades, clevel array is handled explicitly with int nlevel, but
     //nlevel will disappear for final javabind.c version.
     //
     {
	double shade_min, shade_max, shade_color;
	int i, init_color, init_width;
	
	for (i = 0; i < nlevel-1; i++) {
	   shade_min = clevel[i];
	   shade_max = clevel[i+1];
	   shade_color = i / (double) (nlevel-2);
      /* The constants in order mean 
       * (1) color map1,
       * (0, 0, 0, 0) all edge effects will be done with plcont rather
       * than the normal plshade drawing which gets partially blocked
       * when sequential shading is done as in the present case */
      
	   pls.plshade(a, xmin, xmax, ymin, ymax,
		   shade_min, shade_max,
		   1, shade_color, fill_width,
		   0, 0, 0, 0,
		   rectangular);
	}
	if(cont_color > 0 && cont_width > 0) {
	   // Note, don't bother with cont_color and cont_width setting.
	   // since this local plshades will disappear soon in any case.
//	   pls.plcol0(cont_color);
//	   pls.plwid(cont_width);
	   pls.plcont(a, clevel, x, y, wrap);
   }
}
   
   // Does a variety of shade plots with continuous (cmap1) colours

   public static void main( String[] args ) 
     {
        x16 x = new x16( args );
     }

   public x16( String[] args )
     {
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
//	pls.plParseOpts( args, pls.PL_PARSE_FULL );

	// Reduce colors in cmap 0 so that cmap 1 is useful on a 
	//16-color display
	pls.plscmap0n(3);

	//Initialize plplot
         
          pls.plinit();
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

	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);

	pls.plpsty(0);

	plshades_local1(z, -1., 1., -1., 1., 
		 shedge, NSHADES+1, fill_width,
		 cont_color, cont_width,
		 1, xg0, yg0, 0);

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);
	pls.pllab("distance", "altitude", "Bogon density");

	// Plot using 1d coordinate transform
    
	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);

	pls.plpsty(0);

	plshades_local1(z, -1., 1., -1., 1., 
	     shedge, NSHADES+1, fill_width,
	     cont_color, cont_width,
	     1, xg1, yg1, 0);

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);

	pls.pllab("distance", "altitude", "Bogon density");

	// Plot using 2d coordinate transform

	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);

	pls.plpsty(0);

	plshades_local2(z, -1., 1., -1., 1., 
		 shedge, NSHADES+1, fill_width,
		 cont_color, cont_width,
		 0, xg2, yg2, 0);

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);
	pls.plcont(w, clevel, xg2, yg2, 0);

	pls.pllab("distance", "altitude", "Bogon density, with streamlines");

	// Plot using 2d coordinate transform

	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);

	pls.plpsty(0);

	plshades_local2(z, -1., 1., -1., 1., 
		 shedge, NSHADES+1, fill_width,
		 2, 3,
		 0, xg2, yg2, 0);

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);

	pls.pllab("distance", "altitude", "Bogon density");

	// Note this exclusion API will probably change so don't bother
	// with it for x16.java example.


	//Example with polar coordinates.

	pls.pladv(0);
	pls.plvpor( .1, .9, .1, .9 );
	pls.plwind( -1., 1., -1., 1. );

	pls.plpsty(0);

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

	plshades_local2(z, -1., 1., -1., 1., 
		 shedge, NSHADES+1, fill_width,
		 cont_color, cont_width,
		 0, xg2, yg2, 0);

// Now we can draw the perimeter.  (If do before, shade stuff may overlap.)
	for (i = 0; i < PERIMETERPTS; i++) {
	   t = (2.*Math.PI/(PERIMETERPTS-1))*(double)i;
	   px[i] = Math.cos(t);
	   py[i] = Math.sin(t);
	}
	pls.plcol0(1);
	pls.plline(PERIMETERPTS, px, py);
                  
	// And label the plot.

	pls.plcol0(2);
	pls.pllab( "", "",  "Tokamak Bogon Instability" );

	// Clean up 

	pls.plend();

     }
}

//---------------------------------------------------------------------------//
//                              End of x16.java
//---------------------------------------------------------------------------//

