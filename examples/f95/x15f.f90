!      $Id$
!      Shade plot demo.
!      Does a variety of shade plots.
!
!      Copyright (C) 2004  Alan W. Irwin
!
!      This file is part of PLplot.
!
!      PLplot is free software; you can redistribute it and/or modify
!      it under the terms of the GNU General Library Public License as
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

      use plplot
      implicit none

      integer  xdim, ydim, XPTS, YPTS
!      xdim and ydim are the static dimensions of the 2D arrays while
!      NX and NY are the defined area.
      parameter( xdim = 99, XPTS = 35, ydim = 100, YPTS = 46 )

      integer i,  j
      real(kind=plflt)  xx, yy
      real(kind=plflt)  z(xdim, ydim), zmin, zmax

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

!      Set up color map 1 (but comment out because this makes
!      no difference for the current cmap0 plshade calls in plot1 and plot2.)

!      call cmap1_init2()

!      Initialize plplot

      call plinit()

!      Set up data array

      do i = 1,XPTS
        xx = dble(i-1 - (XPTS / 2)) / dble(XPTS / 2)
        do j = 1,YPTS
          yy = dble(j-1 - (YPTS / 2)) / dble(YPTS / 2) - 1.0_plflt
          z(i,j) = xx*xx - yy*yy + (xx - yy)/(xx*xx+yy*yy + 0.1_plflt)
        enddo
      enddo

      call a2mnmx(z, XPTS, YPTS, zmin, zmax, xdim)

      call plot1(z, XPTS, YPTS, zmin, zmax, xdim)
      call plot2(z, XPTS, YPTS, zmin, zmax, xdim)

      call plend()
      end

! -------------------------------------------------------------------------
!      cmap1_init1
!
!      Initializes color map 1 in HLS space.
! -------------------------------------------------------------------------

      subroutine cmap1_init1()
      use plplot
      implicit none
      real(kind=plflt)   i(4), h(4), l(4), s(4)

      i(1) = 0.0_plflt    ! left boundary
      i(2) = 0.45_plflt   ! just before center
      i(3) = 0.55_plflt   ! just after center
      i(4) = 1.0_plflt    ! right boundary

      h(1) = 260.0_plflt  ! hue -- low: blue-violet
      h(2) = 260.0_plflt  ! only change as we go over vertex
      h(3) = 20.0_plflt   ! hue -- high: red
      h(4) = 20.0_plflt   ! keep fixed


      l(1) = 0.5_plflt    ! lightness -- low
      l(2) = 0.0_plflt    ! lightness -- center
      l(3) = 0.0_plflt    ! lightness -- center
      l(4) = 0.5_plflt    ! lightness -- high

!     call plscolbg(255,255,255)
!     l(1) = 0.5_plflt    ! lightness -- low
!     l(2) = 1.0_plflt    ! lightness -- center
!     l(3) = 1.0_plflt    ! lightness -- center
!     l(4) = 0.5_plflt    ! lightness -- high

      s(1) = 1.0_plflt    ! maximum saturation
      s(2) = 1.0_plflt    ! maximum saturation
      s(3) = 1.0_plflt    ! maximum saturation
      s(4) = 1.0_plflt    ! maximum saturation

      call plscmap1l(.false., i, h, l, s)
      end

! -------------------------------------------------------------------------
!      cmap1_init2
!
!      Initializes color map 1 in HLS space.
! -------------------------------------------------------------------------

      subroutine cmap1_init2()
      use plplot
      implicit none
      real(kind=plflt) i(4), h(4), l(4), s(4)

      i(1) = 0.0_plflt    ! left boundary
      i(2) = 0.45_plflt   ! just before center
      i(3) = 0.55_plflt   ! just after center
      i(4) = 1.0_plflt    ! right boundary

      h(1) = 260.0_plflt  ! hue -- low: blue-violet
      h(2) = 260.0_plflt  ! only change as we go over vertex
      h(3) = 20.0_plflt   ! hue -- high: red
      h(4) = 20.0_plflt   ! keep fixed


      l(1) = 0.6_plflt    ! lightness -- low
      l(2) = 0.0_plflt    ! lightness -- center
      l(3) = 0.0_plflt    ! lightness -- center
      l(4) = 0.6_plflt    ! lightness -- high

