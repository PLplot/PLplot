C      $Id$
C      common blocks required for routines in sfstubs.f
C
C      Copyright (C) 2004  Alan W. Irwin
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

      integer maxlen
      parameter (maxlen = 320)
      character*(maxlen) string1, string2, string3
      character*(maxlen) string4, string5, string6
      character*(maxlen) string7, string8, string9
      integer s1(80), s2(80), s3(80),
     &  s4(80), s5(80), s6(80), s7(80), s8(80), s9(80)
      equivalence ( s1, string1 ), ( s2, string2 )
      equivalence ( s3, string3 ), ( s4, string4 )
      equivalence ( s5, string5 ), ( s6, string6 )
      equivalence ( s7, string7 ), ( s8, string8 )
      equivalence ( s9, string9 )
      common /zzplstr1/ string1
      common /zzplstr2/ string2
      common /zzplstr3/ string3
      common /zzplstr4/ string4
      common /zzplstr5/ string5
      common /zzplstr6/ string6
      common /zzplstr7/ string7
      common /zzplstr8/ string8
      common /zzplstr9/ string9


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
