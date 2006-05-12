!      $Id$
!
!      Copyright (C) 2004  Alan W. Irwin
!
!      This file is part of PLplot.
!
!      PLplot is free software; you can redistribute it and/or modify
!      it under the terms of the GNU General Library Public License as
!      published by the Free Software Foundation; either version 2 of the
!      License, or (at your option) any later version.
!
!      PLplot is distributed in the hope that it will be useful,
!      but WITHOUT ANY WARRANTY; without even the implied warranty of
!      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!      GNU Library General Public License for more details.
!
!      You should have received a copy of the GNU Library General Public
!      License along with PLplot; if not, write to the Free Software
!      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


!--------------------------------------------------------------------------
! ident
!
! Defines identity transformation for example 19.
! x(), y() are the coordinates to be plotted.
! This is a 0-OP routine, to play the role of NULL in the C version!
!--------------------------------------------------------------------------

      subroutine ident(n, x, y)
      use plplot
      implicit none

      integer n
      real(kind=plflt)    x
      real(kind=plflt)    y

      return
      end

!--------------------------------------------------------------------------
! mapform19
!
! Defines specific coordinate transformation for example 19.
! Not to be confused with mapform in src/plmap.c.
! x(), y() are the coordinates to be plotted.
!--------------------------------------------------------------------------

      subroutine mapform19(n, x, y)
      use plplot, PI => PL_PI
      implicit none

      integer n
      real(kind=plflt)    x(n)
      real(kind=plflt)    y(n)

      integer i
      real(kind=plflt)    xp, yp, radius

      do i = 1,n
         radius = 90.0_plflt - y(i)
         xp = radius * cos(x(i) * PI / 180.0_plflt)
         yp = radius * sin(x(i) * PI / 180.0_plflt)
         x(i) = xp
         y(i) = yp
      enddo
      return
      end

!--------------------------------------------------------------------------
! main
!
! Shows two views of the world map.
!--------------------------------------------------------------------------

      program x19f
      use plplot
      implicit none
      real(kind=plflt)    minx, maxx, miny, maxy
      integer c
      external ident
      external mapform19

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

! Longitude (x) and latitude (y)

      miny = -70
      maxy = 80

      call plinit()

! Cartesian plots
! Most of world

      minx = 190
      maxx = 190+360

      call plcol0(1)
      call plenv(minx, maxx, miny, maxy, 1, -1)
      call plmap(ident, 'usaglobe', minx, maxx, miny, maxy)

! The Americas

      minx = 190
      maxx = 340

      call plcol0(1)
      call plenv(minx, maxx, miny, maxy, 1, -1)
      call plmap(ident, 'usaglobe', minx, maxx, miny, maxy)

! Polar, Northern hemisphere

      minx = 0
      maxx = 360

      call plenv(-75._plflt, 75._plflt, -75._plflt, &
              75._plflt, 1, -1)
      call plmap(mapform19,'globe', minx, maxx, miny, maxy)

      call pllsty(2)
      call plmeridians(mapform19,10.0_plflt, 10.0_plflt, &
              0.0_plflt, 360.0_plflt, -10.0_plflt, &
              80.0_plflt)
      call plend()
      end