!     call plscolbg(255,255,255)
!     l(1) = 0.5_plflt    ! lightness -- low
!     l(2) = 1.0_plflt    ! lightness -- center
!     l(3) = 1.0_plflt    ! lightness -- center
!     l(4) = 0.5_plflt    ! lightness -- high

      s(1) = 1.0_plflt    ! maximum saturation
      s(2) = 0.5_plflt    ! maximum saturation
      s(3) = 0.5_plflt    ! maximum saturation
      s(4) = 1.0_plflt    ! maximum saturation

      call plscmap1l(.false., i, h, l, s)
      end

! -------------------------------------------------------------------------
!      plot1
!
!      Illustrates a single shaded region.
! -------------------------------------------------------------------------

      subroutine plot1(z, XPTS, YPTS, zmin, zmax, xdim)
      use plplot
      implicit none

      integer xdim, XPTS, YPTS
      real(kind=plflt)  z(xdim,YPTS), zmin, zmax

      character*1 undefined
      real(kind=plflt)  shade_min, shade_max, sh_color
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width

      sh_cmap   = 0
      min_color = 0
      min_width = 0
      max_color = 0
      max_width = 0

      call pladv(0)
      call plvpor( 0.1_plflt, 0.9_plflt,  0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

!      Plot using identity transform

      shade_min = zmin + (zmax-zmin)*0.4_plflt
      shade_max = zmin + (zmax-zmin)*0.6_plflt
      sh_color  = 7
      sh_width  = 2
      min_color = 9
      max_color = 2
      min_width = 2
      max_width = 2

      call plpsty(8)

!      Use_ plshade0 instead of plshade1 - identity mapping
      call plshade(z(:XPTS,:YPTS), undefined, &
        -1._plflt, 1._plflt, -1._plflt, 1._plflt, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width )

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon flux')

      end

! -------------------------------------------------------------------------
!      plot2
!
!      Illustrates multiple adjacent shaded regions, using different fill
!      patterns for each region.
! -------------------------------------------------------------------------

      subroutine plot2(z, XPTS, YPTS, zmin, zmax, xdim)
      use plplot
      implicit none

      integer xdim, XPTS, YPTS
      real(kind=plflt)  z(xdim,YPTS), zmin, zmax

      character*1 undefined
      real(kind=plflt)  shade_min, shade_max, sh_color
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width
      integer i

      sh_cmap   = 0
      min_color = 0
      min_width = 0
      max_color = 0
      max_width = 0
      sh_width  = 2

      call pladv(0)
      call plvpor( 0.1_plflt, 0.9_plflt,  0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

!      Plot using identity transform

      do  i = 1,10
        shade_min = zmin + (zmax - zmin) * (i-1) / 10.0_plflt
        shade_max = zmin + (zmax - zmin) * i / 10.0_plflt
        sh_color = i+5
        call plpsty(mod((i + 1),8) + 1)
!        Use_ plshade0 instead of plshade1 - identity mapping
        call plshade(z(:XPTS,:YPTS), undefined, &
          -1._plflt, 1._plflt, -1._plflt, 1._plflt, &
          shade_min, shade_max, &
          sh_cmap, sh_color, sh_width, &
          min_color, min_width, max_color, max_width)
      enddo

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon flux')

      end

!----------------------------------------------------------------------------
!      Subroutine a2mnmx
!      Minimum and the maximum elements of a 2-d array.

      subroutine a2mnmx(f, nx, ny, fmin, fmax, xdim)
      use plplot
      implicit none

      integer   i, j, nx, ny, xdim
      real(kind=plflt)    f(xdim, ny), fmin, fmax

      fmax = f(1, 1)
      fmin = fmax
      do j = 1, ny
        do  i = 1, nx
          fmax = max(fmax, f(i, j))
          fmin = min(fmin, f(i, j))
        enddo
      enddo
      end
