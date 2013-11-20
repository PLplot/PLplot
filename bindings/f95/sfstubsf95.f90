!***********************************************************************
!  $Id$
!  sfstubsf95.f
!
!  Copyright (C) 2005, 2006  Arjen Markus
!  Copyright (C) 2006  Alan W. Irwin
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
!  This file contains the interfaces for Fortran 95:
!  - it includes the actual FORTRAN routines from the FORTRAN 95 bindings
!  - it includes interfaces to the C routines from these bindings
!  - it defines a few Fortran 95 specific items and interfaces
!
!  NB
!  This module is written in fixed form.
!  To enable a redefinition of certain interfaces, we actually have
!  two modules.
!
!  NB
!  The INTENT attribute is not currently used. This is a matter to
!  be looked into.
!
!  NB
!  It is assumed in the current implementation that all arrays are
!  passed with correct dimensions. It would be wiser, perhaps, to
!  use the minimum dimension instead of just the dimension of the
!  first array.
!
!  NOTE:
!  Some of the C routines will have to be renamed (via macros)
!
!***********************************************************************

!
! Parameters for identifying the kind of PLplot's real
! numbers (a configuration parameter)
! Use whatever name suits you better.
!
module plplot_flt
   include 'plflt.inc'
end module

!
! Parameters and variables for strings / arrays for
! string conversion
!
module plplot_str
   integer :: maxleni
   parameter (maxlen = 320)
   parameter (maxleni = 80)
   character (len = maxlen) :: string1, string2, string3
   character (len = maxlen) :: string4, string5, string6
   character (len = maxlen) :: string7, string8, string9
   integer, dimension(maxleni) :: s1, s2, s3, s4, s5, s6, s7, s8, s9

   character(len=1), parameter :: PL_END_OF_STRING = achar(0)
end module

module plplotp
    use plplot_flt
    use plplot_str
    use plplot_strutils
    implicit none

    interface plcont
        module procedure plcontour_0
        module procedure plcontour_1
        module procedure plcontour_2
        module procedure plcontour_tr
        module procedure plcontour_0_all
        module procedure plcontour_1_all
        module procedure plcontour_2_all
        module procedure plcontour_tr_all
    end interface
    private :: plcontour_0, plcontour_1, plcontour_2, plcontour_tr
    private :: plcontour_0_all, plcontour_1_all, plcontour_2_all, plcontour_tr_all

    interface plvect
        module procedure plvectors_0
        module procedure plvectors_1
        module procedure plvectors_2
        module procedure plvectors_tr
    end interface
    private :: plvectors_0, plvectors_1, plvectors_2, plvectors_tr

    interface plshade
        module procedure plshade_single_0
        module procedure plshade_single_1
        module procedure plshade_single_2
        module procedure plshade_single_tr
    end interface
    private :: plshade_single_0, plshade_single_1, plshade_single_2, plshade_single_tr

    interface plshades
        module procedure plshades_multiple_0
        module procedure plshades_multiple_1
        module procedure plshades_multiple_2
        module procedure plshades_multiple_tr
    end interface
    private :: plshades_multiple_0, plshades_multiple_1, &
               plshades_multiple_2, plshades_multiple_tr

    interface plimagefr
        module procedure plimagefr_0
        module procedure plimagefr_1
        module procedure plimagefr_2
        module procedure plimagefr_tr
    end interface
    private :: plimagefr_0, plimagefr_1, plimagefr_2, plimagefr_tr

contains
    include 'sfstubs.f90'
end module plplotp

module plplot_types
  use plplot_flt
  type :: PLGraphicsIn
     integer type          ! of event (CURRENTLY UNUSED)
     integer state         ! key or button mask
     integer keysym        ! key selected
     integer button        ! mouse button selected
     integer subwindow     ! subwindow (alias subpage, alias subplot) number
     character(len=16) string   ! translated string
     integer pX, pY        ! absolute device coordinates of pointer
     real(kind=plflt) dX, dY    ! relative device coordinates of pointer
     real(kind=plflt) wX, wY    ! world coordinates of pointer
  end type PLGraphicsIn
end module plplot_types

