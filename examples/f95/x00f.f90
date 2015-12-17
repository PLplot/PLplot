!   Simple demo of a 2D line plot.
!
!   Copyright (C) 2011  Alan W. Irwin
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU Library General Public License as published
!   by the Free Software Foundation; either version 2 of the License, or
!   (at your option) any later version.
!
!   PLplot is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU Library General Public License for more details.
!
!   You should have received a copy of the GNU Library General Public License
!   along with PLplot; if not, write to the Free Software
!   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

program x00f95
    use plf95demolib

    integer, parameter :: NSIZE = 101

    real(kind=plflt), dimension(NSIZE) :: x, y

    real(kind=plflt) :: xmin = 0._plflt, xmax = 1._plflt, ymin = 0._plflt, ymax = 100._plflt
    ! integer          :: i

    ! Prepare data to be plotted.
    x = arange(0, NSIZE) / real(NSIZE-1,plflt)
    y = ymax * x**2

    ! Or alternatively, using a DO-loop
    !do i = 1,NSIZE
    !    x(i) = real( i - 1, plflt ) / real( NSIZE - 1, plflt )
    !    y(i) = ymax * x(i)**2
    !enddo

    ! Parse and process command line arguments
    call plparseopts( PL_PARSE_FULL )

    ! Initialize plplot
    call plinit

    ! Create a labelled box to hold the plot.
    call plenv( xmin, xmax, ymin, ymax, 0, 0 )
    call pllab( "x", "y=100 x#u2#d", "Simple PLplot demo of a 2D line plot" )

    ! Plot the data that was prepared above.
    call plline( x, y )

    ! Close PLplot library
    call plend

end program x00f95
