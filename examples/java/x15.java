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

// Does a variety of shade plots with discrete colours and patterns.

    public static void main( String[] args ) 
    {
        x15 x = new x15( args );
    }

    public x15( String[] args )
    {
        int i, j;
	
        double xx;
        double yy;
        double x;
        double y;
        double[][] z = new double[XPTS][YPTS];
	double zmin, zmax;
	double[][] xg0 = new double[XPTS][YPTS];
	double[][] yg0 = new double[XPTS][YPTS];
	
    // Parse and process command line arguments.

//        pls.plParseOpts( args, pls.PL_PARSE_FULL );
	
    // Initialize plplot.

        pls.plinit();

    // no need to fiddle with color maps since example uses default cmap0
    // and the cmap1 fiddling that x15c.c does is completely irrelevant
    // (although interesting).

        for( i = 0; i < XPTS; i++ )
        {
            xx = (double) (i - (XPTS/2)) / (double) (XPTS/2);
            for( j = 0; j < YPTS; j++ )
            {
                yy = ((double) (j - (YPTS/2)) / (double) (YPTS/2)) - 1.;
                z[i][j] = xx*xx - yy*yy + (xx - yy)/(xx*xx+yy*yy + 0.1);
	       x = tr[0] * i + tr[1] * j + tr[2];
	       y = tr[3] * i + tr[4] * j + tr[5];
	       // Note xg0 ==> yg1 are one-dimensional because of arrangement of
	       //zeros in the final tr definition above.  However, for now
	       //we are using raw interface here so must nominally treat them
	       //as two-dimensional.
	       xg0[i][j] = x;
	       yg0[i][j] = y;
            }
        }
	
        f2mnmx( z, XPTS, YPTS );
        zmin = fmin;
        zmax = fmax;
	
	plot1( xg0, yg0, z, zmin, zmax );
	plot2( xg0, yg0, z, zmin, zmax );
	
        pls.plend();
    }

// Illustrates a single shaded region.

    void plot1( double[][] xg0, double[][] yg0, double[][] z, double zmin, double zmax)
    {
	double shade_min, shade_max, sh_color;
	int sh_cmap = 0, sh_width;
	int min_color = 0, min_width = 0, max_color = 0, max_width = 0;
	
	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);
	
    /* Plot using identity transform */

	shade_min = zmin + (zmax-zmin)*0.4;
	shade_max = zmin + (zmax-zmin)*0.6;
	sh_color = 7;
	sh_width = 2;
	min_color = 9;
	max_color = 2;
	min_width = 2;
	max_width = 2;
     
	pls.plpsty(8);

        pls.plshade( z, -1., 1., -1., 1.,
                   shade_min, shade_max,
                   sh_cmap, sh_color, sh_width,
                   min_color, min_width, max_color, max_width,
		     1, xg0, yg0 );

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);
	pls.pllab("distance", "altitude", "Bogon flux");
    }

// Illustrates multiple adjacent shaded regions, using different fill
// patterns for each region.

    void plot2(double[][] xg0, double[][] yg0, double[][] z, double zmin, double zmax)
    {
	double shade_min, shade_max, sh_color;
	int sh_cmap = 0, sh_width;
	int min_color = 0, min_width = 0, max_color = 0, max_width = 0;
	int i;
	sh_width = 2;
	
	pls.pladv(0);
	pls.plvpor(0.1, 0.9, 0.1, 0.9);
	pls.plwind(-1.0, 1.0, -1.0, 1.0);
	
    /* Plot using identity transform */

	for (i = 0; i < 10; i++) {
            shade_min = zmin + (zmax - zmin) * i / 10.0;
            shade_max = zmin + (zmax - zmin) * (i +1) / 10.0;
            sh_color = i+6;
            pls.plpsty((i + 2) % 8 + 1);
       
            pls.plshade( z, -1., 1., -1., 1.,
                       shade_min, shade_max,
                       sh_cmap, sh_color, sh_width,
                       min_color, min_width, max_color, max_width,
			 1, xg0, yg0 );
	}

	pls.plcol0(1);
	pls.plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
	pls.plcol0(2);
	pls.pllab("distance", "altitude", "Bogon flux");
    }

}

//---------------------------------------------------------------------------//
//                              End of x15.java
//---------------------------------------------------------------------------//
