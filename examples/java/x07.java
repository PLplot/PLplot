//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 7 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x07 {

    static int base[] = {
        0, 200, 500, 600, 700, 800, 900,
        2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900
    };

    public static void main( String[] args ) 
    {
        x07 x = new x07( args );
    }

    public x07( String[] args )
    {
       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

//        NumberFormat nf = NumberFormat.getNumberInstance();
//        Change to this custom format to get stupid locale commas
//        separating hundreds and thousands place out of labels.
        DecimalFormat nf = new DecimalFormat("#####");

        int i, j, k, l;
        double x[] = new double[1];
        double y[] = new double[1];

    // Parse and process command line arguments.

//        pls.plParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.plinit();

        pls.plfontld(1);
        for( l = 0; l < base.length; l++ ) {
            pls.pladv(0);

        // Set up viewport and window.

            pls.plcol0(2);
            pls.plvpor(0.15, 0.95, 0.1, 0.9);
            pls.plwind(0.0, 1.0, 0.0, 1.0);

        // Draw the grid using plbox.

            pls.plbox("bcgt", 0.1, 0, "bcgt", 0.1, 0);

        // Write the digits below the frame.

            pls.plcol0(15);
            for (i = 0; i <= 9; i++) {
                String text = nf.format(i);
                pls.plmtex("b", 1.5, (0.1 * i + 0.05), 0.5, text);
            }

            k = 0;
            for( i = 0; i <= 9; i++ ) {

            // Write the digits to the left of the frame.

                String text = nf.format( base[l] + 10*i );
                pls.plmtex("lv", 1.0, (0.95 - 0.1 * i), 1.0, text);
                for( j = 0; j <= 9; j++ ) {
                    x[0] = 0.1 * j + 0.05;
                    y[0] = 0.95 - 0.1 * i;

                // Display the symbols.
		// N.B. plsym expects arrays so that is what we give it.

                    pls.plsym( x, y, base[l] + k );
                    k = k + 1;
                }
            }

            pls.plmtex("t", 1.5, 0.5, 0.5, "PLplot Example 7 - PLSYM symbols");
        }
        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x07.java
//---------------------------------------------------------------------------//
