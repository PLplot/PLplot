!      Grid data demo.
!
!      Copyright (C) 2004  Joao Cardoso
!      Copyright (C) 2008  Andrew Ross
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

      use plplot, PI => PL_PI

      implicit none

      external myisnan
      logical myisnan

      integer pts, xp, yp, nl, knn_order, randn, rosen
      real(kind=plflt) threshold, wmin
      parameter (pts = 500)
      parameter (xp = 25)
      parameter (yp = 20)
      parameter (nl = 16)
      parameter (knn_order = 20)
      parameter (threshold = 1.001_plflt)
      parameter (wmin = -1e3_plflt)
      parameter (randn = 0)
      parameter (rosen = 0)

      real(kind=plflt) xmin, xmax, ymin, ymax

      real(kind=plflt) x(pts), y(pts), z(pts), clev(nl)
      real(kind=plflt) xg(xp), yg(yp), zg(xp,yp)
      real(kind=plflt) zmin, zmax, lzmin, lzmax
      integer i, j, k
      integer alg
      character(len=80) title(6)
      data title /'Cubic Spline Approximation', &
                 'Delaunay Linear Interpolation', &
                 'Natural Neighbors Interpolation', &
                 'KNN Inv. Distance Weighted', &
                 '3NN Linear Interpolation', &
                 '4NN Around Inv. Dist. Weighted'/

      real(kind=plflt) opt(6)
      data opt /0._plflt, 0._plflt, 0._plflt, 0._plflt, 0._plflt, 0._plflt/

      real(kind=plflt) xt, yt

      real(kind=plflt) r
      integer ii, jj
      real(kind=plflt) dist, d

      character(len=1) defined

      xmin = -0.2_plflt
      ymin = -0.2_plflt
      xmax = 0.6_plflt
      ymax = 0.6_plflt

!      call plMergeOpts(options, "x21c options", NULL);
      call plparseopts(PL_PARSE_FULL)

      opt(3) = wmin
      opt(4) = dble(knn_order)
      opt(5) = threshold

! Initialize plplot

      call plinit

      call cmap1_init

      call plseed(5489)

      do i=1,pts
         xt = (xmax-xmin)*plrandd()
         yt = (ymax-ymin)*plrandd()
         if (randn.eq.0) then
            x(i) = xt + xmin
            y(i) = yt + ymin
         else
            x(i) = sqrt(-2._plflt*log(xt)) * cos(2._plflt*PI*yt) + xmin
            y(i) = sqrt(-2._plflt*log(xt)) * sin(2._plflt*PI*yt) + ymin
         endif
         if (rosen.eq.0) then
            r = sqrt(x(i)*x(i) + y(i)*y(i))
            z(i) = exp(-r*r)*cos(2._plflt*PI*r)
         else
            z(i) = log((1._plflt-x(i))**2 + 100._plflt*(y(i)-x(i)**2)**2)
         endif
      enddo

      zmin = z(1)
      zmax = z(1)
      do i=2,pts
         zmax = max(zmax,z(i))
         zmin = min(zmin,z(i))
      enddo

      do i=1,xp
         xg(i) = xmin + (xmax-xmin)*(i-1._plflt)/(xp-1._plflt)
      enddo
      do i=1,yp
         yg(i) = ymin + (ymax-ymin)*(i-1._plflt)/(yp-1._plflt)
      enddo

      call plcol0(1)
      call plenv(xmin, xmax, ymin, ymax, 2, 0)
      call plcol0(15)
      call pllab("X", "Y", "The original data sampling")
      do i=1,pts
         call plcol1( ( z(i) - zmin ) / ( zmax - zmin ) )
!     The following plstring call should be the the equivalent of
!     plpoin( 1, &x[i], &y[i], 5 ); Use plstring because it is
!     not deprecated like plpoin and has much more powerful
!     capabilities.  N.B. symbol 141 works for Hershey devices
!     (e.g., -dev xwin) only if plfontld( 0 ) has been called
!     while symbol 727 works only if plfontld( 1 ) has been
!     called.  The latter is the default which is why we use 727
!     here to represent a centred X (multiplication) symbol.
!     This dependence on plfontld is one of the limitations of
!     the Hershey escapes for PLplot, but the upside is you get
!     reasonable results for both Hershey and Unicode devices.
        call plstring( x(i:i), y(i:i), '#(727)' );
      enddo
      call pladv(0)

      call plssub(3,2)

      do k=1,2
         call pladv(0)
         do alg=1,6

            call plgriddata(x, y, z, xg, yg, zg, alg, opt(alg))

