!***********************************************************************
!  sfstubsf95.f
!
!  Copyright (C) 2005, 2006  Arjen Markus
!  Copyright (C) 2006-2014 Alan W. Irwin
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
! Parameters and variables for strings / arrays for
! string conversion
!
module plplot_str
   use plplot_types
   implicit none
   integer(kind=plint), parameter :: maxlen = 320
   integer(kind=plint), parameter :: maxleni = 80
   character (len = maxlen) :: string1, string2, string3
   character (len = maxlen) :: string4, string5, string6
   character (len = maxlen) :: string7, string8, string9
   integer(kind=plint), dimension(maxleni) :: s1, s2, s3, s4, s5, s6, s7, s8, s9

   character(len=1), parameter :: PL_END_OF_STRING = achar(0)
end module

module plplotp
    use plplot_types
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
        module procedure plshades_multiple_1r
        module procedure plshades_multiple_2
        module procedure plshades_multiple_2r
        module procedure plshades_multiple_tr
        module procedure plshades_multiple_trr
    end interface
    private :: plshades_multiple_0, plshades_multiple_1, plshades_multiple_1r, &
               plshades_multiple_2, plshades_multiple_2r, &
               plshades_multiple_tr, plshades_multiple_trr

    interface plimagefr
        module procedure plimagefr_0
        module procedure plimagefr_1
        module procedure plimagefr_2
        module procedure plimagefr_tr
    end interface
    private :: plimagefr_0, plimagefr_1, plimagefr_2, plimagefr_tr

contains
    include 'sfstubs.inc'
end module plplotp

module plplot_graphics
  use plplot_types
  implicit none
  type :: PLGraphicsIn
     integer(kind=plint) type          ! of event (CURRENTLY UNUSED)
     integer(kind=plint) state         ! key or button mask
     integer(kind=plint) keysym        ! key selected
     integer(kind=plint) button        ! mouse button selected
     integer(kind=plint) subwindow     ! subwindow (alias subpage, alias subplot) number
     character(len=16) string   ! translated string
     integer(kind=plint) pX, pY        ! absolute device coordinates of pointer
     real(kind=plflt) dX, dY    ! relative device coordinates of pointer
     real(kind=plflt) wX, wY    ! world coordinates of pointer
  end type PLGraphicsIn
end module plplot_graphics

