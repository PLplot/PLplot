//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 2 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x02 {

    public static void main( String[] args ) 
    {
        x02 x = new x02( args );
    }

    x02( String[] args )
    {
        int i, j;
        double vmin, vmax;
        NumberFormat nf = NumberFormat.getNumberInstance();

       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

    // Divide screen into 16 regions.

        pls.plssub(4, 4);

    // Parse and process command line arguments.

//        pls.plParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.plinit();

        pls.plschr(0.0, 3.5);
        pls.plfont(4);

        for (i = 0; i <= 15; i++) {
            pls.plcol0(i);

            String text = nf.format(i);

            pls.pladv(0);
            vmin = 0.1;
            vmax = 0.9;
            for (j = 0; j <= 2; j++) {
                pls.plwid(j + 1);
                pls.plvpor(vmin, vmax, vmin, vmax);
                pls.plwind(0.0, 1.0, 0.0, 1.0);
                pls.plbox("bc", 0.0, 0, "bc", 0.0, 0);
                vmin = vmin + 0.1;
                vmax = vmax - 0.1;
            }
            pls.plwid(1);
            pls.plptex(0.5, 0.5, 1.0, 0.0, 0.5, text);
        }

        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x02.java
//---------------------------------------------------------------------------//
