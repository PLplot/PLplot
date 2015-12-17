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

! TODO: plsurf3d, plsurf3dl

module plplot_types
    include 'plplot_interface_private_types.inc'
end module plplot_types

module plplot_single
    use, intrinsic :: iso_c_binding
    use plplot_types
    implicit none

    integer, parameter :: wp = private_single
    private :: wp, private_single, private_double, private_plint, private_plunicode

    include 'plplot_real_interfaces.inc'
end module plplot_single

module plplot_double
    use, intrinsic :: iso_c_binding
    use plplot_types
    implicit none

    integer, parameter :: wp = private_double
    private :: wp, private_single, private_double, private_plint, private_plunicode

    include 'plplot_real_interfaces.inc'

end module plplot_double

module plplot
    use plplot_single
    use plplot_double
    use plplot_types, only: plflt => private_plflt, private_plint, private_plunicode
    implicit none
    integer(kind=private_plint), parameter :: maxlen = 320
    include 'plplot_parameters.inc'
    private :: private_plint, private_plunicode
    private :: copystring
!
! Interfaces that do not depend on the real kind
!
    interface
        subroutine plbop() bind(c,name='c_plbop')
        end subroutine plbop
    end interface

    interface
        subroutine plclear() bind(c,name='c_plclear')
        end subroutine plclear
    end interface

    interface
        subroutine plend() bind( c, name = 'c_plend' )
        end subroutine plend
    end interface

    interface
        subroutine plend1() bind( c, name = 'c_plend1' )
        end subroutine plend1
    end interface

    interface
        subroutine pleop() bind( c, name = 'c_pleop' )
        end subroutine pleop
    end interface

    interface
        subroutine plfamadv() bind( c, name = 'c_plfamadv' )
        end subroutine plfamadv
    end interface

    interface
        subroutine plflush() bind( c, name = 'c_plflush' )
        end subroutine plflush
    end interface

    interface
        subroutine plgra() bind( c, name = 'c_plgra' )
        end subroutine plgra
    end interface

    interface
        subroutine plinit() bind( c, name = 'c_plinit' )
        end subroutine plinit
    end interface

    interface
        subroutine plreplot() bind(c,name='c_plreplot')
        end subroutine plreplot
    end interface

    interface plstyl
        module procedure plstyl_scalar
        module procedure plstyl_n_array
        module procedure plstyl_array
    end interface
    private :: plstyl_scalar, plstyl_n_array, plstyl_array

    interface
        subroutine pltext() bind(c,name='c_pltext')
        end subroutine pltext
    end interface

    interface
        subroutine plvsta() bind(c,name='c_plvsta')
        end subroutine plvsta
    end interface

contains

!
! Private utilities
!
subroutine copystring( fstring, cstring )
    character(len=*), intent(out) :: fstring
    character(len=1), dimension(:), intent(in) :: cstring

    integer :: i

    fstring = ' '
    do i = 1,min(len(fstring),size(cstring))
        if ( cstring(i) /= c_null_char ) then
            fstring(i:i) = cstring(i)
        else
            exit
        endif
    enddo

end subroutine copystring

