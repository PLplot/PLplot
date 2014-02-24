!      Demonstration of plshade plotting
!      Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display
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

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

      call plscmap0n(3)

!      Initialize plplot

      call plinit()

!      Rectangular coordinate plot

      call rect()

!      Polar coordinate plot

      call polar()

      call plend
      end

!      Plot function using the identity transform

      subroutine rect()

      use plplot
      implicit none
      integer   xdim, ydim, NX, NY, NCONTR
!      xdim and ydim are the static dimensions of the 2D arrays while
!      NX and NY are the defined area.
      parameter (xdim = 99, NX = 35, ydim = 100, NY = 46, NCONTR = 14)

      real(kind=plflt)    z(xdim, ydim), w(xdim, ydim), clevel(NCONTR)
      real(kind=plflt)    xmin, xmax, ymin, ymax, zmin, zmax, x, y
      real(kind=plflt)    shade_min, shade_max, sh_color
      integer   i, j, sh_cmap
      integer   min_color, max_color
      real(kind=plflt)    sh_width, min_width, max_width

      xmin = -1._plflt
      ymin = -1._plflt
      xmax =  1._plflt
      ymax =  1._plflt

!      Set up for plshade call

      sh_cmap   = 1
      min_color = 1
      min_width = 0
      max_color = 0
      max_width = 0

!      Set up data arrays

      do i = 1, NX
        x = (i - 1 - (NX/2)) / dble (NX/2)
        do j = 1, NY
          y = (j - 1 - (NY/2)) / dble (NY/2) - 1.0_plflt
          z(i,j) = x*x - y*y + (x - y) / (x*x + y*y + 0.1_plflt)
          w(i,j) = 2*x*y
        enddo
      enddo

      call a2mnmx(z, NX, NY, zmin, zmax, xdim)
      do  i = 1, NCONTR
        clevel(i) = zmin + (zmax - zmin) * (i + 0.5_plflt) / &
          dble (NCONTR)
      enddo

!      Plot using identity transform

      call pladv(0)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

      do i = 1, NCONTR
        shade_min = zmin + (zmax - zmin) * dble (i - 1) / &
          dble (NCONTR)
        shade_max = zmin + (zmax - zmin) * dble (i)     / &
          dble (NCONTR)
        sh_color = dble (i - 1) / dble (NCONTR - 1)
        sh_width = 2
        call plpsty(0)
        call plshade(z(:NX,:NY), ' ', &
          -1._plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, &
          shade_min, shade_max, &
          sh_cmap, sh_color, sh_width, &
          min_color, min_width, max_color, max_width )
      enddo

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon flux')

      end

!      Routine for demonstrating use_ of transformation arrays in contour plots.

      subroutine polar()

      use plplot, TWOPI => PL_TWOPI
      implicit none
      integer   xdim, ydim, NX, NY, NCONTR, NBDRY
!      xdim and ydim are the static dimensions of the 2D arrays while
!      NX and NY are the defined area.
      parameter (xdim = 99, NX = 40, ydim = 100, NY = 64)
      parameter (NCONTR = 14, NBDRY=200)

      real(kind=plflt)    z(xdim, ydim), ztmp(xdim, ydim+1)
      real(kind=plflt)    xg(xdim, ydim+1), yg(xdim, ydim+1), &
         xtm(NBDRY), ytm(NBDRY)
      real(kind=plflt)    clevel(NCONTR)
      real(kind=plflt)    xmin, xmax, ymin, ymax, zmin, zmax
      real(kind=plflt)    xpmin, xpmax, ypmin, ypmax
      real(kind=plflt)    r, theta, rmax, x0, y0
      real(kind=plflt)    eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i
      real(kind=plflt)    div1, div1i, div2, div2i

      real(kind=plflt)    shade_min, shade_max, sh_color
      real(kind=plflt)    xtick, ytick
      integer   nxsub, nysub
      integer   ncolbox, ncollab
      integer   i, j, kx, lx, ky, ly
      integer   sh_cmap
      integer   min_color, max_color
      real(kind=plflt) sh_width, min_width, max_width
      character(len=8) xopt, yopt

!      Set up for plshade call

      sh_cmap = 1
      min_color = 1
      min_width = 0
      max_color = 0
      max_width = 0

      kx = 1
      lx = NX
      ky = 1
      ly = NY

