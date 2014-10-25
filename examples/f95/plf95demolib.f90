!      Demonstrates multiple windows and color map 0
!
!      Copyright (C) 2011  Arjen Markus
!      Copyright (C) 2005  Andrew Ross
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

!      Module containing auxiliary routines for the demo programs

module plf95demolib
    use plplot
    implicit none

    interface arange
        module procedure arange_plflt_1
        module procedure arange_plflt_2
        module procedure arange_plflt_3
    end interface

contains

! Function to return an arithmetic series (modelled after the Numpy
! function of the same name)
!
function arange_plflt_1( start, stop, step )
    integer, intent(in) :: start, stop, step

    real(kind=plflt), dimension((stop-start)/step) :: arange_plflt_1

    integer             :: i

    arange_plflt_1 = (/ (real(i, plflt), i = start, stop-step, step) /)
end function arange_plflt_1

function arange_plflt_2( start, stop )
    integer, intent(in) :: start, stop

    real(kind=plflt), dimension((stop-start)) :: arange_plflt_2

    arange_plflt_2 = arange_plflt_1( start, stop, 1 )
end function arange_plflt_2

function arange_plflt_3( stop )
    integer, intent(in) :: stop

    real(kind=plflt), dimension(stop-1) :: arange_plflt_3

    arange_plflt_3 = arange_plflt_1( 0, stop, 1 )
end function arange_plflt_3

end module plf95demolib
