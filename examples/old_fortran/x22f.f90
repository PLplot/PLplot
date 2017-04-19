!      Vector plot demo.
!
!      Copyright (C) 2004  Alan W. Irwin
!      Copyright (C) 2004  Andrew Ross
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

!      Does several contour plots using different coordinate mappings.
      use plplot
      implicit none

      integer narr
      logical fill
      parameter (narr=6)
      real(kind=plflt) arrow_x(narr),arrow_y(narr), &
        arrow2_x(narr),arrow2_y(narr)

      data arrow_x/-0.5_plflt, 0.5_plflt, 0.3_plflt, 0.5_plflt, 0.3_plflt, 0.5_plflt/
      data arrow_y/0._plflt, 0._plflt, 0.2_plflt, 0._plflt, -0.2_plflt, 0._plflt/
      data arrow2_x/-0.5_plflt, 0.3_plflt, 0.3_plflt, 0.5_plflt, 0.3_plflt, 0.3_plflt/
      data arrow2_y/0._plflt, 0._plflt, 0.2_plflt, 0._plflt, -0.2_plflt, 0._plflt/

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

      call plinit


      call circulation

      fill = .false.

!     Set arrow style using arrow_x and arrow_y the
!     plot using these arrows
      call plsvect(arrow_x, arrow_y, fill)
      call constriction( 1 )

!     Set arrow style using arrow_x and arrow_y the
!     plot using these arrows
      fill = .true.
      call plsvect(arrow2_x, arrow2_y, fill)
      call constriction( 2 )

      call constriction2

      call plsvect

      call potential

      call plend

      end

!     vector plot of the circulation around the origin
      subroutine circulation()
      use plplot
      implicit none

      integer i, j, nx, ny
      parameter (nx=20, ny=20)

      real(kind=plflt) u(nx, ny), v(nx, ny), xg(nx,ny), yg(nx,ny)

      real(kind=plflt) dx, dy, xmin, xmax, ymin, ymax
      real(kind=plflt) xx, yy, scaling

      dx = 1.0_plflt
      dy = 1.0_plflt

      xmin = -dble(nx)/2.0_plflt*dx
      xmax = dble(nx)/2.0_plflt*dx
      ymin = -dble(ny)/2.0_plflt*dy
      ymax = dble(ny)/2.0_plflt*dy

      do i=1,nx
        xx = (dble(i)-nx/2.0_plflt-0.5_plflt)*dx
        do j=1,ny
          yy = (dble(j)-ny/2.0_plflt-0.5_plflt)*dy
          xg(i,j) = xx
          yg(i,j) = yy
          u(i,j) = yy
          v(i,j) = -xx
        enddo
      enddo

      call plenv(xmin, xmax, ymin, ymax, 0, 0)
      call pllab('(x)', '(y)',  &
       '#frPLplot Example 22 - circulation')
      call plcol0(2)
      scaling = 0.0_plflt
      call plvect(u,v,scaling,xg,yg)
      call plcol0(1)

      end

!     vector plot of the flow through a constricted pipe
      subroutine constriction( astyle )
      use plplot, PI => PL_PI
      implicit none

      integer i, j, nx, ny, astyle
      parameter (nx=20, ny=20)

      character(len=80) :: title

      real(kind=plflt) u(nx, ny), v(nx, ny), xg(nx,ny), yg(nx,ny)

      real(kind=plflt) dx, dy, xmin, xmax, ymin, ymax
      real(kind=plflt) xx, yy, Q, b, dbdx, scaling

      dx = 1.0_plflt
      dy = 1.0_plflt

      xmin = -dble(nx)/2.0_plflt*dx
      xmax = dble(nx)/2.0_plflt*dx
      ymin = -dble(ny)/2.0_plflt*dy
      ymax = dble(ny)/2.0_plflt*dy

      Q = 2.0_plflt
      do i=1,nx
        xx = (dble(i)-dble(nx)/2.0_plflt-0.5_plflt)*dx
        do j=1,ny
          yy = (dble(j)-dble(ny)/2.0_plflt-0.5_plflt)*dy
          xg(i,j) = xx
          yg(i,j) = yy
          b = ymax/4.0_plflt*(3.0_plflt-cos(PI*xx/xmax))
          if (abs(yy).lt.b) then
             dbdx = ymax/4.0_plflt*sin(PI*xx/xmax)*PI/xmax*yy/b
             u(i,j) = Q*ymax/b
             v(i,j) = u(i,j)*dbdx
          else
             u(i,j) = 0.0_plflt
             v(i,j) = 0.0_plflt
          endif
        enddo
      enddo

      call plenv(xmin, xmax, ymin, ymax, 0, 0)
      write(title,'(A,I0,A)') '#frPLplot Example 22 - constriction (arrow style ', astyle,')'
      call pllab('(x)', '(y)', title)
      call plcol0(2)
      scaling = -1.0_plflt
      call plvect(u,v,scaling,xg,yg)
      call plcol0(1)

      end

! Global transform function for a constriction using data passed in
! This is the same transformation used in constriction.
      subroutine transform( x, y, xt, yt )
      use plplot, PI => PL_PI
      implicit none

      real(kind=plflt) x, y, xt, yt

      real(kind=plflt) xmax
      common /transform_data/ xmax

      xt = x
      yt = y / 4.0_plflt * ( 3.0_plflt - cos( PI * x / xmax ) )
      end subroutine transform

