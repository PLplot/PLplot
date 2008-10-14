!      $Id$
!      Generates polar plot with, 1-1 scaling
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

      use plplot, PI => PL_PI
      implicit none

      character*3 text
      real(kind=plflt) x0(0:360), y0(0:360)
      real(kind=plflt) x(0:360), y(0:360), dtr, theta, dx, dy, r, offset
      integer i, j, nsp
!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)


      dtr = PI/180.0_plflt
      do i=0,360
        x0(i) = cos(dtr * dble (i))
        y0(i) = sin(dtr * dble (i))
      enddo

!      Initialize PLplot

      call plinit()

!      Set up viewport and window, but do not draw box

      call plenv(-1.3_plflt, 1.3_plflt, -1.3_plflt, 1.3_plflt, 1, -2)
      do i = 1,10
        do j = 0,360
          x(j) = 0.1_plflt*i*x0(j)
          y(j) = 0.1_plflt*i*y0(j)
        enddo
!        Draw circles for polar grid

        call plline(x,y)
      enddo
      call plcol0(2)
      do i = 0,11
        theta = 30.0_plflt*i
        dx = cos(dtr*theta)
        dy = sin(dtr*theta)

!        Draw radial spokes for polar grid

        call pljoin(0.0_plflt, 0.0_plflt, dx, dy)
        write (text,'(i3)') nint(theta)

!        Write labels for angle

        text = text(nsp(text):)

        if (theta .lt. 9.99) then
           offset = 0.45
        elseif (theta .lt. 99.9) then
           offset = 0.30
        else
           offset = 0.15
        endif
!        Slightly off zero to avoid floating point logic flips at
!        90 and 270 deg.
        if (dx.ge.-0.00001_plflt) then
          call plptex(dx, dy, dx, dy, -offset, text)
        else
          call plptex(dx, dy, -dx, -dy, 1._plflt+offset, text)
        end if
      enddo
!      Draw the graph

      do i=0,360
        r = sin(dtr*dble (5*i))
        x(i) = x0(i) * r
        y(i) = y0(i) * r
      enddo
      call plcol0(3)
      call plline(x,y)

      call plcol0(4)
      call plmtex('t', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
        '#frPLplot Example 3 - r(#gh)=sin 5#gh')

!      Close the plot at end

      call plend
      end

      integer function nsp(text)
!      ==================

!      Find first non-space character
      use plplot
      implicit none

      character*(*) text
      integer l, len

      l = len(text)
      nsp = 1
      do while(text(nsp:nsp).eq.' ' .and. nsp.lt.l)
        nsp = nsp+1
      enddo
      end
