!***********************************************************************
!  plplot_binding.f90
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
!  This file is a limited version of what should become the new style
!  Fortran bindings. It is geared to example x00f only.
!
!***********************************************************************

module plplot_types
  use iso_c_binding, only: c_ptr
  include 'included_plplot_interface_private_types.f90'

  ! The PLfGrid and PLcGrid types transfer information about a multidimensional
  ! array to the plcontour/plshade family of routines.

  type, bind(c) :: PLfGrid
     type(c_ptr) :: f
     integer(kind=private_plint) :: nx, ny, nz
  end type PLfGrid

  type, bind(c) :: PLcGrid
     type(c_ptr) :: xg, yg, zg
     integer(kind=private_plint) :: nx, ny, nz
  end type PLcGrid
end module plplot_types

module plplot_graphics
  use plplot_types, only: private_single, private_double
  use iso_c_binding, only: c_ptr
  implicit none
  private :: private_single, private_double
    
  ! Some references say to use sequence for these derived data types
  ! that are going to be passed to C, but sequence was not used
  ! in the old Fortran binding so I will continue that here.

  ! N.B. use naming convention that double-precision entity has no
  ! special suffix and single-precision entity has the "_single" suffix.
  type :: PLGraphicsIn
     integer type                     ! of event (CURRENTLY UNUSED)
     integer state                    ! key or button mask
     integer  keysym                  ! key selected
     integer  button                  ! mouse button selected
     integer subwindow                ! subwindow (alias subpage, alias subplot) number
     character(len=16) string         ! translated string
     integer pX, pY                   ! absolute device coordinates of pointer
     real(kind=private_double) dX, dY ! relative device coordinates of pointer
     real(kind=private_double) wX, wY ! world coordinates of pointer
  end type PLGraphicsIn

  type :: PLGraphicsIn_single
     integer type                     ! of event (CURRENTLY UNUSED)
     integer state                    ! key or button mask
     integer  keysym                  ! key selected
     integer  button                  ! mouse button selected
     integer subwindow                ! subwindow (alias subpage, alias subplot) number
     character(len=16) string         ! translated string
     integer pX, pY                   ! absolute device coordinates of pointer
     real(kind=private_single) dX, dY ! relative device coordinates of pointer
     real(kind=private_single) wX, wY ! world coordinates of pointer
  end type PLGraphicsIn_single
  
  ! Need to define two versions of plGetCursor (one with a
  ! PLGraphicsIn argument, one with a PLGraphicsIn_single
  ! argument).  There may be a less bulky way to do it (as with
  ! included_plplot_real_interfaces.f90 for the wp type), but I could
  ! not figure out a similar method for derived types.

  interface plGetCursor
     module procedure plGetCursor_double
     module procedure plGetCursor_single
  end interface plGetCursor
  private :: plGetCursor_double, plGetCursor_single

  contains

subroutine plGetCursor_double( gin )
  type(PLGraphicsIn), intent(out) :: gin

  type(c_ptr) :: gin_out

  ! FIXME: still need to work out details of copying gin_out back to Fortran gin argument
  interface
     subroutine interface_plGetCursor_double( gin ) bind(c,name='plGetCursor')
       use iso_c_binding, only:  c_ptr
       implicit none
       type(c_ptr), intent(out) :: gin
     end subroutine interface_plGetCursor_double
  end interface
  call interface_plGetCursor_double( gin_out )
end subroutine plGetCursor_double

subroutine plGetCursor_single( gin )
  type(PLGraphicsIn_single), intent(out) :: gin

  type(c_ptr) :: gin_out

  ! FIXME: still need to work out details of copying gin_out back to Fortran gin argument
  interface
     subroutine interface_plGetCursor_single( gin ) bind(c,name='plGetCursor')
       use iso_c_binding, only: c_ptr
       implicit none
       type(c_ptr), intent(out) :: gin
     end subroutine interface_plGetCursor_single
  end interface
  call interface_plGetCursor_single( gin_out )
end subroutine plGetCursor_single

end module plplot_graphics

! The bind(c) attribute exposes the pltr routine which ought to be private
module plplot_private_exposed
    implicit none
contains
subroutine plplot_private_pltr( x, y, tx, ty, tr ) bind(c)
   use iso_c_binding
   use plplot_types
   real(kind=private_plflt), value, intent(in) :: x, y
   real(kind=private_plflt), intent(out) :: tx, ty
   real(kind=private_plflt), dimension(*), intent(in) :: tr

   tx = tr(1) * x + tr(2) * y + tr(3)
   ty = tr(4) * x + tr(5) * y + tr(6)
end subroutine plplot_private_pltr
end module plplot_private_exposed

