//      Contour plotter front-ends for Fortran.
//
// Copyright (C) 2004-2014 Alan W. Irwin
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//

#include "plstubs.h"

// Function prototypes
void pltr0f( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data );
void PLCON07( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
              PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel );
void PLCON17( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
              PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
              PLFLT *xg, PLFLT *yg );
void PLCON27( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
              PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
              PLFLT *xg, PLFLT *yg );
void PLVEC07( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale );
void PLVEC17( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
              PLFLT *xg, PLFLT *yg );
void PLVEC27( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
              PLFLT *xg, PLFLT *yg );
static void pltr( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data );
void PLCONT7( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
              PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel, PLFLT *ftr );
void PLVECT7( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
              PLFLT *ftr );
void PLSHADE07( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                PLFLT *shade_min, PLFLT *shade_max,
                PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
                PLINT *min_color, PLFLT *min_width,
                PLINT *max_color, PLFLT *max_width, PLINT *lx );
void PLSHADE17( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                PLFLT *shade_min, PLFLT *shade_max,
                PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
                PLINT *min_color, PLFLT *min_width,
                PLINT *max_color, PLFLT *max_width,
                PLFLT *xg1, PLFLT *yg1, PLINT *lx );
void PLSHADE27( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                PLFLT *shade_min, PLFLT *shade_max,
                PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
                PLINT *min_color, PLFLT *min_width,
                PLINT *max_color, PLFLT *max_width,
                PLFLT *xg2, PLFLT *yg2, PLINT *lx );
void PLSHADE7( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
               PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
               PLFLT *shade_min, PLFLT *shade_max,
               PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
               PLINT *min_color, PLFLT *min_width,
               PLINT *max_color, PLFLT *max_width, PLFLT *ftr, PLINT *lx );
void PLSHADES07( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                 PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                 PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
                 PLINT *cont_color, PLFLT *cont_width, PLINT *lx, PLINT *rect );
void PLSHADES17( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                 PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                 PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
                 PLINT *cont_color, PLFLT *cont_width,
                 PLFLT *xg1, PLFLT *yg1, PLINT *lx, PLINT *rect );
void PLSHADES27( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                 PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                 PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
                 PLINT *cont_color, PLFLT *cont_width,
                 PLFLT *xg2, PLFLT *yg2, PLINT *lx, PLINT *rect );
void PLSHADES7( PLFLT *z, PLINT *nx, PLINT *ny, const char *defined,
                PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
                PLINT *cont_color, PLFLT *cont_width, PLFLT *ftr, PLINT *lx, PLINT *rect );
void PLGRIDDATA( PLFLT *x, PLFLT *y, PLFLT *z, PLINT *npts, PLFLT *xg,
                 PLINT *nx, PLFLT *yg, PLINT *ny, PLFLT *zg, PLINT *type,
                 PLFLT *data );
void PLIMAGEFR07( PLFLT *idata, PLINT *nx, PLINT *ny,
                  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                  PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
                  PLINT *lx );
void PLIMAGEFR17( PLFLT *idata, PLINT *nx, PLINT *ny,
                  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                  PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
                  PLFLT *xg, PLFLT *yg, PLINT *lx );
void PLIMAGEFR27( PLFLT *idata, PLINT *nx, PLINT *ny,
                  PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                  PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
                  PLFLT *xg, PLFLT *yg, PLINT *lx );
void PLIMAGEFR7( PLFLT *idata, PLINT *nx, PLINT *ny,
                 PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
                 PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
                 PLFLT *ftr, PLINT *lx );




//--------------------------------------------------------------------------
// pltr0f()
//
// Identity transformation for plots from Fortran.
// Only difference from C-language identity function (pltr0) is that the
// Fortran paradigm for array index is used, i.e. starting at 1.
//--------------------------------------------------------------------------

void
pltr0f( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *PL_UNUSED( pltr_data ) )
{
    *tx = x + 1.0;
    *ty = y + 1.0;
}

