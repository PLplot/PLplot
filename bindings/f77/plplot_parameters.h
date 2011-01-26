C      $Id$
C      parameters required for routines that use the plplotf77 library
C      such as some of our f77 examples.
C
C      Copyright (C) 2011  Alan W. Irwin
C
C      This file is part of PLplot.
C
C      PLplot is free software; you can redistribute it and/or modify
C      it under the terms of the GNU General Library Public License as
C      published by the Free Software Foundation; either version 2 of the
C      License, or (at your option) any later version.
C
C      PLplot is distributed in the hope that it will be useful,
C      but WITHOUT ANY WARRANTY; without even the implied warranty of
C      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C      GNU Library General Public License for more details.
C
C      You should have received a copy of the GNU Library General Public
C      License along with PLplot; if not, write to the Free Software
C      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

      integer PL_PARSE_FULL
      parameter(PL_PARSE_FULL = 1)
      real*8 PI, TWOPI
      parameter (PI = 3.1415926535897932384d0)
      parameter (TWOPI = 2.d0*PI)

c flags used for position argument of both pllegend and plcolorbar
      integer PL_POSITION_LEFT
      integer PL_POSITION_RIGHT
      integer PL_POSITION_UPPER
      integer PL_POSITION_LOWER
      integer PL_POSITION_INSIDE
      integer PL_POSITION_OUTSIDE
      integer PL_POSITION_VIEWPORT
      integer PL_POSITION_SUBPAGE

      parameter (PL_POSITION_LEFT           = 1 )
      parameter (PL_POSITION_RIGHT          = 2 )
      parameter (PL_POSITION_UPPER          = 4 )
      parameter (PL_POSITION_LOWER          = 8 )
      parameter (PL_POSITION_INSIDE         = 16 )
      parameter (PL_POSITION_OUTSIDE        = 32 )
      parameter (PL_POSITION_VIEWPORT       = 64 )
      parameter (PL_POSITION_SUBPAGE        = 128 )

c Flags for plot3dc() and plsurf3d().
      integer DRAW_LINEX, DRAW_LINEY, DRAW_LINEXY, MAG_COLOR,
     &  BASE_CONT, TOP_CONT, SURF_CONT, DRAW_SIDES, FACETED, MESH
      parameter(DRAW_LINEX = 1)
      parameter(DRAW_LINEY = 2)
      parameter(DRAW_LINEXY = 3)
      parameter(MAG_COLOR = 4)
      parameter(BASE_CONT = 8)
      parameter(TOP_CONT = 16)
      parameter(SURF_CONT = 32)
      parameter(DRAW_SIDES = 64)
      parameter(FACETED = 128)
      parameter(MESH = 256)

c Flags for pllegend.
      integer PL_LEGEND_NONE
      integer PL_LEGEND_COLOR_BOX
      integer PL_LEGEND_LINE
      integer PL_LEGEND_SYMBOL
      integer PL_LEGEND_TEXT_LEFT
      integer PL_LEGEND_BACKGROUND
      integer PL_LEGEND_BOUNDING_BOX
      integer PL_LEGEND_ROW_MAJOR

      parameter (PL_LEGEND_NONE           = 1     )
      parameter (PL_LEGEND_COLOR_BOX      = 2     )
      parameter (PL_LEGEND_LINE           = 4     )
      parameter (PL_LEGEND_SYMBOL         = 8     )
      parameter (PL_LEGEND_TEXT_LEFT      = 16    )
      parameter (PL_LEGEND_BACKGROUND     = 32    )
      parameter (PL_LEGEND_BOUNDING_BOX   = 64    )
      parameter (PL_LEGEND_ROW_MAJOR      = 128   )

c Flags for plcolorbar
      integer PL_COLORBAR_LEFT
      integer PL_COLORBAR_RIGHT
      integer PL_COLORBAR_UPPER
      integer PL_COLORBAR_LOWER
      integer PL_COLORBAR_LABEL_LEFT
      integer PL_COLORBAR_LABEL_RIGHT
      integer PL_COLORBAR_LABEL_UPPER
      integer PL_COLORBAR_LABEL_LOWER
      integer PL_COLORBAR_IMAGE
      integer PL_COLORBAR_SHADE
      integer PL_COLORBAR_GRADIENT
      integer PL_COLORBAR_CAP_LOW
      integer PL_COLORBAR_CAP_HIGH
      integer PL_COLORBAR_SHADE_LABEL

      parameter (PL_COLORBAR_LEFT         = 1     )
      parameter (PL_COLORBAR_RIGHT        = 2     )
      parameter (PL_COLORBAR_UPPER        = 4     )
      parameter (PL_COLORBAR_LOWER        = 8     )
      parameter (PL_COLORBAR_LABEL_LEFT   = 16    )
      parameter (PL_COLORBAR_LABEL_RIGHT  = 32    )
      parameter (PL_COLORBAR_LABEL_UPPER  = 64    )
      parameter (PL_COLORBAR_LABEL_LOWER  = 128   )
      parameter (PL_COLORBAR_IMAGE        = 256   )
      parameter (PL_COLORBAR_SHADE        = 512   )
      parameter (PL_COLORBAR_GRADIENT     = 1024  )
      parameter (PL_COLORBAR_CAP_LOW      = 2048  )
      parameter (PL_COLORBAR_CAP_HIGH     = 4096  )
      parameter (PL_COLORBAR_SHADE_LABEL  = 8192  )
