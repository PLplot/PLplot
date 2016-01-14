!   Sample plots using date / time formatting for axes
!
!   Copyright (C) 2008  Andrew Ross
!   Copyright (C) 2008-2016 Alan W. Irwin
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
!

   program x29f
     use plplot, double_PI => PL_PI
     implicit none

     ! This example _requires_ double precision to work because of time representation
     ! issues with single precision.  Therefore, cannot use plflt if that happens
     ! to refer to single-precision.
     integer, parameter :: double = kind(1.0d0)
     
     real(kind=double), parameter :: PI = double_PI

   real(kind=double), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
   common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

   call plparseopts(PL_PARSE_FULL)

   call plinit()

!  This is the ASCII value for character @
   call plsesc(64)

   call plot1()
   call plot2()
   call plot3()
   call plot4()

   call plend()

 contains

   
!====================================================================
!
!     Plot a model diurnal cycle of temperature

      subroutine plot1()

      real(kind=double), parameter :: PI = double_PI
      real(kind=double), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

      integer :: i, npts
      real(kind=double) :: xmin, xmax, ymin, ymax

      parameter(npts = 73)
      parameter(xmin = 0.0_double)
      parameter(xmax = 60.0_double*60.0_double*24.0_double)
      parameter(ymin = 10.0_double)
      parameter(ymax = 20.0_double)

      do i = 1,npts
         x(i) = xmax*(real(i-1,kind=double)/real(npts,kind=double))
         y(i) = 15.0_double - 5.0_double*cos(2.0_double*PI*real(i-1,kind=double)/real(npts,kind=double))
!     Set x error bars to +/- 5 minute
         xerr1(i) = x(i)-60.0_double*5.0_double
         xerr2(i) = x(i)+60.0_double*5.0_double
!     Set y error bars to +/- 0.1 deg C
         yerr1(i) = y(i)-0.1_double
         yerr2(i) = y(i)+0.1_double
      enddo

      call pladv(0)

!     Rescale major ticks marks by 0.5
      call plsmaj(0.0_double,0.5_double)
!     Rescale minor ticks and error bar marks by 0.5
      call plsmin(0.0_double,0.5_double)

      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

!     Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
      call plcol0(1)
!     Set time format to be hours:minutes
      call pltimefmt("%H:%M")
      call plbox("bcnstd", 3.0_double*60.0_double*60.0_double, 3, "bcnstv", &
           1.0_double, 5)

      call plcol0(3)
      call pllab("Time (hours:mins)", "Temperature (degC)", &
           "@frPLplot Example 29 - Daily temperature")

      call plcol0(4)

      call plline(x(1:npts), y(1:npts))
      call plcol0(2)
      call plerrx(xerr1(1:npts), xerr2(1:npts), y(1:npts))
      call plcol0(3)
      call plerry(x(1:npts), yerr1(1:npts), yerr2(1:npts))

!     Rescale major / minor tick marks back to default
      call plsmin(0.0_double,1.0_double)
      call plsmaj(0.0_double,1.0_double)

    end subroutine plot1

!
!     Plot the number of hours of daylight as a function of day for a year
!
      subroutine plot2()

      real(kind=double), parameter :: PI = double_PI
      integer ::  j, npts
      real(kind=double) :: xmin, xmax, ymin, ymax
      real(kind=double) :: lat, p, d
      real(kind=double), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2


      ! Latitude for London
      parameter (lat = 51.5_double)

      parameter (npts = 365)

      parameter(xmin = 0.0_double)
      parameter(xmax = npts*60.0_double*60.0_double*24.0_double)
      parameter(ymin = 0)
      parameter(ymax = 24)