//--------------------------------------------------------------------------
// Contour plotter front-ends.
// These specify the row-dominant function evaluator in the plfcont
// argument list.  NO TRANSPOSE IS NECESSARY.  The routines are as follows:
//
// - plcon0	no transformation
// - plcon1	linear interpolation from singly dimensioned coord arrays
// - plcon2	linear interpolation from doubly dimensioned coord arrays
//
// The latter two work by calling plfcont() with the appropriate grid
// structure for input to pltr2f().
//--------------------------------------------------------------------------

// no transformation

void
PLCON07( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
         PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel )
{
    PLfGrid fgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f  = z;

    plfcont( plf2evalr, (void *) &fgrid,
        *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
        pltr0f, NULL );
}

// 1-d transformation

void
PLCON17( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
         PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
         PLFLT *xg, PLFLT *yg )
{
    PLfGrid fgrid;
    PLcGrid cgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f  = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfcont( plf2evalr, (void *) &fgrid,
        *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
        pltr1, (void *) &cgrid );
}

// 2-d transformation

void
PLCON27( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
         PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel,
         PLFLT *xg, PLFLT *yg )
{
    PLfGrid fgrid;
    PLcGrid cgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f  = z;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfcont( plf2evalr, (void *) &fgrid,
        *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
        pltr2f, (void *) &cgrid );
}

//--------------------------------------------------------------------------
// Vector plotter front-ends.
// These specify the row-dominant function evaluator in the plfvect
// argument list.  NO TRANSPOSE IS NECESSARY.  The routines are as follows:
//
// - plvec0	no transformation
// - plvec1	linear interpolation from singly dimensioned coord arrays
// - plvec2	linear interpolation from doubly dimensioned coord arrays
//
// The latter two work by calling plfvect() with the appropriate grid
// structure for input to pltr2f().
//--------------------------------------------------------------------------

// no transformation

void
PLVEC07( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale )
{
    PLfGrid fgrid1, fgrid2;

    fgrid1.nx = *nx;
    fgrid1.ny = *ny;
    fgrid1.f  = u;

    fgrid2.nx = *nx;
    fgrid2.ny = *ny;
    fgrid2.f  = v;

    plfvect( plf2evalr, (void *) &fgrid1, (void *) &fgrid2,
        *nx, *ny, *scale, pltr0f, NULL );
}

// 1-d transformation

void
PLVEC17( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
         PLFLT *xg, PLFLT *yg )
{
    PLfGrid fgrid1;
    PLfGrid fgrid2;
    PLcGrid cgrid;

    fgrid1.nx = *nx;
    fgrid1.ny = *ny;
    fgrid1.f  = u;

    fgrid2.nx = *nx;
    fgrid2.ny = *ny;
    fgrid2.f  = v;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfvect( plf2evalr, (void *) &fgrid1, (void *) &fgrid2,
        *nx, *ny, *scale, pltr1, (void *) &cgrid );
}

// 2-d transformation

void
PLVEC27( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
         PLFLT *xg, PLFLT *yg )
{
    PLfGrid fgrid1;
    PLfGrid fgrid2;
    PLcGrid cgrid;

    fgrid1.nx = *nx;
    fgrid1.ny = *ny;
    fgrid1.f  = u;

    fgrid2.nx = *nx;
    fgrid2.ny = *ny;
    fgrid2.f  = v;

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg;
    cgrid.yg = yg;

    plfvect( plf2evalr, (void *) &fgrid1, (void *) &fgrid2,
        *nx, *ny, *scale, pltr2f, (void *) &cgrid );
}

//--------------------------------------------------------------------------
// Here are the old contour plotters.
//--------------------------------------------------------------------------

static void
pltr( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data )
{
    PLFLT *tr = (PLFLT *) pltr_data;

    *tx = tr[0] * x + tr[1] * y + tr[2];
    *ty = tr[3] * x + tr[4] * y + tr[5];
}

