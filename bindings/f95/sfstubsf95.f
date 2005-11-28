!***********************************************************************
!  $Id$
!  sfstubsf90.f
!
!  Copyright (C) 2005  Arjen Markus
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
!  This file contains the interfaces for Fortran 90:
!  - it includes the actual FORTRAN routines from the FORTRAN 77 bindings
!  - it includes interfaces to the C routines from these bindings
!  - it defines a few Fortran 90 specific items and interfaces
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
      contains
         include 'sfstubs.f'
      end module plplotp

      module plplot
!     use plplotp,  plbin_f77   => plbin,     &
!                   plerrx_f77  => plerrx,    &
!                   plerry_f77  => plerry,    &
!                   plfill_f77  => plfill,    &
!                   plfill3_f77 => plfill3,   &
!                   plpoin_f77  => plpoin
      use plplot_flt
      use plplotp
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
         subroutine plbin_f77( nbin, x, y, center )
            use plplot_flt
            integer                        :: nbin, center
            real(kind=plflt), dimension(*) :: x, y
         end subroutine plbin_f77
         module procedure plbin_f77_log
         module procedure plbin_f95_int
         module procedure plbin_f95_log
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

      interface
         subroutine plcpstrm( iplsr, flags )
            integer          :: iplsr, flags
         end subroutine plcpstrm
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
         subroutine plerrx_f77( n, xmin, xmax, y )
            use plplot_flt
            real(kind=plflt), dimension(*) :: xmin, xmax, y
         end subroutine plerrx_f77
         module procedure plerrx_f95
      end interface

      interface plerry
         subroutine plerry_f77( n, x, ymin, ymax )
            use plplot_flt
            real(kind=plflt), dimension(*) :: x, ymin, ymax
         end subroutine plerry_f77
         module procedure plerry_f95
      end interface

      interface plfamadv
         subroutine plfamadv
         end subroutine plfamadv
      end interface

      interface plfill
         subroutine plfill_f77( n, x, y )
            use plplot_flt
            real(kind=plflt), dimension(*) :: x, y
         end subroutine plfill_f77
         module procedure plfill_f95
      end interface

      interface plfill3
         subroutine plfill3_f77( n, x, y, z )
            use plplot_flt
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plfill3_f77
         module procedure plfill_f95
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
         subroutine plgcolbg( r, g, b )
            integer :: r, g, b
         end subroutine plgcolbg
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
         subroutine plgriddata_f77( x, y, z, npts, xg, nptsx,
     &                              yg, nptsy, zg, type, data )
            use plplot_flt
            real(kind=plflt), dimension(*) :: x, y, z, xg, yg, zg, data
            integer                        :: type
            ! ERROR in scstubs.c !
         end subroutine plgriddata_f77
         module procedure plgriddata_f95
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
         subroutine plhist_f77( n, data, datmin, datmax, nbin, oldwin )
            use plplot_flt
            real(kind=plflt), dimension(*) :: data
            real(kind=plflt)               :: datmin, datmax
            integer                        :: n, nbin, oldwin
         end subroutine plhist_f77
         module procedure plhist_f95
      end interface

      interface
         subroutine plhls( h, l, s )
            use plplot_flt
            real(kind=plflt) :: h, l, s
         end subroutine plhls
      end interface

      interface
         subroutine plhlsrgb( r, g, b, h, l, s )
            use plplot_flt
            real(kind=plflt) :: r, g, b, h, l, s
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
         subroutine plline_f77( n, x, y )
            use plplot_flt
            integer                        :: n
            real(kind=plflt), dimension(*) :: x, y
         end subroutine plline_f77
         module procedure plline_f95
      end interface

      interface plline3
         subroutine plline3_f77( n, x, y, z )
            use plplot_flt
            integer                        :: n
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plline3_f77
         module procedure plline3_f95
      end interface

      interface pllsty
         subroutine pllsty( lin )
            integer :: lin
         end subroutine pllsty
      end interface

      interface plmesh
         subroutine plmesh_f77( x, y, z, nx, ny, opt,
     &                          lx )
            use plplot_flt
            integer                        :: nx, ny, lx,
     &                                        opt
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plmesh_f77
         module procedure plmesh_f95
      end interface

      interface plmeshc
         subroutine plmeshc_f77( x, y, z, nx, ny, opt,
     &                           clevel, nlevel, lx )
            use plplot_flt
            integer                        :: nx, ny, lx,
     &                                        opt, nlevel
            real(kind=plflt), dimension(*) :: x, y, z, clevel
         end subroutine plmeshc_f77
         module procedure plmeshc_f95
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
         subroutine plot3d_f77( x, y, z, nx, ny, opt, side,
     &                          lx )
            use plplot_flt
            integer                        :: nx, ny, lx,
     &                                        opt, side
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plot3d_f77
         module procedure plot3d_f95
      end interface

      interface plot3dc
         subroutine plot3dc_f77( x, y, z, nx, ny, opt, clevel,
     &                           nlevel, lx )
            use plplot_flt
            integer                        :: nx, ny, nlevel, lx,
     &                                        opt
            real(kind=plflt), dimension(*) :: x, y, z, clevel
         end subroutine plot3dc_f77
         module procedure plot3dc_f95
      end interface

      interface plpoin
         subroutine plpoin_f77( n, x, y, code )
            use plplot_flt
            integer                        :: n, code
            real(kind=plflt), dimension(*) :: x, y
         end subroutine plpoin_f77
         module procedure plpoin_f95
      end interface

      interface plpoin3
         subroutine plpoin3_f77( n, x, y, z, code )
            use plplot_flt
            integer                        :: n, code
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plpoin3_f77
         module procedure plpoin3_f95
      end interface

      interface plpoly3
         subroutine plpoly3_f77( n, x, y, z, draw, ifcc )
            use plplot_flt
            integer                        :: ifcc
            integer, dimension(*)          :: draw
            real(kind=plflt), dimension(*) :: x, y, z
         end subroutine plpoly3_f77
         module procedure plpoly3_f95
      end interface

      interface
         subroutine plrec( setp, prec )
            integer :: setp, prec
         end subroutine plrec
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

      interface plsmap0
         subroutine plsmap0_f77( r, g, b, ncol0)
            integer, dimension(*) :: r, g, b
            integer               :: ncol0
         end subroutine plsmap0_f77
         module procedure plsmap0_f95
      end interface

      interface
         subroutine plsmap0n( n )
            integer :: n
         end subroutine plsmap0n
      end interface

      interface
         subroutine plscol0( icol, r, g, b )
            integer :: icol, r, g, b
         end subroutine plscol0
      end interface

      interface
         subroutine plscolbg( r, g, b )
            integer :: r, g, b
         end subroutine plscolbg
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
         subroutine plsdimap( dimxmi, dimxmax, diymin, dimymax,
     &                         dimxpmm, diypmm )
            use plplot_flt
            real(kind=plflt) :: dimxmi, dimxmax, diymin, dimymax,
     &                           dimxpmm, diypmm
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
         subroutine plsetmapformc_f77( mapform )
            use plplot_flt
            interface
               subroutine mapform( n, x, y )
                  use plplot_flt
                  integer                        :: n
                  real(kind=plflt), dimension(*) :: x, y
               end subroutine mapform
            end interface
         end subroutine plsetmapformc_f77
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

      interface
         subroutine plspause( pause )
            integer :: pause
         end subroutine plspause
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
         subroutine plsurf3d_f77( x, y, z, nx, ny, opt, clevel,
     &                             nlevel, lx )
            use plplot_flt
            integer                        :: nx, ny, nlevel, lx
            real(kind=plflt), dimension(*) :: x, y, z, clevel
         end subroutine plsurf3d_f77
         module procedure plsurf3d_f95
      end interface

      interface plsvect
         subroutine plsvect_f77( arrowx, arrowy, npts, fill )
            use plplot_flt
            real(kind=plflt), dimension(*) :: arrowx, arrowy
            integer                        :: npts, fill
         end subroutine plsvect_f77
         module procedure plsvect_f95
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
         subroutine plw3d( basex, basey, height, xmin, xmax,
     &                      ymin, ymax, zmin, zmax, alt, az )
            use plplot_flt
            real(kind=plflt) :: basex, basey, height, xmin, xmax,
     &                           ymin, ymax, zmin, zmax, alt, az
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

      interface
         subroutine plxormod( mode, status )
            integer :: mode, status
         end subroutine plxormod
      end interface


      private :: convert_to_int

