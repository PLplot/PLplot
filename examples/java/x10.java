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
       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

    // Initialize plplot.

        pls.plinit();

        pls.pladv(0);
        pls.plvpor(0.0, 1.0, 0.0, 1.0);
        pls.plwind(0.0, 1.0, 0.0, 1.0);
        pls.plbox("bc", 0.0, 0, "bc", 0.0, 0);

        pls.plsvpa(50.0, 150.0, 50.0, 100.0);
        pls.plwind(0.0, 1.0, 0.0, 1.0);
        pls.plbox("bc", 0.0, 0, "bc", 0.0, 0);
        pls.plptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)");
        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x10.java
//---------------------------------------------------------------------------//
