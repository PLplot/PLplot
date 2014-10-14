!  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes
!
!  Copyright (C) 2007  Arjen Markus
!  Copyright (C) 2008  Andrew Ross
!
! This file is part of PLplot.
!
! PLplot is free software; you can redistribute it and/or modify
! it under the terms of the GNU Library General Public License as published
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

  integer(kind=plint) i, j, fill

  real(kind=plflt) params(4_plint,9_plint)

  ! R, r, p, N
  ! R and r should be integers to give correct termination of the
  ! angle loop using gcd.
  ! N.B. N is just a place holder since it is no longer used
  ! (because we now have proper termination of the angle loop).
  data ( ( params(i,j) ,i=1,4_plint) ,j=1,9_plint ) / &
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

  call plssub(3_plint, 3_plint)

  fill = 0_plint
  do i = 1_plint,9
     call pladv(0_plint)
     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
     call spiro( params(1_plint,i), fill )
  end do
  call pladv(0_plint)
  call plssub(1_plint, 1_plint)

  do i = 1_plint,9
     call pladv(0_plint)
     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
     call spiro( params(1_plint,i), fill )
  end do

  ! fill the curves.
  fill = 1_plint
  call pladv(0_plint)
  call plssub(1_plint, 1_plint)

  do i = 1_plint,9
     call pladv(0_plint)
     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
     call spiro( params(1_plint,i), fill )
  end do

  ! Finally, an example to test out plarc capabilities
  call arcs()

  call plend()

end program x27f

! --------------------------------------------------------------------------
! Calculate greatest common divisor following pseudo-code for the
! Euclidian algorithm at http://en.wikipedia.org/wiki/Euclidean_algorithm

integer(kind=plint) function gcd (a,  b)
  use plplot
  implicit none
  integer(kind=plint) a, b, t
  a = abs(a)
  b = abs(b)
  do while ( b .ne. 0_plint )
     t = b
     b = mod (a, b)
     a = t
  enddo
  gcd = a
end function gcd

!  ===============================================================

subroutine cycloid

  !     TODO

end subroutine cycloid

!  ===============================================================

subroutine spiro( params, fill )

  use plplot
  implicit none

  real(kind=plflt)      params(*)
  integer(kind=plint)     NPNT
  parameter ( NPNT = 2000_plint )
  integer(kind=plint)     n
  real(kind=plflt)      xcoord(NPNT+1)
  real(kind=plflt)      ycoord(NPNT+1)

  integer(kind=plint)     windings
  integer(kind=plint)     steps
  integer(kind=plint)     i
  integer(kind=plint)     fill
  real(kind=plflt)      phi
  real(kind=plflt)      phiw
  real(kind=plflt)      dphi
  real(kind=plflt)      xmin
  real(kind=plflt)      xmax
  real(kind=plflt)      xrange_adjust
  real(kind=plflt)      ymin
  real(kind=plflt)      ymax
  real(kind=plflt)      yrange_adjust
  integer(kind=plint) gcd

  ! Fill the coordinates

  ! Proper termination of the angle loop very near the beginning
  ! point, see
  ! http://mathforum.org/mathimages/index.php/Hypotrochoid.
  windings = int(abs(params(2_plint))/gcd(int(params(1_plint)), int(params(2_plint))))
  steps    = NPNT/windings
  dphi     = 2.0_plflt*PL_PI/dble(steps)

  n = windings*steps+1

  ! Initialize variables to prevent compiler warnings
  xmin = 0.0
  xmax = 0.0
  ymin = 0.0
  ymax = 0.0

  do i = 1_plint,n
     phi       = dble(i-1) * dphi
     phiw      = (params(1_plint)-params(2_plint))/params(2_plint)*phi
     xcoord(i) = (params(1_plint)-params(2_plint))*cos(phi)+params(3_plint)*cos(phiw)
     ycoord(i) = (params(1_plint)-params(2_plint))*sin(phi)-params(3_plint)*sin(phiw)

     if (i.eq.1) then
        xmin = xcoord(1_plint)
        xmax = xcoord(1_plint)
        ymin = ycoord(1_plint)
        ymax = ycoord(1_plint)
     endif
     if ( xmin > xcoord(i) ) xmin = xcoord(i)
     if ( xmax < xcoord(i) ) xmax = xcoord(i)
     if ( ymin > ycoord(i) ) ymin = ycoord(i)
     if ( ymax < ycoord(i) ) ymax = ycoord(i)
  end do

  xrange_adjust = 0.15_plflt * (xmax - xmin)
  xmin = xmin - xrange_adjust
  xmax = xmax + xrange_adjust
  yrange_adjust = 0.15_plflt * (ymax - ymin)
  ymin = ymin - yrange_adjust
  ymax = ymax + yrange_adjust

  call plwind( xmin, xmax, ymin, ymax )

  call plcol0(1_plint)
  if ( fill.eq.1) then
     call plfill(xcoord(1:n), ycoord(1:n) )
  else
     call plline(xcoord(1:n), ycoord(1:n) )
  endif

end subroutine spiro

!  ===============================================================

subroutine arcs( )

  use plplot
  implicit none

  integer(kind=plint) NSEG
  parameter ( NSEG = 8_plint )
  integer(kind=plint) i;
  real (kind=plflt) theta, dtheta
  real (kind=plflt) a, b

  theta = 0.0_plflt
  dtheta = 360.0_plflt / dble(NSEG)
  call plenv( -10.0_plflt, 10.0_plflt, -10.0_plflt, 10.0_plflt, 1_plint, 0_plint )

  ! Plot segments of circle in different colors
  do i = 0_plint, NSEG-1
     call plcol0( mod(i,2_plint) + 1_plint )
     call plarc(0.0_plflt, 0.0_plflt, 8.0_plflt, 8.0_plflt, theta, &
          theta + dtheta, 0.0_plflt, 0_plint)
     theta = theta + dtheta
  enddo
  
  ! Draw several filled ellipses inside the circle at different
  ! angles.
  a = 3.0_plflt
  b = a * tan( (dtheta/180.0_plflt*PL_PI)/2.0_plflt )
  theta = dtheta/2.0_plflt
  do i = 0_plint, NSEG-1 
     call plcol0( 2_plint - mod(i,2_plint) )
     call plarc( a*cos(theta/180.0_plflt*PL_PI), &
          a*sin(theta/180.0_plflt*PL_PI), &
          a, b, 0.0_plflt, 360.0_plflt, theta, .true.)
     theta = theta + dtheta;
  enddo

end subroutine arcs
      