module plplot_single
    use iso_c_binding, only: c_ptr, c_null_char, c_null_ptr, c_loc, c_null_ptr, c_null_funptr, c_funloc, c_f_pointer
    use plplot_types, only: private_plflt, private_plint, private_single, PLcGrid, PLfGrid
    use plplot_private_exposed
    implicit none

    integer, parameter :: wp = private_single
    private :: wp, private_plflt, private_plint, private_single

    include 'included_plplot_real_interfaces.f90'
end module plplot_single

module plplot_double
    use iso_c_binding, only: c_ptr, c_null_char, c_null_ptr, c_loc, c_null_ptr, c_null_funptr, c_funloc, c_f_pointer
    use plplot_types, only: private_plflt, private_plint, private_double, PLcGrid, PLfGrid
    use plplot_private_exposed
    implicit none

    integer, parameter :: wp = private_double
    private :: wp, private_plflt, private_plint, private_double

    include 'included_plplot_real_interfaces.f90'

end module plplot_double

module plplot
    use plplot_single
    use plplot_double
    use plplot_types, only: private_plflt, private_plint, private_plunicode, private_single, private_double
    use plplot_graphics
    implicit none
    ! For backwards compatibility define plflt, but use of this
    ! parameter is deprecated since any real precision should work
    ! for users so long as the precision of the real arguments
    ! of a given call to a PLplot routine are identical.
    integer, parameter :: plflt = private_plflt
    integer(kind=private_plint), parameter :: maxlen = 320
    character(len=1), parameter :: PL_END_OF_STRING = achar(0)
    include 'included_plplot_parameters.f90'
    private :: private_plflt, private_plint, private_plunicode, private_single, private_double
    private :: copystring, maxlen
!
    ! Interfaces that do not depend on the real kind or which
    ! have optional real components (e.g., plsvect) that generate
    ! an ambiguous interface if implemented with plplot_single and plplot_double
!
    interface pl_setcontlabelformat
        module procedure pl_setcontlabelformat_impl
    end interface pl_setcontlabelformat
    private :: pl_setcontlabelformat_impl

    interface plgfci
        module procedure plgfci_impl
    end interface plgfci
    private :: plgfci_impl

    interface plscmap1
       module procedure plscmap1_impl
    end interface plscmap1
    private :: plscmap1_impl

    interface plsetopt
        module procedure plsetopt_impl
    end interface plsetopt
    private :: plsetopt_impl

    interface plsfci
        module procedure plsfci_impl
    end interface plsfci
    private :: plsfci_impl

    interface plstyl
        module procedure plstyl_scalar
        module procedure plstyl_n_array
        module procedure plstyl_array
    end interface plstyl
    private :: plstyl_scalar, plstyl_n_array, plstyl_array

    interface plsvect
       module procedure plsvect_none
       module procedure plsvect_single
       module procedure plsvect_double
    end interface plsvect
    private :: plsvect_none, plsvect_single, plsvect_double

    interface pltimefmt
        module procedure pltimefmt_impl
    end interface pltimefmt
    private :: pltimefmt_impl

    interface plxormod
        module procedure plxormod_impl
    end interface plxormod
    private :: plxormod_impl

contains

!
! Private utilities
!
subroutine copystring( fstring, cstring )
    character(len=*), intent(out) :: fstring
    character(len=1), dimension(:), intent(in) :: cstring

    integer :: i_local

    fstring = ' '
    do i_local = 1,min(len(fstring),size(cstring))
        if ( cstring(i_local) /= c_null_char ) then
            fstring(i_local:i_local) = cstring(i_local)
        else
            exit
        endif
    enddo

end subroutine copystring

!
! Interface routines
!
subroutine pl_setcontlabelformat_impl( lexp, sigdig )
   integer, intent(in) :: lexp, sigdig

   interface
       subroutine interface_pl_setcontlabelformat( lexp, sigdig ) bind(c,name='c_pl_setcontlabelformat')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: lexp, sigdig
       end subroutine interface_pl_setcontlabelformat
   end interface

   call interface_pl_setcontlabelformat( int(lexp,kind=private_plint), int(sigdig,kind=private_plint) )
end subroutine pl_setcontlabelformat_impl

