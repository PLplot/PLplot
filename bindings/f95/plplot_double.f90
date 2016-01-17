!***********************************************************************
!  plplot_double.f90
!
!  Copyright (C) 2005-2016  Arjen Markus
!  Copyright (C) 2006-2016 Alan W. Irwin
!
!  This file is part of PLplot.
!
!  PLplot is free software; you can redistribute it and/or modify
!  it under the terms of the GNU Library General Public License as published
!  by the Free Software Foundation; either version 2 of the License, or
!  (at your option) any later version.
!
!  PLplot is distributed in the hope that it will be useful,
!  but WITHOUT ANY WARRANTY; without even the implied warranty of
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!  GNU Library General Public License for more details.
!
!  You should have received a copy of the GNU Library General Public License
!  along with PLplot; if not, write to the Free Software
!  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!
!***********************************************************************

module plplot_double
    use iso_c_binding, only: c_ptr, c_char, c_null_char, c_null_ptr, c_loc, c_funptr, c_null_funptr, c_funloc
    use iso_fortran_env, only: error_unit
    use plplot_types, only: private_plflt, private_plint, private_plbool, private_double, PLcGrid, PLfGrid
    use plplot_private_exposed
    use plplot_private_utilities, only: character_array_to_c
    implicit none

    integer, parameter :: wp = private_double
    private :: c_ptr, c_char, c_null_char, c_null_ptr, c_loc, c_funptr, c_null_funptr, c_funloc
    private :: error_unit
    private :: private_plflt, private_plint, private_plbool, private_double, PLcGrid, PLfGrid
    private :: character_array_to_c
    private :: wp

! Interfaces for wp-precision callbacks

    abstract interface
        subroutine plmapform_proc_double( x, y )
            import :: wp
            real(kind=wp), dimension(:), intent(inout) :: x, y
        end subroutine plmapform_proc_double
    end interface
    procedure(plmapform_proc_double), pointer :: plmapform_double

    abstract interface
        subroutine pllabeler_proc_double( axis, value, label )
            import :: wp
            integer, intent(in) :: axis
            real(kind=wp), intent(in) :: value
            character(len=*), intent(out) :: label
        end subroutine pllabeler_proc_double
    end interface
    procedure(pllabeler_proc_double), pointer :: pllabeler_double

    abstract interface
        subroutine pllabeler_proc_data_double( axis, value, label, data )
            import :: wp, c_ptr
            integer, intent(in) :: axis
            real(kind=wp), intent(in) :: value
            character(len=*), intent(out) :: label
            type(c_ptr), intent(in) :: data
        end subroutine pllabeler_proc_data_double
    end interface
    procedure(pllabeler_proc_data_double), pointer :: pllabeler_data_double

    abstract interface
        subroutine pltransform_proc_double( x, y, tx, ty )
            import :: wp
            real(kind=wp), intent(in) :: x, y
            real(kind=wp), intent(out) :: tx, ty
        end subroutine pltransform_proc_double
    end interface
    procedure(pltransform_proc_double), pointer :: pltransform_double

    abstract interface
        subroutine pltransform_proc_data_double( x, y, tx, ty, data )
            import :: wp, c_ptr
            real(kind=wp), intent(in) :: x, y
            real(kind=wp), intent(out) :: tx, ty
            type(c_ptr), intent(in) :: data
        end subroutine pltransform_proc_data_double
    end interface
    procedure(pltransform_proc_data_double), pointer :: pltransform_data_double

    include 'included_plplot_real_interfaces.f90'

! plflt-precision callback routines that are called from C and which wrap a call to wp-precision Fortran routines.

  subroutine plmapformf2c_double( n, x, y ) bind(c, name = 'plplot_private_plmapform2c_double')
        integer(kind=private_plint), value, intent(in) :: n
        real(kind=private_plflt), dimension(n), intent(inout) :: x, y

        real(kind=wp), dimension(:), allocatable :: x_inout, y_inout

        allocate(x_inout(n), y_inout(n))

        x_inout = real(x, kind=wp)
        y_inout = real(y, kind=wp)

        call plmapform_double( x_inout, y_inout )
        x = real(x_inout, kind=private_plflt)
        y = real(y_inout, kind=private_plflt)
    end subroutine plmapformf2c_double

    subroutine pllabelerf2c_double( axis, value, label, length, data ) bind(c, name = 'plplot_private_pllabeler2c_double')
      integer(kind=private_plint), value, intent(in) :: axis, length
      real(kind=private_plflt), value, intent(in) :: value
      character(len=1), dimension(*), intent(out) :: label
      type(c_ptr), intent(in) :: data

      character(len=:), allocatable :: label_out
      integer :: trimmed_length

      allocate(character(length) :: label_out)
      call pllabeler_double( int(axis), real(value,kind=wp), label_out )
      trimmed_length = min(length,len_trim(label_out) + 1)
      label(1:trimmed_length) = transfer(trim(label_out(1:length))//c_null_char, " ", trimmed_length)
      deallocate(label_out)
    end subroutine pllabelerf2c_double

    subroutine pllabelerf2c_data_double( axis, value, label, length, data ) bind(c, name = 'plplot_private_pllabeler2c_data_double')
      integer(kind=private_plint), value, intent(in) :: axis, length
      real(kind=private_plflt), value, intent(in) :: value
      character(len=1), dimension(*), intent(out) :: label
      type(c_ptr), intent(in) :: data

      character(len=:), allocatable :: label_out
      allocate(character(length) :: label_out)

      call pllabeler_data_double( int(axis), real(value,kind=wp), label_out, data )
      label(1:length) = trim(label_out)//c_null_char

      deallocate(label_out)
    end subroutine pllabelerf2c_data_double

  subroutine pltransformf2c_double( x, y, tx, ty, data ) bind(c, name = 'plplot_private_pltransform2c_double')
        real(kind=private_plflt), value, intent(in) :: x, y
        real(kind=private_plflt), intent(out) :: tx, ty
        type(c_ptr), value, intent(in) :: data

        real(kind=wp) :: tx_out, ty_out

        call pltransform_double( real(x,kind=wp), real(y,kind=wp), tx_out, ty_out )
        tx = tx_out
        ty = ty_out
    end subroutine pltransformf2c_double

  subroutine pltransformf2c_data_double( x, y, tx, ty, data ) bind(c, name = 'plplot_private_pltransform2c_data_double')
        real(kind=private_plflt), value, intent(in) :: x, y
        real(kind=private_plflt), intent(out) :: tx, ty
        type(c_ptr), value, intent(in) :: data

        real(kind=wp) :: tx_out, ty_out

        call pltransform_data_double( real(x,kind=wp), real(y,kind=wp), tx_out, ty_out, data )
        tx = tx_out
        ty = ty_out
    end subroutine pltransformf2c_data_double

end module plplot_double
