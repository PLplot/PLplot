//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 5 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x05 {

    final int NPTS = 2047;

    PLStream pls;

    public static void main( String[] args ) 
    {
        x05 x = new x05( args );
    }

    public x05( String[] args )
    {
        pls = new PLStream();

        int i;
        double[] data = new double[NPTS];
        double delta;

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();

    // Fill up data points.

        delta = 2.0 * Math.PI / (double) NPTS;
        for (i = 0; i < NPTS; i++)
            data[i] = Math.sin(i * delta);

        pls.col0(1);
        pls.hist( NPTS, data, -1.1, 1.1, 44, 0);
        pls.col0(2);
        pls.lab( "#frValue", "#frFrequency",
                 "#frPLplot Example 5 - Probability function of Oscillator" );

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x05.java
//---------------------------------------------------------------------------//
