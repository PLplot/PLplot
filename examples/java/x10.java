//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 10 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x10 {

    public static void main( String[] args ) 
    {
        x10 x = new x10( args );
    }

    public x10( String[] args )
    {
        PLStream pls = new PLStream();

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();

        pls.adv(0);
        pls.vpor(0.0, 1.0, 0.0, 1.0);
        pls.wind(0.0, 1.0, 0.0, 1.0);
        pls.box("bc", 0.0, 0, "bc", 0.0, 0);

        pls.svpa(50.0, 150.0, 50.0, 100.0);
        pls.wind(0.0, 1.0, 0.0, 1.0);
        pls.box("bc", 0.0, 0, "bc", 0.0, 0);
        pls.ptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)");
        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x10.java
//---------------------------------------------------------------------------//
