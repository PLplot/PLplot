//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 8 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x08 {

    final int XPTS = 35;
    final int YPTS = 46;

    static int opt[] = {1, 2, 3, 3};
    static double alt[] = {60.0, 20.0, 60.0, 60.0};
    static double az[] = {30.0, 60.0, 120.0, 160.0};

    static String[] title =
    {
        "#frPLplot Example 8 - Alt=60, Az=30, Opt=1",
        "#frPLplot Example 8 - Alt=20, Az=60, Opt=2",
        "#frPLplot Example 8 - Alt=60, Az=120, Opt=3",
        "#frPLplot Example 8 - Alt=60, Az=160, Opt=3"
    };

    PLStream pls;

// Does a series of 3-d plots for a given data set, with different viewing
// options in each plot.

    public static void main( String[] args ) 
    {
        x08 x = new x08( args );
    }

    public x08( String[] args )
    {
        pls = new PLStream();

        double PI = 4. * Math.atan(1.);

        int i, j, k;

        double[] x = new double[ XPTS ];
        double[] y = new double[ YPTS ];
        double[][] z = new double[XPTS][YPTS];

        double xx, yy, r;

        int n_col = 256;
        int[] rr = new int[256];
        int[] gg = new int[256];
        int[] bb = new int[256];

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();

        for( i=0; i < XPTS; i++ )
            x[i] = (i - (XPTS/2.)) / (XPTS/2.);

        for( j=0; j < YPTS; j++ )
            y[j] = (j - (YPTS/2.)) / (YPTS/2.);

        for( i = 0; i < XPTS; i++ )
        {
            xx = x[i];
            for( j = 0; j < YPTS; j++ )
            {
                yy = y[j];
                r = Math.sqrt(xx * xx + yy * yy);
                z[i][j] = Math.exp(-r * r) * Math.cos(2.0 * PI * r);
            }
        }

        for( k = 0; k < 4; k++ )
        {
            pls.adv(0);
            pls.vpor(0.0, 1.0, 0.0, 0.9);
            pls.wind(-1.0, 1.0, -0.9, 1.1);
            pls.col0(1);
            pls.w3d( 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
                     alt[k], az[k] );
            pls.box3( "bnstu", "x axis", 0.0, 0,
                      "bnstu", "y axis", 0.0, 0,
                      "bcdmnstuv", "z axis", 0.0, 0 );

            pls.col0(2);
            pls.plot3d( x, y, z, opt[k], 1 );
            pls.col0(3);
            pls.mtex("t", 1.0, 0.5, 0.5, title[k]);
        }

        pls.lightsource( 1., 1., 1. );
        for( i=0; i < n_col; i++ )
            rr[i] = gg[i] = bb[i] = i*256/n_col;
        pls.scmap1( rr, gg, bb, n_col );
    	
        for( k = 0; k < 4; k++ )
        {
            pls.adv(0);
            pls.vpor(0.0, 1.0, 0.0, 0.9);
            pls.wind(-1.0, 1.0, -0.9, 1.1);
            pls.col0(1);
            pls.w3d( 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
                     alt[k], az[k] );
            pls.box3( "bnstu", "x axis", 0.0, 0,
                      "bnstu", "y axis", 0.0, 0,
                      "bcdmnstuv", "z axis", 0.0, 0 );

            pls.col0(2);
            pls.plotsh3d( x, y, z, 0 );
            pls.col0(3);
            pls.mtex("t", 1.0, 0.5, 0.5, title[k]);
        }

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x08.java
//---------------------------------------------------------------------------//
