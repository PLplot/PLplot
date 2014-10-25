!   Simple line plot and multiple windows demo.
!
!   Copyright (C) 2004  Alan W. Irwin
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU Library General Public License as
!   published by the Free Software Foundation; either version 2 of the
!   License, or (at your option) any later version.
!
!   PLplot is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU Library General Public License for more details.
!
!   You should have received a copy of the GNU Library General Public
!   License along with PLplot; if not, write to the Free Software
!   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

program x01f95
   use plplot, PI => PL_PI
   use plf95demolib

   implicit none

   real(plflt) :: xscale, yscale, xoff, yoff
   character(len=80) :: version
   integer :: digmax

!  Process command-line arguments
   call plparseopts(PL_PARSE_FULL)

!  Print plplot version
   call plgver(version)
   write (*,'(a,a)') 'PLplot library version: ', trim(version)

!  Initialize plplot
!  Divide page into 2x2 plots
   call plstar(2,2)

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

!  Do a plot

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
   real(plflt) :: xmin, xmax, ymin, ymax

   x = xoff + xscale * arange(1,size(x)+1) / real(size(x),plflt)
   y = yoff + yscale * x ** 2

   xmin = minval(x)
   xmax = maxval(x)
   ymin = minval(y)
   ymax = maxval(y)

!   Set up the viewport and window using PLENV. The range in X is
!   0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
!   scaled separately (just = 0), and we just draw a labelled
!   box (axis = 0).

   call plcol0(1)
   call plenv( xmin, xmax, ymin, ymax, 0, 0 )
   call plcol0(2)
   call pllab( '(x)', '(y)', '#frPLplot Example 1 - y=x#u2' )

!   Plot the data points
!   Only plot every tenth!

   call plcol0(4)
   call plpoin( x(4::10), y(4::10), 9 )

!   Draw the line through the data

   call plcol0(3)
   call plline( x, y )

end subroutine plot1

!======================================================================
subroutine plot2()

   real(plflt), dimension(1:100) :: x, y

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


   x = ( arange(size(x)) - 19 ) / 6.0_plflt
   y = merge( sin(x) / x, 1.0_plflt, x /= 0.0_plflt )

!   Draw the line

   call plcol0(3)
   call plwidth(2._plflt)
   call plline( x, y )
   call plwidth(1._plflt)

end subroutine plot2

!======================================================================
   subroutine plot3()

!
!   For the final graph we wish to override the default tick intervals,
!   and so do not use_ PLENV

   real(plflt), dimension(1:101) :: x, y

   call pladv(0)

!   Use_ standard viewport, and define X range from 0 to 360 degrees,
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

   x = 3.6_plflt * arange(size(x))
   y = sin( x * PI/180.0_plflt )

   call plcol0(4)
   call plline( x, y )

end subroutine plot3

end program x01f95
