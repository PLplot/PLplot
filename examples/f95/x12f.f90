!      $Id$
!      Demonstration program for PLplot: Bar chart example.
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

      use plplot
      implicit none

      character*20 string
      integer i
      real(kind=plflt) y0(10)
      real(kind=plflt) pos(5), red(5), green(5), blue(5)

      data pos   /0.0_plflt, 0.25_plflt, 0.5_plflt, 0.75_plflt, 1.0_plflt/
      data red   /0.0_plflt, 0.25_plflt, 0.5_plflt, 1.0_plflt, 1.0_plflt/
      data green /1.0_plflt, 0.5_plflt, 0.5_plflt, 0.5_plflt, 1.0_plflt/
      data blue  /1.0_plflt, 1.0_plflt, 0.5_plflt, 0.25_plflt, 0.0_plflt/

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

      call plinit()

      call pladv(0)
      call plvsta
      call plwind( 1980._plflt, 1990._plflt, 0._plflt, 35._plflt )
      call plbox( 'bc', 1._plflt, 0, 'bcnv', 10._plflt, 0 )
      call plcol0(2)
      call pllab( 'Year', 'Widget Sales (millions)', &
                  '#frPLplot Example 12' )

      y0(1) = 5
      y0(2) = 15
      y0(3) = 12
      y0(4) = 24
      y0(5) = 28
      y0(6) = 30
      y0(7) = 20
      y0(8) = 8
      y0(9) = 12
      y0(10) = 3
      
      call plscmap1l(.true.,pos,red,green,blue)

      do i = 0, 9
!        call plcol0(i + 1)
        call plcol1(dble(i)/9.0_plflt)
        call plpsty(0)
        call plfbox( 1980._plflt+i, y0(i+1) )
        write (string, '(i8)')int(y0(i+1))
        call lalign(string)
        call plptex( 1980._plflt+i+0.5_plflt, y0(i+1)+1._plflt, &
          1._plflt, 0._plflt, 0.5_plflt, string )
        write (string, '(i8)')1980+i
        call lalign(string)
        call plmtex( 'b', 1._plflt, (i+1)*0.1_plflt-0.05_plflt, &
          0.5_plflt, string )
      enddo

!      Don't forget to call PLEND to finish off!
      call plend
      end

!      Suck up leading spaces

      subroutine lalign(string)
      use plplot
      implicit none
      integer i, j, k, max, len
      character*(*) string

      max = len(string)

      do i = 1, max
        if (string(i:i) .ne. ' ') then
          if (i .eq. 1) return
          k = 1
          do j = i, max
            string(k:k) = string(j:j)
            k = k + 1
          enddo
          do j = k, max
            string(j:j) = ' '
          enddo
          return
        endif
      enddo
      end

      subroutine plfbox(x0, y0)
      use plplot
      implicit none

      real(kind=plflt) x0, y0, x(4), y(4)

      x(1) = x0
      y(1) = 0._plflt
      x(2) = x0
      y(2) = y0
      x(3) = x0+1._plflt
      y(3) = y0
      x(4) = x0+1._plflt
      y(4) = 0._plflt
      call plfill(x, y)
      call plcol0(1)
      call pllsty(1)
      call plline(x, y)
      end
