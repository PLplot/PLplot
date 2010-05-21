//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2001  Geoffrey Furnish
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Implementation of PLplot example 19 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class MapTransform implements PLCallbackCT {
    public void coordTransform( double x, double y, double[] xt, double[] yt, Object data )
    {
        double radius;

        radius = 90.0 - y;
        xt[0]  = radius * Math.cos( x * Math.PI / 180.0 );
        yt[0]  = radius * Math.sin( x * Math.PI / 180.0 );
    }
}

class Mapform19 implements PLCallbackMapform {
    public void mapform( double[] x, double[] y )
    {
        int    i;
        double xp, yp, radius;
        for ( i = 0; i < x.length; i++ )
        {
            radius = 90.0 - y[i];
            xp     = radius * Math.cos( x[i] * Math.PI / 180.0 );
            yp     = radius * Math.sin( x[i] * Math.PI / 180.0 );
            x[i]   = xp;
            y[i]   = yp;
        }
    }
}

class LabelFunc19 implements PLCallbackLabel {
    // A custom axis labeling function for longitudes and latitudes.
    public String label( int axis, double value )
    {
        String label           = "";
        String direction_label = "";
        double label_val       = 0.0;

        if ( axis == PLStream.PL_Y_AXIS )
        {
            label_val = value;
            if ( label_val > 0.0 )
            {
                direction_label = " N";
            }
            else if ( label_val < 0.0 )
            {
                direction_label = " S";
            }
            else
            {
                direction_label = "Eq";
            }
        }
        else if ( axis == PLStream.PL_X_AXIS )
        {
            label_val = normalize_longitude( value );
            if ( label_val > 0.0 )
            {
                direction_label = " E";
            }
            else if ( label_val < 0.0 )
            {
                direction_label = " W";
            }
            else
            {
                direction_label = "";
            }
        }
        if ( axis == PLStream.PL_Y_AXIS && value == 0.0 )
        {
            /* A special case for the equator */
            label = direction_label;
        }
        else
        {
            label = "" + ( (int) Math.abs( label_val ) ) + direction_label;
        }
        return label;
    }

    // "Normalize" longitude values so that they always fall between -180.0
    // and 180.0
    double normalize_longitude( double lon )
    {
        double times;

        if ( lon >= -180.0 && lon <= 180.0 )
        {
            return ( lon );
        }
        else
        {
            times = Math.floor( ( Math.abs( lon ) + 180.0 ) / 360.0 );
            if ( lon < 0.0 )
            {
                return ( lon + 360.0 * times );
            }
            else
            {
                return ( lon - 360.0 * times );
            }
        }
    }
}

class x19 {
    PLStream pls = new PLStream();

    public static void main( String[] args )
    {
        new x19( args );
    }

    public x19 ( String[] args )
    {
        double            minx, maxx, miny, maxy;
        PLCallbackMapform nullCallback        = null;
        PLCallbackLabel   nullLabelCallback   = null;
        PLCallbackCT      nullCTCallback      = null;
        LabelFunc19       geolocation_labeler = new LabelFunc19();
        MapTransform      map_transform       = new MapTransform();

        double[] x = new double[1];
        double[] y = new double[1];

        // Parse and process command line arguments.
        pls.parseopts( args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM );

        // Longitude (x) and latitude (y)

        miny = -70;
        maxy = 80;

        // Initialize PLplot.
        pls.init();
        // Cartesian plots
        // Most of world

        minx = 190;
        maxx = 190 + 360;

        // Setup a custom latitude and longitude-based scaling function.
        pls.slabelfunc( geolocation_labeler );

        pls.col0( 1 );
        pls.env( minx, maxx, miny, maxy, 1, 70 );
        pls.map( nullCallback, "usaglobe", minx, maxx, miny, maxy );

        // The Americas

        minx = 190;
        maxx = 340;

        pls.col0( 1 );
        pls.env( minx, maxx, miny, maxy, 1, 70 );
        pls.map( nullCallback, "usaglobe", minx, maxx, miny, maxy );

        // Clear the labelling function.
        pls.slabelfunc( nullLabelCallback );

        // Polar, Northern hemisphere

        // Create callback object containing mapform function
        Mapform19 mapform19 = new Mapform19();

        minx = 0;
        maxx = 360;

        pls.env( -75., 75., -75., 75., 1, -1 );
        pls.map( mapform19, "globe", minx, maxx, miny, maxy );

        pls.lsty( 2 );
        pls.meridians( mapform19, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );


        // Polar, Northern hemisphere, this time with a PLplot-wide transform

        minx = 0;
        maxx = 360;

        pls.stransform( map_transform, null );

        pls.lsty( 1 );
        pls.env( -75., 75., -75., 75., 1, -1 );
        // No need to set the map transform here as the global transform
        // will be used.
        pls.map( nullCallback, "globe", minx, maxx, miny, maxy );

        pls.lsty( 2 );
        pls.meridians( nullCallback, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );

        // Show Baltimore, MD on the map
        pls.col0( 2 );
        pls.ssym( 0.0, 2.0 );
        x[0] = -76.6125;
        y[0] = 39.2902778;
        pls.poin( x, y, 18 );
        pls.ssym( 0.0, 1.0 );
        pls.ptex( -76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD" );

        // For Java, this is how the global transform is cleared
        pls.stransform( nullCTCallback, null );

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x19.java
//---------------------------------------------------------------------------//
