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

    PLStreamc plsdummy = new PLStreamc();
    plplotjavac pls = new plplotjavac();

    static double alt[] = {60.0, 20.0};
    static double az[] = {30.0, 60.0};
   
    static String[] title =
     {
	"#frPLplot Example 8 - Alt=60, Az=30",
	"#frPLplot Example 8 - Alt=20, Az=60",
     };
    // cmap1_init1
    
    // Initializes color map 1 in HLS space.
    // Basic grayscale variation from half-dark (which makes more interesting
    // looking plot compared to dark) to light.
    // An interesting variation on this:
    // s[1] = 1.0
    
    void cmap1_init(int gray)
    {
	double [] i = new double[2];
	double [] h = new double[2];
	double [] l = new double[2];
	double [] s = new double[2];
	int [] rev = new int[2];
	  
        i[0] = 0.0;         // left boundary
        i[1] = 1.0;         // right boundary

        if (gray==1) {
	   h[0] = 0.0;         // hue -- low: red (arbitrary if s=0)
	   h[1] = 0.0;         // hue -- high: red (arbitrary if s=0)
	  
	   l[0] = 0.5;         // lightness -- low: half-dark
	   l[1] = 1.0;         // lightness -- high: light
	   
	   s[0] = 0.0;         // minimum saturation
	   s[1] = 0.0;         // minimum saturation
	}
        else {
	   h[0] = 240; /* blue -> green -> yellow -> */
	   h[1] = 0;   /* -> red */
	   
	   l[0] = 0.6;
	   l[1] = 0.6;
	   
	   s[0] = 0.8;
	   s[1] = 0.8;
	}
       
        rev[0] = 0;         // interpolate on front side of colour wheel.
        rev[1] = 0;         // interpolate on front side of colour wheel.
	  
        pls.plscmap1n(256);
        pls.plscmap1l(0, i, h, l, s, rev);
    }

// Does a series of 3-d plots for a given data set, with different viewing
// options in each plot.

    public static void main( String[] args ) 
    {
        x08 x = new x08( args );
    }

    public x08( String[] args )
    {
        int i, j, k;
	final int LEVELS = 10;

        double[] x = new double[ XPTS ];
        double[] y = new double[ YPTS ];
        double[][] z = new double[XPTS][YPTS];
	double clevel[] = new double[LEVELS];
        double clev_null [] = new double[0];

        double xx, yy, r;
	double zmin=Double.MAX_VALUE, zmax=Double.MIN_VALUE;

        int ifshade;

    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );
        boolean rosen = true;

    // Initialize plplot.

        pls.plinit();

        for( i=0; i < XPTS; i++ ) {
	   x[i] = (double) (i - (XPTS/2)) / (double) (XPTS/2);
	   if (rosen)
	     x[i] *=  1.5;
	}

        for( j=0; j < YPTS; j++ ) {
	   y[j] = (double) (j - (YPTS/2)) / (double) (YPTS/2);
	   if (rosen)
	     y[j] += 0.5;
	}

        for( i = 0; i < XPTS; i++ )
        {
            xx = x[i];
            for( j = 0; j < YPTS; j++ )
            {
	       yy = y[j];
	       if (rosen) {
		  z[i][j] = Math.log(Math.pow(1. - xx,2) + 100 * 
				     Math.pow(yy - Math.pow(xx,2),2));
		  
		  if (Double.isInfinite(z[i][j])) /* the log() of the function may become -inf */
		    z[i][j] = -5.; /* -MAXFLOAT would mess-up up the scale */
	       }
	       else {
		  r = Math.sqrt(xx * xx + yy * yy);
		  z[i][j] = Math.exp(-r * r) * Math.cos(2.0 * Math.PI * r);
	       }
	       if (zmin > z[i][j])
		 zmin = z[i][j];
	       if (zmax < z[i][j])
		 zmax = z[i][j];
            }
        }

	double step = (zmax-zmin)/(LEVELS+1);
	for (i=0; i<LEVELS; i++)
	    clevel[i] = zmin + step*(i+1);

        pls.pllightsource( 1., 1., 1. );
        for( k = 0; k < 2; k++ )
        {
	   for( ifshade = 0; ifshade < 4; ifshade++)
	   {
	      pls.pladv(0);
	      pls.plvpor(0.0, 1.0, 0.0, 0.9);
	      pls.plwind(-1.0, 1.0, -0.9, 1.1);
	      pls.plcol0(3);
	      pls.plmtex("t", 1.0, 0.5, 0.5, title[k]);
	      pls.plcol0(1);
	      if(rosen)
		pls.plw3d( 1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, -5.0, 7.0,
			   alt[k], az[k] );
	      else
		pls.plw3d( 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
			   alt[k], az[k] );
	      pls.plbox3( "bnstu", "x axis", 0.0, 0,
			"bnstu", "y axis", 0.0, 0,
			"bcdmnstuv", "z axis", 0.0, 0 );

	      pls.plcol0(2);

	      if (ifshade == 0) { /* diffuse light surface plot */
		 cmap1_init(1);
		 // with new interface haven't yet made null work so have
		 // to put in specific zero-length array.
		 pls.plsurf3d( x, y, z, 0, clev_null );
	      } else if (ifshade == 1) { /* magnitude colored plot */
		 cmap1_init(0);
		 pls.plsurf3d( x, y, z, pls.MAG_COLOR, clev_null );
	      } else if (ifshade == 2) { /*  magnitude colored plot with faceted squares */
		 pls.plsurf3d( x, y, z, pls.MAG_COLOR | pls.FACETED, clev_null );
	      } else                     /* magnitude colored plot with contours */
		 pls.plsurf3d( x, y, z, pls.MAG_COLOR | pls.SURF_CONT | pls.BASE_CONT, clevel );
	   }
        }

        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x08.java
//---------------------------------------------------------------------------//