void
PLCONT7( PLFLT *z, PLINT *nx, PLINT *ny, PLINT *kx, PLINT *lx,
         PLINT *ky, PLINT *ly, PLFLT *clevel, PLINT *nlevel, PLFLT *ftr )
{
    PLfGrid fgrid;

    fgrid.nx = *nx;
    fgrid.ny = *ny;
    fgrid.f  = z;

    plfcont( plf2evalr, (void *) &fgrid,
        *nx, *ny, *kx, *lx, *ky, *ly, clevel, *nlevel,
        pltr, (void *) ftr );
}

void
PLVECT7( PLFLT *u, PLFLT *v, PLINT *nx, PLINT *ny, PLFLT *scale,
         PLFLT *ftr )
{
    PLfGrid fgrid1;
    PLfGrid fgrid2;

    fgrid1.nx = *nx;
    fgrid1.ny = *ny;
    fgrid1.f  = u;

    fgrid2.nx = *nx;
    fgrid2.ny = *ny;
    fgrid2.f  = v;

    plfvect( plf2evalr, (void *) &fgrid1, (void *) &fgrid2,
        *nx, *ny, *scale,
        pltr, (void *) ftr );
}

//--------------------------------------------------------------------------
// plfshade front-ends.
// These specify the row-dominant function evaluator in the plfshade
// argument list.  NO TRANSPOSE IS NECESSARY.  The routines are as follows:
//
// - plshade0	map indices to xmin, xmax, ymin, ymax.
// The next two work by calling plfshade() with the appropriate grid
// structure for input to pltr2f().
// - plshade1	linear interpolation from singly dimensioned coord arrays
// - plshade2	linear interpolation from doubly dimensioned coord arrays
// - plshade    tr array transformation
//
//--------------------------------------------------------------------------

void
PLSHADE07( PLFLT *z, PLINT *nx, PLINT *ny, const char *PL_UNUSED( defined ),
           PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
           PLFLT *shade_min, PLFLT *shade_max,
           PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
           PLINT *min_color, PLFLT *min_width,
           PLINT *max_color, PLFLT *max_width, PLINT *lx )
{
    PLINT rect = 1;
    PLFLT ** a;
    int   i, j;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    c_plshade( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        *shade_min, *shade_max,
        *sh_cmap, *sh_color, *sh_width,
        *min_color, *min_width, *max_color, *max_width,
        c_plfill, rect, NULL, NULL );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}


// 1-d transformation

void
PLSHADE17( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
           PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
           PLFLT *shade_min, PLFLT *shade_max,
           PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
           PLINT *min_color, PLFLT *min_width,
           PLINT *max_color, PLFLT *max_width,
           PLFLT *xg1, PLFLT *yg1, PLINT *lx )
{
    PLINT   rect = 1;
    PLFLT   ** a;
    int     i, j;
    PLcGrid cgrid;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg1;
    cgrid.yg = yg1;
    c_plshade( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        *shade_min, *shade_max,
        *sh_cmap, *sh_color, *sh_width,
        *min_color, *min_width, *max_color, *max_width,
        c_plfill, rect, pltr1, ( PLPointer ) & cgrid );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

// 2-d transformation

void
PLSHADE27( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
           PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
           PLFLT *shade_min, PLFLT *shade_max,
           PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
           PLINT *min_color, PLFLT *min_width,
           PLINT *max_color, PLFLT *max_width,
           PLFLT *xg2, PLFLT *yg2, PLINT *lx )
{
    PLINT    rect = 0;
    PLFLT    **a;
    int      i, j;
    PLcGrid2 cgrid2;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    plAlloc2dGrid( &cgrid2.xg, *nx, *ny );
    plAlloc2dGrid( &cgrid2.yg, *nx, *ny );
    cgrid2.nx = *nx;
    cgrid2.ny = *ny;
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j]         = z[i + j * *lx];
            cgrid2.xg[i][j] = xg2[i + j * *lx];
            cgrid2.yg[i][j] = yg2[i + j * *lx];
        }
    }

    c_plshade( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        *shade_min, *shade_max,
        *sh_cmap, *sh_color, *sh_width,
        *min_color, *min_width, *max_color, *max_width,
        c_plfill, rect, pltr2, (void *) &cgrid2 );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
    plFree2dGrid( cgrid2.xg, *nx, *ny );
    plFree2dGrid( cgrid2.yg, *nx, *ny );
}