module plplot
    use plplotp
    use plplot_flt
    use plplot_types
    use plplot_strutils
    !
    ! To be added: renaming list
    !

    implicit none
    include 'plplot_parameters.h'

    !
    ! To be added: alternative interfaces
    !
    interface
        subroutine pladv( sub )
            integer :: sub
        end subroutine pladv
    end interface

    interface plbin
        module procedure plbin
    end interface

    interface
        subroutine plbop
        end subroutine plbop
    end interface

    interface
        subroutine plcalc_world( rx, ry, wx, wy, window )
            use plplot_flt
            real(kind=plflt) :: rx, ry, wx, wy
            integer          :: window
        end subroutine plcalc_world
    end interface

    interface
        subroutine plclear
        end subroutine plclear
    end interface

    interface
        subroutine plcol0( icol )
            integer :: icol
        end subroutine plcol0
    end interface

    interface
        subroutine plcol1( col )
            use plplot_flt
            real(kind=plflt) :: col
        end subroutine plcol1
    end interface

    interface plcolorbar
        module procedure plcolorbar_1
        module procedure plcolorbar_2
    end interface

    interface plcpstrm
         module procedure plcpstrm
    end interface

    interface
        subroutine plend
        end subroutine plend
    end interface

    interface
        subroutine plend1
        end subroutine plend1
    end interface

    interface
        subroutine plenv( xmin, xmax, ymin, ymax, just, axis )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
            integer          :: just, axis
        end subroutine plenv
    end interface

    interface
        subroutine pleop
        end subroutine pleop
    end interface

    interface plerrx
        module procedure plerrx
    end interface

    interface plerry
        module procedure plerry
    end interface

    interface plfamadv
        subroutine plfamadv
        end subroutine plfamadv
    end interface

    interface plfill
        module procedure plfill
    end interface

    interface plfill3
        module procedure plfill3
    end interface

    interface
        subroutine plflush
        end subroutine plflush
    end interface

    interface
        subroutine plfont( font )
            integer :: font
        end subroutine plfont
    end interface

    interface
        subroutine plfontld( charset )
            integer :: charset
        end subroutine plfontld
    end interface

    interface
        subroutine plgchr( chrdef, chrht )
            use plplot_flt
            real(kind=plflt) :: chrdef, chrht
        end subroutine plgchr
    end interface

    interface
        subroutine plgcmap1_range( min_color, max_color )
            use plplot_flt
            real(kind=plflt) :: min_color, max_color
        end subroutine plgcmap1_range
    end interface

    interface
        subroutine plgcol0( icol, r, g, b )
            integer :: icol, r, g, b
        end subroutine plgcol0
    end interface

    interface
        subroutine plgcol0a( icol, r, g, b, a )
            use plplot_flt
            integer :: icol, r, g, b
            real(kind=plflt) :: a
        end subroutine plgcol0a
    end interface

    interface
        subroutine plgcolbg( r, g, b )
            integer :: r, g, b
        end subroutine plgcolbg
    end interface

    interface
        subroutine plgcolbga( r, g, b, a )
            use plplot_flt
            integer :: r, g, b
            real(kind=plflt) :: a
        end subroutine plgcolbga
    end interface

    interface
        subroutine plgcompression( compression )
            integer :: compression
        end subroutine plgcompression
    end interface

    interface
        subroutine plgdidev( mar, aspect, jx, jy )
            use plplot_flt
            real(kind=plflt) :: mar, aspect, jx, jy
        end subroutine plgdidev
    end interface

    interface
        subroutine plgdiori( rot )
            use plplot_flt
            real(kind=plflt) :: rot
        end subroutine plgdiori
    end interface

    interface
        subroutine plgdiplt( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgdiplt
    end interface

    interface
        subroutine plgetcursor( gin )
            use plplot_flt
            use plplot_types
            type(PLGraphicsIn) :: gin
        end subroutine plgetcursor
    end interface

    interface
        subroutine plgfam( fam, num, bmax )
            integer :: fam, num, bmax
        end subroutine plgfam
    end interface

    interface
        subroutine plgfci( fci )
            use plplot_flt
            integer(kind=plunicode) :: fci
        end subroutine plgfci
    end interface

    interface
        subroutine plgfont( family, style, weight )
            integer :: family, style, weight
        end subroutine plgfont
    end interface

    interface
        subroutine plglevel( level )
            integer :: level
        end subroutine plglevel
    end interface

    interface
        subroutine plgpage( xpmm, ypmm, xwid, ywid, xoff, yoff )
            use plplot_flt
            real(kind=plflt) :: xpmm, ypmm
            integer          :: xwid, ywid, xoff, yoff
        end subroutine plgpage
    end interface

    interface
        subroutine plgra
        end subroutine plgra
    end interface

    interface plgradient
        module procedure plgradient
    end interface

    interface plgriddata
        module procedure plgriddata
    end interface

    interface
        subroutine plgspa( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgspa
    end interface

    interface
        subroutine plgstrm( strm )
            integer :: strm
        end subroutine plgstrm
    end interface

    interface
        subroutine plgvpd( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgvpd
    end interface

    interface
        subroutine plgvpw( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgvpw
    end interface

    interface
        subroutine plgxax( digmax, digits )
            integer :: digmax, digits
        end subroutine plgxax
    end interface

    interface
        subroutine plgyax( digmax, digits )
            integer :: digmax, digits
        end subroutine plgyax
    end interface

    interface
        subroutine plgzax( digmax, digits )
            integer :: digmax, digits
        end subroutine plgzax
    end interface

    interface plhist
        module procedure plhist
    end interface

    interface
        subroutine plhls( h, l, s )
            use plplot_flt
            real(kind=plflt) :: h, l, s
        end subroutine plhls
    end interface

    interface
        subroutine plhlsrgb( h, l, s, r, g, b )
            use plplot_flt
            real(kind=plflt) :: h, l, s, r, g, b
        end subroutine plhlsrgb
    end interface

    interface
        subroutine plinit
        end subroutine plinit
    end interface

    interface
        subroutine pljoin( x1, y1, x2, y2 )
            use plplot_flt
            real(kind=plflt) :: x1, y1, x2, y2
        end subroutine pljoin
    end interface

    interface
        subroutine pllightsource( x, y, z )
            use plplot_flt
            real(kind=plflt) :: x, y, z
        end subroutine pllightsource
    end interface

    interface pllegend
        module procedure pllegend_1
        module procedure pllegend_2
    end interface

    interface plline
        module procedure plline
    end interface

    interface plline3
        module procedure plline3
    end interface

    interface pllsty
        subroutine pllsty( lin )
            integer :: lin
        end subroutine pllsty
    end interface

    interface plmap
        module procedure plmap1, plmap2
    end interface plmap

    interface plmeridians
        module procedure plmeridians1, plmeridians2
    end interface plmeridians

    interface plmesh
        module procedure plmesh
    end interface

    interface plmeshc
        module procedure plmeshc
    end interface

    interface
        subroutine plmkstrm( strm )
            integer :: strm
        end subroutine plmkstrm
    end interface

    interface
        subroutine plpat( nlin, inc, del )
            integer :: nlin, inc, del
        end subroutine plpat
    end interface

    interface
        subroutine plpath( n, x1, y1, x2, y2 )
            use plplot_flt
            integer :: n
            real(kind=plflt) :: x1, y1, x2, y2
        end subroutine plpath
    end interface

    interface plot3d
        module procedure plot3d
    end interface

    interface plot3dc
        module procedure plot3dc
    end interface

    interface plpoin
        module procedure plpoin
    end interface

    interface plpoin3
        module procedure plpoin3
    end interface

    interface plpoly3
        module procedure plpoly3
    end interface

    interface
        subroutine plprec( setp, prec )
            integer :: setp, prec
        end subroutine plprec
    end interface

    interface
        subroutine plpsty( patt )
            integer :: patt
        end subroutine plpsty
    end interface

    interface
        subroutine plreplot
        end subroutine plreplot
    end interface

    !
    ! Note: plrgb and plrgb1 can be merged
    !
    interface
        subroutine plrgb( r, g, b )
            use plplot_flt
            real(kind=plflt) :: r, g, b
        end subroutine plrgb
    end interface

    interface
        subroutine plrgb1( r, g, b )
            integer :: r, g, b
        end subroutine plrgb1
    end interface

    interface
        subroutine plrgbhls( r, g, b, h, l, s )
            use plplot_flt
            real(kind=plflt) :: r, g, b, h, l, s
        end subroutine plrgbhls
    end interface

    interface
        subroutine plschr( chrdef, chrht )
            use plplot_flt
            real(kind=plflt) :: chrdef, chrht
        end subroutine plschr
    end interface

    interface plscmap0
        module procedure plscmap0
    end interface

    interface plscmap0a
        module procedure plscmap0a
    end interface

    interface
        subroutine plscmap0n( n )
            integer :: n
        end subroutine plscmap0n
    end interface

    interface plscmap1
        module procedure plscmap1
    end interface

    interface plscmap1a
        module procedure plscmap1a
    end interface

    interface plscmap1l
        module procedure plscmap1l
        module procedure plscmap1l2
    end interface

    interface plscmap1la
        module procedure plscmap1la
        module procedure plscmap1la2
    end interface

    interface
        subroutine plscmap1n( n )
            integer :: n
        end subroutine plscmap1n
    end interface

    interface
       subroutine plscmap1_range( min_color, max_color )
         use plplot_flt
         real(kind=plflt) :: min_color, max_color
       end subroutine plscmap1_range
    end interface

    interface
        subroutine plscol0( icol, r, g, b )
            integer :: icol, r, g, b
        end subroutine plscol0
    end interface

    interface
        subroutine plscol0a( icol, r, g, b, a )
            use plplot_flt
            integer :: icol, r, g, b
            real(kind=plflt) :: a
        end subroutine plscol0a
    end interface

    interface
        subroutine plscolbg( r, g, b )
            integer :: r, g, b
        end subroutine plscolbg
    end interface

    interface
        subroutine plscolbga( r, g, b, a )
            use plplot_flt
            integer :: r, g, b
            real(kind=plflt) :: a
        end subroutine plscolbga
    end interface

    interface
        subroutine plscolor( color )
            integer :: color
        end subroutine plscolor
    end interface

    interface
        subroutine plscompression( compression )
            integer :: compression
        end subroutine plscompression
    end interface

    interface
        subroutine plsdidev( mar, aspect, jx, jy )
            use plplot_flt
            real(kind=plflt) :: mar, aspect, jx, jy
        end subroutine plsdidev
    end interface

    interface
        subroutine plsdimap( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm )
            use plplot_flt
            real(kind=plflt) :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm
        end subroutine plsdimap
    end interface

    interface
        subroutine plsdiori( rot )
            use plplot_flt
            real(kind=plflt) :: rot
        end subroutine plsdiori
    end interface

    interface
        subroutine plsdiplt( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsdiplt
    end interface

    interface
        subroutine plsdiplz( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsdiplz
    end interface

    interface
        subroutine plseed( s )
           integer :: s
        end subroutine plseed
    end interface

    ! TODO: character-version
    interface
        subroutine plsesc( esc )
            integer :: esc
        end subroutine plsesc
    end interface

    !
    ! TODO: F95-specific form for these routines
    !
    interface plsetmapformc
        subroutine plsetmapformc( mapform )
            use plplot_flt
            interface
                subroutine mapform( n, x, y )
                    use plplot_flt
                    integer                        :: n
                    real(kind=plflt), dimension(*) :: x, y
                end subroutine mapform
            end interface
        end subroutine plsetmapformc
    end interface

    interface
        subroutine plsfam( fam, num, bmax )
            integer :: fam, num, bmax
        end subroutine plsfam
    end interface

    interface
        subroutine plsfci( fci )
            use plplot_flt
            integer(kind=plunicode) :: fci
        end subroutine plsfci
    end interface

    interface
        subroutine plsfont( family, style, weight )
            integer :: family, style, weight
        end subroutine plsfont
    end interface

    interface plslabelfunc
        subroutine plslabelfunc_on( labelfunc )
            interface
                subroutine labelfunc(axis, value, label, length)
                    use plplot_flt
                    implicit none
                    integer :: axis, length
                    real(kind=plflt) :: value
                    character*(length) label
                end subroutine labelfunc
            end interface
        end subroutine plslabelfunc_on

        subroutine plslabelfunc_off( dummy )
            implicit none
            integer :: dummy
        end subroutine plslabelfunc_off

        subroutine plslabelfunc_none
        end subroutine plslabelfunc_none

    end interface

    interface
        subroutine plsmaj( def, scale )
            use plplot_flt
            real(kind=plflt) :: def, scale
        end subroutine plsmaj
    end interface

    ! plsmem: void * argument tricky - TODO
    ! plsmema: void * argument tricky - TODO

    interface
        subroutine plsmin( def, scale )
            use plplot_flt
            real(kind=plflt) :: def, scale
        end subroutine plsmin
    end interface

    interface
        subroutine plsori( rot )
            integer :: rot
        end subroutine plsori
    end interface

    interface
        subroutine plspage( xpmm, ypmm, xwid, ywid, xoff, yoff )
            use plplot_flt
            real(kind=plflt) :: xpmm, ypmm
            integer          :: xwid, ywid, xoff, yoff
        end subroutine plspage
    end interface

    interface plspause
        module procedure plspause
    end interface

    interface
        subroutine plsstrm( strm )
            integer :: strm
        end subroutine plsstrm
    end interface

    interface
        subroutine plssub( nx, ny )
            integer :: nx, ny
        end subroutine plssub
    end interface

    interface
        subroutine plssym( def, scale )
            use plplot_flt
            real(kind=plflt) :: def, scale
        end subroutine plssym
    end interface

    interface
        subroutine plstar( nx, ny )
            integer :: nx, ny
        end subroutine plstar
    end interface

    interface plstransform
        subroutine plstransform1( transformfunc )
            interface
                subroutine transformfunc(x, y, xt, yt)
                    use plplot_flt
                    implicit none
                    real(kind=plflt) :: x, y, xt, yt
                end subroutine transformfunc
            end interface
        end subroutine plstransform1

        subroutine plstransform2( dummy )
            implicit none
            integer :: dummy
        end subroutine plstransform2

        subroutine plstransform3
        end subroutine plstransform3

    end interface

    interface
        subroutine plstripa( id, pen, x, y )
            use plplot_flt
            integer          :: id, pen
            real(kind=plflt) :: x, y
        end subroutine plstripa
    end interface

    interface
        subroutine plstripd( id )
            integer :: id
        end subroutine plstripd
    end interface

    interface
        subroutine plstyl( n, mark, space )
            integer :: n, mark, space
        end subroutine plstyl
    end interface

    interface plsurf3d
        module procedure plsurf3d
    end interface

    interface plstripc
        module procedure plstripc
    end interface

    interface plsvect
        module procedure plsvect
    end interface

    interface
        subroutine plsvpa( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsvpa
    end interface

    interface
        subroutine plsxax( digmax, digits )
            integer :: digmax, digits
        end subroutine plsxax
    end interface

    interface
        subroutine plsyax( digmax, digits )
            integer :: digmax, digits
        end subroutine plsyax
    end interface

    interface plsym
        module procedure plsym
    end interface

    interface
        subroutine plszax( digmax, digits )
            integer :: digmax, digits
        end subroutine plszax
    end interface

    interface
        subroutine pltext
        end subroutine pltext
    end interface

    interface
        subroutine plvasp( aspect )
            use plplot_flt
            real(kind=plflt) :: aspect
        end subroutine plvasp
    end interface

    interface
        subroutine plvpas( xmin, xmax, ymin, ymax, aspect )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax, aspect
        end subroutine plvpas
    end interface

    interface
        subroutine plvpor( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plvpor
    end interface

    interface
        subroutine plvsta
        end subroutine plvsta
    end interface

    interface
        subroutine plw3d( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az )
            use plplot_flt
            real(kind=plflt) :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az
        end subroutine plw3d
    end interface

    interface
        subroutine plwidth( width )
            use plplot_flt
            real(kind=plflt) :: width
        end subroutine plwidth
    end interface

    interface
        subroutine plwind( xmin, xmax, ymin, ymax )
            use plplot_flt
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plwind
    end interface

    interface plxormod
        module procedure plxormod
    end interface


    private :: convert_to_int
    private :: convert_to_log

! -------------------------------------------------------------------
contains
! -------------------------------------------------------------------
    integer function convert_to_int( logvalue )
       logical :: logvalue
       if ( logvalue ) then
         convert_to_int = 1
       else
         convert_to_int = 0
       endif
    end function convert_to_int

    logical function convert_to_log( intvalue )
       integer :: intvalue
       convert_to_log = intvalue.ne.0
    end function convert_to_log

    subroutine plbin( x, y, center )
       real(kind=plflt), dimension(:) :: x, y
       integer                        :: center

       call plbinf95( size(x), x, y, center )
    end subroutine plbin

    subroutine plcolorbar_1( p_colorbar_width, p_colorbar_height, &
         opt, position, x, y, &
         x_length, y_length, &
         bg_color, bb_color, bb_style, &
         low_cap_color, high_cap_color, &
         cont_color, cont_width, &
         n_labels, label_opts, labels, &
         n_axes, axis_opts, ticks, sub_ticks, &
         n_values, values)
      real (kind=plflt) :: p_colorbar_width, p_colorbar_height
      integer :: opt, position, bg_color, bb_color, bb_style, cont_color
      integer :: n_labels, n_axes
      real (kind=plflt) :: x, y, x_length, y_length, low_cap_color, high_cap_color, cont_width
      integer, dimension(:) :: label_opts, sub_ticks, n_values
      real (kind=plflt), dimension(:) :: ticks
      real (kind=plflt), dimension(:,:) :: values
      character(len=*), dimension(:) :: labels, axis_opts

      !
      ! Convert the text arrays and store the results in a convenient
      ! albeit global location. This way we avoid all manner of complications.
      ! (Though introducing a potentially nasty one: non-threadsafety)
      !
      call pllegend07_cnv_text( 3, n_labels, labels )
      call pllegend07_cnv_text( 4, n_axes, axis_opts )

      call plcolorbar07(p_colorbar_width, p_colorbar_height, &
           opt, position, x, y, &
           x_length, y_length, &
           bg_color, bb_color, bb_style, &
           low_cap_color, high_cap_color, &
           cont_color, cont_width, &
           n_labels, label_opts, n_axes, ticks, sub_ticks, &
           n_values, values)
    end subroutine plcolorbar_1

    subroutine plcolorbar_2( p_colorbar_width, p_colorbar_height, &
         opt, position, x, y, &
         x_length, y_length, &
         bg_color, bb_color, bb_style, &
         low_cap_color, high_cap_color, &
         cont_color, cont_width, &
         label_opts, labels, axis_opts, ticks, sub_ticks, &
         n_values, values)
      real (kind=plflt) :: p_colorbar_width, p_colorbar_height
      integer :: opt, position, bg_color, bb_color, bb_style, cont_color
      real (kind=plflt) :: x, y, x_length, y_length, low_cap_color, high_cap_color, cont_width
      integer, dimension(:) :: label_opts, sub_ticks, n_values
      real (kind=plflt), dimension(:) :: ticks
      real (kind=plflt), dimension(:,:) :: values
      character(len=*), dimension(:) :: labels, axis_opts

      integer :: n_labels, n_axes

      n_labels = size(label_opts,1)
      n_axes = size(axis_opts,1)
      !
      ! Convert the text arrays and store the results in a convenient
      ! albeit global location. This way we avoid all manner of complications.
      ! (Though introducing a potentially nasty one: non-threadsafety)
      !
      call pllegend07_cnv_text( 3, n_labels, labels )
      call pllegend07_cnv_text( 4, n_axes, axis_opts )

      call plcolorbar07(p_colorbar_width, p_colorbar_height, &
           opt, position, x, y, &
           x_length, y_length, &
           bg_color, bb_color, bb_style, &
           low_cap_color, high_cap_color, &
           cont_color, cont_width, &
           n_labels, label_opts, n_axes, ticks, sub_ticks, &
           n_values, values)
    end subroutine plcolorbar_2      

    subroutine plcpstrm( iplsr, flags )
       integer                        :: iplsr
       logical                        :: flags

       integer                        :: iflags

       iflags = convert_to_int( flags )
       call plcpstrmf95( iplsr, iflags )
    end subroutine plcpstrm

    subroutine plerrx( xmin, xmax, y )
       real(kind=plflt), dimension(:) :: xmin, xmax, y

       call plerrxf95( size(xmin), xmin, xmax, y )
    end subroutine plerrx

    subroutine plerry( x, ymin, ymax )
       real(kind=plflt), dimension(:) :: x, ymin, ymax

       call plerryf95( size(x), x, ymin, ymax )
    end subroutine plerry

    subroutine plfill( x, y )
       real(kind=plflt), dimension(:) :: x, y

       call plfillf95( size(x), x, y )
    end subroutine plfill

    subroutine plfill3( x, y, z )
       real(kind=plflt), dimension(:) :: x, y, z

       call plfill3f95( size(x), x, y, z )
    end subroutine plfill3

    subroutine plgradient( x, y, angle )
       real(kind=plflt), dimension(:) :: x, y
       real(kind=plflt)               :: angle

       call plgradientf95( size(x), x, y, angle )
    end subroutine plgradient

    subroutine plgriddata( x, y, z, xg, yg, zg, type, data )
       real(kind=plflt), dimension(:)   :: x, y, z, xg, yg
       real(kind=plflt), dimension(:,:) :: zg
       real(kind=plflt)                 :: data
       integer                          :: type

       call plgriddataf95( x, y, z, size(x), xg, size(xg), yg, size(yg), zg, &
          type, data )

       return
    end subroutine plgriddata

    subroutine plhist( data, datmin, datmax, nbin, oldwin )
       real(kind=plflt), dimension(:) :: data
       real(kind=plflt)               :: datmin, datmax
       integer                        :: nbin, oldwin

       call plhistf95( size(data), data, datmin, datmax, nbin, oldwin )
    end subroutine plhist

!     subroutine plimagefr( idata, xmin, xmax, ymin, ymax, zmin, zmax, &
!                           dxmin, dxmax, dymin, dymax, valuemin, valuemax )
!        real(kind=plflt), dimension(:,:) :: idata
!        real(kind=plflt)                 :: xmin,  xmax,  ymin,  ymax,  zmin, zmax
!        real(kind=plflt)                 :: dxmin, dxmax, dymin, dymax, &
!                                            valuemin, valuemax
!
!        integer                          :: nx, ny
!
!        nx = size(idata,1)
!        ny = size(idata,2)
!        call plimagefrf95( idata, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, &
!                           dxmin, dxmax, dymin, dymax, valuemin, valuemax )
!     end subroutine plimagefr

    subroutine plimage( idata, xmin, xmax, ymin, ymax, zmin, zmax, &
                        dxmin, dxmax, dymin, dymax )
       real(kind=plflt), dimension(:,:) :: idata
       real(kind=plflt)                 :: xmin,  xmax,  ymin,  ymax,  zmin, zmax
       real(kind=plflt)                 :: dxmin, dxmax, dymin, dymax

       integer                          :: nx, ny

       nx = size(idata,1)
       ny = size(idata,2)
       call plimagef95( idata, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, &
                        dxmin, dxmax, dymin, dymax )
    end subroutine plimage

    subroutine pllegend_1( legend_width, legend_height, &
                           opt, position, x, y, &
                           plot_width, bg_color, bb_color, bb_style, &
                           nrow, ncolumn, nlegend, opt_array, &
                           text_offset, text_scale, text_spacing, &
                           text_justification, text_colors, text, &
                           box_colors, box_patterns, box_scales, &
                           box_line_widths, &
                           line_colors, line_styles, line_widths, &
                           symbol_colors, symbol_scales, &
                           symbol_numbers, symbols )

    real(kind=plflt)               :: legend_width, legend_height, plot_width, x, y
    real(kind=plflt)               :: text_offset, text_scale, text_spacing, text_justification
    integer                        :: position, opt, bg_color, bb_color, bb_style
    integer                        :: nrow, ncolumn, nlegend

    character(len=*), dimension(:) :: text, symbols

    integer, dimension(:)          :: opt_array, text_colors, box_colors
    integer, dimension(:)          :: box_patterns
    real(kind=plflt), dimension(:) :: box_line_widths
    integer, dimension(:)          :: line_colors, line_styles
    real(kind=plflt), dimension(:) :: line_widths
    integer, dimension(:)          :: symbol_colors, symbol_numbers
    real(kind=plflt), dimension(:) :: box_scales, symbol_scales

    !
    ! Convert the text arrays and store the results in a convenient
    ! albeit global location. This way we avoid all manner of complications.
    ! (Though introducing a potentially nasty one: non-threadsafety)
    !
    call pllegend07_cnv_text( 1, nlegend, text )
    call pllegend07_cnv_text( 2, nlegend, symbols )

    call pllegend07( legend_width, legend_height, opt, position, x, y, &
                     plot_width, bg_color, bb_color, bb_style, &
                     nrow, ncolumn, nlegend, opt_array, &
                     text_offset, text_scale, text_spacing, &
                     text_justification, text_colors, &
                     box_colors, box_patterns, box_scales, &
                     box_line_widths, &
                     line_colors, line_styles, line_widths, &
                     symbol_colors, symbol_scales, &
                     symbol_numbers )

    end subroutine pllegend_1

    subroutine pllegend_2( legend_width, legend_height, &
                           opt, position, x, y, &
                           plot_width, bg_color, bb_color, bb_style, &
                           nrow, ncolumn, opt_array, &
                           text_offset, text_scale, text_spacing, &
                           text_justification, text_colors, text, &
                           box_colors, box_patterns, box_scales, &
                           box_line_widths, &
                           line_colors, line_styles, line_widths, &
                           symbol_colors, symbol_scales, &
                           symbol_numbers, symbols )

    real(kind=plflt)               :: legend_width, legend_height, plot_width, x, y
    real(kind=plflt)               :: text_offset, text_scale, text_spacing, text_justification
    integer                        :: position, opt, bg_color, bb_color, bb_style
    integer                        :: nrow, ncolumn

    character(len=*), dimension(:) :: text, symbols

    integer, dimension(:)          :: opt_array, text_colors, box_colors
    integer, dimension(:)          :: box_patterns
    real(kind=plflt), dimension(:) :: box_line_widths
    integer, dimension(:)          :: line_colors, line_styles
    real(kind=plflt), dimension(:) :: line_widths
    integer, dimension(:)          :: symbol_colors, symbol_numbers
    real(kind=plflt), dimension(:) :: box_scales, symbol_scales

    integer                        :: nlegend

    !
    ! Determine number of legend entries
    !
    nlegend = min( size(opt_array), size(text) )

    call pllegend_1( legend_width, legend_height, &
                     opt, position, x, y, &
                     plot_width, bg_color, bb_color, bb_style, &
                     nrow, ncolumn, nlegend, opt_array, &
                     text_offset, text_scale, text_spacing, &
                     text_justification, text_colors, text, &
                     box_colors, box_patterns, box_scales, &
                     box_line_widths, &
                     line_colors, line_styles, line_widths, &
                     symbol_colors, symbol_scales, &
                     symbol_numbers, symbols )

    end subroutine pllegend_2

    subroutine plline( x, y )
       real(kind=plflt), dimension(:) :: x, y

       call pllinef95( size(x), x, y )
    end subroutine plline

    subroutine plline3( x, y, z )
       real(kind=plflt), dimension(:) :: x, y, z

       call plline3f95( size(x), x, y, z )
    end subroutine plline3

    subroutine plmap1(mapform,mapname,minx,maxx,miny,maxy)
      use plplot_flt
      use plplot_str
      implicit none
      real(kind=plflt) minx, maxx, miny, maxy
      character*(*) mapname
      external mapform

      call plstrf2c(mapname, string1)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      call plmap7(s1,minx,maxx,miny,maxy)

    end subroutine plmap1

    subroutine plmap2(mapname,minx,maxx,miny,maxy)
      use plplot_flt
      use plplot_str
      implicit none
      real(kind=plflt) minx, maxx, miny, maxy
      character*(*) mapname

      call plstrf2c(mapname, string1)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      call plmap7(s1,minx,maxx,miny,maxy)

    end subroutine plmap2

    subroutine plmeridians1(mapform,dlong,dlat,minlong,maxlong, &
                           minlat,maxlat)

    implicit none
    real(kind=plflt) dlong, dlat, minlong, maxlong, minlat, maxlat
    external mapform

    call plsetmapformc(mapform)
    call plmeridians7(dlong,dlat,minlong,maxlong,minlat,maxlat)

    end subroutine plmeridians1

    subroutine plmeridians2(dlong,dlat,minlong,maxlong, &
                           minlat,maxlat)

    implicit none
    real(kind=plflt) dlong, dlat, minlong, maxlong, minlat, maxlat

    call plclearmapformc
    call plmeridians7(dlong,dlat,minlong,maxlong,minlat,maxlat)

    end subroutine plmeridians2

    subroutine plmesh( x, y, z, opt )
       integer                          :: opt
       real(kind=plflt), dimension(:)   :: x, y
       real(kind=plflt), dimension(:,:) :: z

       call plmeshf95( x, y, z, size(x), size(y), opt, size(x))

    end subroutine plmesh

    subroutine plmeshc( x, y, z, opt, clevel )
       integer                          :: opt
       real(kind=plflt), dimension(:)   :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plmeshcf95( x, y, z, size(x), size(y), opt, &
         clevel, size(clevel), size(x))

    end subroutine plmeshc

    subroutine plot3d( x, y, z, opt, side )
       integer                          :: opt
       logical                          :: side
       real(kind=plflt), dimension(:)   :: x, y
       real(kind=plflt), dimension(:,:) :: z
       integer                          :: iside

       iside = convert_to_int(side)
       call plot3df95( x, y, z, size(x), size(y), opt, iside, size(x))

    end subroutine plot3d

    subroutine plot3dc( x, y, z, opt, clevel )
       integer                          :: opt
       real(kind=plflt), dimension(:)   :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plot3dcf95( x, y, z, size(x), size(y), opt, clevel, &
         size(clevel), size(x))

    end subroutine plot3dc

    subroutine plspause( lpause )
       logical                        :: lpause

       integer                        :: ipause

       ipause = convert_to_int( lpause )
       call plspausef95( ipause )
    end subroutine plspause

    subroutine plsurf3d( x, y, z, opt, clevel )
       integer                        :: opt
       real(kind=plflt), dimension(:) :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plsurf3df95( x, y, z, size(x), size(y), opt, clevel, &
         size(clevel), size(x))

    end subroutine plsurf3d

    subroutine plpoin( x, y, code )
       integer                        :: code
       real(kind=plflt), dimension(:) :: x, y

       call plpoinf95( size(x), x, y, code )
    end subroutine plpoin

    subroutine plpoin3( x, y, z, code )
       integer                        :: code
       real(kind=plflt), dimension(:) :: x, y, z

       call plpoin3f95( size(x), x, y, z, code )
    end subroutine plpoin3

    subroutine plpoly3( x, y, z, draw, ifcc )
       logical                        :: ifcc
       logical, dimension(:)          :: draw
       real(kind=plflt), dimension(:) :: x, y, z

       integer, dimension(size(draw))  :: idraw
       integer                        :: i
       integer                        :: iifcc

       iifcc = convert_to_int( ifcc )
       do i = 1,size(draw)
          idraw(i) = convert_to_int( draw(i) )
       enddo
       call plpoly3f95( size(x), x, y, z, idraw, iifcc )
    end subroutine plpoly3

    real (kind=plflt) function plrandd()
      external plranddf95
      real(kind=plflt) :: plranddf95

      plrandd = plranddf95()
    end function plrandd

    subroutine plscmap0( r, g, b )
       integer, dimension(:) :: r, g, b

       call plscmap0f95( r, g, b, size(r) )
    end subroutine plscmap0

    subroutine plscmap0a( r, g, b, a )
       integer, dimension(:) :: r, g, b
       real(kind=plflt), dimension(:) :: a

       call plscmap0af95( r, g, b, a, size(r) )
    end subroutine plscmap0a

    subroutine plscmap1( r, g, b )
       integer, dimension(:) :: r, g, b

       call plscmap1f95( r, g, b, size(r) )
    end subroutine plscmap1

    subroutine plscmap1a( r, g, b, a )
       integer, dimension(:) :: r, g, b
       real(kind=plflt), dimension(:) :: a

       call plscmap1af95( r, g, b, a, size(r) )
    end subroutine plscmap1a

    subroutine plscmap1l( rgbtype, intensity, coord1, coord2, coord3, alt_hue_path)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3
          logical, dimension(:)          :: alt_hue_path

          integer, dimension(size(alt_hue_path))  :: ialt_hue_path
          integer                        :: i
          integer                        :: type

       type = convert_to_int( rgbtype )
       do i = 1,size(alt_hue_path)
          ialt_hue_path(i) = convert_to_int( alt_hue_path(i) )
       enddo
       call plscmap1lf95( type, size(intensity), intensity, coord1, coord2, coord3, ialt_hue_path )
    end subroutine plscmap1l

    subroutine plscmap1l2( rgbtype, intensity, coord1, coord2, coord3)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3

          integer                        :: type

       type = convert_to_int( rgbtype )
       call plscmap1l2f95( type, size(intensity), intensity, coord1, coord2, coord3)
    end subroutine plscmap1l2

    subroutine plscmap1la( rgbtype, intensity, coord1, coord2, coord3, a, alt_hue_path)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a
          logical, dimension(:)          :: alt_hue_path

          integer, dimension(size(alt_hue_path))  :: ialt_hue_path
          integer                        :: i
          integer                        :: type

       type = convert_to_int( rgbtype )
       do i = 1,size(alt_hue_path)
          ialt_hue_path(i) = convert_to_int( alt_hue_path(i) )
       enddo
       call plscmap1laf95( type, size(intensity), intensity, coord1, coord2, coord3, a, ialt_hue_path )
    end subroutine plscmap1la

    subroutine plscmap1la2( rgbtype, intensity, coord1, coord2, coord3, a)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a

          integer                        :: type

       type = convert_to_int( rgbtype )
       call plscmap1la2f95( type, size(intensity), intensity, coord1, coord2, coord3, a)
    end subroutine plscmap1la2

    subroutine plstripc(id, xspec, yspec, xmin, xmax, xjump, &
      ymin, ymax, xlpos, ylpos, y_ascl, acc, &
      colbox, collab, colline, styline, legline, &
      labx, laby, labtop)

!    use plplot_str
    implicit none
    integer id, colbox, collab, colline(4), styline(4)
    character*(*) xspec, yspec, legline(4), labx, laby, labtop
    real(kind=plflt) xmin, xmax, xjump, ymin, ymax, xlpos, ylpos
    logical y_ascl, acc
    integer iy_ascl, iacc


    call plstrf2c(xspec, string1)
    call plstrf2c(yspec, string2)
    call plstrf2c(legline(1), string3)
    call plstrf2c(legline(2), string4)
    call plstrf2c(legline(3), string5)
    call plstrf2c(legline(4), string6)
    call plstrf2c(labx, string7)
    call plstrf2c(laby, string8)
    call plstrf2c(labtop, string9)

    iy_ascl = convert_to_int( y_ascl )
    iacc = convert_to_int( acc )

    s1 = transfer( string1, s1 )
    s2 = transfer( string2, s2 )
    s3 = transfer( string3, s3 )
    s4 = transfer( string4, s4 )
    s5 = transfer( string5, s5 )
    s6 = transfer( string6, s6 )
    s7 = transfer( string7, s7 )
    s8 = transfer( string8, s8 )
    s9 = transfer( string9, s9 )
    call plstripcf95(id, s1, s2, xmin, xmax, xjump, &
      ymin, ymax, xlpos, ylpos, iy_ascl, iacc, &
      colbox, collab, colline, styline, &
      s3, s4, s5, s6, &
      s7, s8, s9)

    end subroutine plstripc

    subroutine plsvect( arrowx, arrowy, fill )
         logical                        :: fill
         real(kind=plflt), dimension(:) :: arrowx, arrowy
         integer ifill
         ifill = convert_to_int(fill)

       call plsvectf95( arrowx, arrowy, size(arrowx), ifill )
    end subroutine plsvect

    subroutine plsym( x, y, code )
       integer                        :: code
       real(kind=plflt), dimension(:) :: x, y

       call plsymf95( size(x), x, y, code )
    end subroutine plsym

    subroutine plxormod( mode, status )
       logical :: mode, status
       integer :: imode, istatus
       imode = convert_to_int(mode)
       call plxormodf95( imode, istatus)
       status = convert_to_log(istatus)
    end subroutine plxormod
end module plplot