!      Set up r-theta grids
!      Tack on extra cell in theta to handle periodicity.

      do i = 1, NX
        r = i - 0.5_plflt
        do j = 1, NY
          theta = TWOPI/dble(NY) * (j-0.5_plflt)
          xg(i,j) = r * cos(theta)
          yg(i,j) = r * sin(theta)
        enddo
        xg(i, NY+1) = xg(i, 1)
        yg(i, NY+1) = yg(i, 1)
      enddo
      call a2mnmx(xg, NX, NY, xmin, xmax, xdim)
      call a2mnmx(yg, NX, NY, ymin, ymax, xdim)

      rmax = r
      x0 = (xmin + xmax)/2._plflt
      y0 = (ymin + ymax)/2._plflt

!      Potential inside a conducting cylinder (or sphere) by method of images.
!      Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
!      Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
!      Also put in smoothing term at small distances.

      eps = 2._plflt

      q1 = 1._plflt
      d1 = r/4._plflt

      q1i = - q1*r/d1
      d1i = r**2/d1

      q2 = -1._plflt
      d2 = r/4._plflt

      q2i = - q2*r/d2
      d2i = r**2/d2

      do i = 1, NX
        do j = 1, NY
          div1 = sqrt((xg(i,j)-d1)**2 + (yg(i,j)-d1)**2 + eps**2)
          div1i = sqrt((xg(i,j)-d1i)**2 + (yg(i,j)-d1i)**2 + eps**2)

          div2 = sqrt((xg(i,j)-d2)**2 + (yg(i,j)+d2)**2 + eps**2)
          div2i = sqrt((xg(i,j)-d2i)**2 + (yg(i,j)+d2i)**2 + eps**2)

          z(i,j) = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
        enddo
      enddo

!      Tack on extra cell in theta to handle periodicity.

      do i = 1, NX
        do j = 1, NY
          ztmp(i,j) = z(i,j)
        enddo
        ztmp(i, NY+1) = z(i, 1)
      enddo
      call a2mnmx(z, NX, NY, zmin, zmax, xdim)

!      Set up contour levels.

      do i = 1, NCONTR
        clevel(i) = zmin + (i-0.5_plflt)*abs(zmax - zmin)/dble(NCONTR)
      enddo

!      Advance graphics frame and get ready to plot.

      ncolbox = 1
      ncollab = 2

      call pladv(0)
      call plcol0(ncolbox)

!      Scale window to user coordinates.
!      Make a bit larger so the boundary does not get clipped.

      eps = 0.05_plflt
      xpmin = xmin - abs(xmin)*eps
      xpmax = xmax + abs(xmax)*eps
      ypmin = ymin - abs(ymin)*eps
      ypmax = ymax + abs(ymax)*eps

      call plvpas(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt, 1.0_plflt)
      call plwind(xpmin, xpmax, ypmin, ypmax)

      xopt = ' '
      yopt = ' '
      xtick = 0._plflt
      nxsub = 0
      ytick = 0._plflt
      nysub = 0

      call plbox(xopt, xtick, nxsub, yopt, ytick, nysub)

!      Call plotter once for z < 0 (dashed), once for z > 0 (solid lines).

      do i = 1, NCONTR
        shade_min = zmin + (zmax - zmin) * dble (i - 1) / &
          dble (NCONTR)
        shade_max = zmin + (zmax - zmin) * dble (i)     / &
          dble (NCONTR)
        sh_color = dble (i - 1) / dble (NCONTR - 1)
        sh_width = 2
        call plpsty(0)

        call plshade(z(:NX,:NY), ' ', &
          -1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, &
          shade_min, shade_max, &
          sh_cmap, sh_color, sh_width, &
          min_color, min_width, max_color, max_width, &
            xg(:NX,:NY), yg(:NX,:NY) )
      enddo

!      Draw boundary.

      do i = 1, NBDRY
        theta = (TWOPI)/(NBDRY-1) * dble(i-1)
        xtm(i) = x0 + rmax * cos(theta)
        ytm(i) = y0 + rmax * sin(theta)
      enddo
      call plcol0(ncolbox)
      call plline(xtm, ytm)

      call plcol0(ncollab)
      call pllab(' ', ' ', &
      'Shielded potential of charges in a conducting sphere')

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
