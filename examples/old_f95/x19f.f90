!      Copyright (C) 2004  Alan W. Irwin
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


      subroutine map_transform(x, y, xt, yt)
      use plplot, PI => PL_PI
      implicit none

      real(kind=plflt)    x, y, xt, yt, radius

      radius = 90.0_plflt - y
      xt = radius * cos(x * PI / 180.0_plflt)
      yt = radius * sin(x * PI / 180.0_plflt)
      return
      end subroutine

!--------------------------------------------------------------------------
! mapform19
!
! Defines specific coordinate transformation for example 19.
! Not to be confused with mapform in src/plmap.c.
! x(), y() are the coordinates to be plotted.
!--------------------------------------------------------------------------

      subroutine mapform19(n, x, y)
      use plplot, PI => PL_PI
      implicit none

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
      return
      end subroutine

!     "Normalize" longitude values so that they always fall between
!      -180.0 and 180.0
      function normalize_longitude(lon)
      use plplot
      implicit none
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
      return
      end function

!
! A custom axis labeling function for longitudes and latitudes.
!
      subroutine geolocation_labeler(axis, value, label, length)
      use plplot
      implicit none
      integer :: axis, length
      real(kind=plflt) :: value
      character(len=length) label
      character(len=5) direction_label
      real(kind=plflt) :: label_val = 0.0_plflt
      real(kind=plflt) :: normalize_longitude

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
         label_val = normalize_longitude(value)
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
      end subroutine

!--------------------------------------------------------------------------
! main
!
! Shows two views of the world map.
!--------------------------------------------------------------------------

      program x19f
      use plplot
      implicit none
      real(kind=plflt)    minx, maxx, miny, maxy
      real(kind=plflt), dimension(1:1) :: x, y
      external map_transform
      external mapform19
      external geolocation_labeler

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

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

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
      call plslabelfunc(0)

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

      ! For f95, this is how the global transform is cleared
      call plstransform( 0 )

      ! An example using shapefiles. The shapefiles used are from Ordnance Survey, UK.
      ! These were chosen because they provide shapefiles for small grid boxes which
      ! are easilly manageable for this demo.

      call pllsty( 1 )

      minx = 240570._plflt
      maxx = 621109._plflt
      miny = 87822._plflt
      maxy = 722770._plflt
      call plscol0( 0, 255, 255, 255 )
      call plscol0( 1, 0, 0, 0 )
      call plscol0( 2, 150, 150, 150 )
      call plscol0( 3, 0, 50, 200 )
      call plscol0( 4, 50, 50, 50 )
      call plscol0( 5, 150, 0, 0 )
      call plscol0( 6, 100, 100, 255 )

      minx = 265000._plflt
      maxx = 270000._plflt
      miny = 145000._plflt
      maxy = 150000._plflt
      call plscol0( 0, 255, 255, 255 )  !white
      call plscol0( 1, 0, 0, 0 )        !black
      call plscol0( 2, 255, 200, 0 )    !yelow for sand
      call plscol0( 3, 60, 230, 60 )    !green for woodland
      call plscol0( 4, 210, 120, 60 )   !brown for contours
      call plscol0( 5, 150, 0, 0 )      !red for major roads
      call plscol0( 6, 180, 180, 255 )  !pale blue for water
      call plscol0( 7, 100, 100, 100 )  !pale grey for shingle or boulders
      call plscol0( 8, 100, 100, 100 )  !dark grey for custom polygons - generally crags


      call plcol0( 1 )
      call plenv( minx, maxx, miny, maxy, 1, -1 )
      call pllab( "", "", "Martinhoe CP, Exmoor National Park, UK (shapelib only)" )


      ! Beach
      call plcol0( 2 )
      call plmapfill( "ss/ss64ne_Landform_Area", minx, maxx, miny, maxy, beachareas )

      ! Woodland
      call plcol0( 3 )
      do i = 1, nwoodlandareas
          woodlandareas(i) = i + 217
      enddo
      call plmapfill( "ss/ss64ne_Landform_Area", minx, maxx, miny, maxy, woodlandareas )

      ! Shingle or boulders
      call plcol0( 7 )
      call plmapfill( "ss/ss64ne_Landform_Area", minx, maxx, miny, maxy, shingleareas )

      ! Crags
      call plcol0( 8 )
      do i = 1,ncragareas
          cragareas(i) = i + 324
      enddo
      call plmapfill( "ss/ss64ne_Landform_Area", minx, maxx, miny, maxy, cragareas )

      ! draw contours, we need to separate contours from high/low coastline
      ! draw_contours(pls, "ss/SS64_line", 433, 20, 4, 3, minx, maxx, miny, maxy );
      call plcol0( 4 )
      call plmapline( "ss/ss64ne_Height_Contours", minx, maxx, miny, maxy )

      ! draw the sea and surface water
      call plwidth( 0.0_plflt )
      call plcol0( 6 )
      call plmapfill( "ss/ss64ne_Water_Area", minx, maxx, miny, maxy )
      call plwidth( 2.0_plflt )
      call plmapfill( "ss/ss64ne_Water_Line", minx, maxx, miny, maxy )

      ! draw the roads, first with black and then thinner with colour to give an
      ! an outlined appearance
      call plwidth( 5.0_plflt )
      call plcol0( 1 )
      call plmapline( "ss/ss64ne_Road_Centreline", minx, maxx, miny, maxy )
      call plwidth( 3.0_plflt )
      call plcol0( 0 )
      call plmapline( "ss/ss64ne_Road_Centreline", minx, maxx, miny, maxy )
      call plcol0( 5 )
      call plmapline( "ss/ss64ne_Road_Centreline", minx, maxx, miny, maxy, majorroads )

      ! draw buildings
      call plwidth( 1.0_plflt )
      call plcol0( 1 )
      call plmapfill( "ss/ss64ne_Building_Area", minx, maxx, miny, maxy )

      ! labels
      call plsfci( int(z'00000100',kind=plint) )
      call plschr( 0._plflt, 0.8_plflt )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "MARTINHOE CP", minx, maxx, miny, maxy, 202 )
      call plschr( 0._plflt, 0.7_plflt );
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Heale\nDown", minx, maxx, miny, maxy, 13 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "South\nDown", minx, maxx, miny, maxy, 34 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Martinhoe\nCommon", minx, maxx, miny, maxy, 42 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Woody Bay", minx, maxx, miny, maxy, 211 )
      call plschr( 0._plflt, 0.6_plflt );
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Mill Wood", minx, maxx, miny, maxy, 16 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Heale Wood", minx, maxx, miny, maxy, 17 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 1.0_plflt, "Bodley", minx, maxx, miny, maxy, 31 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.0_plflt, "Martinhoe", minx, maxx, miny, maxy, 37 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, "Woolhanger\nCommon", minx, maxx, miny, maxy, 60 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, &
              "West Ilkerton\nCommon", minx, maxx, miny, maxy, 61 )
      call plmaptex( "ss/ss64ne_General_Text", 1.0_plflt, 0.0_plflt, 0.5_plflt, &
              "Caffyns\nHeanton\nDown", minx, maxx, miny, maxy, 62 )

      call plend()
      end program x19f
