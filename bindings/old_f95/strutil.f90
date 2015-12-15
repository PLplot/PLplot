! Copyright (C) 2004-2014 Alan W. Irwin
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

module plplot_strutils
    use plplot_types
    implicit none

contains

!--------------------------------------------------------------------------
! plstrf2c()
!
! Converts Fortran string to C format
!--------------------------------------------------------------------------

subroutine plstrf2c(string1, string2)
    character(len=*) :: string1, string2

    integer(kind=plint) :: limit

    string2 = string1
!    limit = min(len(string2), len(string1))
    limit = len_trim(string2)
    string2(limit+1:limit+1) = char(0)
end subroutine plstrf2c

!--------------------------------------------------------------------------
! plstrf2c_esc()
!
! Converts Fortran string to C format, substituting a line feed for \n
!--------------------------------------------------------------------------

subroutine plstrf2c_esc(string1, string2)
    character(len=*) :: string1, string2

    integer(kind=plint) :: limit
    integer             :: k

    string2 = string1
!    limit = min(len(string2), len(string1))
    limit = len_trim(string2)
    string2(limit+1:limit+1) = char(0)

    do
        k = index( string2, '\n' )
        if ( k > 0 ) then
            string2 = string2(1:k-1) // char(10) // string2(k+2:)
        else
            exit
        endif
    enddo
end subroutine plstrf2c_esc


!--------------------------------------------------------------------------
! plstrc2f()
!
! Converts C string to Fortran conventions
!--------------------------------------------------------------------------

subroutine plstrc2f(string1, string2)
    use plplot_types
    character(len=*) :: string1, string2

    integer(kind=plint) :: limit

    string2 = string1

    limit = index( string2, achar(0) )
    if ( limit > 0 ) then
        string2(limit:) = ' '
    endif

end subroutine plstrc2f

end module plplot_strutils
