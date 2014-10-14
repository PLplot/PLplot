!   Sample plots using date / time formatting for axes
!
!   Copyright (C) 2008  Andrew Ross
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

   program x29f95
   use plplot

   implicit none

   real(kind=plflt), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
   common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

   call plparseopts(PL_PARSE_FULL)
   
   call plinit()

!  This is the ASCII value for character @
   call plsesc(64_plint)
   
   call plot1()
   call plot2()
   call plot3()
   call plot4()
   
   call plend()
   end program x29f95

!====================================================================
!
!     Plot a model diurnal cycle of temperature
     
      subroutine plot1()
      use plplot, PI => PL_PI
      implicit none

      real(kind=plflt), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

      integer(kind=plint) :: i, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      
      parameter(npts = 73_plint)
      parameter(xmin = 0.0_plflt)
      parameter(xmax = 60.0_plflt*60.0_plflt*24.0_plflt)
      parameter(ymin = 10.0_plflt)
      parameter(ymax = 20.0_plflt)

      do i = 1_plint,npts
         x(i) = xmax*(dble(i-1)/dble(npts))
         y(i) = 15.0_plflt - 5.0_plflt*cos(2.0_plflt*PI*dble(i-1)/dble(npts))
!     Set x error bars to +/- 5 minute 
         xerr1(i) = x(i)-60.0_plflt*5.0_plflt
         xerr2(i) = x(i)+60.0_plflt*5.0_plflt
!     Set y error bars to +/- 0.1 deg C
         yerr1(i) = y(i)-0.1_plflt
         yerr2(i) = y(i)+0.1_plflt
      enddo

      call pladv(0_plint)
      
!     Rescale major ticks marks by 0.5
      call plsmaj(0.0_plflt,0.5_plflt)
!     Rescale minor ticks and error bar marks by 0.5
      call plsmin(0.0_plflt,0.5_plflt)

      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

!     Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y.
      call plcol0(1_plint)
!     Set time format to be hours:minutes
      call pltimefmt("%H:%M")
      call plbox("bcnstd", 3.0_plflt*60.0_plflt*60.0_plflt, 3_plint, "bcnstv", &
           1.0_plflt, 5_plint)

      call plcol0(3_plint)
      call pllab("Time (hours:mins)", "Temperature (degC)", &
           "@frPLplot Example 29 - Daily temperature")
      
      call plcol0(4_plint)

      call plline(x(1:npts), y(1:npts))
      call plcol0(2_plint)
      call plerrx(xerr1(1:npts), xerr2(1:npts), y(1:npts))
      call plcol0(3_plint)
      call plerry(x(1:npts), yerr1(1:npts), yerr2(1:npts))

!     Rescale major / minor tick marks back to default
      call plsmin(0.0_plflt,1.0_plflt)
      call plsmaj(0.0_plflt,1.0_plflt)
      
    end subroutine plot1

!
!     Plot the number of hours of daylight as a function of day for a year
!
      subroutine plot2() 
      use plplot, PI => PL_PI

      implicit none
      
      integer(kind=plint) ::  j, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      real(kind=plflt) :: lat, p, d
      real(kind=plflt), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2


      ! Latitude for London
      parameter (lat = 51.5_plflt)
      
      parameter (npts = 365_plint)
      
      parameter(xmin = 0.0_plflt)
      parameter(xmax = npts*60.0_plflt*60.0_plflt*24.0_plflt)
      parameter(ymin = 0_plint)
      parameter(ymax = 24_plint)

!     Formula for hours of daylight from 
!     "A Model Comparison for Daylength as a Function of Latitude and 
!     Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
      do j=1,npts
         x(j) = (j-1)*60.0_plflt*60.0_plflt*24.0_plflt
         p = asin(0.39795_plflt*cos(0.2163108_plflt + 2.0_plflt* &
              atan(0.9671396_plflt*tan(0.00860_plflt*(j-187)))))
         d = 24.0_plflt - (24.0_plflt/PI)* &
              acos( (sin(0.8333_plflt*PI/180.0_plflt) + &
              sin(lat*PI/180.0_plflt)*sin(p)) / (cos(lat*PI/180.0_plflt)* &
              cos(p)) )
         y(j) = d
      enddo

      call plcol0(1_plint)