!     - CSA can generate NaNs (only interpolates? !).
!     - DTLI and NNI can generate NaNs for points outside the convex hull
!     of the data points.
!     - NNLI can generate NaNs if a sufficiently thick triangle is not found
!
!     PLplot should be NaN/Inf aware, but changing it now is quite a job...
!     so, instead of not plotting the NaN regions, a weighted average over
!     the neighbors is done.
!

            if ((alg.eq.GRID_CSA).or.(alg.eq.GRID_DTLI).or. &
                 (alg.eq.GRID_NNLI).or.(alg.eq.GRID_NNI)) then

               do i=1,xp
                  do j=1,yp
                     if (myisnan(zg(i,j))) then
!     average (IDW) over the 8 neighbors

                        zg(i,j) = 0._plflt
                        dist = 0._plflt

                        ii=i-1
                        do while ((ii.le.i+1).and.(ii.le.xp))
                           jj = j-1
                           do while ((jj.le.j+1).and.(jj.le.yp))
                              if ((ii.ge.1) .and. (jj.ge.1) .and. &
                                   (.not.myisnan(zg(ii,jj))) ) then
                                 if (abs(ii-i) + abs(jj-j) .eq. 1) then
                                    d = 1._plflt
                                 else
                                    d = 1.4142_plflt
                                 endif
                                 zg(i,j) = zg(i,j) + zg(ii,jj)/(d*d)
                                 dist = dist + d
                              endif
                              jj = jj+1
                           enddo
                           ii = ii+1
                        enddo
                        if (dist.ne.0._plflt) then
                           zg(i,j) = zg(i,j) / dist
                        else
                           zg(i,j) = zmin
                        endif
                     endif
                  enddo
               enddo
            endif

            call a2mnmx(zg, xp, yp, lzmin, lzmax, xp)

            lzmin = min(lzmin, zmin)
            lzmax = max(lzmax, zmax)

            lzmin = lzmin - 0.01_plflt
            lzmax = lzmax + 0.01_plflt

            call plcol0(1)
            call pladv(alg)

            if (k.eq.1) then

               do i=1,nl
                  clev(i) = lzmin + (lzmax-lzmin)/(nl-1._plflt)*(i-1._plflt)
               enddo
               call plenv0(xmin, xmax, ymin, ymax, 2, 0)
               call plcol0(15)
               call pllab("X", "Y", title(alg))
               call plshades(zg, defined, xmin, xmax, ymin, &
                    ymax, clev, 1._plflt, 0, 1._plflt)
               call plcol0(2)
            else

               do i = 1,nl
                  clev(i) = lzmin + (lzmax-lzmin)/(nl-1._plflt)*(i-1._plflt)
               enddo
               call plvpor(0._plflt, 1._plflt, 0._plflt, 0.9_plflt)
               call plwind(-1.1_plflt, 0.75_plflt, -0.65_plflt, 1.20_plflt)
!
!     For the comparison to be fair, all plots should have the
!     same z values, but to get the max/min of the data generated
!     by all algorithms would imply two passes. Keep it simple.
!
!     plw3d(1., 1., 1., xmin, xmax, ymin, ymax, zmin, zmax, 30, -60);
!

               call plw3d(1._plflt, 1._plflt, 1._plflt, xmin, xmax, ymin, ymax,  &
                    lzmin, lzmax, 30._plflt, -40._plflt)
               call plbox3("bntu", "X", 0._plflt, 0, &
                   "bntu", "Y", 0._plflt, 0, &
                   "bcdfntu", "Z", 0.5_plflt, 0)
               call plcol0(15)
               call pllab("", "", title(alg))
               call plot3dc(xg, yg, zg, ior(ior(DRAW_LINEXY, &
                   MAG_COLOR), BASE_CONT), clev)
            endif
         enddo
      enddo

      call plend

      end

      subroutine cmap1_init
        use plplot
        implicit none
        real(kind=plflt) i(2), h(2), l(2), s(2)

        i(1) = 0._plflt
        i(2) = 1._plflt

        h(1) = 240._plflt
        h(2) = 0._plflt

        l(1) = 0.6_plflt
        l(2) = 0.6_plflt

        s(1) = 0.8_plflt
        s(2) = 0.8_plflt

        call plscmap1n(256)
        call plscmap1l(.false., i, h, l, s)
      end subroutine cmap1_init


!----------------------------------------------------------------------------
!      Subroutine a2mnmx
!      Minimum and the maximum elements of a 2-d array.

      subroutine a2mnmx(f, nx, ny, fmin, fmax, xdim)
        use plplot
        implicit none

        integer   i, j, nx, ny, xdim
        real(kind=plflt) f(xdim, ny), fmin, fmax

        fmax = f(1, 1)
        fmin = fmax
        do j = 1, ny
           do  i = 1, nx
              fmax = max(fmax, f(i, j))
              fmin = min(fmin, f(i, j))
           enddo
        enddo
      end subroutine a2mnmx

      include 'plf95demos.inc'


