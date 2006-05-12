!      $Id$
!      Contour plot demo.
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

!      Does several contour plots using different coordinate mappings.
      use plplot, PI => PL_PI
      implicit none
      integer i, j, nptsx, nptsy, xdim, ydim
!      xdim and ydim are the absolute static dimensions.
!      nptsx, and nptsy are the (potentially dynamic) defined area of the 2D
!      arrays that is actually used.
      parameter (xdim=99, ydim=100, nptsx=35,nptsy=46)

      real(kind=plflt) z(xdim, ydim), w(xdim, ydim), clevel(11), &
        xg1(xdim), yg1(ydim), &
        xg2(xdim, ydim), yg2(xdim, ydim)
      real(kind=plflt) tr, xx, yy, argx, argy, distort

      common /plplot/ tr(6)

      data clevel /-1._plflt, -0.8_plflt, -0.6_plflt, &
        -0.4_plflt, -0.2_plflt, &
        0._plflt, 0.2_plflt, 0.4_plflt, 0.6_plflt ,0.8_plflt, 1._plflt/
!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

      tr(1) = 2._plflt/dble(nptsx-1)
      tr(2) = 0.0_plflt
      tr(3) = -1.0_plflt
      tr(4) = 0.0_plflt
      tr(5) = 2._plflt/dble(nptsy-1)
      tr(6) = -1.0_plflt

!      Calculate the data matrices.
      do i=1,nptsx
        xx = dble(i-1-(nptsx/2))/dble (nptsx/2)
        do j=1,nptsy
          yy = dble(j-1-(nptsy/2))/dble (nptsy/2) - 1.0_plflt
          z(i,j) = xx*xx - yy*yy
          w(i,j) = 2._plflt*xx*yy
        enddo
      enddo

!      Build the 1-d coord arrays.
      distort = 0.4_plflt
      do i=1,nptsx
        xx = -1._plflt + dble(i-1)*2._plflt/dble(nptsx-1)
        xg1(i) = xx + distort*cos(0.5_plflt*PI*xx)
      enddo

      do j=1,nptsy
        yy = -1._plflt + dble(j-1)*2._plflt/dble(nptsy-1)
        yg1(j) = yy - distort*cos(0.5_plflt*PI*yy)
      enddo

!      Build the 2-d coord arrays.
      do i=1,nptsx
        xx = -1._plflt + dble(i-1)*2._plflt/dble(nptsx-1)
        argx = 0.5_plflt*PI*xx
        do j=1,nptsy
          yy = -1._plflt + dble(j-1)*2._plflt/dble(nptsy-1)
          argy = 0.5_plflt*PI*yy
          xg2(i,j) = xx + distort*cos(argx)*cos(argy)
          yg2(i,j) = yy - distort*cos(argx)*cos(argy)
        enddo
      enddo

      call plinit

!      Plot using identity transform
      call pl_setcontlabelparam(0.006_plflt, 0.3_plflt, 0.1_plflt, 1)
      call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
      call plcol0(2)
      call plcont(z,xdim,ydim,1,nptsx,1,nptsy,clevel,11)
      call plstyl(1,1500,1500)
      call plcol0(3)
      call plcont(w,xdim,ydim,1,nptsx,1,nptsy,clevel,11)
      call plstyl(0,1500,1500)
      call plcol0(1)
      call pllab('X Coordinate', 'Y Coordinate', &
        'Streamlines of flow')
      call pl_setcontlabelparam(0.006_plflt, 0.3_plflt, 0.1_plflt, 0)

!      Plot using 1d coordinate transform
      call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
      call plcol0(2)
      call plcon1(z,xdim,ydim,1,nptsx,1,nptsy,clevel,11, xg1, yg1)
      call plstyl(1,1500,1500)
      call plcol0(3)
      call plcon1(w,xdim,ydim,1,nptsx,1,nptsy,clevel,11, xg1, yg1)
      call plstyl(0,1500,1500)
      call plcol0(1)
      call pllab('X Coordinate', 'Y Coordinate', &
        'Streamlines of flow')