void
PLSHADE7( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
          PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
          PLFLT *shade_min, PLFLT *shade_max,
          PLINT *sh_cmap, PLFLT *sh_color, PLFLT *sh_width,
          PLINT *min_color, PLFLT *min_width,
          PLINT *max_color, PLFLT *max_width, PLFLT *ftr, PLINT *lx )
{
    PLINT rect = 1;
    PLFLT ** a;
    int   i, j;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    c_plshade( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        *shade_min, *shade_max,
        *sh_cmap, *sh_color, *sh_width,
        *min_color, *min_width, *max_color, *max_width,
        c_plfill, rect, pltr, (void *) ftr );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

//--------------------------------------------------------------------------
// plshades front-ends.
//
// - plshades0	map indices to xmin, xmax, ymin, ymax
// - plshades1	linear interpolation from singly dimensioned coord arrays
// - plshades2	linear interpolation from doubly dimensioned coord arrays
// - plshades   pass tr information with plplot common block (and
//              then pass tr as last argument of PLSHADES7)
//--------------------------------------------------------------------------

void
PLSHADES07( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
            PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
            PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
            PLINT *cont_color, PLFLT *cont_width, PLINT *lx, PLINT *rect )
{
    PLFLT ** a;
    int   i, j;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    c_plshades( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        clevel, *nlevel, *fill_width,
        *cont_color, *cont_width,
        c_plfill, *rect, NULL, NULL );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

void
PLSHADES17( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
            PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
            PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
            PLINT *cont_color, PLFLT *cont_width,
            PLFLT *xg1, PLFLT *yg1, PLINT *lx, PLINT *rect )
{
    PLFLT   ** a;
    int     i, j;
    PLcGrid cgrid;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    cgrid.nx = *nx;
    cgrid.ny = *ny;
    cgrid.xg = xg1;
    cgrid.yg = yg1;

    c_plshades( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        clevel, *nlevel, *fill_width,
        *cont_color, *cont_width,
        c_plfill, *rect, pltr1, ( PLPointer ) & cgrid );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

void
PLSHADES27( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
            PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
            PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
            PLINT *cont_color, PLFLT *cont_width,
            PLFLT *xg2, PLFLT *yg2, PLINT *lx, PLINT *rect )
{
    PLFLT    **a;
    int      i, j;
    PLcGrid2 cgrid2;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    plAlloc2dGrid( &cgrid2.xg, *nx, *ny );
    plAlloc2dGrid( &cgrid2.yg, *nx, *ny );
    cgrid2.nx = *nx;
    cgrid2.ny = *ny;
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j]         = z[i + j * *lx];
            cgrid2.xg[i][j] = xg2[i + j * *lx];
            cgrid2.yg[i][j] = yg2[i + j * *lx];
        }
    }

    c_plshades( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        clevel, *nlevel, *fill_width,
        *cont_color, *cont_width,
        c_plfill, *rect, pltr2, (void *) &cgrid2 );

// Clean up allocated memory
    plFree2dGrid( a, *nx, *ny );
    plFree2dGrid( cgrid2.xg, *nx, *ny );
    plFree2dGrid( cgrid2.yg, *nx, *ny );
}

void
PLSHADES7( PLFLT *z, PLINT *nx, PLINT *ny, const char * PL_UNUSED( defined ),
           PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
           PLFLT *clevel, PLINT *nlevel, PLFLT *fill_width,
           PLINT *cont_color, PLFLT *cont_width, PLFLT *ftr, PLINT *lx, PLINT *rect )
{
    PLFLT ** a;
    int   i, j;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );
    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            a[i][j] = z[i + j * *lx];
        }
    }

    c_plshades( (const PLFLT * const *) a, *nx, *ny, NULL,
        *xmin, *xmax, *ymin, *ymax,
        clevel, *nlevel, *fill_width,
        *cont_color, *cont_width,
        c_plfill, *rect, pltr, (void *) ftr );

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

