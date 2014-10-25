!      plshades demo, using color fill
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

      use plplot, PI => PL_PI
      implicit none
      real(kind=plflt) xx, yy, argx, argy, distort, r, t
      integer NLEVEL, NX, NY, PERIMETERPTS, xdim, ydim
      parameter(NLEVEL = 20)
!      xdim and ydim are the static dimensions of the 2D arrays while
!      NX and NY are the defined area.
      parameter(xdim=99, NX = 35)
      parameter(ydim=100, NY = 46)
      parameter(PERIMETERPTS = 100)
      real(kind=plflt) clevel(NLEVEL), shedge(NLEVEL+1), &
        z(xdim,ydim), w(xdim,ydim), xg1(xdim), yg1(ydim), &
        xg2(xdim,ydim), yg2(xdim,ydim), zmin, zmax, &
        px(PERIMETERPTS), py(PERIMETERPTS)
      integer cont_color
      real(kind=plflt) fill_width, cont_width
      real(kind=plflt) colorbar_width, colorbar_height
      integer NUM_AXES, NUM_LABELS
      parameter(NUM_AXES=1, NUM_LABELS=1)
      character(len=20) :: axis_opts(NUM_AXES)
      integer num_values(NUM_AXES)
      real(kind=plflt) values(NUM_AXES,NLEVEL+1)
      real(kind=plflt) axis_ticks(NUM_AXES)
      integer axis_subticks(NUM_AXES)
      character(len=100) :: labels(NUM_LABELS)
      integer label_opts(NUM_LABELS)

      integer i, j
!      dummy to fill argument list with something not currently used.
      character(len=1) defined
      real(kind=plflt) tr(6)

!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

!      Load color palettes
      call plspal0('cmap0_black_on_white.pal')
      call plspal1('cmap1_gray.pal',1)

!      Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display
      call plscmap0n(3)

!      Initialize plplot

      call plinit()
!      Set up transformation matrix

      tr(1) = 2._plflt/(NX-1)
      tr(2) = 0._plflt
      tr(3) = -1._plflt
      tr(4) = 0._plflt
      tr(5) = 2._plflt/(NY-1)
      tr(6) = -1._plflt

!      Calculate the data matrices.
      do i=1,NX
        xx = dble(i-1-(NX/2))/dble (NX/2)
        do j=1,NY
          yy = dble(j-1-(NY/2))/dble (NY/2) - 1.0_plflt
          z(i,j) = - sin(7._plflt*xx) * cos(7._plflt*yy) + xx*xx - yy*yy
          w(i,j) = - cos(7._plflt*xx) * sin(7._plflt*yy) + 2._plflt*xx*yy
        enddo
      enddo

      call a2mnmx(z, NX, NY, zmin, zmax, xdim)
      do i = 1, NLEVEL
        clevel(i) = zmin + (zmax - zmin) * (i - 0.5_plflt) / dble(NLEVEL)
      enddo
      do i = 1, NLEVEL+1
        shedge(i) = zmin + (zmax - zmin) * dble(i-1) / dble(NLEVEL)
      enddo

!      Build the 1-d coord arrays.
      distort = 0.4_plflt
      do i=1,NX
        xx = -1._plflt + dble(i-1)*2._plflt/dble(NX-1)
        xg1(i) = xx + distort*cos(0.5_plflt*PI*xx)
      enddo

      do j=1,NY
        yy = -1._plflt + dble(j-1)*2._plflt/dble(NY-1)
        yg1(j) = yy - distort*cos(0.5_plflt*PI*yy)
      enddo

!      Build the 2-d coord arrays.
      do i=1,NX
        xx = -1._plflt + dble(i-1)*2._plflt/dble(NX-1)
        argx = 0.5_plflt*PI*xx
        do j=1,NY
          yy = -1._plflt + dble(j-1)*2._plflt/dble(NY-1)
          argy = 0.5_plflt*PI*yy
          xg2(i,j) = xx + distort*cos(argx)*cos(argy)
          yg2(i,j) = yy - distort*cos(argx)*cos(argy)
        enddo
      enddo

