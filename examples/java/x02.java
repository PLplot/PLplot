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

    PLStream pls;

    public static void main( String[] args ) 
    {
        System.out.println( "x02 not implemented yet." );

        x02 x = new x02( args );
    }

    x02( String[] args )
    {
        int i, j;
        double vmin, vmax;
        NumberFormat nf = NumberFormat.getNumberInstance();

        pls = new PLStream();

    // Divide screen into 16 regions.

        pls.ssub(4, 4);

    // Parse and process command line arguments.

//         (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

    // Initialize plplot.

        pls.init();

        pls.schr(0.0, 3.5);
        pls.font(4);

        for (i = 0; i <= 15; i++) {
            pls.col0(i);

            String text = nf.format(i);

            pls.adv(0);
            vmin = 0.1;
            vmax = 0.9;
            for (j = 0; j <= 2; j++) {
                pls.wid(j + 1);
                pls.vpor(vmin, vmax, vmin, vmax);
                pls.wind(0.0, 1.0, 0.0, 1.0);
                pls.box("bc", 0.0, 0, "bc", 0.0, 0);
                vmin = vmin + 0.1;
                vmax = vmax - 0.1;
            }
            pls.wid(1);
            pls.ptex(0.5, 0.5, 1.0, 0.0, 0.5, text);
        }

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x02.java
//---------------------------------------------------------------------------//