!      Plot using 2d coordinate transform
      call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
      call plcol0(2)
      call plcon2(z,xdim,ydim,1,nptsx,1,nptsy,clevel,11, xg2, yg2)
      call plstyl(1,1500,1500)
      call plcol0(3)
      call plcon2(w,xdim,ydim,1,nptsx,1,nptsy,clevel,11, xg2, yg2)
      call plstyl(0,1500,1500)
      call plcol0(1)
      call pllab('X Coordinate', 'Y Coordinate', &
        'Streamlines of flow')

      call polar()
      call potential()

      call plend
      end

!      polar contour plot example.
      subroutine polar()
      use plplot, PI => PL_PI
      implicit none
      integer PERIMETERPTS, RPTS, THETAPTS, NLEVEL, xdim, ydim
      parameter(PERIMETERPTS = 100)
!      xdim and ydim are the absolute static size of the 2D arrays.
!      RPTS and THETAPTS are associated with the part of the
!      2D arrays that are defined.
      parameter(xdim=99, RPTS = 40)
      parameter(ydim=100, THETAPTS = 40)
      parameter(NLEVEL=10)
      integer i,j
      real(kind=plflt) xg(xdim, ydim),  yg(xdim, ydim), &
        z(xdim, ydim), px(PERIMETERPTS), py(PERIMETERPTS), &
        lev(NLEVEL), t, r, theta
      call plenv(-1._plflt, 1._plflt, -1._plflt, 1._plflt, 0, -2)
      call plcol0(1)
!      perimeter.
      do i = 1, PERIMETERPTS
        t = (2._plflt*PI/(PERIMETERPTS-1))*dble(i-1)
        px(i) = cos(t)
        py(i) = sin(t)
      enddo
      call plline(px, py)

!      create data to be contoured.
      do j = 1, THETAPTS
        theta = (2._plflt*PI/dble(THETAPTS-1))*dble(j-1)
        do i = 1, RPTS
          r = (i-1)/dble(RPTS-1)
          xg(i,j) = r*cos(theta)
          yg(i,j) = r*sin(theta)
          z(i,j) = r
        enddo
      enddo

!      create contour values.
      do i = 1, NLEVEL
        lev(i) = 0.05_plflt + 0.10_plflt*dble(i-1)
      enddo

!      plot the (polar) contours.
      call plcol0(2)
      call plcon2(z, xdim, ydim, 1, RPTS, 1, THETAPTS, &
        lev, NLEVEL, xg, yg)
      call plcol0(1)
      call pllab('', '', 'Polar Contour Plot')
      end

!      shielded potential contour plot example
      subroutine potential()
      use plplot
      implicit none

      integer NCX, NCY, NPLT, i, j, nx, ny, kx, lx, ky, ly, &
        nlevel, ilevgt, ilevlt, nlevlt, nlevgt, &
        ncollin, ncolbox, ncollab, &
        nxsub, nysub, xdim, ydim
      real(kind=plflt) TWOPI, r, theta, rmax, x0, &
        y0, xmin, xmax, eps, q1, d1, &
        ymin, ymax, &
        q1i, d1i, q2, d2, q2i, d2i, div1, div1i, div2, div2i, &
        zmin, zmax, dz, xpmin, xpmax, ypmin, ypmax, &
        xtick, ytick
!      xdim and ydim are the absolute static size of the 2D arrays.
!      NCX and NCY are associated with the part of the
!      2D arrays that are defined.
      parameter (xdim=99, NCX=40, ydim=100, NCY=64, NPLT=100)
      parameter (TWOPI=6.2831853071795864768_plflt)

      real(kind=plflt) z(xdim, ydim), ztmp(xdim, ydim+1)
      real(kind=plflt) xg(xdim, ydim+1), yg(xdim, ydim+1), xtm(NPLT), ytm(NPLT)

      real(kind=plflt) clevel(20)
      character*8 xopt, yopt

      nx = NCX
      ny = NCY

      kx = 1
      lx = nx
      ky = 1
      ly = ny

