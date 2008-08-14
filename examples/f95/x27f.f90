! $Id:$
!
!  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes
!
!  Copyright (C) 2007  Arjen Markus
!  Copyright (C) 2008  Andrew Ross
!
! This file is part of PLplot.
!
! PLplot is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Library Public License as published
! by the Free Software Foundation; either version 2 of the License, or
! (at your option) any later version.
!
! PLplot is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU Library General Public License for more details.
!
! You should have received a copy of the GNU Library General Public License
! along with PLplot; if not, write to the Free Software
! Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!

! --------------------------------------------------------------------------
! main
!
! Generates two kinds of plots:
!   - construction of a cycloid (animated)
!   - series of epitrochoids and hypotrochoids
! --------------------------------------------------------------------------

program x27f

  use plplot
  implicit none

  !     R, r, p, N

  real(kind=plflt) params(4,9)

  integer i, j

  data ( ( params(i,j) ,i=1,4) ,j=1,9 ) / &
    21.0_plflt,  7.0_plflt,  7.0_plflt,  3.0_plflt, &
    21.0_plflt,  7.0_plflt, 10.0_plflt,  3.0_plflt, &
    21.0_plflt, -7.0_plflt, 10.0_plflt,  3.0_plflt, &
    20.0_plflt,  3.0_plflt,  7.0_plflt, 20.0_plflt, &
    20.0_plflt,  3.0_plflt, 10.0_plflt, 20.0_plflt, &
    20.0_plflt, -3.0_plflt, 10.0_plflt, 20.0_plflt, &
    20.0_plflt, 13.0_plflt,  7.0_plflt, 20.0_plflt, &
    20.0_plflt, 13.0_plflt, 20.0_plflt, 20.0_plflt, &
    20.0_plflt,-13.0_plflt, 20.0_plflt, 20.0_plflt/

  !  plplot initialization

  !  Parse and process command line arguments

  call plparseopts(PL_PARSE_FULL)

  !  Initialize plplot

  call plinit()

  !  Illustrate the construction of a cycloid

  call cycloid()

  !  Loop over the various curves
  !  First an overview, then all curves one by one

  call plssub(3, 3)

  do i = 1,9
     call pladv(0)
     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
     call spiro( params(1,i) )
  end do
  call pladv(0)
  call plssub(1, 1)

  do i = 1,9
     call pladv(0)
     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
     call spiro( params(1,i) )
  end do
  !  Don't forget to call plend() to finish off!

  call plend()

end program x27f

!  ===============================================================

subroutine cycloid

  !     TODO

end subroutine cycloid

!  ===============================================================

subroutine spiro( params )

  use plplot
  implicit none

  real(kind=plflt)      params(*)
  integer     NPNT
  parameter ( NPNT = 20000 )
  integer     n
  real(kind=plflt)      xcoord(NPNT+1)
  real(kind=plflt)      ycoord(NPNT+1)

  integer     windings
  integer     steps
  integer     i
  real(kind=plflt)      phi
  real(kind=plflt)      phiw
  real(kind=plflt)      dphi
  real(kind=plflt)      xmin
  real(kind=plflt)      xmax
  real(kind=plflt)      ymin
  real(kind=plflt)      ymax
  real(kind=plflt)      scale

  !     Fill the coordinates

  windings = int(params(4))
  steps    = NPNT/windings
  dphi     = 8.0_plflt*acos(-1.0_plflt)/dble(steps)

  !     This initialisation is safe!
  xmin = 0.0_plflt
  xmax = 0.0_plflt
  ymin = 0.0_plflt
  ymax = 0.0_plflt

  n = windings*steps+1

  do i = 1,n
     phi       = dble(i-1) * dphi
     phiw      = (params(1)-params(2))/params(2)*phi
     xcoord(i) = (params(1)-params(2))*cos(phi)+params(3)*cos(phiw)
     ycoord(i) = (params(1)-params(2))*sin(phi)-params(3)*sin(phiw)

     if ( xmin > xcoord(i) ) xmin = xcoord(i)
     if ( xmax < xcoord(i) ) xmax = xcoord(i)
     if ( ymin > ycoord(i) ) ymin = ycoord(i)
     if ( ymax < ycoord(i) ) ymax = ycoord(i)
  end do

  if ( xmax-xmin > ymax-ymin ) then
     scale = xmax - xmin
  else
     scale = ymax - ymin
  endif
  xmin = - 0.65_plflt * scale
  xmax =   0.65_plflt * scale
  ymin = - 0.65_plflt * scale
  ymax =   0.65_plflt * scale

  call plwind( xmin, xmax, ymin, ymax )

  call plcol0(1)
  call plline(xcoord(1:n), ycoord(1:n) )

end subroutine spiro
