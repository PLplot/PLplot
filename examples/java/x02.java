//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2001  Geoffrey Furnish
// Copyright (C) 2002  Alan W. Irwin
// Copyright (C) 2004  Andrew Ross
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
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

        PLplot pls = new PLplot();

    // Divide screen into 16 regions.

        pls.ssub(4, 4);

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

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
