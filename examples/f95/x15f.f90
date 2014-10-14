!      Shade plot demo.
!      Does a variety of shade plots.
!
!      Copyright (C) 2004  Alan W. Irwin
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

      use plplot
      implicit none

      integer(kind=plint)  xdim, ydim, XPTS, YPTS
!      xdim and ydim are the static dimensions of the 2D arrays while
!      NX and NY are the defined area.
      parameter( xdim = 99_plint, XPTS = 35_plint, ydim = 100_plint, YPTS = 46_plint )

      integer(kind=plint) i,  j
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

      do i = 1_plint,XPTS
        xx = dble(i-1 - (XPTS / 2_plint)) / dble(XPTS / 2_plint)
        do j = 1_plint,YPTS
          yy = dble(j-1 - (YPTS / 2_plint)) / dble(YPTS / 2_plint) - 1.0_plflt
          z(i,j) = xx*xx - yy*yy + (xx - yy)/(xx*xx+yy*yy + 0.1_plflt)
        enddo
      enddo

      call a2mnmx(z, XPTS, YPTS, zmin, zmax, xdim)

      call plot1(z, XPTS, YPTS, zmin, zmax, xdim)
      call plot2(z, XPTS, YPTS, zmin, zmax, xdim)
      call plot3()

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
      real(kind=plflt)   i(4_plint), h(4_plint), l(4_plint), s(4_plint)

      i(1_plint) = 0.0_plflt    ! left boundary
      i(2_plint) = 0.45_plflt   ! just before center
      i(3_plint) = 0.55_plflt   ! just after center
      i(4_plint) = 1.0_plflt    ! right boundary

      h(1_plint) = 260.0_plflt  ! hue -- low: blue-violet
      h(2_plint) = 260.0_plflt  ! only change as we go over vertex
      h(3_plint) = 20.0_plflt   ! hue -- high: red
      h(4_plint) = 20.0_plflt   ! keep fixed


      l(1_plint) = 0.5_plflt    ! lightness -- low
      l(2_plint) = 0.0_plflt    ! lightness -- center
      l(3_plint) = 0.0_plflt    ! lightness -- center
      l(4_plint) = 0.5_plflt    ! lightness -- high

!     call plscolbg(255,255,255)
!     l(1) = 0.5_plflt    ! lightness -- low
!     l(2) = 1.0_plflt    ! lightness -- center
!     l(3) = 1.0_plflt    ! lightness -- center
!     l(4) = 0.5_plflt    ! lightness -- high

      s(1_plint) = 1.0_plflt    ! maximum saturation
      s(2_plint) = 1.0_plflt    ! maximum saturation
      s(3_plint) = 1.0_plflt    ! maximum saturation
      s(4_plint) = 1.0_plflt    ! maximum saturation

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
      real(kind=plflt) i(4_plint), h(4_plint), l(4_plint), s(4_plint)

      i(1_plint) = 0.0_plflt    ! left boundary
      i(2_plint) = 0.45_plflt   ! just before center
      i(3_plint) = 0.55_plflt   ! just after center
      i(4_plint) = 1.0_plflt    ! right boundary

      h(1_plint) = 260.0_plflt  ! hue -- low: blue-violet
      h(2_plint) = 260.0_plflt  ! only change as we go over vertex
      h(3_plint) = 20.0_plflt   ! hue -- high: red
      h(4_plint) = 20.0_plflt   ! keep fixed


      l(1_plint) = 0.6_plflt    ! lightness -- low
      l(2_plint) = 0.0_plflt    ! lightness -- center
      l(3_plint) = 0.0_plflt    ! lightness -- center
      l(4_plint) = 0.6_plflt    ! lightness -- high

