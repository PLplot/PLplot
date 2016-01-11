!    Generates polar plot with, 1-1 scaling
!
!    Copyright (C) 2004  Alan W. Irwin
!
!    This file is part of PLplot.
!
!    PLplot is free software; you can redistribute it and/or modify
!    it under the terms of the GNU Library General Public License as
!    published by the Free Software Foundation; either version 2 of the
!    License, or (at your option) any later version.
!
!    PLplot is distributed in the hope that it will be useful,
!    but WITHOUT ANY WARRANTY; without even the implied warranty of
!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!    GNU Library General Public License for more details.
!
!    You should have received a copy of the GNU Library General Public
!    License along with PLplot; if not, write to the Free Software
!    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

program x03f95
    use plplot, PI => PL_PI
    use plf95demolib
    implicit none

    character (len=3) :: text
    real(kind=plflt), dimension(0:360) :: x0, y0, x, y
    real(kind=plflt) :: dtr, theta, dx, dy, offset
    integer :: i
!    Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

!   Set orientation to portrait - note not all device drivers
!   support this, in particular most interactive drivers do not.
    call plsori(1)

    dtr = PI/180.0_plflt
    x0 = cos(dtr * arange(0,361))
    y0 = sin(dtr * arange(0,361))

!    Initialize PLplot

    call plinit()

!    Set up viewport and window, but do not draw box

    call plenv(-1.3_plflt, 1.3_plflt, -1.3_plflt, 1.3_plflt, 1, -2)
!   Draw circles for polar grid
    do i = 1,10
      call plarc(0.0_plflt, 0.0_plflt, 0.1_plflt*i, 0.1_plflt*i, &
           0.0_plflt, 360.0_plflt, 0.0_plflt, 0)
    enddo
    call plcol0(2)
    do i = 0,11
      theta = 30.0_plflt*i
      dx = cos(dtr*theta)
      dy = sin(dtr*theta)

!      Draw radial spokes for polar grid

      call pljoin(0.0_plflt, 0.0_plflt, dx, dy)
      write (text,'(i3)') nint(theta)

!      Write labels for angle

      text = adjustl(text)

      if (theta .lt. 9.99) then
         offset = 0.45
      elseif (theta .lt. 99.9) then
         offset = 0.30
      else
         offset = 0.15
      endif
!      Slightly off zero to avoid floating point logic flips at
!      90 and 270 deg.
      if (dx >= -0.00001_plflt) then
        call plptex(dx, dy, dx, dy, -offset, text)
      else
        call plptex(dx, dy, -dx, -dy, 1._plflt+offset, text)
      end if
    enddo
!    Draw the graph

    x = x0 * sin(5.0_plflt * dtr * arange(0,361))
    y = y0 * sin(5.0_plflt * dtr * arange(0,361))

    call plcol0(3)
    call plline(x,y)

    call plcol0(4)
    call plmtex('t', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
      '#frPLplot Example 3 - r(#gh)=sin 5#gh')

!    Close the plot at end

    call plend
end program x03f95
