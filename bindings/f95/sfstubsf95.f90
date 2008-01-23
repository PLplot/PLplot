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
!  it under the terms of the GNU General Library Public License as published
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
!  - it includes the actual FORTRAN routines from the FORTRAN 77 bindings
!  - it includes interfaces to the C routines from these bindings
!  - it defines a few Fortran 95 specific items and interfaces
!
!  NB
!  This module is written in fixed form, because that way we can reuse
!  the FORTRAN 77 bindings (including the original file is not possible:
!  the "end" statement has to be replaced by the "end subroutine"
!  statement)
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

      module plplotp
         use plplot_flt
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
      contains
         include 'sfstubs.f90'
      end module plplotp

      module plplot
      use plplotp
      use plplot_flt
      !
      ! To be added: renaming list
      !

      implicit none

      !
      ! Parameters connecting a mnemonic name to the
      ! constants used by various routines
      !
      integer, parameter :: PL_PARSE_FULL = 1

      integer, parameter :: PL_GRID_CSA    = 1
      integer, parameter :: PL_GRID_DTLI   = 2
      integer, parameter :: PL_GRID_NNI    = 3
      integer, parameter :: PL_GRID_NNIDW  = 4
      integer, parameter :: PL_GRID_NNLI   = 5
      integer, parameter :: PL_GRID_NNAIDW = 6

      integer, parameter :: PL_JUST_NONE      = -1
      integer, parameter :: PL_JUST_OPTIMAL   =  0
      integer, parameter :: PL_JUST_SAME      =  1
      integer, parameter :: PL_JUST_ISOMETRIC =  2

      integer, parameter :: PL_AXIS_NOTHING    = -2
      integer, parameter :: PL_AXIS_BOX_ONLY   = -1
      integer, parameter :: PL_AXIS_NORMAL     =  0
      integer, parameter :: PL_AXIS_ATZERO     =  1
      integer, parameter :: PL_AXIS_MAJORGRID  =  2
      integer, parameter :: PL_AXIS_MINORGRID  =  3
      integer, parameter :: PL_AXIS_LOGX       = 10
      integer, parameter :: PL_AXIS_LOGY       = 20
      integer, parameter :: PL_AXIS_LOGXY      = 30

      integer, parameter :: PL_FONT_STANDARD   =  0
      integer, parameter :: PL_FONT_EXTENDED   =  1

      !
      ! TODO:
      ! Parameters for fill patterns and symbols
      !

      real(kind=plflt), parameter :: PL_PI = 3.1415926535897932384_plflt

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
         subroutine plcol( icol )
            integer :: icol
         end subroutine plcol
      end interface

      interface
         subroutine plcol1( col )
            use plplot_flt
            real(kind=plflt) :: col
         end subroutine plcol1
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
         subroutine plgfam( fam, num, bmax )
            integer :: fam, num, bmax
         end subroutine plgfam
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
         subroutine plsmaj( def, scale )
            use plplot_flt
            real(kind=plflt) :: def, scale
         end subroutine plsmaj
      end interface

      ! plsmem: void * argument tricky - TODO

      interface
         subroutine plsmin( def, scale )
            use plplot_flt
            real(kind=plflt) :: def, scale
         end subroutine plsmin
      end interface

      interface
         subroutine plsori( rot )
            use plplot_flt
            real(kind=plflt) :: rot
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
         subroutine plstar( x, y )
            use plplot_flt
            real(kind=plflt) :: x, y
         end subroutine plstar
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
         subroutine plwid( width )
            integer :: width
         end subroutine plwid
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

         call plbinf77( size(x), x, y, center )
      end subroutine plbin

      subroutine plcpstrm( iplsr, flags )
         integer                        :: iplsr
         logical                        :: flags

         integer                        :: iflags

         iflags = convert_to_int( flags )
         call plcpstrmf77( iplsr, iflags )
      end subroutine plcpstrm

      subroutine plerrx( xmin, xmax, y )
         real(kind=plflt), dimension(:) :: xmin, xmax, y

         call plerrxf77( size(xmin), xmin, xmax, y )
      end subroutine plerrx

      subroutine plerry( x, ymin, ymax )
         real(kind=plflt), dimension(:) :: x, ymin, ymax

         call plerryf77( size(x), x, ymin, ymax )
      end subroutine plerry

      subroutine plfill( x, y )
         real(kind=plflt), dimension(:) :: x, y

         call plfillf77( size(x), x, y )
      end subroutine plfill

      subroutine plfill3( x, y, z )
         real(kind=plflt), dimension(:) :: x, y, z

         call plfill3f77( size(x), x, y, z )
      end subroutine plfill3

      subroutine plgriddata( x, y, z, xg, yg, zg, type, data )
         real(kind=plflt), dimension(:)   :: x, y, z, xg, yg, data
         real(kind=plflt), dimension(:,:) :: zg
         integer                          :: type

         call plgriddataf77( x, y, z, size(x), xg, size(xg), yg, size(yg), zg, &
            type, data )

         return
      end subroutine plgriddata

      subroutine plhist( data, datmin, datmax, nbin, oldwin )
         real(kind=plflt), dimension(:) :: data
         real(kind=plflt)               :: datmin, datmax
         integer                        :: nbin, oldwin

         call plhistf77( size(data), data, datmin, datmax, nbin, oldwin )
      end subroutine plhist

      subroutine plline( x, y )
         real(kind=plflt), dimension(:) :: x, y

         call pllinef77( size(x), x, y )
      end subroutine plline

      subroutine plline3( x, y, z )
         real(kind=plflt), dimension(:) :: x, y, z

         call plline3f77( size(x), x, y, z )
      end subroutine plline3

      subroutine plmesh( x, y, z, opt )
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y
         real(kind=plflt), dimension(:,:) :: z

         call plmeshf77( x, y, z, size(x), size(y), opt, size(x))

      end subroutine plmesh

      subroutine plmeshc( x, y, z, opt, clevel )
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y, clevel
         real(kind=plflt), dimension(:,:) :: z

	 call plmeshcf77( x, y, z, size(x), size(y), opt, &
	   clevel, size(clevel), size(x))

      end subroutine plmeshc

      subroutine plot3d( x, y, z, opt, side )
         integer                          :: opt
         logical                          :: side
         real(kind=plflt), dimension(:)   :: x, y
         real(kind=plflt), dimension(:,:) :: z
	 integer                          :: iside

         iside = convert_to_int(side)
	 call plot3df77( x, y, z, size(x), size(y), opt, iside, size(x))

      end subroutine plot3d

      subroutine plot3dc( x, y, z, opt, clevel )
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y, clevel
         real(kind=plflt), dimension(:,:) :: z

         call plot3dcf77( x, y, z, size(x), size(y), opt, clevel, &
           size(clevel), size(x))

      end subroutine plot3dc

      subroutine plspause( lpause )
         logical                        :: lpause

         integer                        :: ipause

         ipause = convert_to_int( lpause )
         call plspausef77( ipause )
      end subroutine plspause

      subroutine plsurf3d( x, y, z, opt, clevel )
         integer                        :: opt
         real(kind=plflt), dimension(:) :: x, y, clevel
	 real(kind=plflt), dimension(:,:) :: z

         call plsurf3df77( x, y, z, size(x), size(y), opt, clevel, &
           size(clevel), size(x))

      end subroutine plsurf3d

      subroutine plpoin( x, y, code )
         integer                        :: code
         real(kind=plflt), dimension(:) :: x, y

         call plpoinf77( size(x), x, y, code )
      end subroutine plpoin

      subroutine plpoin3( x, y, z, code )
         integer                        :: code
         real(kind=plflt), dimension(:) :: x, y, z

         call plpoin3f77( size(x), x, y, z, code )
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
         call plpoly3f77( size(x), x, y, z, idraw, iifcc )
      end subroutine plpoly3

      subroutine plscmap0( r, g, b )
         integer, dimension(:) :: r, g, b

         call plscmap0f77( r, g, b, size(r) )
      end subroutine plscmap0

      subroutine plscmap0a( r, g, b, a )
         integer, dimension(:) :: r, g, b
         real(kind=plflt) :: a

         call plscmap0af77( r, g, b, a, size(r) )
      end subroutine plscmap0a

      subroutine plscmap1( r, g, b )
         integer, dimension(:) :: r, g, b

         call plscmap1f77( r, g, b, size(r) )
      end subroutine plscmap1

      subroutine plscmap1a( r, g, b, a )
         integer, dimension(:) :: r, g, b
         real(kind=plflt) :: a

         call plscmap1af77( r, g, b, a, size(r) )
      end subroutine plscmap1a

      subroutine plscmap1l( rgbtype, intensity, coord1, coord2, coord3, rev)
            logical                        :: rgbtype
            real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3
            logical, dimension(:)          :: rev

            integer, dimension(size(rev))  :: irev
            integer                        :: i
            integer                        :: type

         type = convert_to_int( rgbtype )
         do i = 1,size(rev)
            irev(i) = convert_to_int( rev(i) )
         enddo
         call plscmap1lf77( type, size(intensity), intensity, coord1, coord2, coord3, irev )
      end subroutine plscmap1l

      subroutine plscmap1l2( rgbtype, intensity, coord1, coord2, coord3)
            logical                        :: rgbtype
            real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3

            integer                        :: type

         type = convert_to_int( rgbtype )
         call plscmap1l2f77( type, size(intensity), intensity, coord1, coord2, coord3)
      end subroutine plscmap1l2

      subroutine plscmap1la( rgbtype, intensity, coord1, coord2, coord3, a, rev)
            logical                        :: rgbtype
            real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a
            logical, dimension(:)          :: rev

            integer, dimension(size(rev))  :: irev
            integer                        :: i
            integer                        :: type

         type = convert_to_int( rgbtype )
         do i = 1,size(rev)
            irev(i) = convert_to_int( rev(i) )
         enddo
         call plscmap1laf77( type, size(intensity), intensity, coord1, coord2, coord3, a, irev )
      end subroutine plscmap1la

      subroutine plscmap1la2( rgbtype, intensity, coord1, coord2, coord3, a)
            logical                        :: rgbtype
            real(kind=plflt), dimension(:) :: intensity, coord1, coord2, coord3, a

            integer                        :: type

         type = convert_to_int( rgbtype )
         call plscmap1la2f77( type, size(intensity), intensity, coord1, coord2, coord3, a)
      end subroutine plscmap1la2

      subroutine plstripc(id, xspec, yspec, xmin, xmax, xjump, &
        ymin, ymax, xlpos, ylpos, y_ascl, acc, &
        colbox, collab, colline, styline, legline, &
        labx, laby, labtop)

      implicit none
      integer id, colbox, collab, colline(4), styline(4)
      character*(*) xspec, yspec, legline(4), labx, laby, labtop
      real(kind=plflt) xmin, xmax, xjump, ymin, ymax, xlpos, ylpos
      integer nx, ny
      logical y_ascl, acc
      integer iy_ascl, iacc

      include 'sfstubs.h'

      call plstrf2c(xspec, string1, maxlen)
      call plstrf2c(yspec, string2, maxlen)
      call plstrf2c(legline(1), string3, maxlen)
      call plstrf2c(legline(2), string4, maxlen)
      call plstrf2c(legline(3), string5, maxlen)
      call plstrf2c(legline(4), string6, maxlen)
      call plstrf2c(labx, string7, maxlen)
      call plstrf2c(laby, string8, maxlen)
      call plstrf2c(labtop, string9, maxlen)

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
      call plstripcf77(id, s1, s2, xmin, xmax, xjump, &
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

         call plsvectf77( arrowx, arrowy, size(arrowx), ifill )
      end subroutine plsvect

      subroutine plsym( x, y, code )
         integer                        :: code
         real(kind=plflt), dimension(:) :: x, y

         call plsymf77( size(x), x, y, code )
      end subroutine plsym

      subroutine plxormod( mode, status )
         logical :: mode, status
         integer :: imode, istatus
         imode = convert_to_int(mode)
         call plxormodf77( imode, istatus)
         status = convert_to_log(istatus)
      end subroutine plxormod
      end module plplot