subroutine pladv( sub )
    integer, intent(in) :: sub
    interface
        subroutine interface_pladv( sub ) bind( c, name = 'c_pladv' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: sub
        end subroutine interface_pladv
    end interface

    call interface_pladv( int(sub,kind=private_plint) )
end subroutine pladv

subroutine plbop()
    interface
        subroutine interface_plbop() bind(c,name='c_plbop')
        end subroutine interface_plbop
     end interface
     call interface_plbop()
end subroutine plbop

subroutine plclear()
    interface
        subroutine interface_plclear() bind(c,name='c_plclear')
        end subroutine interface_plclear
     end interface
     call interface_plclear()
end subroutine plclear

subroutine plcol0( icol )
    integer, intent(in)  :: icol
    interface
        subroutine interface_plcol0( icol ) bind(c, name = 'c_plcol0' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: icol
        end subroutine interface_plcol0
    end interface

    call interface_plcol0( int(icol,kind=private_plint) )
end subroutine plcol0

subroutine plcpstrm( iplsr, flags )
  integer, intent(in)  :: iplsr
  logical, intent(in) :: flags
    interface
        subroutine interface_plcpstrm( iplsr, flags ) bind(c, name = 'c_plcpstrm' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: iplsr, flags
        end subroutine interface_plcpstrm
    end interface

    call interface_plcpstrm( int(iplsr,kind=private_plint), int(merge(1,0,flags),kind=private_plint) )
end subroutine plcpstrm

subroutine plend()
    interface
        subroutine interface_plend() bind( c, name = 'c_plend' )
        end subroutine interface_plend
     end interface
     call interface_plend()
   end subroutine plend

subroutine plend1()
    interface
        subroutine interface_plend1() bind( c, name = 'c_plend1' )
        end subroutine interface_plend1
     end interface
     call interface_plend1()
end subroutine plend1

subroutine pleop()
    interface
        subroutine interface_pleop() bind( c, name = 'c_pleop' )
        end subroutine interface_pleop
     end interface
     call interface_pleop()
end subroutine pleop

subroutine plfamadv()
    interface
        subroutine interface_plfamadv() bind( c, name = 'c_plfamadv' )
        end subroutine interface_plfamadv
     end interface
     call interface_plfamadv()
end subroutine plfamadv

subroutine plflush()
    interface
        subroutine interface_plflush() bind( c, name = 'c_plflush' )
        end subroutine interface_plflush
     end interface
     call interface_plflush()
end subroutine plflush

subroutine plfont( font )
    integer, intent(in)  :: font
    interface
        subroutine interface_plfont( font ) bind( c, name = 'c_plfont' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: font
        end subroutine interface_plfont
    end interface

    call interface_plfont( int(font,kind=private_plint) )
end subroutine plfont

subroutine plfontld( charset )
    integer, intent(in)  :: charset
    interface
        subroutine interface_plfontld( charset ) bind( c, name = 'c_plfontld' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: charset
        end subroutine interface_plfontld
    end interface

    call interface_plfontld( int(charset,kind=private_plint) )
end subroutine plfontld

subroutine plgcol0( icol, r, g, b )
    integer, intent(in) :: icol
    integer, intent(out) :: r, g, b

    integer(kind=private_plint) :: r_out, g_out, b_out

    interface
        subroutine interface_plgcol0( icol, r, g, b ) bind( c, name = 'c_plgcol0' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: icol
            integer(kind=private_plint), intent(out) :: r, g, b
        end subroutine interface_plgcol0
    end interface

    call interface_plgcol0( int(icol,kind=private_plint), r_out, g_out, b_out )
    r = int(r_out)
    g = int(g_out)
    b = int(b_out)
end subroutine plgcol0

subroutine plgcolbg( r, g, b )
    integer, intent(out) :: r, g, b

    integer(kind=private_plint) :: r_out, g_out, b_out

    interface
        subroutine interface_plgcolbg( r, g, b ) bind( c, name = 'c_plgcolbg' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: r, g, b
        end subroutine interface_plgcolbg
    end interface

    call interface_plgcolbg( r_out, g_out, b_out )
    r = int(r_out)
    g = int(g_out)
    b = int(b_out)
end subroutine plgcolbg

subroutine plgcompression( compression )
    integer, intent(out) :: compression

    integer(kind=private_plint) :: compression_out

    interface
        subroutine interface_plgcompression( compression ) bind( c, name = 'c_plgcompression' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: compression
        end subroutine interface_plgcompression
    end interface

    call interface_plgcompression( compression_out )
    compression = int(compression_out)
end subroutine plgcompression

subroutine plgdev(dev)
    character*(*), intent(out) :: dev

    character(len=1), dimension(100) :: dev_out

    interface
        subroutine interface_plgdev( dev ) bind(c,name='c_plgdev')
            implicit none
            character(len=1), dimension(*), intent(out) :: dev
        end subroutine interface_plgdev
    end interface

    call interface_plgdev( dev_out )
    call copystring( dev, dev_out )
end subroutine plgdev

function plgdrawmode()

    integer :: plgdrawmode !function type

    interface
        function interface_plgdrawmode() bind(c,name='c_plgdrawmode')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint) :: interface_plgdrawmode !function type
        end function interface_plgdrawmode
    end interface

    plgdrawmode = int(interface_plgdrawmode())
end function plgdrawmode

subroutine plgfam( fam, num, bmax )
    integer, intent(out) :: fam, num, bmax

    integer(kind=private_plint) :: fam_out, num_out, bmax_out

    interface
        subroutine interface_plgfam( fam, num, bmax ) bind( c, name = 'c_plgfam' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: fam, num, bmax
        end subroutine interface_plgfam
    end interface

    call interface_plgfam( fam_out, num_out, bmax_out )
    fam  = int(fam_out)
    num  = int(num_out)
    bmax = int(bmax_out)
end subroutine plgfam

subroutine plgfci_impl( fci )
    integer, intent(out) :: fci

    integer(kind=private_plunicode) :: fci_out

    interface
        subroutine interface_plgfci( fci ) bind( c, name = 'c_plgfci' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plunicode), intent(out) :: fci
        end subroutine interface_plgfci
    end interface

    call interface_plgfci( fci_out )
    fci  = int(fci_out)
end subroutine plgfci_impl

subroutine plgfnam( fnam )
    character*(*), intent(out) :: fnam

    character(len=1), dimension(100) :: fnam_out

    interface
        subroutine interface_plgfnam( fnam ) bind(c,name='c_plgfnam')
            implicit none
            character(len=1), dimension(*), intent(out) :: fnam
        end subroutine interface_plgfnam
    end interface

    call interface_plgfnam( fnam_out )
    call copystring( fnam, fnam_out )
end subroutine plgfnam

subroutine plgfont( family, style, weight )
    integer, intent(out) :: family, style, weight

    integer(kind=private_plint) :: family_out, style_out, weight_out

    interface
        subroutine interface_plgfont( family, style, weight ) bind( c, name = 'c_plgfont' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: family, style, weight
        end subroutine interface_plgfont
    end interface

    call interface_plgfont( family_out, style_out, weight_out )
    family = int(family_out)
    style  = int(style_out)
    weight = int(weight_out)
end subroutine plgfont

subroutine plglevel( level )
    integer, intent(out) :: level

    integer(kind=private_plint) :: level_out

    interface
        subroutine interface_plglevel( level ) bind( c, name = 'c_plglevel' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: level
        end subroutine interface_plglevel
    end interface

    call interface_plglevel( level_out )
    level = int(level_out)
end subroutine plglevel

subroutine plgra()
    interface
        subroutine interface_plgra() bind( c, name = 'c_plgra' )
        end subroutine interface_plgra
     end interface
     call interface_plgra()
end subroutine plgra

subroutine plgstrm( strm )
    integer, intent(out) :: strm

    integer(kind=private_plint) :: strm_out

    interface
        subroutine interface_plgstrm( strm ) bind( c, name = 'c_plgstrm' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: strm
        end subroutine interface_plgstrm
    end interface

    call interface_plgstrm( strm_out )
    strm = int(strm_out)
end subroutine plgstrm

subroutine plgver(ver)
    character*(*), intent(out) :: ver

    character(len=1), dimension(100) :: ver_out

    interface
        subroutine interface_plgver( ver ) bind(c,name='c_plgver')
            implicit none
            character(len=1), dimension(*), intent(out) :: ver
        end subroutine interface_plgver
    end interface

    call interface_plgver( ver_out )
    call copystring( ver, ver_out )
end subroutine plgver

subroutine plgxax( digmax, digits )
    integer, intent(out) :: digmax, digits

    integer(kind=private_plint) :: digmax_out, digits_out

    interface
        subroutine interface_plgxax( digmax, digits ) bind( c, name = 'c_plgxax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine interface_plgxax
    end interface

    call interface_plgxax( digmax_out, digits_out )
    digmax = int(digmax_out)
    digits = int(digits_out)
end subroutine plgxax

subroutine plgyax( digmax, digits )
    integer, intent(out) :: digmax, digits

    integer(kind=private_plint) :: digmax_out, digits_out

    interface
        subroutine interface_plgyax( digmax, digits ) bind( c, name = 'c_plgyax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine interface_plgyax
    end interface

    call interface_plgyax( digmax_out, digits_out )
    digmax = int(digmax_out)
    digits = int(digits_out)
end subroutine plgyax

subroutine plgzax( digmax, digits )
    integer, intent(out) :: digmax, digits

    integer(kind=private_plint) :: digmax_out, digits_out

    interface
        subroutine interface_plgzax( digmax, digits ) bind( c, name = 'c_plgzax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine interface_plgzax
    end interface

    call interface_plgzax( digmax_out, digits_out )
    digmax = int(digmax_out)
    digits = int(digits_out)
end subroutine plgzax

subroutine plinit()
    interface
        subroutine interface_plinit() bind( c, name = 'c_plinit' )
        end subroutine interface_plinit
     end interface
     call interface_plinit()
end subroutine plinit

subroutine pllab( xlab, ylab, title )
   character(len=*), intent(in) :: xlab, ylab, title

   interface
       subroutine interface_pllab( xlab, ylab, title ) bind(c,name='c_pllab')
           implicit none
           character(len=1), dimension(*), intent(in) :: xlab, ylab, title
       end subroutine interface_pllab
   end interface

   call interface_pllab( trim(xlab) // c_null_char, trim(ylab) // c_null_char, trim(title) // c_null_char )

end subroutine pllab

subroutine pllsty( lin )
    integer, intent(in) :: lin
    interface
        subroutine interface_pllsty( lin ) bind( c, name = 'c_pllsty' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: lin
        end subroutine interface_pllsty
    end interface

    call interface_pllsty( int(lin,kind=private_plint) )
end subroutine pllsty

subroutine plmkstrm( strm )
    integer, intent(in) :: strm
    interface
        subroutine interface_plmkstrm( strm ) bind( c, name = 'c_plmkstrm' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: strm
        end subroutine interface_plmkstrm
    end interface

    call interface_plmkstrm( int(strm,kind=private_plint) )
end subroutine plmkstrm

subroutine plparseopts(mode)
  integer, intent(in) :: mode

  integer :: iargs_local, numargs_local

  integer, parameter :: maxargs_local = 100
  character (len=maxlen), dimension(0:maxargs_local) :: arg_local

  interface
     subroutine interface_plparseopts( length, nargs, arg, mode ) bind(c,name='fc_plparseopts')
       use iso_c_binding, only: c_char
       implicit none
       include 'included_plplot_interface_private_types.f90'
       integer(kind=private_plint), value, intent(in) :: length, nargs, mode
       ! This Fortran argument requires special processing done
       ! in fc_plparseopts at the C level to interoperate properly
       ! with the C version of plparseopts.
       character(c_char) arg(length, nargs)
     end subroutine interface_plparseopts
  end interface

  numargs_local = command_argument_count()
  if (numargs_local < 0) then
     !       This actually happened historically on a badly linked Cygwin platform.
     write(0,'(a)') 'f95 plparseopts ERROR: negative number of arguments'
     return
  endif
  if(numargs_local > maxargs_local) then
     write(0,'(a)') 'f95 plparseopts ERROR: too many arguments'
     return
  endif
  do iargs_local = 0, numargs_local
     call get_command_argument(iargs_local, arg_local(iargs_local))
  enddo
  call interface_plparseopts(len(arg_local(0), kind=private_plint), int(numargs_local+1, kind=private_plint), &
       arg_local, int(mode, kind=private_plint))
end subroutine plparseopts

subroutine plpat( inc, del )
    integer, dimension(:), intent(in) :: inc, del

    integer(kind=private_plint) :: nlin_local

    interface
        subroutine interface_plpat( nlin, inc, del ) bind( c, name = 'c_plpat' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nlin
            integer(kind=private_plint), dimension(*), intent(in) :: inc, del
        end subroutine interface_plpat
     end interface

     nlin_local = size(inc, kind=private_plint)
     if(nlin_local /= size(del, kind=private_plint) ) then
        write(0,*) "f95 plpat ERROR: sizes of inc and del are not consistent"
        return
     endif

    call interface_plpat( nlin_local, int(inc,kind=private_plint), int(del,kind=private_plint) )

end subroutine plpat

subroutine plprec( setp, prec )
    integer, intent(in) :: setp, prec
    interface
        subroutine interface_plprec( setp, prec ) bind( c, name = 'c_plprec' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: setp, prec
        end subroutine interface_plprec
    end interface

    call interface_plprec( int(setp,kind=private_plint), int(prec,kind=private_plint) )
end subroutine plprec

subroutine plpsty( patt )
    integer, intent(in) :: patt
    interface
        subroutine interface_plpsty( patt ) bind( c, name = 'c_plpsty' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: patt
        end subroutine interface_plpsty
    end interface

    call interface_plpsty( int(patt,kind=private_plint) )
end subroutine plpsty

! Return type is not part of the disambiguation so must provide two
! explicitly named versions where we use the convention that the
! double-precision version has no name suffix (which ~99 per cent of
! users will use) and the single-precision version has a "_single"
! suffix for those who are strict (for some strange reason) about
! keeping all double precision values out of their Fortran code.
function plrandd()

  real(kind=private_double) :: plrandd !function type

    interface
        function interface_plrandd() bind(c,name='c_plrandd')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: interface_plrandd !function type
        end function interface_plrandd
    end interface

    plrandd = real(interface_plrandd(), kind=private_double)
end function plrandd

function plrandd_single()

  real(kind=private_single) :: plrandd_single !function type

    interface
        function interface_plrandd() bind(c,name='c_plrandd')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: interface_plrandd !function type
        end function interface_plrandd
    end interface

    plrandd_single = real(interface_plrandd(), kind=private_single)
end function plrandd_single

subroutine plreplot()
    interface
        subroutine interface_plreplot() bind(c,name='c_plreplot')
        end subroutine interface_plreplot
     end interface
     call interface_plreplot()
end subroutine plreplot

subroutine plscmap0( r, g, b )
    integer, dimension(:), intent(in) :: r, g, b

    interface
        subroutine interface_plscmap0( r, g, b, n ) bind(c,name='c_plscmap0')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap0
    end interface

    call interface_plscmap0( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                     size(r,kind=private_plint) )
end subroutine plscmap0

subroutine plscmap0n( n )
    integer, intent(in) :: n
    interface
        subroutine interface_plscmap0n( n ) bind( c, name = 'c_plscmap0n' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap0n
    end interface

    call interface_plscmap0n( int(n,kind=private_plint) )
end subroutine plscmap0n

subroutine plscmap1_impl( r, g, b )
    integer, dimension(:), intent(in) :: r, g, b

    interface
        subroutine interface_plscmap1( r, g, b,  n ) bind(c,name='c_plscmap1')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap1
    end interface

    call interface_plscmap1( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
         size(r,kind=private_plint) )
end subroutine plscmap1_impl

subroutine plscmap1n( n )
    integer, intent(in) :: n
    interface
        subroutine interface_plscmap1n( n ) bind( c, name = 'c_plscmap1n' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap1n
    end interface

    call interface_plscmap1n( int(n,kind=private_plint) )
end subroutine plscmap1n

subroutine plscol0( icol, r, g, b )
    integer, intent(in) :: icol, r, g, b
    interface
        subroutine interface_plscol0( icol, r, g, b ) bind( c, name = 'c_plscol0' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: icol, r, g, b
        end subroutine interface_plscol0
    end interface

    call interface_plscol0( int(icol,kind=private_plint), &
                    int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint) )
end subroutine plscol0

subroutine plscolbg( r, g, b )
    integer, intent(in) :: r, g, b
    interface
        subroutine interface_plscolbg( r, g, b ) bind( c, name = 'c_plscolbg' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: r, g, b
        end subroutine interface_plscolbg
    end interface

    call interface_plscolbg( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint) )
end subroutine plscolbg


subroutine plscolor( color )
    integer, intent(in) :: color
    interface
        subroutine interface_plscolor( color ) bind( c, name = 'c_plscolor' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: color
        end subroutine interface_plscolor
    end interface

    call interface_plscolor( int(color,kind=private_plint) )
end subroutine plscolor

subroutine plscompression( compression )
    integer, intent(in) :: compression
    interface
        subroutine interface_plscompression( compression ) bind( c, name = 'c_plscompression' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: compression
        end subroutine interface_plscompression
    end interface

    call interface_plscompression( int(compression,kind=private_plint) )
end subroutine plscompression

subroutine plsdev( devname )
   character(len=*), intent(in) :: devname

   interface
       subroutine interface_plsdev( devname ) bind(c,name='c_plsdev')
           implicit none
           character(len=1), dimension(*), intent(in) :: devname
       end subroutine interface_plsdev
   end interface

   call interface_plsdev( trim(devname) // c_null_char )

end subroutine plsdev

subroutine plseed( s )
    integer, intent(in) :: s
    interface
        subroutine interface_plseed( s ) bind( c, name = 'c_plseed' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: s
        end subroutine interface_plseed
    end interface

    call interface_plseed( int(s,kind=private_plint) )
end subroutine plseed

! TODO: character-version
subroutine plsesc( esc )
    integer, intent(in) :: esc
    interface
        subroutine interface_plsesc( esc ) bind( c, name = 'c_plsesc' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: esc
        end subroutine interface_plsesc
    end interface

    call interface_plsesc( int(esc,kind=private_plint) )
end subroutine plsesc

subroutine plsetopt_impl( opt, optarg )
   character(len=*), intent(in) :: opt, optarg

   interface
       subroutine interface_plsetopt( opt, optarg ) bind(c,name='c_plsetopt')
           implicit none
           character(len=1), dimension(*), intent(in) :: opt, optarg
       end subroutine interface_plsetopt
   end interface

   call interface_plsetopt( trim(opt) // c_null_char, trim(optarg) // c_null_char )

end subroutine plsetopt_impl

subroutine plsfam( fam, num, bmax )
    integer, intent(in) :: fam, num, bmax
    interface
        subroutine interface_plsfam( fam, num, bmax ) bind( c, name = 'c_plsfam' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: fam, num, bmax
        end subroutine interface_plsfam
    end interface

    call interface_plsfam( int(fam,kind=private_plint), int(num,kind=private_plint), int(bmax,kind=private_plint) )
end subroutine plsfam

subroutine plsfci_impl( fci )
    integer, intent(in) :: fci

    interface
        subroutine interface_plsfci( fci ) bind( c, name = 'c_plsfci' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plunicode), value, intent(in) :: fci
        end subroutine interface_plsfci
    end interface

    call interface_plsfci( int(fci,kind=private_plunicode) )

end subroutine plsfci_impl

subroutine plsfnam( fnam )
   character(len=*), intent(in) :: fnam

   interface
       subroutine interface_plsfnam( fnam ) bind(c,name='c_plsfnam')
           implicit none
           character(len=1), dimension(*), intent(in) :: fnam
       end subroutine interface_plsfnam
   end interface

   call interface_plsfnam( trim(fnam) // c_null_char )

end subroutine plsfnam

subroutine plsfont( family, style, weight )
    integer, intent(in) :: family, style, weight
    interface
        subroutine interface_plsfont( family, style, weight ) bind( c, name = 'c_plsfont' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: family, style, weight
        end subroutine interface_plsfont
    end interface

    call interface_plsfont( int(family,kind=private_plint), int(style,kind=private_plint), int(weight,kind=private_plint) )
end subroutine plsfont

subroutine plsori( rot )
    integer, intent(in) :: rot
    interface
        subroutine interface_plsori( rot ) bind( c, name = 'c_plsori' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: rot
        end subroutine interface_plsori
    end interface

    call interface_plsori( int(rot,kind=private_plint) )
end subroutine plsori

subroutine plspal0( filename )
  character(len=*), intent(in) :: filename

   interface
       subroutine interface_plspal0( filename ) bind(c,name='c_plspal0')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           character(len=1), dimension(*), intent(in) :: filename
       end subroutine interface_plspal0
   end interface

   call interface_plspal0( trim(filename) // c_null_char )

end subroutine plspal0

subroutine plspal1( filename, interpolate )
  character(len=*), intent(in) :: filename
  logical, intent(in) :: interpolate

   interface
       subroutine interface_plspal1( filename, interpolate ) bind(c,name='c_plspal1')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: interpolate
           character(len=1), dimension(*), intent(in) :: filename
       end subroutine interface_plspal1
   end interface

   call interface_plspal1( trim(filename) // c_null_char, int( merge(1,0,interpolate),kind=private_plint) )

end subroutine plspal1

subroutine plspause( lpause )
    logical, intent(in) :: lpause

    interface
        subroutine interface_plspause( ipause ) bind(c,name='c_plspause')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: ipause
        end subroutine interface_plspause
    end interface

   call interface_plspause( int( merge(1,0,lpause),kind=private_plint) )
end subroutine plspause

subroutine plsstrm( strm )
    integer, intent(in) :: strm
    interface
        subroutine interface_plsstrm( strm ) bind( c, name = 'c_plsstrm' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: strm
        end subroutine interface_plsstrm
    end interface

    call interface_plsstrm( int(strm,kind=private_plint) )
end subroutine plsstrm

subroutine plssub( nx, ny )
    integer, intent(in) :: nx, ny
    interface
        subroutine interface_plssub( nx, ny ) bind( c, name = 'c_plssub' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny
        end subroutine interface_plssub
    end interface

    call interface_plssub( int(nx,kind=private_plint), int(ny,kind=private_plint) )
end subroutine plssub

subroutine plsvect_double( arrowx, arrowy, fill )
  logical, intent(in) :: fill
  real(kind=private_double), dimension(:), intent(in) :: arrowx, arrowy

  integer(kind=private_plint) :: npts_local
  logical :: if_arg1_local, if_arg2_local, if_arg3_local

    interface
        subroutine interface_plsvect( arrowx, arrowy, npts,  fill ) bind(c,name='c_plsvect')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: npts, fill
            real(kind=private_plflt), dimension(*), intent(in) :: arrowx, arrowy
        end subroutine interface_plsvect
     end interface

     if(npts_local /= size(arrowx, kind=private_plflt) ) then
        write(0,*) "f95 plsvect ERROR: sizes of arrowx and arrowy are not consistent"
        return
     end if

     call interface_plsvect( real(arrowx, kind=private_plflt), real(arrowy, kind=private_plflt),  &
          npts_local, int(merge(1,0,fill), kind=private_plint) )
end subroutine plsvect_double

subroutine plsvect_none( fill )
  logical, optional, intent(in) :: fill

    interface
        subroutine interface_plsvect( arrowx, arrowy, npts,  fill ) bind(c,name='c_plsvect')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: npts, fill
            type(c_ptr), value, intent(in) :: arrowx, arrowy
        end subroutine interface_plsvect
     end interface

     call interface_plsvect( c_null_ptr, c_null_ptr, 0_private_plint, 0_private_plint )
end subroutine plsvect_none

subroutine plsvect_single( arrowx, arrowy, fill )
  logical, intent(in) :: fill
  real(kind=private_single), dimension(:), intent(in) :: arrowx, arrowy

  integer(kind=private_plint) :: npts_local
  logical :: if_arg1_local, if_arg2_local, if_arg3_local

    interface
        subroutine interface_plsvect( arrowx, arrowy, npts,  fill ) bind(c,name='c_plsvect')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: npts, fill
            real(kind=private_plflt), dimension(*), intent(in) :: arrowx, arrowy
        end subroutine interface_plsvect
     end interface

     if(npts_local /= size(arrowx, kind=private_plflt) ) then
        write(0,*) "f95 plsvect ERROR: sizes of arrowx and arrowy are not consistent"
        return
     end if

     call interface_plsvect( real(arrowx, kind=private_plflt), real(arrowy, kind=private_plflt),  &
          npts_local, int(merge(1,0,fill), kind=private_plint) )
end subroutine plsvect_single

subroutine plstar( nx, ny )
    integer, intent(in) :: nx, ny
    interface
        subroutine interface_plstar( nx, ny ) bind( c, name = 'c_plstar' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny
        end subroutine interface_plstar
    end interface

    call interface_plstar( int(nx,kind=private_plint), int(ny,kind=private_plint) )
end subroutine plstar

subroutine plstripd( id )
    integer, intent(in) :: id
    interface
        subroutine interface_plstripd( id ) bind( c, name = 'c_plstripd' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: id
        end subroutine interface_plstripd
    end interface

    call interface_plstripd( int(id,kind=private_plint) )
end subroutine plstripd

subroutine plstyl_scalar( n, mark, space )
    integer, intent(in) :: n, mark, space

    call plstyl_n_array( n, (/ mark /), (/ space /) )
end subroutine plstyl_scalar

subroutine plstyl_array( mark, space )
    integer, dimension(:), intent(in) :: mark, space

    call plstyl_n_array( size(mark), mark, space )
end subroutine plstyl_array

subroutine plstyl_n_array( n, mark, space )
    integer, intent(in) :: n
    integer, dimension(:), intent(in) :: mark, space
    interface
        subroutine interface_plstyl( n, mark, space ) bind( c, name = 'c_plstyl' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            integer(kind=private_plint), dimension(*), intent(in) :: mark, space
        end subroutine interface_plstyl
    end interface

    call interface_plstyl( int(n,kind=private_plint), int(mark,kind=private_plint), int(space,kind=private_plint) )
end subroutine plstyl_n_array

subroutine plsxax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine interface_plsxax( digmax, digits ) bind( c, name = 'c_plsxax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: digmax, digits
        end subroutine interface_plsxax
    end interface

    call interface_plsxax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plsxax

subroutine plsyax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine interface_plsyax( digmax, digits ) bind( c, name = 'c_plsyax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: digmax, digits
        end subroutine interface_plsyax
    end interface

    call interface_plsyax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plsyax

subroutine plszax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine interface_plszax( digmax, digits ) bind( c, name = 'c_plszax' )
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: digmax, digits
        end subroutine interface_plszax
    end interface

    call interface_plszax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plszax

subroutine pltext()
    interface
        subroutine interface_pltext() bind(c,name='c_pltext')
        end subroutine interface_pltext
     end interface
     call interface_pltext()
end subroutine pltext

subroutine pltimefmt_impl( fmt )
   character(len=*), intent(in) :: fmt

   interface
       subroutine interface_pltimefmt( fmt ) bind(c,name='c_pltimefmt')
           implicit none
           character(len=1), dimension(*), intent(in) :: fmt
       end subroutine interface_pltimefmt
   end interface

   call interface_pltimefmt( trim(fmt) // c_null_char )

end subroutine pltimefmt_impl

subroutine plvsta()
    interface
        subroutine interface_plvsta() bind(c,name='c_plvsta')
        end subroutine interface_plvsta
     end interface
     call interface_plvsta()
end subroutine plvsta

subroutine plxormod_impl( mode, status )
  logical, intent(in) :: mode
  logical, intent(out) :: status

  integer(kind=private_plint) :: status_out

  interface
     subroutine interface_plxormod( mode, status ) bind(c,name='c_plxormod')
       implicit none
       include 'included_plplot_interface_private_types.f90'
       integer(kind=private_plint), value, intent(in) :: mode
       integer(kind=private_plint), intent(out) :: status
     end subroutine interface_plxormod
  end interface

  call interface_plxormod( int( merge(1,0,mode),kind=private_plint), status_out )
  status = status_out /= 0_private_plint

end subroutine plxormod_impl

end module plplot
