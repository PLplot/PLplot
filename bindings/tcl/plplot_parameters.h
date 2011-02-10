// $Id$
//
//  Copyright 2011
//  Arjen Markus			arjenmarkus@users.sourceforge.net
//
//  This file is part of PLplot.
//
//  PLplot is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Library Public License as published
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
//  This file contains a function to set the various flags used by PLplot
//

static void set_plplot_parameters( Tcl_Interp *interp ) {

    Tcl_Eval( interp, "namespace eval ::PLPLOT { \n\
# flags used for position argument of both pllegend and plcolorbar \n\
variable PL_POSITION_LEFT            1             \n\
variable PL_POSITION_RIGHT           2             \n\
variable PL_POSITION_TOP             4             \n\
variable PL_POSITION_BOTTOM          8             \n\
variable PL_POSITION_INSIDE          16            \n\
variable PL_POSITION_OUTSIDE         32            \n\
variable PL_POSITION_VIEWPORT        64            \n\
variable PL_POSITION_SUBPAGE         128           \n\
                                                   \n\
# Flags for pllegend.                              \n\
variable PL_LEGEND_NONE              1             \n\
variable PL_LEGEND_COLOR_BOX         2             \n\
variable PL_LEGEND_LINE              4             \n\
variable PL_LEGEND_SYMBOL            8             \n\
variable PL_LEGEND_TEXT_LEFT         16            \n\
variable PL_LEGEND_BACKGROUND        32            \n\
variable PL_LEGEND_BOUNDING_BOX      64            \n\
variable PL_LEGEND_ROW_MAJOR         128           \n\
                                                   \n\
# Flags for plcolorbar                             \n\
variable PL_COLORBAR_LABEL_LEFT      1             \n\
variable PL_COLORBAR_LABEL_RIGHT     2             \n\
variable PL_COLORBAR_LABEL_TOP       4             \n\
variable PL_COLORBAR_LABEL_BOTTOM    8             \n\
variable PL_COLORBAR_IMAGE           16            \n\
variable PL_COLORBAR_SHADE           32            \n\
variable PL_COLORBAR_GRADIENT        64            \n\
variable PL_COLORBAR_CAP_LOW         128           \n\
variable PL_COLORBAR_CAP_HIGH        256           \n\
variable PL_COLORBAR_SHADE_LABEL     512           \n\
                                                   \n\
# FCI (font characterization integer) related constants. \n\
variable PL_FCI_MARK                   0x80000000  \n\
variable PL_FCI_IMPOSSIBLE             0x00000000  \n\
variable PL_FCI_HEXDIGIT_MASK          0xf         \n\
variable PL_FCI_HEXPOWER_MASK          0x7         \n\
variable PL_FCI_HEXPOWER_IMPOSSIBLE    0xf         \n\
# These define hexpower values corresponding to each font attribute. \n\
variable PL_FCI_FAMILY                 0x0         \n\
variable PL_FCI_STYLE                  0x1         \n\
variable PL_FCI_WEIGHT                 0x2         \n\
# These are legal values for font family attribute \n\
variable PL_FCI_SANS                   0x0         \n\
variable PL_FCI_SERIF                  0x1         \n\
variable PL_FCI_MONO                   0x2         \n\
variable PL_FCI_SCRIPT                 0x3         \n\
variable PL_FCI_SYMBOL                 0x4         \n\
# These are legal values for font style attribute  \n\
variable PL_FCI_UPRIGHT                0x0         \n\
variable PL_FCI_ITALIC                 0x1         \n\
variable PL_FCI_OBLIQUE                0x2         \n\
# These are legal values for font weight attribute \n\
variable PL_FCI_MEDIUM                 0x0         \n\
variable PL_FCI_BOLD                   0x1         \n\
}" );

}