!     Formula for hours of daylight from
!     "A Model Comparison for Daylength as a Function of Latitude and
!     Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
      do j=1,npts
         x(j) = (j-1)*60.0_double*60.0_double*24.0_double
         p = asin(0.39795_double*cos(0.2163108_double + 2.0_double* &
              atan(0.9671396_double*tan(0.00860_double*(j-187)))))
         d = 24.0_double - (24.0_double/PI)* &
              acos( (sin(0.8333_double*PI/180.0_double) + &
              sin(lat*PI/180.0_double)*sin(p)) / (cos(lat*PI/180.0_double)* &
              cos(p)) )
         y(j) = d
      enddo

      call plcol0(1)
!     Set time format to be abbreviated month name followed by day of month
      call pltimefmt("%b %d")
      call plprec(1,1)
      call plenv(xmin, xmax, ymin, ymax, 0, 40)

      call plcol0(3)
      call pllab("Date", "Hours of daylight", &
           "@frPLplot Example 29 - Hours of daylight at 51.5N")

      call plcol0(4)

      call plline(x, y)

      call plprec(0,0)

    end subroutine plot2

!
!
!
      subroutine plot3()

      real(kind=double), parameter :: PI = double_PI
      integer :: i, npts
      real(kind=double) :: xmin, xmax, ymin, ymax
      integer :: tstart
!      real(kind=double) :: toff
      real(kind=double), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

!     integer tm(9)

      parameter (npts = 62)

!     number of seconds elapsed since the Unix epoch (1970-01-01, UTC) for
!     2005-12-01, UTC.  This is the same result as the Python
!     calendar.timegm((2005,12,1,0,0,0)) result or the Linux C timegm
!     result corresponding to 2005-12-01.
      tstart = 1133395200

      xmin = real(tstart,kind=double)
      xmax = xmin + npts*60.0_double*60.0_double*24.0_double
      ymin = 0.0_double
      ymax = 5.0_double

      do i=1,npts
         x(i) = xmin + real(i-1,kind=double)*60.0_double*60.0_double*24.0_double
         y(i) = 1.0_double + sin( 2.0_double*PI*real(i-1,kind=double)/7.0_double) + &
              exp( real(min(i-1,npts+1-i),kind=double) / 31.0_double)
      enddo
      call pladv(0)

      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

      call plcol0(1)
!     Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
!     equivalent to %f for C99 compliant implementations of strftime.
      call pltimefmt("%Y-%m-%d")
!     Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
      call plbox("bcnstd", 14.0_double*24.0_double*60.0_double*60.0_double,14, &
           "bcnstv", 1.0_double, 4)

      call plcol0(3)
      call pllab("Date", "Hours of television watched", &
           "@frPLplot Example 29 - Hours of television watched in " // &
           "Dec 2005 / Jan 2006")

      call plcol0(4)

      call plssym(0.0_double, 0.5_double)
      call plpoin(x(1:npts), y(1:npts), 2)
      call plline(x(1:npts), y(1:npts))

    end subroutine plot3

!
!
!
      subroutine plot4()
!     TAI-UTC (seconds) as a function of time.
!     Use Besselian epochs as the continuous time interval just to prove
!     this does not introduce any issues.

      real(kind=double) :: scale, offset1, offset2
      real(kind=double) :: xmin, xmax, ymin, ymax, xlabel_step
      integer :: k, npts = 0, i
      logical :: if_TAI_time_format = .false.
      character(len=10) :: time_format
      character(len=100) :: title_suffix
      character(len=100) :: xtitle
      character(len=100) :: title
      real(kind=double) :: x(1001), y(1001)
      integer :: tai_year, tai_month, tai_day, tai_hour, tai_min
      real(kind=double) :: tai_sec, tai
      integer :: utc_year, utc_month, utc_day, utc_hour, utc_min
      real(kind=double) :: utc_sec, utc