! -------------------------------------------------------------------
      contains
! -------------------------------------------------------------------
      integer function convert_to_int( logvalue )
         logical :: logvalue
         convert_to_int = 0
         if ( logvalue) convert_to_int = 1
      end function convert_to_int


      subroutine plbin_f77_log( nbin, x, y, center )
         integer                        :: nbin
         logical                        :: center
         real(kind=plflt), dimension(:) :: x, y
         integer                        :: icenter
         icenter = convert_to_int( center )

         call plbin_f77( nbin, x, y, icenter )
      end subroutine plbin_f77_log

      subroutine plbin_f95_log( x, y, center )
         logical                        :: center
         real(kind=plflt), dimension(:) :: x, y
         integer                        :: icenter
         icenter = convert_to_int( center )

         call plbin_f77( size(x), x, y, icenter )
      end subroutine plbin_f95_log

      subroutine plbin_f95_int( x, y, icenter )
         real(kind=plflt), dimension(:) :: x, y
         integer                        :: icenter

         call plbin_f77( size(x), x, y, icenter )
      end subroutine plbin_f95_int


      subroutine plerrx_f95( xmin, xmax, y )
         real(kind=plflt), dimension(:) :: xmin, xmax, y

         call plerrx_f77( size(xmin), xmin, xmax, y )
      end subroutine plerrx_f95

      subroutine plerry_f95( x, ymin, ymax )
         real(kind=plflt), dimension(:) :: x, ymin, ymax

         call plerry_f77( size(x), x, ymin, ymax )
      end subroutine plerry_f95

      subroutine plfill_f95( x, y )
         real(kind=plflt), dimension(:) :: x, y

         call plfill_f77( size(x), x, y )
      end subroutine plfill_f95

      subroutine plfill3_f95( x, y, z )
         real(kind=plflt), dimension(:) :: x, y, z

         call plfill3_f77( size(x), x, y, z )
      end subroutine plfill3_f95

      subroutine plgriddata_f95( x, y, z, xg, yg, zg, type, data )
         use plplot_flt
         real(kind=plflt), dimension(:)   :: x, y, z, xg, yg, data
         real(kind=plflt), dimension(:,:) :: zg
         integer                          :: type

         ! TODO
         return
      end subroutine plgriddata_f95

      subroutine plhist_f95( data, datmin, datmax, nbin, oldwin )
         real(kind=plflt), dimension(:) :: data
         real(kind=plflt)               :: datmin, datmax
         integer                        :: nbin, oldwin

         call plhist_f77( size(data), data, datmin, datmax,
     &                    nbin, oldwin )
      end subroutine plhist_f95

      subroutine plline_f95( x, y )
         real(kind=plflt), dimension(:) :: x, y

         call plline_f77( size(x), x, y )
      end subroutine plline_f95

      subroutine plline3_f95( x, y, z )
         real(kind=plflt), dimension(:) :: x, y, z

         call plline3_f77( size(x), x, y, z )
      end subroutine plline3_f95

      subroutine plmesh_f95( x, y, z, opt )
         use plplot_flt
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y
         real(kind=plflt), dimension(:,:) :: z

         ! TODO

      end subroutine plmesh_f95

      subroutine plmeshc_f95( x, y, z, opt, clevel )
         use plplot_flt
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y, clevel
         real(kind=plflt), dimension(:,:) :: z

         ! TODO

      end subroutine plmeshc_f95

      subroutine plot3d_f95( x, y, z, opt, side )
         integer                          :: opt, side
         real(kind=plflt), dimension(:)   :: x, y
         real(kind=plflt), dimension(:,:) :: z

         ! TODO

      end subroutine plot3d_f95

      subroutine plot3dc_f95( x, y, z, opt, clevel )
         integer                          :: opt
         real(kind=plflt), dimension(:)   :: x, y, clevel
         real(kind=plflt), dimension(:,:) :: z

         ! TODO

      end subroutine plot3dc_f95

      subroutine plsurf3d_f95( x, y, z, opt, clevel )
         integer                        :: opt
         real(kind=plflt), dimension(:) :: x, y, z, clevel

         ! TODO

      end subroutine plsurf3d_f95

      subroutine plpoin_f95( x, y, code )
         integer                        :: code
         real(kind=plflt), dimension(:) :: x, y

         call plpoin_f77( size(x), x, y, code )
      end subroutine plpoin_f95

      subroutine plpoin3_f95( x, y, z, code )
         integer                        :: code
         real(kind=plflt), dimension(:) :: x, y, z

         call plpoin3_f77( size(x), x, y, z, code )
      end subroutine plpoin3_f95

      subroutine plpoly3_f95( x, y, z, draw, ifcc )
         integer                        :: ifcc
         integer, dimension(:)          :: draw
         real(kind=plflt), dimension(:) :: x, y, z

         call plpoly3_f77( size(x), x, y, z, draw, ifcc )
      end subroutine plpoly3_f95

      subroutine plsmap0_f95( r, g, b )
         integer, dimension(:) :: r, g, b

         call plsmap0_f77( r, g, b, size(r) )
      end subroutine plsmap0_f95

      subroutine plsvect_f95( arrowx, arrowy, fill )
         integer                        :: fill
         real(kind=plflt), dimension(:) :: arrowx, arrowy

         call plsvect_f77( arrowx, arrowy, size(arrowx), fill )
      end subroutine plsvect_f95

      end module plplot

