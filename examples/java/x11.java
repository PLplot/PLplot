//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 11 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x11 {

    PLStreamc plsdummy = new PLStreamc();
    plplotjavac pls = new plplotjavac();

    final int XPTS = 35;
    final int YPTS = 46;
    final int LEVELS = 10;

    static int opt[] = {3, 3};
    static double alt[] = {33.0, 17.0};
    static double az[] = {24.0, 115.0};

    static String[] title =
    {
        "#frPLplot Example 11 - Alt=33, Az=17, Opt=3",
        "#frPLplot Example 11 - Alt=24, Az=115, Opt=3"
    };

    void cmap1_init()
    {
	double [] i = new double[2];
	double [] h = new double[2];
	double [] l = new double[2];
	double [] s = new double[2];
	int [] rev = new int[2];
	  
        i[0] = 0.0;         // left boundary
        i[1] = 1.0;         // right boundary

        h[0] = 240; /* blue -> green -> yellow -> */
        h[1] = 0;   /* -> red */
	   
        l[0] = 0.6;
        l[1] = 0.6;
	   
        s[0] = 0.8;
        s[1] = 0.8;
       
        rev[0] = 0;         // interpolate on front side of colour wheel.
        rev[1] = 0;         // interpolate on front side of colour wheel.
	  
        pls.plscmap1n(256);
        pls.plscmap1l(0, i, h, l, s, rev);
    }

// Does a series of mesh plots for a given data set, with different viewing
// options in each plot.

    public static void main( String[] args ) 
    {
        x11 x = new x11( args );
    }

    public x11( String[] args )
    {
        int i, j, k;

        double[] x = new double[ XPTS ];
        double[] y = new double[ YPTS ];
        double[][] z = new double[XPTS][YPTS];
	double zmin=Double.MAX_VALUE, zmax=Double.MIN_VALUE;

        double xx, yy;
        int nlevel = LEVELS;
        double[] clevel = new double[LEVELS];
        double step;
       

    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

    // Initialize plplot.

        pls.plinit();

        for( i=0; i < XPTS; i++ )
            x[i] = 3.* (double) (i - (XPTS/2)) / (double) (XPTS/2);

        for( j=0; j < YPTS; j++ )
            y[j] = 3.* (double) (j - (YPTS/2)) / (double) (YPTS/2);

        for( i = 0; i < XPTS; i++ )
        {
            xx = x[i];
            for( j = 0; j < YPTS; j++ )
            {
                yy = y[j];
                z[i][j] = 3. * (1.-xx)*(1.-xx) * Math.exp(-(xx*xx) - (yy+1.)*(yy+1.)) -
		         10. * (xx/5. - Math.pow(xx,3.) - Math.pow(yy,5.)) * Math.exp(-xx*xx-yy*yy) -
		         1./3. * Math.exp(-(xx+1)*(xx+1) - (yy*yy));
	       if(false) { /* Jungfraujoch/Interlaken */
		  if (z[i][j] < -1.)
		    z[i][j] = -1.;
	       }
	       if (zmin > z[i][j])
		 zmin = z[i][j];
	       if (zmax < z[i][j])
		 zmax = z[i][j];
            }
        }

        step = (zmax - zmin)/(nlevel+1);
        for (i=0; i<nlevel; i++)
	    clevel[i] = zmin + step + step*i;
       
        cmap1_init();
        for( k = 0; k < 2; k++ ) {
	    for ( i = 0; i < 4; i++) {
	       pls.pladv(0);
	       pls.plcol0(1);
	       pls.plvpor(0.0, 1.0, 0.0, 0.9);
	       pls.plwind(-1.0, 1.0, -1.0, 1.5);

	       pls.plw3d( 1.0, 1.0, 1.2, -3.0, 3.0, -3.0, 3.0, -7., 8.,
			  alt[k], az[k] );
	       pls.plbox3( "bnstu", "x axis", 0.0, 0,
			   "bnstu", "y axis", 0.0, 0,
			   "bcdmnstuv", "z axis", 0.0, 4 );

	       pls.plcol0(2);

	       /* wireframe plot */
	       if (i==0)
		 pls.plmesh(x, y, z, opt[k]);
	       
	       /* magnitude colored wireframe plot */
	       else if (i==1)
		 pls.plmesh(x, y, z, opt[k] | pls.MAG_COLOR);
	       
	       /* magnitude colored wireframe plot with sides */
	       else if (i==2)
		 pls.plot3d(x, y, z, opt[k] | pls.MAG_COLOR, 1);
	       
	       /* magnitude colored wireframe plot with base contour */
	       else if (i==3)
		 pls.plmeshc(x, y, z, opt[k] | pls.MAG_COLOR | pls.BASE_CONT,
			 clevel);
	       
	       
	       pls.plcol0(3);
	       pls.plmtex("t", 1.0, 0.5, 0.5, title[k]);
	    }
	}

        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x11.java
//---------------------------------------------------------------------------//
