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

    public static void main( String[] args ) 
    {
        x05 x = new x05( args );
    }

    public x05( String[] args )
    {
       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

        int i;
        double[] data = new double[NPTS];
        double delta;

    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

    // Initialize plplot.

        pls.plinit();

    // Fill up data points.

        delta = 2.0 * Math.PI / (double) NPTS;
        for (i = 0; i < NPTS; i++)
            data[i] = Math.sin(i * delta);

        pls.plcol0(1);
        pls.plhist(data, -1.1, 1.1, 44, 0);
        pls.plcol0(2);
        pls.pllab( "#frValue", "#frFrequency",
                 "#frPLplot Example 5 - Probability function of Oscillator" );

        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x05.java
//---------------------------------------------------------------------------//
