// xId: plmem.c 11966 2011-10-14 07:10:05Z andrewross $
//
//  plmem.c
//
//  Copyright (C) 1992, 1993, 1994, 1995
//  Maurice LeBrun			mjl@dino.ph.utexas.edu
//  Institute for Fusion Studies	University of Texas at Austin
//
//  Copyright (C) 2004  Joao Cardoso
//  Copyright (C) 2004  Alan W. Irwin
//  Copyright (C) 2004  Andrew Ross
//
//  This file is part of PLplot.
//
//  PLplot is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Library General Public License as published
//  by the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  PLplot is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with PLplot; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//--------------------------------------------------------------------------
//

//! @file
//!  These functions provide allocation and deallocation of two-dimensional
//!  arrays.
//!

#include "plplotP.h"

//--------------------------------------------------------------------------
// plAlloc2dGrid()
//
//! Allocates a block of memory for use as a 2-d grid of PLFLT's.
//! Resulting array can be indexed as f[i][j] anywhere.  This is to be used
//! instead of PLFLT f[nx][ny], which is less useful.  Note that this type
//! of allocation is required by the PLplot functions which take a 2-d
//! grids of PLFLT's as an argument, such as plcont() and plot3d().
//! Example usage:
//!
//!   PLFLT **z;
//!
//!   Alloc2dGrid(&z, XPTS, YPTS);
//!
//! @param f Location of the storage (address of a **).
//! @param nx Size of the grid in x.
//! @param ny Size of the grid in y.
//!
//--------------------------------------------------------------------------

void
plAlloc2dGrid( PLFLT ***f, PLINT nx, PLINT ny )
{
    PLINT i;

    if ( ( *f = (PLFLT **) calloc( (size_t) nx, sizeof ( PLFLT * ) ) ) == NULL )
        plexit( "Memory allocation error in \"plAlloc2dGrid\"" );

    for ( i = 0; i < nx; i++ )
    {
        if ( ( ( *f )[i] = (PLFLT *) calloc( (size_t) ny, sizeof ( PLFLT ) ) ) == NULL )
            plexit( "Memory allocation error in \"plAlloc2dGrid\"" );
    }
}

//--------------------------------------------------------------------------
// Free2dGrid()
//
//! Frees a block of memory allocated with Alloc2dGrid().
//!
//! @param f The [][] to the storage.
//! @param nx Size of the grid in x.
//! @param PL_UNUSED( ny) Not used.
//--------------------------------------------------------------------------

void
plFree2dGrid( PLFLT **f, PLINT nx, PLINT PL_UNUSED( ny ) )
{
    PLINT i;

    for ( i = 0; i < nx; i++ )
        free( (void *) f[i] );

    free( (void *) f );
}

//--------------------------------------------------------------------------
// MinMax2dGrid()
//
//! Finds the maximum and minimum of a 2d matrix allocated with plAllc2dGrid().
//! NaN and +/- infinity values are ignored.
//!
//! param f 2d matrix pointer.
//! param nx Size of the grid in x.
//! param ny Size of the grid in y.
//! param fnmax Maximum value in the matrix.
//! param fnmin Minimum value in the matrix.
//!
//--------------------------------------------------------------------------

void
plMinMax2dGrid( PLFLT_MATRIX f, PLINT nx, PLINT ny, PLFLT *fnmax, PLFLT *fnmin )
{
    int   i, j;
    PLFLT m, M;

    if ( !isfinite( f[0][0] ) )
    {
        M = -HUGE_VAL;
        m = HUGE_VAL;
    }
    else
        M = m = f[0][0];

    for ( i = 0; i < nx; i++ )
    {
        for ( j = 0; j < ny; j++ )
        {
            if ( !isfinite( f[i][j] ) )
                continue;
            if ( f[i][j] > M )
                M = f[i][j];
            if ( f[i][j] < m )
                m = f[i][j];
        }
    }
    *fnmax = M;
    *fnmin = m;
}
