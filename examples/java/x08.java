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
   
    // cmap1_init1
    
    // Initializes color map 1 in HLS space.
    // Basic grayscale variation from half-dark (which makes more interesting
    // looking plot compared to dark) to light.
    // An interesting variation on this:
    // s[1] = 1.0
    
    void cmap1_init()
    {
	double [] i = new double[2];
	double [] h = new double[2];
	double [] l = new double[2];
	double [] s = new double[2];
	int [] rev = new int[2];
	  
        i[0] = 0.0;         // left boundary
        i[1] = 1.0;         // right boundary
	  
        h[0] = 0.0;         // hue -- low: red (arbitrary if s=0)
        h[1] = 0.0;         // hue -- high: red (arbitrary if s=0)
	  
        l[0] = 0.5;         // lightness -- low: half-dark
        l[1] = 1.0;         // lightness -- high: light
	  
        s[0] = 0.0;         // minimum saturation
        s[1] = 0.0;         // minimum saturation
        rev[0] = 0;         // interpolate on front side of colour wheel.
        rev[1] = 0;         // interpolate on front side of colour wheel.
	  
        pls.scmap1n(256);
        //API pls.scmap1l(0, 2, i, h, l, s, rev);
    }
   
     

// Does a series of 3-d plots for a given data set, with different viewing
// options in each plot.

    public static void main( String[] args ) 
    {
        x08 x = new x08( args );
    }

    public x08( String[] args )
    {
        pls = new PLStream();

        int i, j, k;

        double[] x = new double[ XPTS ];
        double[] y = new double[ YPTS ];
        double[][] z = new double[XPTS][YPTS];

        double xx, yy, r;

        int ifshade;

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();

        for( i=0; i < XPTS; i++ )
            x[i] = (double) (i - (XPTS/2)) / (double) (XPTS/2);

        for( j=0; j < YPTS; j++ )
            y[j] = (double) (j - (YPTS/2)) / (double) (YPTS/2);

        for( i = 0; i < XPTS; i++ )
        {
            xx = x[i];
            for( j = 0; j < YPTS; j++ )
            {
                yy = y[j];
                r = Math.sqrt(xx * xx + yy * yy);
                z[i][j] = Math.exp(-r * r) * Math.cos(2.0 * Math.PI * r);
            }
        }

        pls.lightsource( 1., 1., 1. );
        cmap1_init();
    	
        for( k = 0; k < 4; k++ )
        {
	   for( ifshade = 0; ifshade < 2; ifshade++)
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
	      if( ifshade == 1) 
		pls.plotsh3d( x, y, z, 0 );
	      else
		pls.plot3d( x, y, z, opt[k], 1 );
	      pls.col0(3);
	      pls.mtex("t", 1.0, 0.5, 0.5, title[k]);
	   }
        }

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x08.java
//---------------------------------------------------------------------------//