module plplot
    use plplot_types
    use plplotp
    use plplot_graphics
    use plplot_strutils
    !
    ! To be added: renaming list
    !

    implicit none
    include 'plplot_parameters.inc'

    !
    ! To be added: alternative interfaces
    !
    interface
        subroutine pladv( sub )
            use plplot_types
            implicit none
            integer(kind=plint) :: sub
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
        subroutine plbtime( year, month, day, hour, min, sec, ctime )
            use plplot_types
            implicit none
            integer(kind=plint) :: year, month, day, hour, min
            real(kind=plflt)    :: sec, ctime
        end subroutine plbtime
    end interface

    interface
        subroutine plcalc_world( rx, ry, wx, wy, window )
            use plplot_types
            implicit none
            real(kind=plflt) :: rx, ry, wx, wy
            integer(kind=plint)          :: window
        end subroutine plcalc_world
    end interface

    interface
        subroutine plclear
        end subroutine plclear
    end interface

    interface
        subroutine plcol0( icol )
            use plplot_types
            implicit none
            integer(kind=plint) :: icol
        end subroutine plcol0
    end interface

    interface
        subroutine plcol1( col )
            use plplot_types
            implicit none
            real(kind=plflt) :: col
        end subroutine plcol1
    end interface

    interface plcolorbar
        module procedure plcolorbar_1
        module procedure plcolorbar_2
    end interface

    interface
        subroutine plconfigtime( scale, offset1, offset2, control, ifbtime_offset, &
                                 year, month, day, hour, min, sec )
            use plplot_types
            implicit none
            real(kind=plflt)    :: scale, offset1, offset2, sec
            integer(kind=plint) :: control, ifbtime_offset, year, month, day, hour, min
        end subroutine plconfigtime
    end interface

    interface plcpstrm
         module procedure plcpstrm
    end interface

    interface
        subroutine plctime( year, month, day, hour, min, sec, ctime )
            use plplot_types
            implicit none
            integer(kind=plint) :: year, month, day, hour, min
            real(kind=plflt)    :: sec, ctime
        end subroutine plctime
    end interface

    interface
        subroutine plend1
        end subroutine plend1
    end interface

    interface
        subroutine plenv( xmin, xmax, ymin, ymax, just, axis )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
            integer(kind=plint)          :: just, axis
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
            use plplot_types
            implicit none
            integer(kind=plint) :: font
        end subroutine plfont
    end interface

    interface
        subroutine plfontld( charset )
            use plplot_types
            implicit none
            integer(kind=plint) :: charset
        end subroutine plfontld
    end interface

    interface
        subroutine plgchr( chrdef, chrht )
            use plplot_types
            implicit none
            real(kind=plflt) :: chrdef, chrht
        end subroutine plgchr
    end interface

    interface
        subroutine plgcmap1_range( min_color, max_color )
            use plplot_types
            implicit none
            real(kind=plflt) :: min_color, max_color
        end subroutine plgcmap1_range
    end interface

    interface
        subroutine plgcol0( icol, r, g, b )
            use plplot_types
            implicit none
            integer(kind=plint) :: icol, r, g, b
        end subroutine plgcol0
    end interface

    interface
        subroutine plgcol0a( icol, r, g, b, a )
            use plplot_types
            implicit none
            integer(kind=plint) :: icol, r, g, b
            real(kind=plflt) :: a
        end subroutine plgcol0a
    end interface

    interface
        subroutine plgcolbg( r, g, b )
            use plplot_types
            implicit none
            integer(kind=plint) :: r, g, b
        end subroutine plgcolbg
    end interface

    interface
        subroutine plgcolbga( r, g, b, a )
            use plplot_types
            implicit none
            integer(kind=plint) :: r, g, b
            real(kind=plflt) :: a
        end subroutine plgcolbga
    end interface

    interface
        subroutine plgcompression( compression )
            use plplot_types
            implicit none
            integer(kind=plint) :: compression
        end subroutine plgcompression
    end interface

    interface
        subroutine plgdidev( mar, aspect, jx, jy )
            use plplot_types
            implicit none
            real(kind=plflt) :: mar, aspect, jx, jy
        end subroutine plgdidev
    end interface

    interface
        subroutine plgdiori( rot )
            use plplot_types
            implicit none
            real(kind=plflt) :: rot
        end subroutine plgdiori
    end interface

    interface
        subroutine plgdiplt( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgdiplt
    end interface

    interface
        subroutine plgetcursor( gin )
            use plplot_types
            use plplot_graphics
            implicit none
            type(PLGraphicsIn) :: gin
        end subroutine plgetcursor
    end interface

    interface
        subroutine plgfam( fam, num, bmax )
            use plplot_types
            implicit none
            integer(kind=plint) :: fam, num, bmax
        end subroutine plgfam
    end interface

    interface
        subroutine plgfci( fci )
            use plplot_types
            implicit none
            integer(kind=plunicode) :: fci
        end subroutine plgfci
    end interface

    interface
        subroutine plgfont( family, style, weight )
            use plplot_types
            implicit none
            integer(kind=plint) :: family, style, weight
        end subroutine plgfont
    end interface

    interface
        subroutine plglevel( level )
            use plplot_types
            implicit none
            integer(kind=plint) :: level
        end subroutine plglevel
    end interface

    interface
        subroutine plgpage( xpmm, ypmm, xwid, ywid, xoff, yoff )
            use plplot_types
            implicit none
            real(kind=plflt) :: xpmm, ypmm
            integer(kind=plint)          :: xwid, ywid, xoff, yoff
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
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgspa
    end interface

    interface
        subroutine plgstrm( strm )
            use plplot_types
            implicit none
            integer(kind=plint) :: strm
        end subroutine plgstrm
    end interface

    interface
        subroutine plgvpd( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgvpd
    end interface

    interface
        subroutine plgvpw( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plgvpw
    end interface

    interface
        subroutine plgxax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plgxax
    end interface

    interface
        subroutine plgyax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plgyax
    end interface

    interface
        subroutine plgzax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plgzax
    end interface

    interface plhist
        module procedure plhist
    end interface

    interface
        subroutine plhls( h, l, s )
            use plplot_types
            implicit none
            real(kind=plflt) :: h, l, s
        end subroutine plhls
    end interface

    interface
        subroutine plhlsrgb( h, l, s, r, g, b )
            use plplot_types
            implicit none
            real(kind=plflt) :: h, l, s, r, g, b
        end subroutine plhlsrgb
    end interface

    interface
        subroutine plinit
        end subroutine plinit
    end interface

    interface
        subroutine pljoin( x1, y1, x2, y2 )
            use plplot_types
            implicit none
            real(kind=plflt) :: x1, y1, x2, y2
        end subroutine pljoin
    end interface

    interface
        subroutine pllightsource( x, y, z )
            use plplot_types
            implicit none
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
            use plplot_types
            implicit none
            integer(kind=plint) :: lin
        end subroutine pllsty
    end interface

    interface plmap
        module procedure plmap1, plmap2
    end interface

    interface plmapline
        module procedure plmapline1, plmapline2
    end interface

    interface plmapfill
        module procedure plmapfill1, plmapfill2
    end interface

    interface plmapstring
        module procedure plmapstring1, plmapstring2
    end interface

    interface plmaptex
        module procedure plmaptex1, plmaptex2
    end interface

    interface plmeridians
        module procedure plmeridians1, plmeridians2
    end interface

    interface plmesh
        module procedure plmesh
    end interface

    interface plmeshc
        module procedure plmeshc
    end interface

    interface
        subroutine plmkstrm( strm )
            use plplot_types
            implicit none
            integer(kind=plint) :: strm
        end subroutine plmkstrm
    end interface

    interface
        subroutine plpat( nlin, inc, del )
            use plplot_types
            implicit none
            integer(kind=plint) :: nlin, inc, del
        end subroutine plpat
    end interface

    interface
        subroutine plpath( n, x1, y1, x2, y2 )
            use plplot_types
            implicit none
            integer(kind=plint) :: n
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
            use plplot_types
            implicit none
            integer(kind=plint) :: setp, prec
        end subroutine plprec
    end interface

    interface
        subroutine plpsty( patt )
            use plplot_types
            implicit none
            integer(kind=plint) :: patt
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
            use plplot_types
            implicit none
            real(kind=plflt) :: r, g, b
        end subroutine plrgb
    end interface

    interface
        subroutine plrgb1( r, g, b )
            use plplot_types
            implicit none
            integer(kind=plint) :: r, g, b
        end subroutine plrgb1
    end interface

    interface
        subroutine plrgbhls( r, g, b, h, l, s )
            use plplot_types
            implicit none
            real(kind=plflt) :: r, g, b, h, l, s
        end subroutine plrgbhls
    end interface

    interface
        subroutine plschr( chrdef, chrht )
            use plplot_types
            implicit none
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
            use plplot_types
            implicit none
            integer(kind=plint) :: n
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
            use plplot_types
            implicit none
            integer(kind=plint) :: n
        end subroutine plscmap1n
    end interface

    interface
        subroutine plscmap1_range( min_color, max_color )
            use plplot_types
            implicit none
            real(kind=plflt) :: min_color, max_color
        end subroutine plscmap1_range
    end interface

    interface
        subroutine plscol0( icol, r, g, b )
            use plplot_types
            implicit none
            integer(kind=plint) :: icol, r, g, b
        end subroutine plscol0
    end interface

    interface
        subroutine plscol0a( icol, r, g, b, a )
            use plplot_types
            implicit none
            integer(kind=plint) :: icol, r, g, b
            real(kind=plflt) :: a
        end subroutine plscol0a
    end interface

    interface
        subroutine plscolbg( r, g, b )
            use plplot_types
            implicit none
            integer(kind=plint) :: r, g, b
        end subroutine plscolbg
    end interface

    interface
        subroutine plscolbga( r, g, b, a )
            use plplot_types
            implicit none
            integer(kind=plint) :: r, g, b
            real(kind=plflt) :: a
        end subroutine plscolbga
    end interface

    interface
        subroutine plscolor( color )
            use plplot_types
            implicit none
            integer(kind=plint) :: color
        end subroutine plscolor
    end interface

    interface
        subroutine plscompression( compression )
            use plplot_types
            implicit none
            integer(kind=plint) :: compression
        end subroutine plscompression
    end interface

    interface
        subroutine plsdidev( mar, aspect, jx, jy )
            use plplot_types
            implicit none
            real(kind=plflt) :: mar, aspect, jx, jy
        end subroutine plsdidev
    end interface

    interface
        subroutine plsdimap( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm )
            use plplot_types
            implicit none
            real(kind=plflt) :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm
        end subroutine plsdimap
    end interface

    interface
        subroutine plsdiori( rot )
            use plplot_types
            implicit none
            real(kind=plflt) :: rot
        end subroutine plsdiori
    end interface

    interface
        subroutine plsdiplt( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsdiplt
    end interface

    interface
        subroutine plsdiplz( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsdiplz
    end interface

    interface
        subroutine plseed( s )
            use plplot_types
            implicit none
            integer(kind=plint) :: s
        end subroutine plseed
    end interface

    ! TODO: character-version
    interface
        subroutine plsesc( esc )
            use plplot_types
            implicit none
            integer(kind=plint) :: esc
        end subroutine plsesc
    end interface

    !
    ! TODO: F95-specific form for these routines
    !
    interface plsetmapformc
        subroutine plsetmapformc( mapform )
            interface
                subroutine mapform( n, x, y )
                    use plplot_types
                    implicit none
                    integer(kind=plint)                        :: n
                    real(kind=plflt), dimension(*) :: x, y
                end subroutine mapform
            end interface
        end subroutine plsetmapformc
    end interface

    interface
        subroutine plsfam( fam, num, bmax )
            use plplot_types
            implicit none
            integer(kind=plint) :: fam, num, bmax
        end subroutine plsfam
    end interface

    interface
        subroutine plsfci( fci )
            use plplot_types
            implicit none
            integer(kind=plunicode) :: fci
        end subroutine plsfci
    end interface

    interface
        subroutine plsfont( family, style, weight )
            use plplot_types
            implicit none
            integer(kind=plint) :: family, style, weight
        end subroutine plsfont
    end interface

    interface plslabelfunc
        subroutine plslabelfunc_on( labelfunc )
            interface
                subroutine labelfunc(axis, value, label, length)
                    use plplot_types
                    implicit none
                    integer(kind=plint) :: axis, length
                    real(kind=plflt) :: value
                    character*(length) label
                end subroutine labelfunc
            end interface
        end subroutine plslabelfunc_on

        subroutine plslabelfunc_off( dummy )
            use plplot_types
            implicit none
            integer(kind=plint) :: dummy
        end subroutine plslabelfunc_off

        subroutine plslabelfunc_none
        end subroutine plslabelfunc_none

    end interface

    interface
        subroutine plsmaj( def, scale )
            use plplot_types
            implicit none
            real(kind=plflt) :: def, scale
        end subroutine plsmaj
    end interface

    ! plsmem: void * argument tricky - TODO
    ! plsmema: void * argument tricky - TODO

    interface
        subroutine plsmin( def, scale )
            use plplot_types
            implicit none
            real(kind=plflt) :: def, scale
        end subroutine plsmin
    end interface

    interface
        subroutine plsori( rot )
            use plplot_types
            implicit none
            integer(kind=plint) :: rot
        end subroutine plsori
    end interface

    interface
        subroutine plspage( xpmm, ypmm, xwid, ywid, xoff, yoff )
            use plplot_types
            implicit none
            real(kind=plflt) :: xpmm, ypmm
            integer(kind=plint)          :: xwid, ywid, xoff, yoff
        end subroutine plspage
    end interface

    interface plspause
        module procedure plspause
    end interface

    interface
        subroutine plsstrm( strm )
            use plplot_types
            implicit none
            integer(kind=plint) :: strm
        end subroutine plsstrm
    end interface

    interface
        subroutine plssub( nx, ny )
            use plplot_types
            implicit none
            integer(kind=plint) :: nx, ny
        end subroutine plssub
    end interface

    interface
        subroutine plssym( def, scale )
            use plplot_types
            implicit none
            real(kind=plflt) :: def, scale
        end subroutine plssym
    end interface

    interface
        subroutine plstar( nx, ny )
            use plplot_types
            implicit none
            integer(kind=plint) :: nx, ny
        end subroutine plstar
    end interface

    interface plstransform
        subroutine plstransform1( transformfunc )
            interface
                subroutine transformfunc(x, y, xt, yt)
                    use plplot_types
                    implicit none
                    real(kind=plflt) :: x, y, xt, yt
                end subroutine transformfunc
            end interface
        end subroutine plstransform1

        subroutine plstransform2( dummy )
            use plplot_types
            implicit none
            integer(kind=plint) :: dummy
        end subroutine plstransform2

        subroutine plstransform3
        end subroutine plstransform3

    end interface

    interface
        subroutine plstripa( id, pen, x, y )
            use plplot_types
            implicit none
            integer(kind=plint)          :: id, pen
            real(kind=plflt) :: x, y
        end subroutine plstripa
    end interface

    interface
        subroutine plstripd( id )
            use plplot_types
            implicit none
            integer(kind=plint) :: id
        end subroutine plstripd
    end interface

    interface
        subroutine plstyl( n, mark, space )
            use plplot_types
            implicit none
            integer(kind=plint) :: n, mark, space
        end subroutine plstyl
    end interface

    interface plsurf3d
        module procedure plsurf3d
    end interface

    interface plsurf3dl
        module procedure plsurf3dl
    end interface

    interface plstripc
        module procedure plstripc
    end interface

    interface plsvect
        module procedure plsvect1
    end interface

    interface plsvect
        subroutine plsvect2
        end subroutine plsvect2
    end interface

    interface
        subroutine plsvpa( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plsvpa
    end interface

    interface
        subroutine plsxax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plsxax
    end interface

    interface
        subroutine plsyax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plsyax
    end interface

    interface plsym
        module procedure plsym
    end interface

    interface
        subroutine plszax( digmax, digits )
            use plplot_types
            implicit none
            integer(kind=plint) :: digmax, digits
        end subroutine plszax
    end interface

    interface
        subroutine pltext
        end subroutine pltext
    end interface

    interface
        subroutine plvasp( aspect )
            use plplot_types
            implicit none
            real(kind=plflt) :: aspect
        end subroutine plvasp
    end interface

    interface
        subroutine plvpas( xmin, xmax, ymin, ymax, aspect )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax, aspect
        end subroutine plvpas
    end interface

    interface
        subroutine plvpor( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
            real(kind=plflt) :: xmin, xmax, ymin, ymax
        end subroutine plvpor
    end interface

    interface
        subroutine plvsta
        end subroutine plvsta
    end interface

    interface
        subroutine plw3d( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az )
            use plplot_types
            implicit none
            real(kind=plflt) :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az
        end subroutine plw3d
    end interface

    interface
        subroutine plwidth( width )
            use plplot_types
            implicit none
            real(kind=plflt) :: width
        end subroutine plwidth
    end interface

    interface
        subroutine plwind( xmin, xmax, ymin, ymax )
            use plplot_types
            implicit none
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
    integer(kind=plint) function convert_to_int( logvalue )
       logical :: logvalue
       if ( logvalue ) then
         convert_to_int = 1_plint
       else
         convert_to_int = 0_plint
       endif
    end function convert_to_int

    logical function convert_to_log( intvalue )
       integer(kind=plint) :: intvalue
       convert_to_log = intvalue.ne.0_plint
    end function convert_to_log

    subroutine plbin( x, y, center )
       real(kind=plflt), dimension(:) :: x, y
       integer(kind=plint)                        :: center

       call plbinf95( size(x,kind=plint), x, y, center )
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
      integer(kind=plint) :: opt, position, bg_color, bb_color, bb_style, cont_color
      integer(kind=plint) :: n_labels, n_axes
      real (kind=plflt) :: x, y, x_length, y_length, low_cap_color, high_cap_color, cont_width
      integer(kind=plint), dimension(:) :: label_opts, sub_ticks, n_values
      real (kind=plflt), dimension(:) :: ticks
      real (kind=plflt), dimension(:,:) :: values
      character(len=*), dimension(:) :: labels, axis_opts

      !
      ! Convert the text arrays and store the results in a convenient
      ! albeit global location. This way we avoid all manner of complications.
      ! (Though introducing a potentially nasty one: non-threadsafety)
      !
      call pllegend07_cnv_text( 3_plint, n_labels, labels )
      call pllegend07_cnv_text( 4_plint, n_axes, axis_opts )

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
      integer(kind=plint) :: opt, position, bg_color, bb_color, bb_style, cont_color
      real (kind=plflt) :: x, y, x_length, y_length, low_cap_color, high_cap_color, cont_width
      integer(kind=plint), dimension(:) :: label_opts, sub_ticks, n_values
      real (kind=plflt), dimension(:) :: ticks
      real (kind=plflt), dimension(:,:) :: values
      character(len=*), dimension(:) :: labels, axis_opts

      integer(kind=plint) :: n_labels, n_axes

      n_labels = size(label_opts,1,kind=plint)
      n_axes = size(axis_opts,1,kind=plint)
      !
      ! Convert the text arrays and store the results in a convenient
      ! albeit global location. This way we avoid all manner of complications.
      ! (Though introducing a potentially nasty one: non-threadsafety)
      !
      call pllegend07_cnv_text( 3_plint, n_labels, labels )
      call pllegend07_cnv_text( 4_plint, n_axes, axis_opts )

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
       integer(kind=plint)                        :: iplsr
       logical                        :: flags

       integer(kind=plint)                        :: iflags

       iflags = convert_to_int( flags )
       call plcpstrmf95( iplsr, iflags )
    end subroutine plcpstrm

    subroutine plerrx( xmin, xmax, y )
       real(kind=plflt), dimension(:) :: xmin, xmax, y

       call plerrxf95( size(xmin,kind=plint), xmin, xmax, y )
    end subroutine plerrx

    subroutine plerry( x, ymin, ymax )
       real(kind=plflt), dimension(:) :: x, ymin, ymax

       call plerryf95( size(x,kind=plint), x, ymin, ymax )
    end subroutine plerry

    subroutine plfill( x, y )
       real(kind=plflt), dimension(:) :: x, y

       call plfillf95( size(x,kind=plint), x, y )
    end subroutine plfill

    subroutine plfill3( x, y, z )
       real(kind=plflt), dimension(:) :: x, y, z

       call plfill3f95( size(x,kind=plint), x, y, z )
    end subroutine plfill3

    subroutine plgradient( x, y, angle )
       real(kind=plflt), dimension(:) :: x, y
       real(kind=plflt)               :: angle

       call plgradientf95( size(x,kind=plint), x, y, angle )
    end subroutine plgradient

    subroutine plgriddata( x, y, z, xg, yg, zg, type, data )
       real(kind=plflt), dimension(:)   :: x, y, z, xg, yg
       real(kind=plflt), dimension(:,:) :: zg
       real(kind=plflt)                 :: data
       integer(kind=plint)                          :: type

       call plgriddataf95( x, y, z, size(x,kind=plint), xg, size(xg,kind=plint), yg, size(yg,kind=plint), zg, &
          type, data )

       return
    end subroutine plgriddata

    subroutine plhist( data, datmin, datmax, nbin, oldwin )
       real(kind=plflt), dimension(:) :: data
       real(kind=plflt)               :: datmin, datmax
       integer(kind=plint)                        :: nbin, oldwin

       call plhistf95( size(data,kind=plint), data, datmin, datmax, nbin, oldwin )
    end subroutine plhist

!     subroutine plimagefr( idata, xmin, xmax, ymin, ymax, zmin, zmax, &
!                           dxmin, dxmax, dymin, dymax, valuemin, valuemax )
!        real(kind=plflt), dimension(:,:) :: idata
!        real(kind=plflt)                 :: xmin,  xmax,  ymin,  ymax,  zmin, zmax
!        real(kind=plflt)                 :: dxmin, dxmax, dymin, dymax, &
!                                            valuemin, valuemax
!
!        integer(kind=plint)                          :: nx, ny
!
!        nx = size(idata,1,kind=plint)
!        ny = size(idata,2,kind=plint)
!        call plimagefrf95( idata, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, &
!                           dxmin, dxmax, dymin, dymax, valuemin, valuemax )
!     end subroutine plimagefr

    subroutine plimage( idata, xmin, xmax, ymin, ymax, zmin, zmax, &
                        dxmin, dxmax, dymin, dymax )
       real(kind=plflt), dimension(:,:) :: idata
       real(kind=plflt)                 :: xmin,  xmax,  ymin,  ymax,  zmin, zmax
       real(kind=plflt)                 :: dxmin, dxmax, dymin, dymax

       integer(kind=plint)                          :: nx, ny

       nx = size(idata,1,kind=plint)
       ny = size(idata,2,kind=plint)
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
    integer(kind=plint)                        :: position, opt, bg_color, bb_color, bb_style
    integer(kind=plint)                        :: nrow, ncolumn, nlegend

    character(len=*), dimension(:) :: text, symbols

    integer(kind=plint), dimension(:)          :: opt_array, text_colors, box_colors
    integer(kind=plint), dimension(:)          :: box_patterns
    real(kind=plflt), dimension(:) :: box_line_widths
    integer(kind=plint), dimension(:)          :: line_colors, line_styles
    real(kind=plflt), dimension(:) :: line_widths
    integer(kind=plint), dimension(:)          :: symbol_colors, symbol_numbers
    real(kind=plflt), dimension(:) :: box_scales, symbol_scales

    !
    ! Convert the text arrays and store the results in a convenient
    ! albeit global location. This way we avoid all manner of complications.
    ! (Though introducing a potentially nasty one: non-threadsafety)
    !
    call pllegend07_cnv_text( 1_plint, nlegend, text )
    call pllegend07_cnv_text( 2_plint, nlegend, symbols )

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
    integer(kind=plint)                        :: position, opt, bg_color, bb_color, bb_style
    integer(kind=plint)                        :: nrow, ncolumn

    character(len=*), dimension(:) :: text, symbols

    integer(kind=plint), dimension(:)          :: opt_array, text_colors, box_colors
    integer(kind=plint), dimension(:)          :: box_patterns
    real(kind=plflt), dimension(:) :: box_line_widths
    integer(kind=plint), dimension(:)          :: line_colors, line_styles
    real(kind=plflt), dimension(:) :: line_widths
    integer(kind=plint), dimension(:)          :: symbol_colors, symbol_numbers
    real(kind=plflt), dimension(:) :: box_scales, symbol_scales

    integer(kind=plint)                        :: nlegend

    !
    ! Determine number of legend entries
    !
    nlegend = min( size(opt_array,kind=plint), size(text,kind=plint) )

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

       call pllinef95( size(x,kind=plint), x, y )
    end subroutine plline

    subroutine plline3( x, y, z )
       real(kind=plflt), dimension(:) :: x, y, z

       call plline3f95( size(x,kind=plint), x, y, z )
    end subroutine plline3

    subroutine plmap1(mapform,mapname,minx,maxx,miny,maxy)
      real(kind=plflt) minx, maxx, miny, maxy
      character*(*) mapname
      external mapform

      call plstrf2c(mapname, string1)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      call plmap7(s1,minx,maxx,miny,maxy)

    end subroutine plmap1

    subroutine plmap2(mapname,minx,maxx,miny,maxy)
      real(kind=plflt) minx, maxx, miny, maxy
      character*(*) mapname

      call plstrf2c(mapname, string1)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      call plmap7(s1,minx,maxx,miny,maxy)

    end subroutine plmap2

    subroutine plmapfill1(mapform,mapname,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname
      external mapform
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      if ( present(plotentries) ) then
        call plmapfill7(s1,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapfill7(s1,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapfill1

    subroutine plmapfill2(mapname,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      if ( present(plotentries) ) then
        call plmapfill7(s1,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapfill7(s1,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapfill2

    subroutine plmapline1(mapform,mapname,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname
      external mapform
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      if ( present(plotentries) ) then
        call plmapline7(s1,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapline7(s1,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapline1

    subroutine plmapline2(mapname,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      if ( present(plotentries) ) then
        call plmapline7(s1,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapline7(s1,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapline2

    subroutine plmapstring1(mapform,mapname,string,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname, string
      external mapform
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)
      call plstrf2c(string, string2)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      if ( present(plotentries) ) then
        call plmapstring7(s1,s2,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapstring7(s1,s2,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapstring1

    subroutine plmapstring2(mapname,string,minx,maxx,miny,maxy,plotentries)
      real(kind=plflt) minx, maxx, miny, maxy
      integer(kind=plint), dimension(:), optional :: plotentries
      character*(*) mapname, string
      integer(kind=plint), dimension(1) :: dummy

      call plstrf2c(mapname, string1)
      call plstrf2c(string, string2)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      if ( present(plotentries) ) then
        call plmapstring7(s1,s2,minx,maxx,miny,maxy,plotentries,size(plotentries,kind=plint))
      else
        call plmapstring7(s1,s2,minx,maxx,miny,maxy,dummy,0_plint)
      endif

    end subroutine plmapstring2

    subroutine plmaptex1(mapform,mapname,dx,dy,just,text,minx,maxx,miny,maxy,plotentry)
      real(kind=plflt) dx, dy, just, minx, maxx, miny, maxy
      integer(kind=plint) :: plotentry
      character*(*) mapname, text
      external mapform
      integer(kind=plint) :: dummy

      call plstrf2c(mapname, string1)
      call plstrf2c_esc(text, string2)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plmaptex7(s1,dx,dy,just,s2,minx,maxx,miny,maxy,plotentry)

    end subroutine plmaptex1

    subroutine plmaptex2(mapname,dx,dy,just,text,minx,maxx,miny,maxy,plotentry)
      real(kind=plflt) dx, dy, just, minx, maxx, miny, maxy
      integer(kind=plint) :: plotentry
      character*(*) mapname, text
      integer(kind=plint) :: dummy

      call plstrf2c(mapname, string1)
      call plstrf2c_esc(text, string2)

      call plclearmapformc()
      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plmaptex7(s1,dx,dy,just,s2,minx,maxx,miny,maxy,plotentry)

    end subroutine plmaptex2

    subroutine plmeridians1(mapform,dlong,dlat,minlong,maxlong, &
                           minlat,maxlat)

    real(kind=plflt) dlong, dlat, minlong, maxlong, minlat, maxlat
    external mapform

    call plsetmapformc(mapform)
    call plmeridians7(dlong,dlat,minlong,maxlong,minlat,maxlat)

    end subroutine plmeridians1

    subroutine plmeridians2(dlong,dlat,minlong,maxlong, &
                           minlat,maxlat)

    real(kind=plflt) dlong, dlat, minlong, maxlong, minlat, maxlat

    call plclearmapformc
    call plmeridians7(dlong,dlat,minlong,maxlong,minlat,maxlat)

    end subroutine plmeridians2

    subroutine plmesh( x, y, z, opt )
       integer(kind=plint)                          :: opt
       real(kind=plflt), dimension(:)   :: x, y
       real(kind=plflt), dimension(:,:) :: z

       call plmeshf95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, size(x,kind=plint))

    end subroutine plmesh

    subroutine plmeshc( x, y, z, opt, clevel )
       integer(kind=plint)                          :: opt
       real(kind=plflt), dimension(:)   :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plmeshcf95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, &
         clevel, size(clevel,kind=plint), size(x,kind=plint))

    end subroutine plmeshc

    subroutine plot3d( x, y, z, opt, side )
       integer(kind=plint)                          :: opt
       logical                          :: side
       real(kind=plflt), dimension(:)   :: x, y
       real(kind=plflt), dimension(:,:) :: z
       integer(kind=plint)                          :: iside

       iside = convert_to_int(side)
       call plot3df95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, iside, size(x,kind=plint))

    end subroutine plot3d

    subroutine plot3dc( x, y, z, opt, clevel )
       integer(kind=plint)                          :: opt
       real(kind=plflt), dimension(:)   :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plot3dcf95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, clevel, &
         size(clevel,kind=plint), size(x,kind=plint))

    end subroutine plot3dc

    subroutine plspause( lpause )
       logical                        :: lpause

       integer(kind=plint)                        :: ipause

       ipause = convert_to_int( lpause )
       call plspausef95( ipause )
    end subroutine plspause

    subroutine plsurf3d( x, y, z, opt, clevel )
       integer(kind=plint)                        :: opt
       real(kind=plflt), dimension(:) :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z

       call plsurf3df95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, clevel, &
         size(clevel,kind=plint), size(x,kind=plint))

    end subroutine plsurf3d

    subroutine plsurf3dl( x, y, z, opt, clevel, indexxmin, indexxmax, indexymin, indexymax )
       integer(kind=plint)                        :: opt
       real(kind=plflt), dimension(:) :: x, y, clevel
       real(kind=plflt), dimension(:,:) :: z
       integer(kind=plint)                        :: indexxmin, indexxmax
       integer(kind=plint), dimension(:)          :: indexymin, indexymax

       integer(kind=plint), dimension(size(x))    :: indexyminm1, indexymaxm1

       indexyminm1 = indexymin - 1
       indexymaxm1 = indexymax - 1

       call plsurf3dlf95( x, y, z, size(x,kind=plint), size(y,kind=plint), opt, clevel, &
         size(clevel,kind=plint), size(x,kind=plint), indexxmin-1, indexxmax-1, indexyminm1, indexymaxm1 )

    end subroutine plsurf3dl

    subroutine plpoin( x, y, code )
       integer(kind=plint)                        :: code
       real(kind=plflt), dimension(:) :: x, y

       call plpoinf95( size(x,kind=plint), x, y, code )
    end subroutine plpoin

    subroutine plpoin3( x, y, z, code )
       integer(kind=plint)                        :: code
       real(kind=plflt), dimension(:) :: x, y, z

       call plpoin3f95( size(x,kind=plint), x, y, z, code )
    end subroutine plpoin3

    subroutine plpoly3( x, y, z, draw, ifcc )
       logical                        :: ifcc
       logical, dimension(:)          :: draw
       real(kind=plflt), dimension(:) :: x, y, z

       integer(kind=plint), dimension(size(draw,kind=plint))  :: idraw
       integer(kind=plint)                        :: i
       integer(kind=plint)                        :: iifcc

       iifcc = convert_to_int( ifcc )
       do i = 1_plint,size(draw,kind=plint)
          idraw(i) = convert_to_int( draw(i) )
       enddo
       call plpoly3f95( size(x,kind=plint), x, y, z, idraw, iifcc )
    end subroutine plpoly3

    real (kind=plflt) function plrandd()
      external plranddf95
      real(kind=plflt) :: plranddf95

      plrandd = plranddf95()
    end function plrandd

    subroutine plscmap0( r, g, b )
       integer(kind=plint), dimension(:) :: r, g, b

       call plscmap0f95( r, g, b, size(r,kind=plint) )
    end subroutine plscmap0

    subroutine plscmap0a( r, g, b, a )
       integer(kind=plint), dimension(:) :: r, g, b
       real(kind=plflt), dimension(:) :: a

       call plscmap0af95( r, g, b, a, size(r,kind=plint) )
    end subroutine plscmap0a

    subroutine plscmap1( r, g, b )
       integer(kind=plint), dimension(:) :: r, g, b

       call plscmap1f95( r, g, b, size(r,kind=plint) )
    end subroutine plscmap1

    subroutine plscmap1a( r, g, b, a )
       integer(kind=plint), dimension(:) :: r, g, b
       real(kind=plflt), dimension(:) :: a

       call plscmap1af95( r, g, b, a, size(r,kind=plint) )
    end subroutine plscmap1a

    subroutine plscmap1l( rgbtype, intensity, coord1, coord2, coord3, alt_hue_path)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3
          logical, dimension(:)          :: alt_hue_path

          integer(kind=plint), dimension(size(alt_hue_path,kind=plint))  :: ialt_hue_path
          integer(kind=plint)                        :: i
          integer(kind=plint)                        :: type

       type = convert_to_int( rgbtype )
       do i = 1_plint,size(alt_hue_path,kind=plint)
          ialt_hue_path(i) = convert_to_int( alt_hue_path(i) )
       enddo
       call plscmap1lf95( type, size(intensity,kind=plint), intensity, coord1, coord2, coord3, ialt_hue_path )
    end subroutine plscmap1l

    subroutine plscmap1l2( rgbtype, intensity, coord1, coord2, coord3)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3

          integer(kind=plint)                        :: type

       type = convert_to_int( rgbtype )
       call plscmap1l2f95( type, size(intensity,kind=plint), intensity, coord1, coord2, coord3)
    end subroutine plscmap1l2

    subroutine plscmap1la( rgbtype, intensity, coord1, coord2, coord3, a, alt_hue_path)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a
          logical, dimension(:)          :: alt_hue_path

          integer(kind=plint), dimension(size(alt_hue_path,kind=plint))  :: ialt_hue_path
          integer(kind=plint)                        :: i
          integer(kind=plint)                        :: type

       type = convert_to_int( rgbtype )
       do i = 1_plint,size(alt_hue_path,kind=plint)
          ialt_hue_path(i) = convert_to_int( alt_hue_path(i) )
       enddo
       call plscmap1laf95( type, size(intensity,kind=plint), intensity, coord1, coord2, coord3, a, ialt_hue_path )
    end subroutine plscmap1la

    subroutine plscmap1la2( rgbtype, intensity, coord1, coord2, coord3, a)
          logical                        :: rgbtype
          real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a

          integer(kind=plint)                        :: type

       type = convert_to_int( rgbtype )
       call plscmap1la2f95( type, size(intensity,kind=plint), intensity, coord1, coord2, coord3, a)
    end subroutine plscmap1la2

    subroutine plstripc(id, xspec, yspec, xmin, xmax, xjump, &
      ymin, ymax, xlpos, ylpos, y_ascl, acc, &
      colbox, collab, colline, styline, legline, &
      labx, laby, labtop)

    integer(kind=plint) id, colbox, collab, colline(4), styline(4)
    character*(*) xspec, yspec, legline(4), labx, laby, labtop
    real(kind=plflt) xmin, xmax, xjump, ymin, ymax, xlpos, ylpos
    logical y_ascl, acc
    integer(kind=plint) iy_ascl, iacc


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

    subroutine plsvect1( arrowx, arrowy, fill )
         logical                        :: fill
         real(kind=plflt), dimension(:) :: arrowx, arrowy
         integer(kind=plint) ifill
         ifill = convert_to_int(fill)

       call plsvect1f95( arrowx, arrowy, size(arrowx,kind=plint), ifill )
    end subroutine plsvect1

    subroutine plsym( x, y, code )
       integer(kind=plint)                        :: code
       real(kind=plflt), dimension(:) :: x, y

       call plsymf95( size(x,kind=plint), x, y, code )
    end subroutine plsym

    subroutine plxormod( mode, status )
       logical :: mode, status
       integer(kind=plint) :: imode, istatus
       imode = convert_to_int(mode)
       call plxormodf95( imode, istatus)
       status = convert_to_log(istatus)
    end subroutine plxormod
end module plplot