!     call plscolbg(255,255,255)
!     l(1) = 0.5_plflt    ! lightness -- low
!     l(2) = 1.0_plflt    ! lightness -- center
!     l(3) = 1.0_plflt    ! lightness -- center
!     l(4) = 0.5_plflt    ! lightness -- high

      s(1_plint) = 1.0_plflt    ! maximum saturation
      s(2_plint) = 0.5_plflt    ! maximum saturation
      s(3_plint) = 0.5_plflt    ! maximum saturation
      s(4_plint) = 1.0_plflt    ! maximum saturation

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

      integer(kind=plint) xdim, XPTS, YPTS
      real(kind=plflt)  z(xdim,YPTS), zmin, zmax

      character(len=1) undefined
      real(kind=plflt)  shade_min, shade_max, sh_color
      integer(kind=plint) sh_cmap
      integer(kind=plint) min_color, max_color
      real(kind=plflt) sh_width, min_width, max_width

      sh_cmap   = 0_plint
      min_color = 0_plint
      min_width = 0_plint
      max_color = 0_plint
      max_width = 0_plint

      call pladv(0_plint)
      call plvpor( 0.1_plflt, 0.9_plflt,  0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

!      Plot using identity transform

      shade_min = zmin + (zmax-zmin)*0.4_plflt
      shade_max = zmin + (zmax-zmin)*0.6_plflt
      sh_color  = 7_plint
      sh_width  = 2_plint
      min_color = 9_plint
      max_color = 2_plint
      min_width = 2_plint
      max_width = 2_plint

      call plpsty(8_plint)

!      Use_ plshade0 instead of plshade1 - identity mapping
      call plshade(z(:XPTS,:YPTS), undefined, &
        -1._plflt, 1._plflt, -1._plflt, 1._plflt, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width )

      call plcol0(1_plint)
      call plbox('bcnst', 0.0_plflt, 0_plint, 'bcnstv', 0.0_plflt, 0_plint)
      call plcol0(2_plint)
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

      integer(kind=plint) xdim, XPTS, YPTS
      real(kind=plflt)  z(xdim,YPTS), zmin, zmax

      character(len=1) undefined
      real(kind=plflt)  shade_min, shade_max, sh_color
      integer(kind=plint) sh_cmap
      integer(kind=plint) min_color, max_color
      real(kind=plflt) sh_width, min_width, max_width
      integer(kind=plint) i, j

      integer(kind=plint) nlin(10_plint), inc(2_plint,10_plint), del(2_plint,10_plint)
      data nlin /1, 1_plint, 1_plint, 1_plint, 1_plint, 2_plint, 2_plint, 2_plint, 2_plint, 2/
      data ( (inc(i,j), i=1,2_plint), j=1,10_plint) / &
           450_plint, 0_plint, -450, 0_plint, 0_plint, 0_plint, 900_plint, 0_plint, &
           300_plint, 0_plint, 450_plint,-450, 0_plint, 900_plint, 0_plint, 450_plint, &
           450_plint, -450, 0_plint, 900/
      data ( (del(i,j), i=1,2_plint), j=1,10_plint) / &
           2000_plint, 2000_plint, 2000_plint, 2000_plint, 2000_plint, 2000_plint, &
           2000_plint, 2000_plint, 2000_plint, 2000_plint, 2000_plint, 2000_plint, &
           2000_plint, 2000_plint, 2000_plint, 2000_plint, 4000_plint, 4000_plint, &
           4000_plint, 2000/

      sh_cmap   = 0_plint
      min_color = 0_plint
      min_width = 0_plint
      max_color = 0_plint
      max_width = 0_plint
      sh_width  = 2_plint

      call pladv(0_plint)
      call plvpor( 0.1_plflt, 0.9_plflt,  0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

!      Plot using identity transform

      do  i = 1_plint,10
        shade_min = zmin + (zmax - zmin) * (i-1) / 10.0_plflt
        shade_max = zmin + (zmax - zmin) * i / 10.0_plflt
        sh_color = i+5
        call plpat(nlin(i),inc(1_plint,i),del(1_plint,i))
!        Use_ plshade0 instead of plshade1 - identity mapping
        call plshade(z(:XPTS,:YPTS), undefined, &
          -1._plflt, 1._plflt, -1._plflt, 1._plflt, &
          shade_min, shade_max, &
          sh_cmap, sh_color, sh_width, &
          min_color, min_width, max_color, max_width)
      enddo

      call plcol0(1_plint)
      call plbox('bcnst', 0.0_plflt, 0_plint, 'bcnstv', 0.0_plflt, 0_plint)
      call plcol0(2_plint)
      call pllab('distance', 'altitude', 'Bogon flux')

      end
!--------------------------------------------------------------------------
!     plot3
!
!     Illustrates shaded regions in 3d, using a different fill pattern for
!     each region.
!--------------------------------------------------------------------------

      subroutine plot3
      use plplot
      implicit none

      real(kind=plflt) xx1(5_plint), xx2(5_plint), yy1(5_plint), yy2(5_plint), zz1(5_plint), zz2(5_plint)
      data xx1 / -1.0_plflt, 1.0_plflt, 1.0_plflt, -1.0_plflt, -1.0_plflt/
      data xx2 / -1.0_plflt, 1.0_plflt, 1.0_plflt, -1.0_plflt, -1.0_plflt/
      data yy1 /1.0_plflt, 1.0_plflt, 0.0_plflt, 0.0_plflt, 1.0_plflt/
      data yy2 / -1.0_plflt, -1.0_plflt, 0.0_plflt, 0.0_plflt, -1.0_plflt/
      data zz1 / 0.0_plflt, 0.0_plflt, 1.0_plflt, 1.0_plflt, 0.0_plflt/
      data zz2 / 0.0_plflt, 0.0_plflt, 1.0_plflt, 1.0_plflt, 0.0_plflt/

      call pladv(0_plint)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)
      call plw3d(1._plflt, 1._plflt, 1._plflt, -1.0_plflt, 1.0_plflt, &
           -1.0_plflt, 1.0_plflt, 0.0_plflt,1.5_plflt, 30._plflt, -40._plflt)

!     Plot using identity transform

      call plcol0(1_plint)
      call plbox3("bntu", "X", 0.0_plflt, 0_plint, "bntu", "Y", 0.0_plflt, 0_plint, &
           "bcdfntu", "Z", 0.5_plflt, 0_plint)
      call plcol0(2_plint)
      call pllab("","","3-d polygon filling")

      call plcol0(3_plint)
      call plpsty(1_plint)
      call plline3(xx1, yy1, zz1)
      call plfill3(xx1(1:4), yy1(1:4), zz1(1:4))
      call plpsty(2_plint)
      call plline3(xx2, yy2, zz2)
      call plfill3(xx2(1:4), yy2(1:4), zz2(1:4))

      end

!----------------------------------------------------------------------------
!      Subroutine a2mnmx
!      Minimum and the maximum elements of a 2-d array.

      subroutine a2mnmx(f, nx, ny, fmin, fmax, xdim)
      use plplot
      implicit none

      integer(kind=plint)   i, j, nx, ny, xdim
      real(kind=plflt)    f(xdim, ny), fmin, fmax

      fmax = f(1_plint, 1_plint)
      fmin = fmax
      do j = 1_plint, ny
        do  i = 1_plint, nx
          fmax = max(fmax, f(i, j))
          fmin = min(fmin, f(i, j))
        enddo
      enddo
      end
