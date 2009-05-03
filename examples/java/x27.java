//---------------------------------------------------------------------------//
// $Id$
// Drawing "spirograph" curves - epitrochoids, cycolids, roulettes
//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
// Copyright (C) 2007  Arjen Markus
// Copyright (C) 2008  Andrew Ross 
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
// Implementation of PLplot example 27 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x27 {

    PLStream pls = new PLStream();

    public static void main( String[] args )
    {
	new x27( args );
    }
    
    //--------------------------------------------------------------------
    // Generates two kinds of plots:
    //   - construction of a cycloid (animated)
    //   - series of epitrochoids and hypotrochoids
    //--------------------------------------------------------------------

    public x27( String[] args) 
    {
	// R, r, p, N
	double params[][] = {
	    {21.0,  7.0,  7.0,  3.0},  // Deltoid
	    {21.0,  7.0, 10.0,  3.0},
	    {21.0, -7.0, 10.0,  3.0},
	    {20.0,  3.0,  7.0, 20.0},
	    {20.0,  3.0, 10.0, 20.0},
	    {20.0, -3.0, 10.0, 20.0},
	    {20.0, 13.0,  7.0, 20.0},
	    {20.0, 13.0, 20.0, 20.0},
	    {20.0,-13.0, 20.0, 20.0} } ;
	
	int i ;
    
	// plplot initialization

	// Parse and process command line arguments
	pls.parseopts(args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM);
	
	// Initialize plplot
	pls.init();
	
	// Illustrate the construction of a cycloid
	cycloid();
	
	// Loop over the various curves
	// First an overview, then all curves one by one
	pls.ssub(3, 3) ; // Three by three window 
	
	for ( i = 0; i < 9; i ++ ) {
	    pls.adv(0) ;
	    pls.vpor( 0.0, 1.0, 0.0, 1.0 ) ;
	    spiro( params[i] ) ;
	}
	
	pls.adv(0);
	pls.ssub(1, 1) ; // One window per curve
	
	for ( i = 0; i < 9; i ++ ) {
	    pls.adv(0) ;
	    pls.vpor( 0.0, 1.0, 0.0, 1.0 ) ;
	    spiro( params[i] ) ;
	}

	pls.end();
	
    }

    // ===============================================================
    
    void cycloid()
    {
	// TODO
    }

    // ===============================================================
    
    void spiro( double params[] )
    {
	int NPNT = 20000;
	double xcoord[];
	double ycoord[];
	
	int   windings     ;
	int   steps        ;
	int   i            ;
	double phi          ;
	double phiw         ;
	double dphi         ;
	double xmin         ;
	double xmax         ;
	double ymin         ;
	double ymax         ;
	double scale        ;
	
	// Fill the coordinates
	
	windings = (int) params[3] ;
	steps    = NPNT/windings ;
	dphi     = 8.0*Math.acos(-1.0)/(double)steps ;
	
	xmin = 0.0 ; // This initialisation is safe!
	xmax = 0.0 ;
	ymin = 0.0 ;
	ymax = 0.0 ;
	
	xcoord = new double[windings*steps+1];
	ycoord = new double[windings*steps+1];

	for ( i = 0; i <= windings*steps ; i ++ ) {
	    phi       = (double)i * dphi ;
	    phiw      = (params[0]-params[1])/params[1]*phi ;
	    xcoord[i] = (params[0]-params[1])*Math.cos(phi) + params[2]*Math.cos(phiw) ;
	    ycoord[i] = (params[0]-params[1])*Math.sin(phi) - params[2]*Math.sin(phiw) ;
	    
	    if ( xmin > xcoord[i] ) xmin = xcoord[i] ;
	    if ( xmax < xcoord[i] ) xmax = xcoord[i] ;
	    if ( ymin > ycoord[i] ) ymin = ycoord[i] ;
	    if ( ymax < ycoord[i] ) ymax = ycoord[i] ;
	}
	
	if ( xmax-xmin > ymax-ymin ) {
	    scale = xmax - xmin ;
	} else {
	    scale = ymax - ymin ;
	}
	xmin = - 0.65 * scale ;
	xmax =   0.65 * scale ;
	ymin = - 0.65 * scale ;
	ymax =   0.65 * scale ;
	
	pls.wind( xmin, xmax, ymin, ymax ) ;
	
	pls.col0(1);
	pls.line( xcoord, ycoord ) ;
	
    }
    
}

//---------------------------------------------------------------------------//
//                              End of x27.java
//---------------------------------------------------------------------------//