!      Set up r-theta grids
!      Tack on extra cell in theta to handle periodicity.

      do i = 1, nx
        r = i - 0.5_plflt
        do j = 1, ny
          theta = TWOPI/dble(ny-1) * (j-0.5_plflt)
          xg(i,j) = r * cos(theta)
          yg(i,j) = r * sin(theta)
        enddo
        xg(i, ny+1) = xg(i, 1)
        yg(i, ny+1) = yg(i, 1)
      enddo
      call a2mnmx(xg, nx, ny, xmin, xmax, xdim)
      call a2mnmx(yg, nx, ny, ymin, ymax, xdim)
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

      do i = 1, nx
        do j = 1, ny
          div1 = sqrt((xg(i,j)-d1)**2 + (yg(i,j)-d1)**2 + eps**2)
          div1i = sqrt((xg(i,j)-d1i)**2 + (yg(i,j)-d1i)**2 + eps**2)

          div2 = sqrt((xg(i,j)-d2)**2 + (yg(i,j)+d2)**2 + eps**2)
          div2i = sqrt((xg(i,j)-d2i)**2 + (yg(i,j)+d2i)**2 + eps**2)

          z(i,j) = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
        enddo
      enddo

!      Tack on extra cell in theta to handle periodicity.

      do i = 1, nx
        do j = 1, ny
          ztmp(i,j) = z(i,j)
        enddo
        ztmp(i, ny+1) = z(i, 1)
      enddo
      call a2mnmx(z, nx, ny, zmin, zmax, xdim)

!      Set up contour levels.

      nlevel = 20
      dz = abs(zmax - zmin)/dble (nlevel)
      do i = 1, nlevel
        clevel(i) = zmin + (i-0.5_plflt)*dz
      enddo

!      Split contours into two parts, z > 0, and z < 0.
!      Dashed contours will be at levels 'ilevlt' through 'ilevlt+nlevlt'.
!      Solid  contours will be at levels 'ilevgt' through 'ilevgt+nlevgt'.

      ilevlt = 1
      i = 1
      do while(i.le.nlevel.and.clevel(min(i,nlevel)).le.0._plflt)
        i = i + 1
      enddo
      nlevlt = i - 1
      ilevgt = ilevlt + nlevlt
      nlevgt = nlevel - nlevlt

!      Advance graphics frame and get ready to plot.

      ncollin = 11
      ncolbox = 1
      ncollab = 2

      call pladv(0)
      call plcol0(ncolbox)

!      Scale window to user coordinates.
!      Make a bit larger so the boundary doesn't get clipped.

      eps = 0.05_plflt
      xpmin = xmin - abs(xmin)*eps
      xpmax = xmax + abs(xmax)*eps
      ypmin = ymin - abs(ymin)*eps
      ypmax = ymax + abs(ymax)*eps

      call plvpas(0.1_plflt, 0.9_plflt, 0.1_plflt, &
        0.9_plflt, 1.0_plflt )
      call plwind(xpmin, xpmax, ypmin, ypmax)

      xopt = ' '
      yopt = ' '
      xtick = 0._plflt
      nxsub = 0
      ytick = 0._plflt
      nysub = 0

      call plbox(xopt, xtick, nxsub, yopt, ytick, nysub)

!      Call plotter once for z < 0 (dashed), once for z > 0 (solid lines).

      call plcol0(ncollin)
      if(nlevlt .gt. 0) then
         call pllsty(2)
         call plcon2(ztmp, xdim, ydim+1, kx, lx, ky, ly+1, &
           clevel(ilevlt), nlevlt, xg, yg)
      endif
      if(nlevgt .gt. 0) then
        call pllsty(1)
        call plcon2(ztmp, xdim, ydim+1, kx, lx, ky, ly+1, &
          clevel(ilevgt), nlevgt, xg, yg)
      endif

!      Draw boundary.

      do i = 1, NPLT
        theta = (TWOPI)/(NPLT-1) * dble (i-1)
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
