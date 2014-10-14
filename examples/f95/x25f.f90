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

    integer(kind=plint)          :: i, j, k
    integer(kind=plint)          :: npts
    real(kind=plflt) :: xextreme(2_plint,10_plint)
    real(kind=plflt) :: yextreme(2_plint,10_plint)
    real(kind=plflt) :: x0(10_plint)
    real(kind=plflt) :: y0(10_plint)

    data ( xextreme(1_plint,i), xextreme(2_plint,i), yextreme(1_plint,i), yextreme(2_plint,i), i=1,9_plint) / &
         -120.0_plflt,      120.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -120.0_plflt,      120.0_plflt,       20.0_plflt,      120.0_plflt, &
         -120.0_plflt,      120.0_plflt,      -20.0_plflt,      120.0_plflt, &
         -80.0_plflt,       80.0_plflt,      -20.0_plflt,      120.0_plflt, &
         -220.0_plflt,     -120.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -20.0_plflt,       20.0_plflt,     -120.0_plflt,      120.0_plflt, &
         -20.0_plflt,       20.0_plflt,      -20.0_plflt,       20.0_plflt, &
         -80.0_plflt,       80.0_plflt,      -80.0_plflt,       80.0_plflt, &
         20.0_plflt,      120.0_plflt,     -120.0_plflt,      120.0_plflt/

    npts = 0_plint

    !  Parse and process command line arguments

    call plparseopts(PL_PARSE_FULL)

    !  Initialize plplot

    call plssub(3_plint,3_plint)
    call plinit()

    do k = 1_plint,2
        do j = 1_plint,4

            select case ( j )
                 case ( 1_plint ) !  Polygon 1: a diamond
                     x0(1_plint) =    0.0_plflt
                     y0(1_plint) = -100.0_plflt
                     x0(2_plint) = -100.0_plflt
                     y0(2_plint) =    0.0_plflt
                     x0(3_plint) =    0.0_plflt
                     y0(3_plint) =  100.0_plflt
                     x0(4_plint) =  100.0_plflt
                     y0(4_plint) =    0.0_plflt
                     npts = 4_plint

                  case( 2_plint )
                     !  Polygon 1: a diamond - reverse direction
                     x0(4_plint) =    0.0_plflt
                     y0(4_plint) = -100.0_plflt
                     x0(3_plint) = -100.0_plflt
                     y0(3_plint) =    0.0_plflt
                     x0(2_plint) =    0.0_plflt
                     y0(2_plint) =  100.0_plflt
                     x0(1_plint) =  100.0_plflt
                     y0(1_plint) =    0.0_plflt
                     npts = 4_plint

                  case( 3_plint )
                     !  Polygon 2: a square with punctures
                     x0(1_plint)  = -100.0_plflt
                     y0(1_plint)  = -100.0_plflt
                     x0(2_plint)  = -100.0_plflt
                     y0(2_plint)  =  -80.0_plflt
                     x0(3_plint)  =   80.0_plflt
                     y0(3_plint)  =    0.0_plflt
                     x0(4_plint)  = -100.0_plflt
                     y0(4_plint)  =   80.0_plflt
                     x0(5_plint)  = -100.0_plflt
                     y0(5_plint)  =  100.0_plflt
                     x0(6_plint)  =  -80.0_plflt
                     y0(6_plint)  =  100.0_plflt
                     x0(7_plint)  =    0.0_plflt
                     y0(7_plint)  =   80.0_plflt
                     x0(8_plint)  =   80.0_plflt
                     y0(8_plint)  =  100.0_plflt
                     x0(9_plint)  =  100.0_plflt
                     y0(9_plint)  =  100.0_plflt
                     x0(10_plint) =  100.0_plflt
                     y0(10_plint) = -100.0_plflt
                     npts = 10_plint

                 case( 4_plint )
                     !  Polygon 2: a square with punctures - reversed direction
                     x0(10_plint) = -100.0_plflt
                     y0(10_plint) = -100.0_plflt
                     x0(9_plint)  = -100.0_plflt
                     y0(9_plint)  =  -80.0_plflt
                     x0(8_plint)  =   80.0_plflt
                     y0(8_plint)  =    0.0_plflt
                     x0(7_plint)  = -100.0_plflt
                     y0(7_plint)  =   80.0_plflt
                     x0(6_plint)  = -100.0_plflt
                     y0(6_plint)  =  100.0_plflt
                     x0(5_plint)  =  -80.0_plflt
                     y0(5_plint)  =  100.0_plflt
                     x0(4_plint)  =    0.0_plflt
                     y0(4_plint)  =   80.0_plflt
                     x0(3_plint)  =   80.0_plflt
                     y0(3_plint)  =  100.0_plflt
                     x0(2_plint)  =  100.0_plflt
                     y0(2_plint)  =  100.0_plflt
                     x0(1_plint)  =  100.0_plflt
                     y0(1_plint)  = -100.0_plflt
                     npts = 10_plint
            end select

            do i = 1_plint,9
               call pladv(0_plint)
               call plvsta()
               call plwind(xextreme(1_plint,i), xextreme(2_plint,i), yextreme(1_plint,i), yextreme(2_plint,i))

               call plcol0(2_plint)
               call plbox('bc', 1.0d0, 0_plint, 'bcnv', 10.0d0, 0_plint)
               call plcol0(1_plint)
               call plpsty(0_plint)
               if(k.eq.1) then
                  call plfill(x0(1:npts),y0(1:npts))
               else
                  call plgradient(x0(1:npts),y0(1:npts),45.d0)
               endif
               call plcol0(2_plint)
               call pllsty(1_plint)
               call plline(x0(1:npts),y0(1:npts))
            end do
        end do
    end do

    !  Don't forget to call plend() to finish off!
    call plend

end program x25f
