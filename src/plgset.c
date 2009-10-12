/* plget/plset()
 *
 * Copyright (C) 2009  Hazen Babcock
 *
 * This file is part of PLplot.
 *
 * PLplot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Library Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PLplot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with PLplot; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "plplotP.h"
#include "plstrm.h"

/*-------------------------------------------------------------------------
 * plget : Get the value of the specified variable in the current plot stream
 *-------------------------------------------------------------------------*/
void
c_plget( enum PLAttributeName attrName, PLAttribute *attrValue )
{
    attrValue->attributeType = -1;
    switch ( attrName )
    {
    case PL_CURCOLOR:
        attrValue->attributeType = PL_COLOR;
        attrValue->colorValue.r  = plsc->curcolor.r;
        attrValue->colorValue.g  = plsc->curcolor.g;
        attrValue->colorValue.b  = plsc->curcolor.b;
        attrValue->colorValue.a  = plsc->curcolor.a;
        break;
    case PL_ICOL0:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->icol0;
        break;
    case PL_ICOL1:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->icol1;
        break;
    case PL_NCOL0:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ncol0;
        break;
    case PL_NCOL1:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->ncol1;
        break;
    case PL_PENWIDTH:
        attrValue->attributeType = PL_INT;
        attrValue->intValue      = plsc->width;
        break;
    default:
        break;
    }
}

/*-------------------------------------------------------------------------
 * plset : Set the value of the specified variable in the current plot stream
 *
 * Note: This is a little tricker since we can't just set the value & expect
 * the driver to respond, instead we have to call the appropriate function
 * in PLplot core.
 *
 *-------------------------------------------------------------------------*/
void
c_plset( enum PLAttributeName attrName, PLAttribute attrValue )
{
    switch ( attrName )
    {
    case PL_CURCOLOR:
        if ( attrValue.attributeType == PL_COLOR )
        {
            plscol0( plsc->icol0,
                attrValue.colorValue.r,
                attrValue.colorValue.g,
                attrValue.colorValue.b );
            plcol0( plsc->icol0 );
        }
        break;
    case PL_PENWIDTH:
        if ( attrValue.attributeType == PL_INT )
        {
            plwid( attrValue.intValue );
        }
        break;
    default:
        break;
    }
}

