!   Filling and clipping polygons.
!
!   Copyright (C) 2005 Arjen Markus
!   Copyright (C) 2008 Andrew Ross
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
!
!
! --------------------------------------------------------------------------
! main
!
! Test program for filling polygons and proper clipping
! --------------------------------------------------------------------------

program x25f

    use plplot
    implicit none

    integer          :: i, j, k
    integer          :: npts
    real(kind=plflt) :: xextreme(2,10)
    real(kind=plflt) :: yextreme(2,10)
    real(kind=plflt) :: x0(10)
    real(kind=plflt) :: y0(10)

    data ( xextreme(1,i), xextreme(2,i), yextreme(1,i), yextreme(2,i), i=1,9) / &
         -120.0_plflt,      120.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -120.0_plflt,      120.0_plflt,       20.0_plflt,      120.0_plflt, &
         -120.0_plflt,      120.0_plflt,      -20.0_plflt,      120.0_plflt, &
         -80.0_plflt,       80.0_plflt,      -20.0_plflt,      120.0_plflt, &
         -220.0_plflt,     -120.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -20.0_plflt,       20.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -20.0_plflt,       20.0_plflt,      -20.0_plflt,       20.0_plflt, &
         -80.0_plflt,       80.0_plflt,      -80.0_plflt,       80.0_plflt, &
         20.0_plflt,      120.0_plflt,     -120.0_plflt,      120.0_plflt/

    npts = 0

    !  Parse and process command line arguments

    call plparseopts(PL_PARSE_FULL)

    !  Initialize plplot

    call plssub(3,3)
    call plinit()

    do k = 1,2
        do j = 1,4

            select case ( j )
                 case ( 1 ) !  Polygon 1: a diamond
                     x0(1) =    0.0_plflt
                     y0(1) = -100.0_plflt
                     x0(2) = -100.0_plflt
                     y0(2) =    0.0_plflt
                     x0(3) =    0.0_plflt
                     y0(3) =  100.0_plflt
                     x0(4) =  100.0_plflt
                     y0(4) =    0.0_plflt
                     npts = 4

                  case( 2 )
                     !  Polygon 1: a diamond - reverse direction
                     x0(4) =    0.0_plflt
                     y0(4) = -100.0_plflt
                     x0(3) = -100.0_plflt
                     y0(3) =    0.0_plflt
                     x0(2) =    0.0_plflt
                     y0(2) =  100.0_plflt
                     x0(1) =  100.0_plflt
                     y0(1) =    0.0_plflt
                     npts = 4

                  case( 3 )
                     !  Polygon 2: a square with punctures
                     x0(1)  = -100.0_plflt
                     y0(1)  = -100.0_plflt
                     x0(2)  = -100.0_plflt
                     y0(2)  =  -80.0_plflt
                     x0(3)  =   80.0_plflt
                     y0(3)  =    0.0_plflt
                     x0(4)  = -100.0_plflt
                     y0(4)  =   80.0_plflt
                     x0(5)  = -100.0_plflt
                     y0(5)  =  100.0_plflt
                     x0(6)  =  -80.0_plflt
                     y0(6)  =  100.0_plflt
                     x0(7)  =    0.0_plflt
                     y0(7)  =   80.0_plflt
                     x0(8)  =   80.0_plflt
                     y0(8)  =  100.0_plflt
                     x0(9)  =  100.0_plflt
                     y0(9)  =  100.0_plflt
                     x0(10) =  100.0_plflt
                     y0(10) = -100.0_plflt
                     npts = 10

                 case( 4 )
                     !  Polygon 2: a square with punctures - reversed direction
                     x0(10) = -100.0_plflt
                     y0(10) = -100.0_plflt
                     x0(9)  = -100.0_plflt
                     y0(9)  =  -80.0_plflt
                     x0(8)  =   80.0_plflt
                     y0(8)  =    0.0_plflt
                     x0(7)  = -100.0_plflt
                     y0(7)  =   80.0_plflt
                     x0(6)  = -100.0_plflt
                     y0(6)  =  100.0_plflt
                     x0(5)  =  -80.0_plflt
                     y0(5)  =  100.0_plflt
                     x0(4)  =    0.0_plflt
                     y0(4)  =   80.0_plflt
                     x0(3)  =   80.0_plflt
                     y0(3)  =  100.0_plflt
                     x0(2)  =  100.0_plflt
                     y0(2)  =  100.0_plflt
                     x0(1)  =  100.0_plflt
                     y0(1)  = -100.0_plflt
                     npts = 10
            end select

            do i = 1,9
               call pladv(0)
               call plvsta()
               call plwind(xextreme(1,i), xextreme(2,i), yextreme(1,i), yextreme(2,i))

               call plcol0(2)
               call plbox('bc', 1.0d0, 0, 'bcnv', 10.0d0, 0)
               call plcol0(1)
               call plpsty(0)
               if(k.eq.1) then
                  call plfill(x0(1:npts),y0(1:npts))
               else
                  call plgradient(x0(1:npts),y0(1:npts),45.d0)
               endif
               call plcol0(2)
               call pllsty(1)
               call plline(x0(1:npts),y0(1:npts))
            end do
        end do
    end do

    !  Don't forget to call plend() to finish off!
    call plend

end program x25f
