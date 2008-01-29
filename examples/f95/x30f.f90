!  $Id$
!
!   Alpha color values demonstration
!
!   Copyright (C) 2008 Hazen Babcock
!   Copyright (C) 2008 Andrew Ross
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

   program x30f95
   use plplot

   implicit none

   integer, dimension(4) ::  red, green, blue
   real(kind=plflt), dimension (4) :: alpha, px, py
   real(kind=plflt), dimension (2) :: pos, rcoord, gcoord, bcoord, acoord
   logical, dimension(2) :: rev

   data red   / 0, 255, 0, 0 /
   data green / 0, 0, 255, 0 /
   data blue  / 0, 0, 0, 255 /
   data alpha / 1.0_plflt, 1.0_plflt, 1.0_plflt, 1.0_plflt /
   data px / 0.1_plflt, 0.5_plflt, 0.5_plflt, 0.1_plflt /
   data py / 0.1_plflt, 0.1_plflt, 0.5_plflt, 0.5_plflt /
   data pos / 0.0_plflt, 1.0_plflt / 
   data rcoord / 1.0_plflt, 1.0_plflt / 
   data gcoord / 0.0_plflt, 0.0_plflt / 
   data bcoord / 0.0_plflt, 0.0_plflt / 
   data acoord / 0.0_plflt, 1.0_plflt /
   data rev / .false. , .false. /

   integer i, j
   integer icol, r, g, b
   real(kind=plflt) :: a, clevel(101), z(2,2)

   character*1 defined

   call plparseopts(PL_PARSE_FULL)
   
   call plinit()

   call plscmap0n(4)

   call plscmap0a (red, green, blue, alpha)

   ! 
   ! Page 1:
   !
   ! This is a series of red, green and blue rectangles overlaid
   ! on each other with gradually increasing transparency.
   !

   ! Set up the window
   call pladv (0)
   call plvpor (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
   call plwind (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
   call plcol0 (0)
   call plbox ("", 1.0_plflt, 0, "", 1.0_plflt, 0)
   
   ! Draw the boxes 
   do i = 1,9
      icol = mod(i-1,3) + 1
      
      ! Get a color, change its transparency and 
      ! set it as the current color.
      call plgcol0a (icol, r, g, b, a)
      call plscol0a (icol, r, g, b, 1.0_plflt - dble(i-1)/9.0_plflt)
      call plcol0 (icol)
      
      ! Draw the rectangle
      call plfill (px, py)

      ! Shift the rectangles coordinates
      do j = 1,4
         px(j) = px(j) + 0.5_plflt/9.0_plflt
         py(j) = py(j) + 0.5_plflt/9.0_plflt
      enddo
   enddo
   !
   ! Page 2:
   !
   ! This is a bunch of boxes colored red, green or blue with a single 
   ! large (red) box of linearly varying transparency overlaid. The
   ! overlaid box is completely transparent at the bottom and completely
   ! opaque at the top.
   ! 

   ! Set up the window
   call pladv(0)
   call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
   call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)

   ! Draw the boxes. There are 25 of them drawn on a 5 x 5 grid.
   do i = 1,5
      ! Set box X position
      px(1) = 0.05_plflt + 0.2_plflt * dble(i-1)
      px(2) = px(1) + 0.1_plflt
      px(3) = px(2)
      px(4) = px(1)
      
      ! We don't want the boxes to be transparent, so since we changed
      ! the colors transparencies in the first example we have to change
      ! the transparencies back to completely opaque.
      icol = mod(i-1,3) + 1
      call plgcol0a (icol, r, g, b, a)
      call plscol0a (icol, r, g, b, 1.0_plflt);
      call plcol0 (icol)
      do j = 1, 5
         ! Set box y position and draw the box.
         py(1) = 0.05_plflt + 0.2_plflt * dble(j-1)
         py(2) = py(1)
         py(3) = py(1) + 0.1_plflt
         py(4) = py(3)
         call plfill(px, py)
      enddo
   enddo
   
   ! The overlaid box is drawn using plshades with a color map that is
   ! the same color but has a linearly varying transparency.

   ! Create the color map with 128 colors and call plscmap1la to initialize
   ! the color values with a linear varying transparency (or alpha)
   call plscmap1n(128)
   call plscmap1la(.true., pos, rcoord, gcoord, bcoord, acoord, rev)
   
   ! Create a 2 x 2 array that contains the z values (0.0 to 1.0) that will
   ! used for the shade plot. plshades uses linear interpolation to
   ! calculate the z values of all the intermediate points in this array.
   z(1,1) = 0.0_plflt
   z(2,1) = 0.0_plflt
   z(1,2) = 1.0_plflt
   z(2,2) = 1.0_plflt

   ! Set the color levels array. These levels are also between 0.0 and 1.0
   do i = 1,101 
      clevel(i) = 0.01_plflt * dble(i-1)
   enddo

   ! Draw the shade plot with zmin = 0.0, zmax = 1.0 and x and y coordinate ranges
   ! such that it fills the entire plotting area.
   call plshades(z, defined, 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt, &
        clevel, 0, -1, 2)

   call plend()

   end program x30f95