!     Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
!     B = 1900. + (JD -2415020.31352)/365.242198781
!     ==> (as calculated with aid of "bc -l" command)
!     B = (MJD + 678940.364163900)/365.242198781
!     ==>
!     MJD = B*365.24219878 - 678940.364163900
      scale = 365.242198781_double
      offset1 = -678940.0_double
      offset2 = -0.3641639_double
      call plconfigtime(scale, offset1, offset2, 0, .false., 0, 0, 0, 0, &
           0, 0._double)

      do k = 0,6
         if (k .eq. 0) then
            call plctime(1950,0,2,0,0,0._double,xmin)
            call plctime(2020,0,2,0,0,0._double,xmax)
            npts = 70*12 + 1
            ymin = 0.0_double
            ymax = 36.0_double
            time_format="%Y%"
            if_TAI_time_format = .true.
            title_suffix = "from 1950 to 2020"
            xtitle =  "Year"
            xlabel_step = 10.0_double
         elseif ((k .eq. 1) .or. (k .eq. 2)) then
            call plctime(1961,7,1,0,0,1.64757_double-.20_double, xmin)
            call plctime(1961,7,1,0,0,1.64757_double+.20_double, xmax)
            npts = 1001
            ymin = 1.625_double
            ymax = 1.725_double
            time_format = "%S%2%"
            title_suffix = "near 1961-08-01 (TAI)"
            xlabel_step = 0.05_double/(scale*86400.0_double)
            if (k .eq. 1) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         elseif ((k .eq. 3) .or. (k .eq. 4)) then
            call plctime(1963,10,1,0,0,2.6972788_double-.20_double, xmin)
            call plctime(1963,10,1,0,0,2.6972788_double+.20_double, xmax)
            npts = 1001
            ymin = 2.55_double
            ymax = 2.75_double
            time_format = "%S%2%"
            title_suffix = "near 1963-11-01 (TAI)"
            xlabel_step = 0.05_double/(scale*86400.0_double)
            if (k .eq. 3) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         elseif ((k .eq. 5) .or. (k .eq. 6)) then
            call plctime(2009,0,1,0,0,34._double-5._double,xmin)
            call plctime(2009,0,1,0,0,34._double+5._double,xmax)
            npts = 1001
            ymin = 32.5_double
            ymax = 34.5_double
            time_format = "%S%2%"
            title_suffix = "near 2009-01-01 (TAI)"
            xlabel_step = 1._double/(scale*86400._double)
            if (k .eq. 5) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         endif

         do i=0,npts-1
            x(i+1) = xmin + i*(xmax-xmin)/(real(npts-1,kind=double))
            call plconfigtime(scale, offset1, offset2, 0, .false., 0, 0, 0, &
                 0, 0, 0._double)
            tai = x(i+1)
            call plbtime(tai_year, tai_month, tai_day, tai_hour, &
                 tai_min, tai_sec, tai)
            call plconfigtime(scale, offset1, offset2, 2, .false., 0, 0, &
                 0, 0, 0, 0._double)
            call plbtime(utc_year, utc_month, utc_day, utc_hour, &
                 utc_min, utc_sec, tai)
            call plconfigtime(scale, offset1, offset2, 0, .false., 0, 0, &
                 0, 0, 0, 0._double)
            call plctime(utc_year, utc_month, utc_day, utc_hour, &
                 utc_min, utc_sec, utc)
            y(i+1)=(tai-utc)*scale*86400._double
         enddo

         call pladv(0)
         call plvsta()
         call plwind(xmin, xmax, ymin, ymax)
         call plcol0(1)
         if (if_TAI_time_format) then
            call plconfigtime(scale, offset1, offset2, 0, .false., 0, 0, &
                 0, 0, 0, 0._double)
         else
            call plconfigtime(scale, offset1, offset2, 2, .false., 0, 0, &
                 0, 0, 0, 0._double)
         endif
         call pltimefmt(time_format)
         call plbox("bcnstd", xlabel_step, 0, "bcnstv", 0._double, 0)
         call plcol0(3)
         title = "@frPLplot Example 29 - TAI-UTC "// &
             trim(title_suffix)
         call pllab(xtitle, "TAI-UTC (sec)", title)

         call plcol0(4)

         call plline(x(1:npts), y(1:npts))
      enddo
      end subroutine plot4
    end program x29f
