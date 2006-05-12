!      $Id$
!      3-d plot demo
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

      use plplot, PI => PL_PI
      implicit none
      integer i, j, k, ifshade, xpts, ypts, xdim, ydim
!      xdim is the leading dimension of z, xpts <= xdim is the leading
!      dimension of z that is defined.
      parameter (xdim=99, ydim=100, xpts=35, ypts=46)
      real(kind=plflt) x(xdim), y(ydim), z(xdim,ypts), xx, yy, r

      character*80 title(2)
      real(kind=plflt) alt(2),az(2)
      data alt /60.0_plflt,20.0_plflt/
      data az  /30.0_plflt,60.0_plflt/
      data title /'#frPLplot Example 8 - Alt=60, Az=30', &
                  '#frPLplot Example 8 - Alt=20, Az=60'/
      integer rosen
      integer nlevel
      parameter (nlevel = 10)
      real(kind=plflt) zmin, zmax, step, clevel(nlevel)
!      Plotting options for 3d plots, see plplot.h for the C definitions
!      of these options.
      integer DRAW_LINEX, DRAW_LINEY, DRAW_LINEXY, MAG_COLOR, &
        BASE_CONT, TOP_CONT, SURF_CONT, DRAW_SIDES, FACETED, MESH
      parameter(DRAW_LINEX = 1)
      parameter(DRAW_LINEY = 2)
      parameter(DRAW_LINEXY = 3)
      parameter(MAG_COLOR = 4)
      parameter(BASE_CONT = 8)
      parameter(TOP_CONT = 16)
      parameter(SURF_CONT = 32)
      parameter(DRAW_SIDES = 64)
      parameter(FACETED = 128)
      parameter(MESH = 256)
!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)


      rosen = 1

      do i = 1,xpts
        x(i) = dble(i-1-(xpts/2))/dble (xpts/2)
        if(rosen.eq.1) x(i) = 1.5_plflt*x(i)
      enddo
      do j = 1,ypts
        y(j) = dble(j-1-(ypts/2))/dble (ypts/2)
        if(rosen.eq.1) y(j) = y(j) + 0.5_plflt
      enddo

      do i=1,xpts
        xx = x(i)
        do j=1,ypts
          yy = y(j)
          if(rosen.eq.1) then
            z(i,j) = (1._plflt - xx)**2 + 100._plflt*(yy - xx**2)**2
!            The log argument may be zero for just the right grid.
            if(z(i,j).gt.0._plflt) then
              z(i,j) = log(z(i,j))
            else
              z(i,j) = -5._plflt
            endif
          else
!            sombrero function
            r = sqrt(xx*xx + yy*yy)
            z(i,j) = exp(-r*r) * cos(2.0_plflt*PI*r)
          endif
        enddo
      enddo
      call a2mnmx(z, xpts, ypts, zmin, zmax, xdim)
      step = (zmax-zmin)/(nlevel+1)
      do i = 1, nlevel
        clevel(i) = zmin + step*i
      enddo

      call plinit()
      call pllightsource(1._plflt, 1._plflt, 1._plflt)
      do k=1,2
        do ifshade = 0, 3
          call pladv(0)
          call plvpor(0.0_plflt, 1.0_plflt, 0.0_plflt, 0.9_plflt )
          call plwind(-1.0_plflt, 1.0_plflt, -0.9_plflt, 1.1_plflt )
          call plcol0(3)
          call plmtex('t', 1.0_plflt, 0.5_plflt, 0.5_plflt, title(k))
          call plcol0(1)
          if(rosen.eq.1) then
            call plw3d(1.0_plflt, 1.0_plflt, 1.0_plflt, -1.5_plflt, &
              1.5_plflt, -0.5_plflt, 1.5_plflt, zmin, zmax, alt(k),az(k))
          else
            call plw3d(1.0_plflt, 1.0_plflt, 1.0_plflt, -1.0_plflt, &
              1.0_plflt, -1.0_plflt, 1.0_plflt, zmin, zmax, alt(k),az(k))
          endif
          call plbox3('bnstu','x axis', 0.0_plflt, 0, &
            'bnstu', 'y axis', 0.0_plflt, 0, &
            'bcdmnstuv','z axis', 0.0_plflt, 0)
          call plcol0(2)
          if(ifshade.eq.0) then
!            diffuse light surface plot
            call cmap1_init(1)
            call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              0, clevel(nlevel:1))
          elseif(ifshade.eq.1) then
!            magnitude colored plot
            call cmap1_init(0)
            call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              MAG_COLOR, clevel(nlevel:1))
          elseif(ifshade.eq.2) then
!            magnitude colored plot with faceted squares
            call cmap1_init(0)
            call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              ior(MAG_COLOR, FACETED), clevel(nlevel:1))
          elseif(ifshade.eq.3) then
!            magnitude colored plot with contours
            call cmap1_init(0)
            call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              ior(MAG_COLOR, ior(SURF_CONT, BASE_CONT)), clevel)
          else
            stop 'x08f: bad logic'
          endif
        enddo
      enddo
      call plend
      end

!----------------------------------------------------------------------------
      subroutine cmap1_init(gray)
!      For gray.eq.1, basic grayscale variation from half-dark
!      to light.  Otherwise, hue variations around the front of the
!      colour wheel from blue to green to red with constant lightness
!      and saturation.

      use plplot
      implicit none
      integer gray
      real(kind=plflt) i(0:1), h(0:1), l(0:1), s(0:1)
      logical rev(0:1)
!      left boundary
      i(0) = 0._plflt
!      right boundary
      i(1) = 1._plflt
      if (gray.eq.1) then
!        hue -- low: red (arbitrary if s=0)
        h(0) = 0.0_plflt
!        hue -- high: red (arbitrary if s=0)
        h(1) = 0.0_plflt
!        lightness -- low: half-dark
        l(0) = 0.5_plflt
!        lightness -- high: light
        l(1) = 1.0_plflt
!        minimum saturation
        s(0) = 0.0_plflt
!        minimum saturation
        s(1) = 0.0_plflt
      else
!        This combination of hues ranges from blue to cyan to green to yellow
!        to red (front of colour wheel) with constant lightness = 0.6
!        and saturation = 0.8.

!        hue -- low: blue
        h(0) = 240._plflt
!        hue -- high: red
        h(1) = 0.0_plflt
!        lightness -- low:
        l(0) = 0.6_plflt
!        lightness -- high:
        l(1) = 0.6_plflt
!        saturation
        s(0) = 0.8_plflt
!        minimum saturation
        s(1) = 0.8_plflt
      endif
      rev(0) = .false.
      rev(1) = .false.
      call plscmap1n(256)
      call plscmap1l(.false., i, h, l, s, rev)
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