!     Set time format to be abbreviated month name followed by day of month
      call pltimefmt("%b %d")
      call plprec(1_plint,1_plint)
      call plenv(xmin, xmax, ymin, ymax, 0_plint, 40_plint)
      
      call plcol0(3_plint)
      call pllab("Date", "Hours of daylight", &
           "@frPLplot Example 29 - Hours of daylight at 51.5N")
  
      call plcol0(4_plint)

      call plline(x, y)
  
      call plprec(0_plint,0_plint)

    end subroutine plot2

!
!
!
      subroutine plot3()
      use plplot, PI => PL_PI
      implicit none
      
      integer(kind=plint) :: i, npts
      real(kind=plflt) :: xmin, xmax, ymin, ymax
      integer(kind=plint) :: tstart
!      real(kind=plflt) :: toff
      real(kind=plflt), dimension(365) :: x, y, xerr1, xerr2, yerr1, yerr2
      common /plotdat/ x, y, xerr1, xerr2, yerr1, yerr2

!     integer(kind=plint) tm(9)

      parameter (npts = 62_plint)

!     number of seconds elapsed since the Unix epoch (1970-01-01, UTC) for
!     2005-12-01, UTC.  This is the same result as the Python
!     calendar.timegm((2005,12,1,0,0,0)) result or the Linux C timegm
!     result corresponding to 2005-12-01.
      tstart = 1133395200_plint

      xmin = dble(tstart)
      xmax = xmin + npts*60.0_plflt*60.0_plflt*24.0_plflt
      ymin = 0.0_plflt
      ymax = 5.0_plflt
  
      do i=1,npts
         x(i) = xmin + dble(i-1)*60.0_plflt*60.0_plflt*24.0_plflt
         y(i) = 1.0_plflt + sin( 2.0_plflt*PI*dble(i-1)/7.0_plflt) + &
              exp( dble(min(i-1,npts+1-i)) / 31.0_plflt)
      enddo
      call pladv(0_plint)

      call plvsta()
      call plwind(xmin, xmax, ymin, ymax)

      call plcol0(1_plint)
!     Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
!     equivalent to %f for C99 compliant implementations of strftime.
      call pltimefmt("%Y-%m-%d")
!     Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
      call plbox("bcnstd", 14.0_plflt*24.0_plflt*60.0_plflt*60.0_plflt,14_plint, &
           "bcnstv", 1.0_plflt, 4_plint)

      call plcol0(3_plint)
      call pllab("Date", "Hours of television watched", &
           "@frPLplot Example 29 - Hours of television watched in " // &
           "Dec 2005 / Jan 2006")
  
      call plcol0(4_plint)

      call plssym(0.0_plflt, 0.5_plflt)
      call plpoin(x(1:npts), y(1:npts), 2_plint)
      call plline(x(1:npts), y(1:npts))
 
    end subroutine plot3

!
!
!
      subroutine plot4() 
      use plplot, PI => PL_PI
      implicit none

!     TAI-UTC (seconds) as a function of time.
!     Use Besselian epochs as the continuous time interval just to prove
!     this does not introduce any issues.
  
      real(kind=plflt) :: scale, offset1, offset2
      real(kind=plflt) :: xmin, xmax, ymin, ymax, xlabel_step
      integer(kind=plint) :: k, npts = 0_plint, i
      logical :: if_TAI_time_format = .false.
      character(len=10) :: time_format
      character(len=100) :: title_suffix
      character(len=100) :: xtitle
      character(len=100) :: title
      real(kind=plflt) :: x(1001_plint), y(1001_plint)
      integer(kind=plint) :: tai_year, tai_month, tai_day, tai_hour, tai_min
      real(kind=plflt) :: tai_sec, tai
      integer(kind=plint) :: utc_year, utc_month, utc_day, utc_hour, utc_min
      real(kind=plflt) :: utc_sec, utc
      