!      Plot using transform of index range to xmin, xmax, ymin, ymax
      call pladv(0)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

      call plpsty(0)

      fill_width = 2
      cont_color = 0
      cont_width = 0
      axis_opts(1) = 'bcvtm'
      axis_ticks(1) = 0.0_plflt
      axis_subticks(1) = 0
      label_opts(1) = PL_COLORBAR_LABEL_BOTTOM
      labels(1) = 'Magnitude'

      call plshades(z(:NX,:NY), defined, -1._plflt, 1._plflt, -1._plflt, &
        1._plflt, &
        shedge, fill_width, &
        cont_color, cont_width )

      ! Smaller text
      call  plschr( 0.0_plflt, 0.75_plflt )
      ! Small ticks on the vertical axis
      call plsmaj( 0.0_plflt, 0.5_plflt )
      call plsmin( 0.0_plflt, 0.5_plflt )

      num_values(1) = NLEVEL + 1;
      values(1,:)   = shedge;
      call plcolorbar( colorbar_width, colorbar_height, &
            ior(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0, &
            0.005_plflt, 0.0_plflt, 0.0375_plflt, 0.875_plflt, 0, 1, 1, &
            0.0_plflt, 0.0_plflt, &
            cont_color, cont_width, &
            label_opts, labels, &
            axis_opts, &
            axis_ticks, axis_subticks, &
            num_values, values )

      ! Reset text and tick sizes
      call plschr( 0.0_plflt, 1.0_plflt )
      call plsmaj( 0.0_plflt, 1.0_plflt )
      call plsmin( 0.0_plflt, 1.0_plflt )

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon density')

!      Plot using 1d coordinate transform

      call plspal0('cmap0_black_on_white.pal')
      call plspal1('cmap1_blue_yellow.pal',1)
      call plscmap0n(3)

      call pladv(0)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

      call plpsty(0)

      fill_width = 2
      cont_color = 0
      cont_width = 0
      call plshades(z(:NX,:NY), defined, -1._plflt, 1._plflt, -1._plflt, &
        1._plflt, &
        shedge, fill_width, &
        cont_color, cont_width, xg1(:NX), yg1(:NY))

      ! Smaller text
      call  plschr( 0.0_plflt, 0.75_plflt )
      ! Small ticks on the vertical axis
      call plsmaj( 0.0_plflt, 0.5_plflt )
      call plsmin( 0.0_plflt, 0.5_plflt )

      num_values(1) = NLEVEL + 1;
      values(1,:)   = shedge;
      call plcolorbar( colorbar_width, colorbar_height, &
            ior(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0, &
            0.005_plflt, 0.0_plflt, 0.0375_plflt, 0.875_plflt, 0, 1, 1, &
            0.0_plflt, 0.0_plflt, &
            cont_color, cont_width, &
            label_opts, labels, &
            axis_opts, &
            axis_ticks, axis_subticks, &
            num_values, values )

      ! Reset text and tick sizes
      call plschr( 0.0_plflt, 1.0_plflt )
      call plsmaj( 0.0_plflt, 1.0_plflt )
      call plsmin( 0.0_plflt, 1.0_plflt )

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon density')

!      Plot using 2d coordinate transform

      call plspal0('cmap0_black_on_white.pal')
      call plspal1('cmap1_blue_red.pal',1)
      call plscmap0n(3)

      call pladv(0)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

      call plpsty(0)

      fill_width = 2
      cont_color = 0
      cont_width = 0
      call plshades(z(:NX,:NY), defined, -1._plflt, 1._plflt, -1._plflt, &
        1._plflt, &
        shedge, fill_width, &
        cont_color, cont_width, xg2(:NX,:NY), yg2(:NX,:NY) )

      ! Smaller text
      call  plschr( 0.0_plflt, 0.75_plflt )
      ! Small ticks on the vertical axis
      call plsmaj( 0.0_plflt, 0.5_plflt )
      call plsmin( 0.0_plflt, 0.5_plflt )

      num_values(1) = NLEVEL + 1;
      values(1,:)   = shedge;
      call plcolorbar( colorbar_width, colorbar_height, &
            ior(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0, &
            0.005_plflt, 0.0_plflt, 0.0375_plflt, 0.875_plflt, 0, 1, 1, &
            0.0_plflt, 0.0_plflt, &
            cont_color, cont_width, &
            label_opts, labels, &
            axis_opts, &
            axis_ticks, axis_subticks, &
            num_values, values )

      ! Reset text and tick sizes
      call plschr( 0.0_plflt, 1.0_plflt )
      call plsmaj( 0.0_plflt, 1.0_plflt )
      call plsmin( 0.0_plflt, 1.0_plflt )

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call plcont(w, 1, nx, 1, ny, clevel, xg2, yg2)
      call pllab('distance', 'altitude', &
        'Bogon density, with streamlines')

!      Plot using 2d coordinate transform and plshades contours.
      call plspal0('')
      call plspal1('',1)
      call plscmap0n(3)

      call pladv(0)
      call plvpor(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt)

      call plpsty(0)

      fill_width = 2
      cont_color = 2
      cont_width = 3
      call plshades(z(:NX,:NY), defined, -1._plflt, 1._plflt, -1._plflt, &
        1._plflt, &
        shedge, fill_width, &
        cont_color, cont_width, xg2(:NX,:NY), yg2(:NX,:NY) )

      ! Smaller text
      call  plschr( 0.0_plflt, 0.75_plflt )
      ! Small ticks on the vertical axis
      call plsmaj( 0.0_plflt, 0.5_plflt )
      call plsmin( 0.0_plflt, 0.5_plflt )

      num_values(1) = NLEVEL + 1;
      values(1,:)   = shedge;
      call plcolorbar( colorbar_width, colorbar_height, &
            ior(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0, &
            0.005_plflt, 0.0_plflt, 0.0375_plflt, 0.875_plflt, 0, 1, 1, &
            0.0_plflt, 0.0_plflt, &
            2, 3._plflt, &
            label_opts, labels, &
            axis_opts, &
            axis_ticks, axis_subticks, &
            num_values, values )

      ! Reset text and tick sizes
      call plschr( 0.0_plflt, 1.0_plflt )
      call plsmaj( 0.0_plflt, 1.0_plflt )
      call plsmin( 0.0_plflt, 1.0_plflt )

      call plcol0(1)
      call plbox('bcnst', 0.0_plflt, 0, 'bcnstv', 0.0_plflt, 0)
      call plcol0(2)
      call pllab('distance', 'altitude', 'Bogon density')

!      Example with polar coordinates.
      call plspal0('cmap0_black_on_white.pal')
      call plspal1('cmap1_gray.pal',1)
      call plscmap0n(3)

      call pladv(0)
      call plvpor(0.1d0, 0.9d0, 0.1d0, 0.9d0)
      call plwind(-1.d0, 1.d0, -1.d0, 1.d0)

      call plpsty(0)

!      Build new coordinate matrices.
      do i = 1, NX
        r = dble(i-1)/dble(NX-1)
        do j = 1, NY
          t = (2._plflt*PI/dble(NY-1))*dble(j-1)
          xg2(i,j) = r*cos(t)
          yg2(i,j) = r*sin(t)
          z(i,j) = exp(-r*r)*cos(5._plflt*PI*r)*cos(5._plflt*t)
        enddo
      enddo

!      Need a new shedge to go along with the new data set.
      call a2mnmx(z, NX, NY, zmin, zmax, xdim)
      do i = 1, NLEVEL+1
        shedge(i) = zmin + (zmax - zmin) * dble(i-1) / dble(NLEVEL)
      enddo

!      Now we can shade the interior region.
      fill_width = 2
      cont_color = 0
      cont_width = 0
      call plshades(z(:NX,:NY), defined, -1._plflt, 1._plflt, -1._plflt, &
        1._plflt, &
        shedge, fill_width, &
        cont_color, cont_width, xg2(:NX,:NY), yg2(:NX,:NY) )

      ! Smaller text
      call  plschr( 0.0_plflt, 0.75_plflt )
      ! Small ticks on the vertical axis
      call plsmaj( 0.0_plflt, 0.5_plflt )
      call plsmin( 0.0_plflt, 0.5_plflt )

      num_values(1) = NLEVEL + 1;
      values(1,:)   = shedge;
      call plcolorbar( colorbar_width, colorbar_height, &
            ior(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0, &
            0.005_plflt, 0.0_plflt, 0.0375_plflt, 0.875_plflt, 0, 1, 1, &
            0.0_plflt, 0.0_plflt, &
            cont_color, cont_width, &
            label_opts, labels, &
            axis_opts, &
            axis_ticks, axis_subticks, &
            num_values, values )

      ! Reset text and tick sizes
      call plschr( 0.0_plflt, 1.0_plflt )
      call plsmaj( 0.0_plflt, 1.0_plflt )
      call plsmin( 0.0_plflt, 1.0_plflt )

!      Now we can draw the perimeter.  (If do before, shade stuff may overlap.)
      do i = 1, PERIMETERPTS
        t = (2._plflt*PI/dble(PERIMETERPTS-1))*dble(i-1)
        px(i) = cos(t)
        py(i) = sin(t)
      enddo

      call plcol0(1)
      call plline(px, py)

!      And label the plot.
      call plcol0(2)
      call pllab( '', '',  'Tokamak Bogon Instability' )

      call plend
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
