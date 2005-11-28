!      $Id$
!      Simple line plot and multiple windows demo.
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

program x01f95
   use plplot, PI => PL_PI

   implicit none

   real(plflt) :: xscale, yscale, xoff, yoff
   character(len=80) version
   integer digmax

!  Process command-line arguments
   call plparseopts(PL_PARSE_FULL)

!  Divide page into 2x2 plots
   call plssub(2, 2)

!  Print plplot version
   call plgver(version)
   write (0,'(a,a)') 'PLplot library version: ', &
     &  trim(version)

!  Initialize plplot
   call plinit()

!  Set up the data
!  Original case

   xscale = 6._plflt
   yscale = 1._plflt
   xoff = 0._plflt
   yoff = 0._plflt

!  Do a plot

   call plot1()

!  Set up the data

   xscale = 1._plflt
   yscale = 0.0014_plflt
   yoff = 0.0185_plflt

!      Do a plot

   digmax = 5
   call plsyax(digmax,  0)
   call plot1()

   call plot2()
   call plot3()
!  Don't forget to call PLEND to finish off!

   call plend()

contains

!======================================================================
subroutine plot1()

   real(plflt), dimension(1:60) :: x, y
   real(plflt), dimension(1:6)  :: xs, ys
   integer :: i

   do i = 1, 60
     x(i) = xoff + xscale * dble(i)/60.0_plflt
     y(i) = yoff + yscale * x(i)**2
   enddo

   xmin = x(1)
   xmax = x(60)
   ymin = y(1)
   ymax = y(60)

   do i = 1, 6
     xs(i) = x((i-1)*10+4)
     ys(i) = y((i-1)*10+4)
   enddo

!      Set up the viewport and window using PLENV. The range in X is
!      0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
!      scaled separately (just = 0), and we just draw a labelled
!      box (axis = 0).

   call plcol0(1)
   call plenv( xmin, xmax, ymin, ymax, 0, 0 )
   call plcol0(2)
   call pllab( '(x)', '(y)', '#frPLplot Example 1 - y=x#u2' )

!   Plot the data points

   call plcol0(4)
   call plpoin( xs, ys, 9 )

!   Draw the line through the data

   call plcol0(3)
   call plline( x, y )

end subroutine plot1

!======================================================================
subroutine plot2()

   real(plflt), dimension(1:100) :: x, y
   integer :: i

!
!   Set up the viewport and window using PLENV. The range in X is
!   -2.0 to 10.0, and the range in Y is -0.4 to 2.0. The axes are
!   scaled separately (just = 0), and we draw a box with axes
!   (axis = 1).

   call plcol0(1)
   call plenv(-2.0_plflt, 10.0_plflt, -0.4_plflt, 1.2_plflt, 0, 1 )
   call plcol0(2)
   call pllab( '(x)', 'sin(x)/x', '#frPLplot Example 1 - Sinc Function' )

!   Fill up the arrays

   do i = 1, 100
     x(i) = (i-20.0_plflt)/6.0_plflt
     y(i) = 1.0_plflt
     if (x(i) .ne. 0.0_plflt) y(i) = sin(x(i)) / x(i)
   enddo

!   Draw the line

   call plcol0(3)
   call plwid(2)
   call plline( x, y )
   call plwid(1)

end subroutine plot2

!======================================================================
subroutine plot3()

!
!   For the final graph we wish to override the default tick intervals,
!   and so do not use PLENV

   real(plflt), dimension(1:101) :: x, y

   integer i
   call pladv(0)

!   Use standard viewport, and define X range from 0 to 360 degrees,
!   Y range from -1.2 to 1.2.

   call plvsta()
   call plwind( 0.0_plflt, 360.0_plflt, -1.2_plflt, 1.2_plflt )

!   Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

   call plcol0(1)
   call plbox( 'bcnst', 60.0_plflt, 2, 'bcnstv', 0.2_plflt, 2 )

!   Superimpose a dashed line grid, with 1.5 mm marks and spaces. With
!   only a single mark and space element, we do not need arrays

   call plstyl( 1, 1500, 1500 )
   call plcol0(2)
   call plbox( 'g', 30.0_plflt, 0, 'g', 0.2_plflt, 0 )
   call plstyl( 0, 0, 0 )

   call plcol0(3)
   call pllab( 'Angle (degrees)', 'sine', '#frPLplot Example 1 - Sine function' )

   do i = 1, 101
     x(i) = 3.6_plflt * (i-1)
     y(i) = sin( x(i) * PI/180.0_plflt )
   enddo

   call plcol0(4)
   call plline( x, y )

end subroutine plot3

end program x01f95
