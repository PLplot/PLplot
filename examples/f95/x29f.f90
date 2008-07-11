!  $Id$
!
!   Sample plots using date / time formatting for axes
!
!   Copyright (C) 2008  Andrew Ross
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU General Library Public License as
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
!

   program x29f95
   use plplot

   implicit none

   real(kind=plflt), dimension(365) :: x, y
   common /plotdat/ x, y

   call plparseopts(PL_PARSE_FULL)
   
   call plinit()
   
   call plot1()
   call plot2()
   call plot3()
   
   call plend()
   end program x29f95

!====================================================================
!
!     Plot a model diurnal cycle of temperature
     
      subroutine plot1()
      use plplot, PI => PL_PI
      implicit none

      real(kind=plflt), dimension(365) :: x, y
      common /plotdat/ x, y

      integer :: i, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      
      parameter(npts = 145)
      parameter(xmin = 0.0_plflt)
      parameter(xmax = 60.0_plflt*60.0_plflt*24.0_plflt)
      parameter(ymin = 10.0_plflt)
      parameter(ymax = 20.0_plflt)

      do i = 1,npts
      	 x(i) = xmax*(dble(i-1)/dble(npts))
         y(i) = 15.0_plflt - 5.0_plflt*cos(2.0_plflt*PI*dble(i-1)/dble(npts))
      enddo

      call pladv(0)
      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

!     Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
      call plcol0(1)
!     Set time format to be hours:minutes
      call pltimefmt("%H:%M")
      call plbox("bcnstd", 3.0_plflt*60.0_plflt*60.0_plflt, 3, "bcnstv", &
           1.0_plflt, 5)

      call plcol0(3)
      call pllab("Time (hours:mins)", "Temperature (degC)", &
           "#frPLplot Example 29 - Daily temperature")
      
      call plcol0(4)

      call plline(x(1:npts), y(1:npts))
      
    end subroutine plot1

!
!     Plot the number of hours of daylight as a function of day for a year
!
      subroutine plot2() 
      use plplot, PI => PL_PI

      implicit none
      
      integer ::  j, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      real(kind=plflt) :: lat, p, d
      real(kind=plflt), dimension(365) :: x, y
      common /plotdat/ x, y


      ! Latitude for London
      parameter (lat = 51.5_plflt)
      
      parameter (npts = 365)
      
      parameter(xmin = 0.0_plflt)
      parameter(xmax = npts*60.0_plflt*60.0_plflt*24.0_plflt)
      parameter(ymin = 0)
      parameter(ymax = 24)

!     Formula for hours of daylight from 
!     "A Model Comparison for Daylength as a Function of Latitude and 
!     Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
      do j=1,npts
         x(j) = (j-1)*60.0_plflt*60.0_plflt*24.0_plflt;
         p = asin(0.39795_plflt*cos(0.2163108_plflt + 2.0_plflt* &
              atan(0.9671396_plflt*tan(0.00860_plflt*(j-187)))))
         d = 24.0_plflt - (24.0_plflt/PI)* &
              acos( (sin(0.8333_plflt*PI/180.0_plflt) + &
              sin(lat*PI/180.0_plflt)*sin(p)) / (cos(lat*PI/180.0_plflt)* &
              cos(p)) )
         y(j) = d
      enddo

      call plcol0(1)
!     Set time format to be abbreviated month name followed by day of month
      call pltimefmt("%b %d")
      call plenv(xmin, xmax, ymin, ymax, 0, 40)
      
      call plcol0(3)
      call pllab("Date", "Hours of daylight", &
           "#frPLplot Example 29 - Hours of daylight at 51.5N")
  
      call plcol0(4)

      call plline(x, y)
  
    end subroutine plot2

!
!
!
      subroutine plot3()
      use plplot, PI => PL_PI
      implicit none
      
      integer :: i, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      integer :: tstart, t1, t2
!      real(kind=plflt) :: toff;
      real(kind=plflt), dimension(365) :: x, y
      common /plotdat/ x, y

!     integer tm(9)

      parameter (npts = 62)

!     number of seconds elapsed since the Unix epoch (1970-01-01, UTC) for
!     2005-12-01, UTC.  This is the same result as the Python
!     calendar.timegm((2005,12,1,0,0,0)) result or the Linux C timegm
!     result corresponding to 2005-12-01.
      tstart = 1133395200

      xmin = dble(tstart)
      xmax = xmin + npts*60.0_plflt*60.0_plflt*24.0_plflt
      ymin = 0.0_plflt
      ymax = 5.0_plflt
  
      do i=1,npts
         x(i) = xmin + dble(i-1)*60.0_plflt*60.0_plflt*24.0_plflt
         y(i) = 1.0_plflt + sin( 2.0_plflt*PI*dble(i-1)/7.0_plflt) + &
              exp( dble(min(i-1,npts+1-i)) / 31.0_plflt)
      enddo
      call pladv(0)

      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

      call plcol0(1)
!     Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
!     equivalent to %f for C99 compliant implementations of strftime.
      call pltimefmt("%Y-%m-%d")
!     Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
      call plbox("bcnstd", 14.0_plflt*24.0_plflt*60.0_plflt*60.0_plflt,14, &
           "bcnstv", 1.0_plflt, 4)

      call plcol0(3)
      call pllab("Date", "Hours of television watched", &
           "#frPLplot Example 29 - Hours of television watched in " // &
           "Dec 2005 / Jan 2006");
  
      call plcol0(4)

      call plpoin(x(1:npts), y(1:npts), 2)
      call plline(x(1:npts), y(1:npts))
 
    end subroutine plot3
