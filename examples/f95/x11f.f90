!      $Id$
!      Mesh plot demo
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
      integer i, j, k, ifshade, xpts, ypts
      parameter (xpts=35, ypts=46)

      real(kind=plflt) x(xpts), y(ypts), z(xpts,ypts), xx, yy, r

      character*80 title(2)
      real(kind=plflt) alt(2),az(2)
      integer opt(2)
      data alt /33.0_plflt,17.0_plflt/
      data az  /24.0_plflt,115.0_plflt/
!      DRAW_LINEXY
      data opt /2*3/
      data title /'#frPLplot Example 11 - Alt=33, Az=24, Opt=3', &
                  '#frPLplot Example 11 - Alt=17, Az=115, Opt=3'/
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


      do i = 1,xpts
        x(i) = 3._plflt*dble(i-1-(xpts/2))/dble (xpts/2)
      enddo
      do j = 1,ypts
        y(j) = 3._plflt*dble(j-1-(ypts/2))/dble (ypts/2)
      enddo

      do i=1,xpts
        xx = x(i)
        do j=1,ypts
          yy = y(j)
          z(i,j) = 3._plflt * (1._plflt-xx)*(1._plflt-xx) * &
            exp(-(xx*xx) - (yy+1._plflt)*(yy+1._plflt)) - &
            10._plflt * (xx/5._plflt - xx**3 - yy**5) * exp(-xx*xx-yy*yy) - &
            1._plflt/3._plflt * exp(-(xx+1._plflt)*(xx+1._plflt) - (yy*yy))
          if(.false.) then
!            Jungfraujoch/Interlaken
            if(z(i,j).lt.-1._plflt) z(i,j) = -1._plflt
          endif
        enddo
      enddo
      call a2mnmx(z, xpts, ypts, zmin, zmax)
      step = (zmax-zmin)/(nlevel+1)
      do i = 1, nlevel
        clevel(i) = zmin + step*i
      enddo

      call plinit()
      call cmap1_init(0)
      do k=1,2
        do ifshade = 0, 3
          call pladv(0)
          call plcol0(1)
          call plvpor(0.0_plflt, 1.0_plflt, 0.0_plflt, 0.9_plflt )
          call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.5_plflt )
          call plw3d(1.0_plflt, 1.0_plflt, 1.2_plflt, -3.0_plflt, &
            3.0_plflt, -3.0_plflt, 3.0_plflt, zmin, zmax, alt(k),az(k))
          call plbox3('bnstu','x axis', 0.0_plflt, 0, &
            'bnstu', 'y axis', 0.0_plflt, 0, &
            'bcdmnstuv','z axis', 0.0_plflt, 0)
          call plcol0(2)
          if(ifshade.eq.0) then
!            wireframe plot
            call plmesh(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              opt(k))
          elseif(ifshade.eq.1) then
!            magnitude colored wireframe plot
            call plmesh(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              ior(opt(k), MAG_COLOR))
          elseif(ifshade.eq.2) then
!            magnitude colored wireframe plot with sides
            call plot3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              ior(opt(k), MAG_COLOR), .true.)
          elseif(ifshade.eq.3) then
!             magnitude colored wireframe plot with base contour
            call plmeshc(x(:xpts), y(:ypts), z(:xpts,:ypts), &
              ior(opt(k), ior(MAG_COLOR, BASE_CONT)), clevel)
          else
            stop 'x11f: bad logic'
          endif
          call plcol0(3)
          call plmtex('t', 1.0_plflt, 0.5_plflt, 0.5_plflt, title(k))
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
      call plscmap1n(256)
      call plscmap1l(.false., i, h, l, s)
      end

!----------------------------------------------------------------------------
!      Subroutine a2mnmx
!      Minimum and the maximum elements of a 2-d array.

      subroutine a2mnmx(f, nx, ny, fmin, fmax)
      use plplot
      implicit none

      integer   i, j, nx, ny
      real(kind=plflt)    f(nx, ny), fmin, fmax

      fmax = f(1, 1)
      fmin = fmax
      do j = 1, ny
        do  i = 1, nx
          fmax = max(fmax, f(i, j))
          fmin = min(fmin, f(i, j))
        enddo
      enddo
      end
