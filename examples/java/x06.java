//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 6 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x06 {

    public static void main( String[] args ) 
    {
        x06 x = new x06( args );
    }

    public x06( String[] args )
    {
       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

        NumberFormat nf = NumberFormat.getNumberInstance();

        int i, j, k;
        double x[] = new double[1];
        double y[] = new double[1];

    // Parse and process command line arguments.

//        pls.plParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot

        pls.plinit();

        pls.pladv(0);

    // Set up viewport and window.

        pls.plcol0(2);
        pls.plvpor(0.1, 1.0, 0.1, 0.9);
        pls.plwind(0.0, 1.0, 0.0, 1.3);

    // Draw the grid using plbox.

        pls.plbox("bcgt", 0.1, 0, "bcgt", 0.1, 0);

    // Write the digits below the frame.

        pls.plcol0(15);
        for( i = 0; i <= 9; i++ ) {
            String text = nf.format(i);
            pls.plmtex("b", 1.5, (0.1 * i + 0.05), 0.5, text);
        }

        k = 0;
        for( i = 0; i <= 12; i++ ) {

        // Write the digits to the left of the frame.

            String text = nf.format( 10*i);
            pls.plmtex("lv", 1.0, (1.0 - (2 * i + 1) / 26.0), 1.0, text);
            for( j = 0; j <= 9; j++ ) {
                x[0] = 0.1 * j + 0.05;
                y[0] = 1.25 - 0.1 * i;

            // Display the symbols (plpoin expects that x and y are arrays so
            // that is what we feed it).

                if (k < 128)
                    pls.plpoin( x, y, k );
                k = k + 1;
            }
        }

        pls.plmtex("t", 1.5, 0.5, 0.5, "PLplot Example 6 - plpoin symbols");
        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x06.java
//---------------------------------------------------------------------------//