! Vector plot of flow through a constricted pipe
! with a coordinate transform
      subroutine constriction2()
      use plplot, PI => PL_PI
      implicit none

      integer i, j, nx, ny, nc, nseg
      parameter (nx=20, ny=20, nc=11, nseg=20)

      real(kind=plflt) dx, dy, xx, yy
      real(kind=plflt) xmin, xmax, ymin, ymax
      real(kind=plflt) Q, b, scaling
      real(kind=plflt) u(nx, ny), v(nx, ny), xg(nx,ny), yg(nx,ny)
      real(kind=plflt) clev(nc);
      common /transform_data/ ymax
      character(len=1) defined

      external transform

      dx = 1.0_plflt
      dy = 1.0_plflt

      xmin = -dble(nx)/2.0_plflt*dx
      xmax = dble(nx)/2.0_plflt*dx
      ymin = -dble(ny)/2.0_plflt*dy
      ymax = dble(ny)/2.0_plflt*dy


      call plstransform( transform )

      Q = 2.0_plflt
      do i=1,nx
        xx = (dble(i)-dble(nx)/2.0_plflt-0.5_plflt)*dx
        do j=1,ny
          yy = (dble(j)-dble(ny)/2.0_plflt-0.5_plflt)*dy
          xg(i,j) = xx
          yg(i,j) = yy
          b = ymax/4.0_plflt*(3.0_plflt-cos(PI*xx/xmax))
          u(i,j) = Q*ymax/b
          v(i,j) = 0.0_plflt
        enddo
      enddo

      do i=1,nc
         clev(i) = Q + dble(i-1) * Q / ( dble(nc) - 1.0_plflt )
      enddo

      call plenv(xmin, xmax, ymin, ymax, 0, 0)
      call pllab('(x)', '(y)', &
           '#frPLplot Example 22 - constriction with plstransform')
      call plcol0(2)
      call plshades(u, defined, xmin + dx / 2.0_plflt, &
           xmax - dx / 2.0_plflt, &
           ymin + dy / 2.0_plflt, ymax - dy / 2.0_plflt, &
           clev, 0.0_plflt, 1, 1.0_plflt, .false. )
      scaling = -1.0_plflt
      call plvect(u,v,scaling,xg,yg)
      call plpath(nseg, xmin, ymax, xmax, ymax)
      call plpath(nseg, xmin, ymin, xmax, ymin)
      call plcol0(1)

      call plstransform

      end subroutine constriction2

      subroutine potential()
      use plplot, PI => PL_PI
      implicit none

      integer i, j, nr, ntheta, nper, nlevel
      parameter (nr=20, ntheta=20, nper=100, nlevel=10)

      real(kind=plflt) u(nr, ntheta), v(nr, ntheta), z(nr, ntheta)
      real(kind=plflt) xg(nr,ntheta), yg(nr,ntheta)
      real(kind=plflt) clevel(nlevel), px(nper), py(nper)

      real(kind=plflt) xmin, xmax, ymin, ymax, zmin, zmax, rmax
      real(kind=plflt) xx, yy, r, theta, scaling, dz

      real(kind=plflt) eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i
      real(kind=plflt) div1, div1i, div2, div2i

      rmax = dble(nr)

      eps = 2.0_plflt

      q1 = 1.0_plflt
      d1 = rmax/4.0_plflt

      q1i = - q1*rmax/d1
      d1i = rmax**2.0_plflt/d1

      q2 = -1.0_plflt
      d2 = rmax/4.0_plflt

      q2i = - q2*rmax/d2
      d2i = rmax**2.0_plflt/d2

      do i = 1, nr
         r = 0.5 + dble(i-1)
        do j = 1, ntheta
          theta = 2.*PI/dble(ntheta-1)*(dble(j)-0.5)
          xx = r*cos(theta)
          yy = r*sin(theta)
          xg(i,j) = xx
          yg(i,j) = yy
          div1 = sqrt((xg(i,j)-d1)**2 + (yg(i,j)-d1)**2 + eps**2)
          div1i = sqrt((xg(i,j)-d1i)**2 + (yg(i,j)-d1i)**2 + eps**2)

          div2 = sqrt((xg(i,j)-d2)**2 + (yg(i,j)+d2)**2 + eps**2)
          div2i = sqrt((xg(i,j)-d2i)**2 + (yg(i,j)+d2i)**2 + eps**2)

          z(i,j) = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
          u(i,j) = -q1*(xx-d1)/div1**3 - q1i*(xx-d1i)/div1i**3 - &
              q2*(xx-d2)/div2**3 - q2i*(xx-d2i)/div2i**3
          v(i,j) = -q1*(yy-d1)/div1**3 - q1i*(yy-d1i)/div1i**3 - &
              q2*(yy+d2)/div2**3 - q2i*(yy+d2i)/div2i**3
        enddo
      enddo

      call a2mnmx(xg, nr, ntheta, xmin, xmax, nr)
      call a2mnmx(yg, nr, ntheta, ymin, ymax, nr)
      call a2mnmx(z, nr, ntheta, zmin, zmax, nr)

      call plenv(xmin, xmax, ymin, ymax, 0, 0)
      call pllab('(x)', '(y)',  &
        '#frPLplot Example 22 - potential gradient vector plot')

!     plot contours of the potential
      dz = abs(zmax - zmin)/dble (nlevel)
      do i = 1, nlevel
         clevel(i) = zmin + (i-0.5_plflt)*dz
      enddo
      call plcol0(3)
      call pllsty(2)
      call plcont(z,1,nr,1,ntheta,clevel,xg,yg)
      call pllsty(1)
      call plcol0(1)

      call plcol0(2)
      scaling = 25.0_plflt
      call plvect(u,v,scaling,xg,yg)
      call plcol0(1)

      do i=1,nper
         theta = 2.0_plflt*PI/dble(nper-1)*dble(i)
         px(i) = rmax*cos(theta)
         py(i) = rmax*sin(theta)
      enddo

      call plline(px,py)

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