!
! Interface routines
!
subroutine pladv( sub )
    integer, intent(in) :: sub
    interface
        subroutine c_pladv( sub ) bind( c, name = 'c_pladv' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: sub
        end subroutine c_pladv
    end interface

    call c_pladv( int(sub,kind=private_plint) )
end subroutine pladv

subroutine plcol0( icol )
    integer, intent(in)  :: icol
    interface
        subroutine c_plcol0( icol ) bind(c, name = 'c_plcol0' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: icol
        end subroutine c_plcol0
    end interface

    call c_plcol0( int(icol,kind=private_plint) )
end subroutine plcol0

subroutine plfont( font )
    integer, intent(in)  :: font
    interface
        subroutine c_plfont( font ) bind( c, name = 'c_plfont' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: font
        end subroutine c_plfont
    end interface

    call c_plfont( int(font,kind=private_plint) )
end subroutine plfont

subroutine plfontld( charset )
    integer, intent(in)  :: charset
    interface
        subroutine c_plfontld( charset ) bind( c, name = 'c_plfontld' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: charset
        end subroutine c_plfontld
    end interface

    call c_plfontld( int(charset,kind=private_plint) )
end subroutine plfontld

subroutine plgcol0( icol, r, g, b )
    integer, intent(in) :: icol
    integer, intent(out) :: r, g, b
    integer(kind=private_plint) :: r_p, g_p, b_p
    interface
        subroutine c_plgcol0( icol, r, g, b ) bind( c, name = 'c_plgcol0' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: icol
            integer(kind=private_plint), intent(out) :: r, g, b
        end subroutine c_plgcol0
    end interface

    call c_plgcol0( int(icol,kind=private_plint), r_p, g_p, b_p )
    r = r_p
    g = g_p
    b = b_p
end subroutine plgcol0

subroutine plgcolbg( r, g, b )
    integer, intent(out) :: r, g, b
    integer(kind=private_plint) :: r_p, g_p, b_p
    interface
        subroutine c_plgcolbg( r, g, b ) bind( c, name = 'c_plgcolbg' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: r, g, b
        end subroutine c_plgcolbg
    end interface

    call c_plgcolbg( r_p, g_p, b_p )
    r = r_p
    g = g_p
    b = b_p
end subroutine plgcolbg

subroutine plgcompression( compression )
    integer, intent(out) :: compression
    integer(kind=private_plint) :: compression_p
    interface
        subroutine c_plgcompression( compression ) bind( c, name = 'c_plgcompression' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: compression
        end subroutine c_plgcompression
    end interface

    call c_plgcompression( compression_p )
    compression = compression_p
end subroutine plgcompression

subroutine plgfam( fam, num, bmax )
    integer, intent(out) :: fam, num, bmax
    integer(kind=private_plint) :: fam_p, num_p, bmax_p
    interface
        subroutine c_plgfam( fam, num, bmax ) bind( c, name = 'c_plgfam' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: fam, num, bmax
        end subroutine c_plgfam
    end interface

    call c_plgfam( fam_p, num_p, bmax_p )
    fam  = fam_p
    num  = num_p
    bmax = bmax_p
end subroutine plgfam

subroutine plgfont( family, style, weight )
    integer, intent(out) :: family, style, weight
    integer(kind=private_plint) :: family_p, style_p, weight_p
    interface
        subroutine c_plgfont( family, style, weight ) bind( c, name = 'c_plgfont' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: family, style, weight
        end subroutine c_plgfont
    end interface

    call c_plgfont( family_p, style_p, weight_p )
    family = family_p
    style  = style_p
    weight = weight_p
end subroutine plgfont

subroutine plglevel( level )
    integer, intent(out) :: level
    integer(kind=private_plint) :: level_p
    interface
        subroutine c_plglevel( level ) bind( c, name = 'c_plglevel' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: level
        end subroutine c_plglevel
    end interface

    call c_plglevel( level_p )
    level = level_p
end subroutine plglevel

subroutine plgstrm( strm )
    integer, intent(out) :: strm
    integer(kind=private_plint) :: strm_p
    interface
        subroutine c_plgstrm( strm ) bind( c, name = 'c_plgstrm' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: strm
        end subroutine c_plgstrm
    end interface

    call c_plgstrm( strm_p )
    strm = strm_p
end subroutine plgstrm

subroutine plgver(ver)
    character*(*), intent(out) :: ver

    character(len=1), dimension(100) :: verc

    interface
        subroutine c_plgver( ver ) bind(c,name='c_plgver')
            implicit none
            character(len=1), dimension(*), intent(out) :: ver
        end subroutine c_plgver
    end interface

    call c_plgver( verc )
    call copystring( ver, verc )
end subroutine plgver

subroutine plgxax( digmax, digits )
    integer, intent(out) :: digmax, digits
    integer(kind=private_plint) :: digmax_p, digits_p
    interface
        subroutine c_plgxax( digmax, digits ) bind( c, name = 'c_plgxax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine c_plgxax
    end interface

    call c_plgxax( digmax_p, digits_p )
    digmax = digmax_p
    digits = digits_p
end subroutine plgxax

subroutine plgyax( digmax, digits )
    integer, intent(out) :: digmax, digits
    integer(kind=private_plint) :: digmax_p, digits_p
    interface
        subroutine c_plgyax( digmax, digits ) bind( c, name = 'c_plgyax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine c_plgyax
    end interface

    call c_plgyax( digmax_p, digits_p )
    digmax = digmax_p
    digits = digits_p
end subroutine plgyax

subroutine plgzax( digmax, digits )
    integer, intent(out) :: digmax, digits
    integer(kind=private_plint) :: digmax_p, digits_p
    interface
        subroutine c_plgzax( digmax, digits ) bind( c, name = 'c_plgzax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), intent(out) :: digmax, digits
        end subroutine c_plgzax
    end interface

    call c_plgzax( digmax_p, digits_p )
    digmax = digmax_p
    digits = digits_p
end subroutine plgzax

subroutine pllab( xlab, ylab, title )
   character(len=*), intent(in) :: xlab, ylab, title

   interface
       subroutine c_pllab( xlab, ylab, title ) bind(c,name='c_pllab')
           implicit none
           character(len=1), dimension(*), intent(in) :: xlab, ylab, title
       end subroutine c_pllab
   end interface

   call c_pllab( trim(xlab) // c_null_char, trim(ylab) // c_null_char, trim(title) // c_null_char )

end subroutine pllab

subroutine pllsty( lin )
    integer, intent(in) :: lin
    interface
        subroutine c_pllsty( lin ) bind( c, name = 'c_pllsty' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: lin
        end subroutine c_pllsty
    end interface

    call c_pllsty( int(lin,kind=private_plint) )
end subroutine pllsty

subroutine plmkstrm( strm )
    integer, intent(in) :: strm
    interface
        subroutine c_plmkstrm( strm ) bind( c, name = 'c_plmkstrm' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: strm
        end subroutine c_plmkstrm
    end interface

    call c_plmkstrm( int(strm,kind=private_plint) )
end subroutine plmkstrm

subroutine plpat( nlin, inc, del )
    integer, intent(in) :: nlin, inc, del
    interface
        subroutine c_plpat( nlin, inc, del ) bind( c, name = 'c_plpat' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: nlin, inc, del
        end subroutine c_plpat
    end interface

    call c_plpat( int(nlin,kind=private_plint), int(inc,kind=private_plint), int(del,kind=private_plint) )

end subroutine plpat

! Should be defined only once - return type not part of disambiguation
real (kind=private_plflt) function plrandd_impl()
    interface
        function c_plrandd() bind(c,name='c_plrandd')
            include 'plplot_interface_private_types.inc'
            real(kind=private_plflt) :: c_plrandd
        end function c_plrandd
    end interface

    plrandd_impl = c_plrandd()
end function plrandd_impl

subroutine plprec( setp, prec )
    integer, intent(in) :: setp, prec
    interface
        subroutine c_plprec( setp, prec ) bind( c, name = 'c_plprec' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: setp, prec
        end subroutine c_plprec
    end interface

    call c_plprec( int(setp,kind=private_plint), int(prec,kind=private_plint) )
end subroutine plprec

subroutine plpsty( patt )
    integer, intent(in) :: patt
    interface
        subroutine c_plpsty( patt ) bind( c, name = 'c_plpsty' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: patt
        end subroutine c_plpsty
    end interface

    call c_plpsty( int(patt,kind=private_plint) )
end subroutine plpsty

subroutine plscmap0( r, g, b )
    integer, dimension(:), intent(in) :: r, g, b

    interface
        subroutine c_plscmap0( r, g, b, n ) bind(c,name='c_plscmap0')
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            integer(kind=private_plint), value :: n
        end subroutine c_plscmap0
    end interface

    call c_plscmap0( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                     size(r,kind=private_plint) )
end subroutine plscmap0

subroutine plscmap0n( n )
    integer, intent(in) :: n
    interface
        subroutine c_plscmap0n( n ) bind( c, name = 'c_plscmap0n' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: n
        end subroutine c_plscmap0n
    end interface

    call c_plscmap0n( int(n,kind=private_plint) )
end subroutine plscmap0n

subroutine plscmap1n( n )
    integer, intent(in) :: n
    interface
        subroutine c_plscmap1n( n ) bind( c, name = 'c_plscmap1n' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: n
        end subroutine c_plscmap1n
    end interface

    call c_plscmap1n( int(n,kind=private_plint) )
end subroutine plscmap1n

subroutine plscol0( icol, r, g, b )
    integer, intent(in) :: icol, r, g, b
    interface
        subroutine c_plscol0( icol, r, g, b ) bind( c, name = 'c_plscol0' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: icol, r, g, b
        end subroutine c_plscol0
    end interface

    call c_plscol0( int(icol,kind=private_plint), &
                    int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint) )
end subroutine plscol0

subroutine plscolbg( r, g, b )
    integer, intent(in) :: r, g, b
    interface
        subroutine c_plscolbg( r, g, b ) bind( c, name = 'c_plscol0' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: r, g, b
        end subroutine c_plscolbg
    end interface

    call c_plscolbg( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint) )
end subroutine plscolbg


subroutine plscolor( color )
    integer, intent(in) :: color
    interface
        subroutine c_plscolor( color ) bind( c, name = 'c_plscolor' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: color
        end subroutine c_plscolor
    end interface

    call c_plscolor( int(color,kind=private_plint) )
end subroutine plscolor

subroutine plscompression( compression )
    integer, intent(in) :: compression
    interface
        subroutine c_plscompression( compression ) bind( c, name = 'c_plscompression' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: compression
        end subroutine c_plscompression
    end interface

    call c_plscompression( int(compression,kind=private_plint) )
end subroutine plscompression

subroutine plseed( s )
    integer, intent(in) :: s
    interface
        subroutine c_plseed( s ) bind( c, name = 'c_plseed' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: s
        end subroutine c_plseed
    end interface

    call c_plseed( int(s,kind=private_plint) )
end subroutine plseed

! TODO: character-version
subroutine plsesc( esc )
    integer, intent(in) :: esc
    interface
        subroutine c_plsesc( esc ) bind( c, name = 'c_plsesc' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: esc
        end subroutine c_plsesc
    end interface

    call c_plsesc( int(esc,kind=private_plint) )
end subroutine plsesc

subroutine plsfam( fam, num, bmax )
    integer, intent(in) :: fam, num, bmax
    interface
        subroutine c_plsfam( fam, num, bmax ) bind( c, name = 'c_plsfam' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: fam, num, bmax
        end subroutine c_plsfam
    end interface

    call c_plsfam( int(fam,kind=private_plint), int(num,kind=private_plint), int(bmax,kind=private_plint) )
end subroutine plsfam

subroutine plsfont( family, style, weight )
    integer, intent(in) :: family, style, weight
    interface
        subroutine c_plsfont( family, style, weight ) bind( c, name = 'c_plsfont' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: family, style, weight
        end subroutine c_plsfont
    end interface

    call c_plsfont( int(family,kind=private_plint), int(style,kind=private_plint), int(weight,kind=private_plint) )
end subroutine plsfont

subroutine plsori( rot )
    integer, intent(in) :: rot
    interface
        subroutine c_plsori( rot ) bind( c, name = 'c_plsori' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: rot
        end subroutine c_plsori
    end interface

    call c_plsori( int(rot,kind=private_plint) )
end subroutine plsori

subroutine plspause( lpause )
    logical, intent(in) :: lpause

    interface
        subroutine c_plspause( ipause ) bind(c,name='c_plspause')
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: ipause
        end subroutine c_plspause
    end interface

   call c_plspause( int( merge(1,0,lpause),kind=private_plint) )
end subroutine plspause

subroutine plsstrm( strm )
    integer, intent(in) :: strm
    interface
        subroutine c_plsstrm( strm ) bind( c, name = 'c_plsstrm' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: strm
        end subroutine c_plsstrm
    end interface

    call c_plsstrm( int(strm,kind=private_plint) )
end subroutine plsstrm

subroutine plssub( nx, ny )
    integer, intent(in) :: nx, ny
    interface
        subroutine c_plssub( nx, ny ) bind( c, name = 'c_plssub' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: nx, ny
        end subroutine c_plssub
    end interface

    call c_plssub( int(nx,kind=private_plint), int(ny,kind=private_plint) )
end subroutine plssub

subroutine plstar( nx, ny )
    integer, intent(in) :: nx, ny
    interface
        subroutine c_plstar( nx, ny ) bind( c, name = 'c_plstar' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: nx, ny
        end subroutine c_plstar
    end interface

    call c_plstar( int(nx,kind=private_plint), int(ny,kind=private_plint) )
end subroutine plstar

subroutine plstripd( id )
    integer, intent(in) :: id
    interface
        subroutine c_plstripd( id ) bind( c, name = 'c_plstripd' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: id
        end subroutine c_plstripd
    end interface

    call c_plstripd( int(id,kind=private_plint) )
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
        subroutine c_plstyl( n, mark, space ) bind( c, name = 'c_plstyl' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: n
            integer(kind=private_plint), dimension(*) :: mark, space
        end subroutine c_plstyl
    end interface

    call c_plstyl( int(n,kind=private_plint), int(mark,kind=private_plint), int(space,kind=private_plint) )
end subroutine plstyl_n_array

subroutine plsxax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine c_plsxax( digmax, digits ) bind( c, name = 'c_plsxax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: digmax, digits
        end subroutine c_plsxax
    end interface

    call c_plsxax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plsxax

subroutine plsyax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine c_plsyax( digmax, digits ) bind( c, name = 'c_plsyax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: digmax, digits
        end subroutine c_plsyax
    end interface

    call c_plsyax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plsyax

subroutine plszax( digmax, digits )
    integer, intent(in) :: digmax, digits
    interface
        subroutine c_plszax( digmax, digits ) bind( c, name = 'c_plszax' )
            implicit none
            include 'plplot_interface_private_types.inc'
            integer(kind=private_plint), value :: digmax, digits
        end subroutine c_plszax
    end interface

    call c_plszax( int(digmax,kind=private_plint), int(digits,kind=private_plint) )
end subroutine plszax

subroutine plparseopts(mode)
  integer                :: mode
  integer                :: iargs, numargs
  integer, parameter     :: maxargs = 100
  character (len=maxlen), dimension(0:maxargs) :: arg
  
  interface
     subroutine interface_plparseopts( length, nargs, arg, mode ) bind(c,name='fc_plparseopts')
       use iso_c_binding, only: c_int, c_char
       include 'plplot_interface_private_types.inc'
       integer(kind=private_plint), value :: length, nargs, mode
       ! This Fortran argument requires special processing done
       ! in fc_plparseopts at the C level to interoperate properly
       ! with the C version of plparseopts.
       character(c_char) arg(length, nargs)
     end subroutine interface_plparseopts
  end interface
  
  numargs = command_argument_count()
  if (numargs < 0) then
     !       This actually happened historically on a badly linked Cygwin platform.
     write(0,'(a)') 'plparseopts: negative number of arguments'
     return
  endif
  if(numargs > maxargs) then
     write(0,'(a)') 'plparseopts: too many arguments'
     return
  endif
  do iargs = 0, numargs
     call get_command_argument(iargs, arg(iargs))
  enddo
  call interface_plparseopts(len(arg(0), kind=private_plint), int(numargs+1, kind=private_plint), arg, &
       int(mode, kind=private_plint))
end subroutine plparseopts
end module plplot