void
PLGRIDDATA( PLFLT *x, PLFLT *y, PLFLT *z, PLINT *npts, PLFLT *xg,
            PLINT *nx, PLFLT *yg, PLINT *ny, PLFLT *zg, PLINT *type, PLFLT *data )
{
    PLFLT ** a;
    int   i, j;

// Create a vectored a array from transpose of the fortran z array.
    plAlloc2dGrid( &a, *nx, *ny );

    c_plgriddata( x, y, z, *npts, xg, *nx, yg, *ny, a, *type, *data );

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            zg[i + j * *nx] = a[i][j];
        }
    }

// Clean up memory allocated for a
    plFree2dGrid( a, *nx, *ny );
}

void
PLIMAGEFR07( PLFLT *idata, PLINT *nx, PLINT *ny,
             PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
             PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
             PLINT *lx )
{
    int   i, j;
    PLFLT **pidata;

    plAlloc2dGrid( &pidata, *nx, *ny );

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *lx )];
        }
    }

    c_plimagefr( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *valuemin, *valuemax, pltr0, NULL );

    plFree2dGrid( pidata, *nx, *ny );
}

void
PLIMAGEFR17( PLFLT *idata, PLINT *nx, PLINT *ny,
             PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
             PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
             PLFLT *xg, PLFLT *yg, PLINT *lx )
{
    int     i, j;
    PLFLT   **pidata;
    PLcGrid cgrid;

    plAlloc2dGrid( &pidata, *nx, *ny );

    cgrid.nx = ( *nx ) + 1;
    cgrid.ny = ( *ny ) + 1;
    cgrid.xg = xg;
    cgrid.yg = yg;

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *lx )];
        }
    }

    c_plimagefr( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *valuemin, *valuemax, pltr1, (void *) &cgrid );

    plFree2dGrid( pidata, *nx, *ny );
}

void
PLIMAGEFR27( PLFLT *idata, PLINT *nx, PLINT *ny,
             PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
             PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
             PLFLT *xg, PLFLT *yg, PLINT *lx )
{
    int      i, j;
    PLFLT    **pidata;
    PLcGrid2 cgrid2;

    plAlloc2dGrid( &pidata, *nx, *ny );
    plAlloc2dGrid( &cgrid2.xg, ( *nx ) + 1, ( *ny ) + 1 );
    plAlloc2dGrid( &cgrid2.yg, ( *nx ) + 1, ( *ny ) + 1 );

    cgrid2.nx = ( *nx ) + 1;
    cgrid2.ny = ( *ny ) + 1;
    for ( i = 0; i <= *nx; i++ )
    {
        for ( j = 0; j <= *ny; j++ )
        {
            cgrid2.xg[i][j] = xg[i + j * ( ( *lx ) + 1 )];
            cgrid2.yg[i][j] = yg[i + j * ( ( *lx ) + 1 )];
        }
    }

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *lx )];
        }
    }

    c_plimagefr( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *valuemin, *valuemax, pltr2, (void *) &cgrid2 );

    plFree2dGrid( pidata, *nx, *ny );
    plFree2dGrid( cgrid2.xg, ( *nx ) + 1, ( *ny ) + 1 );
    plFree2dGrid( cgrid2.yg, ( *nx ) + 1, ( *ny ) + 1 );
}

void
PLIMAGEFR7( PLFLT *idata, PLINT *nx, PLINT *ny,
            PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
            PLFLT *zmin, PLFLT *zmax, PLFLT *valuemin, PLFLT *valuemax,
            PLFLT *ftr, PLINT *lx )
{
    int   i, j;
    PLFLT **pidata;

    plAlloc2dGrid( &pidata, *nx, *ny );

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *lx )];
        }
    }

    c_plimagefr( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *valuemin, *valuemax, pltr, (void *) ftr );

    plFree2dGrid( pidata, *nx, *ny );
}
