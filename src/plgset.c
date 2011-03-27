// plget/plset()
//
// Copyright (C) 2009  Hazen Babcock
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

#include "plplotP.h"
#include "plstrm.h"

//--------------------------------------------------------------------------
// plget : Get the value of the specified variable in the current plot stream
//--------------------------------------------------------------------------
void
c_plget( enum PLAttributeName attrName, PLAttribute *attrValue )
{
    PLINT i;

    attrValue->attributeType = -1;
    switch ( attrName )
    {
    // get the entire color map 0
    case PL_CMAP0:
        attrValue->attributeType = PL_COLORPTR;
        attrValue->colorValues   = (PLColor *) malloc( sizeof ( PLColor ) * plsc->ncol0 );
        for ( i = 0; i < plsc->ncol0; i++ )
        {
            attrValue->colorValues[i].r = plsc->cmap0[i].r;
            attrValue->colorValues[i].g = plsc->cmap0[i].g;
            attrValue->colorValues[i].b = plsc->cmap0[i].b;
            attrValue->colorValues[i].a = plsc->cmap0[i].a;
        }
        attrValue->nValues = plsc->ncol0;
        break;
    // get the entire color map 1
    case PL_CMAP1:
        attrValue->attributeType = PL_COLORPTR;
        attrValue->colorValues   = (PLColor *) malloc( sizeof ( PLColor ) * plsc->ncol1 );
        for ( i = 0; i < plsc->ncol1; i++ )
        {
            attrValue->colorValues[i].r = plsc->cmap1[i].r;
            attrValue->colorValues[i].g = plsc->cmap1[i].g;
            attrValue->colorValues[i].b = plsc->cmap1[i].b;
            attrValue->colorValues[i].a = plsc->cmap1[i].a;
        }
        attrValue->nValues = plsc->ncol1;
        break;
    // get the current (scaled) character height
    case PL_CURCHARSIZE:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->chrht;
        break;
    // get the current color map 0 color
    case PL_CURCOLOR0:
        attrValue->attributeType = PL_COLOR;
        attrValue->colorValue.r  = plsc->curcolor.r;
        attrValue->colorValue.g  = plsc->curcolor.g;
        attrValue->colorValue.b  = plsc->curcolor.b;
        attrValue->colorValue.a  = plsc->curcolor.a;
        break;
    // get the current (scaled) major tick size
    case PL_CURMAJORTICK:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->majht;
        break;
    // get the current (scaled) minor tick size
    case PL_CURMINORTICK:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->minht;
        break;
    // get the default character height (in mm)
    case PL_DEFCHARSIZE:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->chrdef;
        break;
    // get the default major tick size (in mm)
    case PL_DEFMAJORTICK:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->majdef;
        break;
    // get the default minor tick size (in mm)
    case PL_DEFMINORTICK:
        attrValue->attributeType = PL_FLT;
        attrValue->fltValue      = plsc->mindef;
        break;
    // get the index of the current color map 0 color
    case PL_ICOL0:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->icol0;
        break;
    // get the index of the current color map 1 color
    case PL_ICOL1:
        attrValue->attributeType = PL_FLT;
        attrValue->intValue      = ( (PLFLT) plsc->icol1 ) / ( (PLFLT) plsc->ncol1 );
        break;
    // get the number of colors in color map 0
    case PL_NCOL0:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ncol0;
        break;
    // get the number of colors in color map 1
    case PL_NCOL1:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ncol1;
        break;
    // get the current pen width
    case PL_PENWIDTH:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->width;
        break;
    // get the current number of digits of precision
    case PL_PRECISION:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->precis;
        break;
    // get whether or not to use user specified number of digits of precision
    case PL_SETPRECISION:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->setpre;
        break;
    // get x fields digit value (?)
    case PL_XDIGITS:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->xdigits;
        break;
    // get x maximum digits (0 = no maximum)
    case PL_XDIGMAX:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->xdigmax;
        break;
    // get y fields digit value (?)
    case PL_YDIGITS:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ydigits;
        break;
    // get y maximum digits (0 = no maximum)
    case PL_YDIGMAX:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ydigmax;
        break;
    // get z fields digit value (?)
    case PL_ZDIGITS:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->zdigits;
        break;
    // get z maximum digits (0 = no maximum)
    case PL_ZDIGMAX:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->zdigmax;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------
// plset : Set the value of the specified variable in the current plot stream
//
// Note: This is a little tricker since we can't just set the value & expect
// the driver to respond, instead we have to call the appropriate function
// in PLplot core.
//
//--------------------------------------------------------------------------
void
c_plset( enum PLAttributeName attrName, PLAttribute attrValue )
{
    PLINT i;
    PLINT *r, *g, *b;
    PLFLT *a;

    switch ( attrName )
    {
    // set color map 0 from an array of PL_COLOR values
    case PL_CMAP0:
        if ( attrValue.attributeType == PL_COLORPTR )
        {
            r = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            g = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            b = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            a = (PLFLT *) malloc( sizeof ( PLFLT ) * attrValue.nValues );
            for ( i = 0; i < attrValue.nValues; i++ )
            {
                r[i] = attrValue.colorValues[i].r;
                g[i] = attrValue.colorValues[i].g;
                b[i] = attrValue.colorValues[i].b;
                a[i] = attrValue.colorValues[i].a;
            }
            plscmap0a( r, g, b, a, attrValue.nValues );
            free( r );
            free( g );
            free( b );
            free( a );
        }
        break;
    // set color map 1 from an array of PL_COLOR values
    case PL_CMAP1:
        if ( attrValue.attributeType == PL_COLORPTR )
        {
            r = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            g = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            b = (PLINT *) malloc( sizeof ( PLINT ) * attrValue.nValues );
            a = (PLFLT *) malloc( sizeof ( PLFLT ) * attrValue.nValues );
            for ( i = 0; i < attrValue.nValues; i++ )
            {
                r[i] = attrValue.colorValues[i].r;
                g[i] = attrValue.colorValues[i].g;
                b[i] = attrValue.colorValues[i].b;
                a[i] = attrValue.colorValues[i].a;
            }
            plscmap1a( r, g, b, a, attrValue.nValues );
            free( r );
            free( g );
            free( b );
            free( a );
        }
        break;
    // set the (scaled) character height
    case PL_CURCHARSIZE:
        if ( attrValue.attributeType == PL_FLT )
        {
            plschr( 0.0, attrValue.fltValue );
        }
        break;
    // set the current color map 0 color
    case PL_CURCOLOR0:
        if ( attrValue.attributeType == PL_COLOR )
        {
            plscol0( plsc->icol0,
                attrValue.colorValue.r,
                attrValue.colorValue.g,
                attrValue.colorValue.b );
            plcol0( plsc->icol0 );
        }
        break;
    // set the (scaled) major tick length
    case PL_CURMAJORTICK:
        if ( attrValue.attributeType == PL_FLT )
        {
            plsmaj( 0.0, attrValue.fltValue );
        }
        break;
    // set the (scaled) minor tick length
    case PL_CURMINORTICK:
        if ( attrValue.attributeType == PL_FLT )
        {
            plsmin( 0.0, attrValue.fltValue );
        }
        break;
    // set the default character height (mm)
    case PL_DEFCHARSIZE:
        if ( attrValue.attributeType == PL_FLT )
        {
            plschr( attrValue.fltValue, plsc->chrht );
        }
        break;
    // set the default major tick size (mm)
    case PL_DEFMAJORTICK:
        if ( attrValue.attributeType == PL_FLT )
        {
            plsmaj( attrValue.fltValue, plsc->majht );
        }
        break;
    // set the default minor tick size (mm)
    case PL_DEFMINORTICK:
        if ( attrValue.attributeType == PL_FLT )
        {
            plsmin( attrValue.fltValue, plsc->minht );
        }
        break;
    // set the index of the current color map 0 color
    case PL_ICOL0:
        if ( attrValue.attributeType == PL_INT )
        {
            plcol0( attrValue.intValue );
        }
        break;
    // set the index of the current color map 1 color
    case PL_ICOL1:
        if ( attrValue.attributeType == PL_FLT )
        {
            plcol1( attrValue.fltValue );
        }
        break;
    // set the number of colors in color map 0
    case PL_NCOL0:
        if ( attrValue.attributeType == PL_INT )
        {
            plscmap0n( attrValue.intValue );
        }
        break;
    // set the number of colors in color map 1
    case PL_NCOL1:
        if ( attrValue.attributeType == PL_INT )
        {
            plscmap1n( attrValue.intValue );
        }
        break;
    // set the current pen width
    case PL_PENWIDTH:
        if ( attrValue.attributeType == PL_INT )
        {
            plwid( attrValue.intValue );
        }
        break;
    // set the current number of digits of precision
    case PL_PRECISION:
        if ( attrValue.attributeType == PL_INT )
        {
            plprec( plsc->setpre, attrValue.intValue );
        }
        break;
    // set whether or not to use user specified number of digits of precision
    case PL_SETPRECISION:
        if ( attrValue.attributeType == PL_INT )
        {
            plprec( attrValue.intValue, plsc->precis );
        }
        break;
    // set x fields digit value (?)
    case PL_XDIGITS:
        if ( attrValue.attributeType == PL_INT )
        {
            plsxax( plsc->xdigmax, attrValue.intValue );
        }
        break;
    // get x maximum digits (0 = no maximum)
    case PL_XDIGMAX:
        if ( attrValue.attributeType == PL_INT )
        {
            plsxax( attrValue.intValue, plsc->xdigits );
        }
        break;
    // set y fields digit value (?)
    case PL_YDIGITS:
        if ( attrValue.attributeType == PL_INT )
        {
            plsyax( plsc->ydigmax, attrValue.intValue );
        }
        break;
    // set y maximum digits (0 = no maximum)
    case PL_YDIGMAX:
        if ( attrValue.attributeType == PL_INT )
        {
            plsyax( attrValue.intValue, plsc->ydigits );
        }
        break;
    // set z fields digit value (?)
    case PL_ZDIGITS:
        if ( attrValue.attributeType == PL_INT )
        {
            plszax( plsc->zdigmax, attrValue.intValue );
        }
        break;
    // set z maximum digits (0 = no maximum)
    case PL_ZDIGMAX:
        if ( attrValue.attributeType == PL_INT )
        {
            plszax( attrValue.intValue, plsc->zdigits );
        }
        break;
    default:
        break;
    }
}