!     Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
!     B = 1900. + (JD -2415020.31352)/365.242198781 
!     ==> (as calculated with aid of "bc -l" command)
!     B = (MJD + 678940.364163900)/365.242198781
!     ==>
!     MJD = B*365.24219878 - 678940.364163900
      scale = 365.242198781_plflt
      offset1 = -678940.0_plflt
      offset2 = -0.3641639_plflt
      call plconfigtime(scale, offset1, offset2, 0_plint, 0_plint, 0_plint, 0_plint, 0_plint, 0_plint, &
           0_plint, 0._plflt)

      do k = 0_plint,6
         if (k .eq. 0_plint) then
            call plctime(1950_plint,0_plint,2,0_plint,0,0.,xmin)
            call plctime(2020_plint,0_plint,2,0_plint,0,0.,xmax)
            npts = 70*12 + 1_plint
            ymin = 0.0_plflt
            ymax = 36.0_plflt
            time_format="%Y%"
            if_TAI_time_format = .true.
            title_suffix = "from 1950 to 2020"
            xtitle =  "Year"
            xlabel_step = 10.0_plflt
         elseif ((k .eq. 1_plint) .or. (k .eq. 2_plint)) then
            call plctime(1961_plint,7_plint,1,0_plint,0,1.64757_plflt-.20_plflt, xmin)
            call plctime(1961_plint,7_plint,1,0_plint,0,1.64757_plflt+.20_plflt, xmax)
            npts = 1001_plint
            ymin = 1.625_plflt
            ymax = 1.725_plflt
            time_format = "%S%2%"
            title_suffix = "near 1961-08-01 (TAI)"
            xlabel_step = 0.05_plflt/(scale*86400.0_plflt)
            if (k .eq. 1_plint) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         elseif ((k .eq. 3_plint) .or. (k .eq. 4_plint)) then
            call plctime(1963_plint,10_plint,1,0_plint,0,2.6972788_plflt-.20_plflt, xmin)
            call plctime(1963_plint,10_plint,1,0_plint,0,2.6972788_plflt+.20_plflt, xmax)
            npts = 1001_plint
            ymin = 2.55_plflt
            ymax = 2.75_plflt
            time_format = "%S%2%"
            title_suffix = "near 1963-11-01 (TAI)"
            xlabel_step = 0.05_plflt/(scale*86400.0_plflt)
            if (k .eq. 3_plint) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         elseif ((k .eq. 5_plint) .or. (k .eq. 6_plint)) then
            call plctime(2009_plint,0_plint,1,0_plint,0,34._plflt-5._plflt,xmin)
            call plctime(2009_plint,0_plint,1,0_plint,0,34._plflt+5._plflt,xmax)
            npts = 1001_plint
            ymin = 32.5_plflt
            ymax = 34.5_plflt
            time_format = "%S%2%"
            title_suffix = "near 2009-01-01 (TAI)"
            xlabel_step = 1._plflt/(scale*86400._plflt)
            if (k .eq. 5_plint) then
               if_TAI_time_format = .true.
               xtitle = "Seconds (TAI)"
            else 
               if_TAI_time_format = .false.
               xtitle = "Seconds (TAI) labelled with corresponding UTC"
            endif
         endif

         do i=0,npts-1
            x(i+1) = xmin + i*(xmax-xmin)/(dble(npts-1))
            call plconfigtime(scale, offset1, offset2, 0_plint, 0_plint, 0_plint, 0_plint, 0_plint, &
                 0_plint, 0_plint, 0._plflt)
            tai = x(i+1)
            call plbtime(tai_year, tai_month, tai_day, tai_hour, &
                 tai_min, tai_sec, tai)
            call plconfigtime(scale, offset1, offset2, 2_plint, 0_plint, 0_plint, 0_plint, &
                 0_plint, 0_plint, 0_plint, 0._plflt)
            call plbtime(utc_year, utc_month, utc_day, utc_hour, &
                 utc_min, utc_sec, tai)
            call plconfigtime(scale, offset1, offset2, 0_plint, 0_plint, 0_plint, 0_plint, &
                 0_plint, 0_plint, 0_plint, 0._plflt)
            call plctime(utc_year, utc_month, utc_day, utc_hour, &
                 utc_min, utc_sec, utc)
            y(i+1)=(tai-utc)*scale*86400._plflt
         enddo

         call pladv(0_plint)
         call plvsta()
         call plwind(xmin, xmax, ymin, ymax)
         call plcol0(1_plint)
         if (if_TAI_time_format) then
            call plconfigtime(scale, offset1, offset2, 0_plint, 0_plint, 0_plint, 0_plint, &
                 0_plint, 0_plint, 0_plint, 0._plflt)
         else
            call plconfigtime(scale, offset1, offset2, 2_plint, 0_plint, 0_plint, 0_plint, &
                 0_plint, 0_plint, 0_plint, 0._plflt)
         endif
         call pltimefmt(time_format)
         call plbox("bcnstd", xlabel_step, 0_plint, "bcnstv", 0._plflt, 0_plint)
         call plcol0(3_plint)
         title = "@frPLplot Example 29 - TAI-UTC "// &
             trim(title_suffix)
         call pllab(xtitle, "TAI-UTC (sec)", title)
    
         call plcol0(4_plint)
         
         call plline(x(1:npts), y(1:npts))
      enddo
      end subroutine plot4
