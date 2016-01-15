!      Copyright (C) 2004-2016  Alan W. Irwin
!
!      This file is part of PLplot.
!
!      PLplot is free software; you can redistribute it and/or modify
!      it under the terms of the GNU Library General Public License as
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
! main
!
! Shows two views of the world map.
!--------------------------------------------------------------------------

      program x19f
      use plplot, doublePI => PL_PI
      implicit none
      real(kind=plflt), parameter :: PI = double_PI

      integer, parameter :: double = kind(1.0d0)

      real(kind=plflt)    minx, maxx, miny, maxy
      real(kind=plflt), dimension(1:1) :: x, y

 !     Variables for the shapelib example
      integer, parameter                 :: nbeachareas    = 2
      integer, dimension(nbeachareas)    :: beachareas     = (/ 23, 24 /)
      integer, parameter                 :: nwoodlandareas = 94
      integer, dimension(nwoodlandareas) :: woodlandareas
      integer, parameter                 :: nshingleareas  = 22
      integer, dimension(nshingleareas)  :: shingleareas = (/ 0, 1, 24, 25, 26, 27, 28, 29, 30, 31, 32, &
                                                              33, 34, 35, 217, 2424, 2425, 2426, 2427,  &
                                                              2428, 2491, 2577 /)
      integer, parameter                 :: ncragareas     = 2024
      integer, dimension(ncragareas)     :: cragareas
      integer, dimension(9)              :: majorroads     = (/ 33, 48, 71, 83, 89, 90, 101, 102, 111 /)
      integer                            :: i
      integer :: plparseopts_rc

!      Process command-line arguments
      plparseopts_rc = plparseopts(PL_PARSE_FULL)

! Longitude (x) and latitude (y)

      miny = -70._plflt
      maxy = 80._plflt

      call plinit()

! Cartesian plots
! Most of world

      minx = -170._plflt
      maxx = minx+360._plflt

! Setup a custom latitude and longitude-based scaling function.
      call plslabelfunc(geolocation_labeler)

      call plcol0(1)
      call plenv(minx, maxx, miny, maxy, 1, 70)

      call plmap('usaglobe', minx, maxx, miny, maxy)

! The Americas

      minx = 190._plflt
      maxx = 340._plflt

      call plcol0(1)
      call plenv(minx, maxx, miny, maxy, 1, 70)
      call plmap('usaglobe', minx, maxx, miny, maxy)

! Clear the labeling function
      call plslabelfunc

! Polar, Northern hemisphere

      minx = 0._plflt
      maxx = 360._plflt

      call plenv(-75._plflt, 75._plflt, -75._plflt, &
              75._plflt, 1, -1)
      call plmap(mapform19,'globe', minx, maxx, miny, maxy)

      call pllsty(2)
      call plmeridians(mapform19,10.0_plflt, 10.0_plflt, &
              0.0_plflt, 360.0_plflt, -10.0_plflt, &
              80.0_plflt)

! Polar, Northern hemisphere, this time with a PLplot-wide transform

      minx = 0._plflt
      maxx = 360._plflt

      call plstransform( map_transform )

      call pllsty( 1 )
      call plenv( -75._plflt, 75._plflt, -75._plflt, &
           75._plflt, 1, -1 )
      ! No need to set the map transform here as the global
      ! transform will be used.
      call plmap('globe', minx, maxx, miny, maxy )

      call pllsty( 2 )
      call plmeridians(10.0_plflt, 10.0_plflt, &
           0.0_plflt, 360.0_plflt, -10.0_plflt, &
           80.0_plflt )

      ! Show Baltimore, MD on the map
      call plcol0( 2 )
      call plssym( 0.0_plflt, 2.0_plflt )
      x=-76.6125_plflt
      y=39.2902778_plflt
      call plpoin( x, y, 18 )
      call plssym( 0.0_plflt, 1.0_plflt )
      call plptex( -76.6125_plflt, 43.0_plflt, 0.0_plflt, &
           0.0_plflt, 0.0_plflt, 'Baltimore, MD' )

      ! For Fortran, this is how the global transform is cleared.
      call plstransform

      call plend()

      contains

      subroutine map_transform(x, y, xt, yt)

      ! These callback arguments must have exactly these attributes.
      real(kind=double), intent(in) :: x, y
      real(kind=double), intent(out) :: xt, yt

      real(kind=plflt) :: radius

      radius = 90.0_plflt - y
      xt = radius * cos(x * PI / 180.0_plflt)
      yt = radius * sin(x * PI / 180.0_plflt)
      end subroutine map_transform
!--------------------------------------------------------------------------
! mapform19
!
! Defines specific coordinate transformation for example 19.
! Not to be confused with mapform in src/plmap.c.
! x(), y() are the coordinates to be plotted.
!--------------------------------------------------------------------------

      subroutine mapform19(n, x, y)

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
      end subroutine mapform19

!     "Normalize" longitude values so that they always fall between
!      -180.0 and 180.0
      function normalize_longitude(lon)
      real(kind=plflt) :: normalize_longitude
      real(kind=plflt) :: lon, times

      if ((lon .ge. -180.0_plflt) .and. (lon .le. 180.0_plflt)) then
         normalize_longitude = lon
      else
         times = floor ((abs(lon) + 180.0_plflt) / 360.0_plflt)
        if (lon .lt. 0.0_plflt) then
           normalize_longitude = lon + 360.0_plflt * times
        else
           normalize_longitude = lon - 360.0_plflt * times
        endif
      endif
      end function normalize_longitude
!
! A custom axis labeling function for longitudes and latitudes.
!
      subroutine geolocation_labeler(axis, value, label)
      integer, intent(in) :: axis
      real(kind=plflt), intent(in) :: value
      character(len=*), intent(out) :: label

      integer :: length
      character(len=5) direction_label
      real(kind=plflt) :: label_val = 0.0_plflt

      length = len(label)

      if (axis .eq. 2) then
         label_val = value
         if (label_val .gt. 0.0_plflt) then
            direction_label = ' N'
         else if (label_val .lt. 0.0_plflt) then
            direction_label = ' S'
         else
            direction_label = 'Eq'
         endif
      else if (axis .eq. 1) then
         !FIXME.  How does a callback get access to another routine within the same contains block?
         ! The only way I know to fix this is to make normalize_longitude a separate routine outside
         ! the x22f program.  But that is pretty ugly.
!         label_val = normalize_longitude(value)
         label_val = value !temporary until a fix for the above can be found.
         if (label_val .gt. 0.0_plflt) then
            direction_label = ' E'
         else if (label_val .lt. 0.0_plflt) then
            direction_label = ' W'
         else
            direction_label = ''
         endif
      endif
      if (axis .eq. 2 .and. value .eq. 0.0_plflt) then
!     A special case for the equator
         label = direction_label
      else if (abs(label_val) .lt. 10.0_plflt) then
         write(label,'(I1.1,A2)') iabs(int(label_val)),direction_label
      else if (abs(label_val) .lt. 100.0_plflt) then
         write(label,'(I2.1,A2)') iabs(int(label_val)),direction_label
      else
        write(label,'(I3.1,A2)') iabs(int(label_val)),direction_label
     endif
      end subroutine geolocation_labeler

      end program x19f
