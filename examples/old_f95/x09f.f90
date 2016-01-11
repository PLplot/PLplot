!   Contour plot demo.
!
!   Copyright (C) 2004  Alan W. Irwin
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU Library General Public License as
!   published by the Free Software Foundation; either version 2 of the
!   License, or (at your option) any later version.
!
!   PLplot is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU Library General Public License for more details.
!
!   You should have received a copy of the GNU Library General Public
!   License along with PLplot; if not, write to the Free Software
!   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

!   Does several contour plots using different coordinate mappings.

program x09f95
    use plplot, PI => PL_PI, TWOPI => PL_TWOPI
    use plf95demolib
    implicit none
    integer i, j

!   xdim and ydim are the absolute static dimensions.
!   nptsx, and nptsy are the (potentially dynamic) defined area of the 2D
!   arrays that is actually used.
    integer, parameter :: xdim=99, ydim=100, nptsx=35, nptsy=46

    real(kind=plflt)   :: z(xdim, ydim), w(xdim, ydim), &
                          xg1(xdim), yg1(ydim), &
                          xg2(xdim, ydim), yg2(xdim, ydim)
    real(kind=plflt)   :: xc(nptsx), yc(nptsy)
    real(kind=plflt)   :: xx, yy, argx, argy, distort
    real(kind=plflt)   :: tr(6)

    real(kind=plflt)   :: clevel(11) = &
        (/ -1._plflt, -0.8_plflt, -0.6_plflt, -0.4_plflt, -0.2_plflt, &
            0._plflt,  0.2_plflt,  0.4_plflt,  0.6_plflt,  0.8_plflt, 1._plflt /)

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

    tr = (/ 2._plflt/dble(nptsx-1), 0.0_plflt, -1.0_plflt, &
            0.0_plflt, 2._plflt/dble(nptsy-1), -1.0_plflt /)

!   Calculate the data matrices.
    xc = (arange(0,nptsx) - (nptsx/2)) / dble(nptsx/2)
    yc = (arange(0,nptsy) - (nptsy/2)) / dble(nptsy/2) - 1.0_plflt

    do i=1,nptsx
        do j=1,nptsy
            z(i,j) = xc(i)**2 - yc(j)**2
            w(i,j) = 2._plflt*xc(i)*yc(j)
        enddo
    enddo

!   Build the 1-d coord arrays.
    distort = 0.4_plflt

    xg1(1:nptsx) = coord_function( arange(0,nptsx) / dble(nptsx-1),  distort )
    yg1(1:nptsy) = coord_function( arange(0,nptsy) / dble(nptsy-1), -distort )

!   Build the 2-d coord arrays.
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

!   Plot using identity transform
    call pl_setcontlabelformat(4, 3)
    call pl_setcontlabelparam(0.006_plflt, 0.3_plflt, 0.1_plflt, 1)
    call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
    call plcol0(2)
    call plcont(z(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, tr)
    call plstyl(1, 1500, 1500)
    call plcol0(3)
    call plcont(w(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, tr)
    call plstyl(0, 1500, 1500)
    call plcol0(1)
    call pllab('X Coordinate', 'Y Coordinate', 'Streamlines of flow')
    call pl_setcontlabelparam(0.006_plflt, 0.3_plflt, 0.1_plflt, 0)

!   Plot using 1d coordinate transform
    call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
    call plcol0(2)
    call plcont(z(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, xg1(1:nptsx), yg1(1:nptsy))
    call plstyl(1, 1500, 1500)
    call plcol0(3)
    call plcont(w(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, xg1(1:nptsx), yg1(1:nptsy))
    call plstyl(0, 1500, 1500)
    call plcol0(1)
    call pllab('X Coordinate', 'Y Coordinate', 'Streamlines of flow')

!   Plot using 2d coordinate transform
    call plenv(-1.0_plflt, 1.0_plflt, -1.0_plflt, 1.0_plflt, 0, 0)
    call plcol0(2)
    call plcont(z(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, xg2(1:nptsx,1:nptsy), yg2(1:nptsx,1:nptsy))
    call plstyl(1, 1500, 1500)
    call plcol0(3)
    call plcont(w(1:nptsx,1:nptsy), 1, nptsx, 1, nptsy, clevel, xg2(1:nptsx,1:nptsy), yg2(1:nptsx,1:nptsy))
    call plstyl(0, 1500, 1500)
    call plcol0(1)
    call pllab('X Coordinate', 'Y Coordinate', 'Streamlines of flow')

    call polar()
    call potential()

    call plend
contains

!----------------------------------------------------------------------------
!   Auxiliary function to compute the coordinates

elemental real(kind=plflt) function coord_function( coord, factor )
    real(kind=plflt), intent(in) :: coord
    real(kind=plflt), intent(in) :: factor

    real(kind=plflt)             :: tcoord

    tcoord         = -1.0_plflt + coord * 2.0_plflt
    coord_function = tcoord + factor*cos(0.5_plflt*PI*tcoord)
end function coord_function

!----------------------------------------------------------------------------
!   polar contour plot example.
subroutine polar()
    integer, parameter :: PERIMETERPTS = 100

!   xdim and ydim are the absolute static size of the 2D arrays.
!   RPTS and THETAPTS are associated with the part of the
!   2D arrays that are defined.
    integer, parameter :: xdim=99, RPTS = 40
    integer, parameter :: ydim=100, THETAPTS = 40
    integer, parameter :: NLEVEL=10
    integer            :: i,j
    real(kind=plflt)   :: xg(xdim, ydim),  yg(xdim, ydim), &
        z(xdim, ydim), px(PERIMETERPTS), py(PERIMETERPTS), &
        lev(NLEVEL), r, theta, delta

    call plenv(-1._plflt, 1._plflt, -1._plflt, 1._plflt, 0, -2)
    call plcol0(1)

!   perimeter.
    delta = 2._plflt*PI/(PERIMETERPTS-1)
    px = cos(delta*arange(0, PERIMETERPTS))
    py = sin(delta*arange(0, PERIMETERPTS))

    call plline(px, py)

!   create data to be contoured.
    do j = 1, THETAPTS
        theta = (2._plflt*PI/dble(THETAPTS-1))*dble(j-1)
        do i = 1, RPTS
            r = (i-1)/dble(RPTS-1)
            xg(i,j) = r*cos(theta)
            yg(i,j) = r*sin(theta)
            z(i,j) = r
        enddo
    enddo

!   create contour values.
    lev = 0.05_plflt + 0.10_plflt * arange(0,nlevel)

!   plot the (polar) contours.
    call plcol0(2)
    call plcont(z, 1, RPTS, 1, THETAPTS, lev, xg, yg)
    call plcol0(1)
    call pllab('', '', 'Polar Contour Plot')
end subroutine polar

!----------------------------------------------------------------------------
!    shielded potential contour plot example
subroutine potential()

    integer          :: i, j, nx, ny, kx, lx, ky, ly, &
        nlevel, ilevgt, ilevlt, nlevlt, nlevgt, &
        ncollin, ncolbox, ncollab, &
        nxsub, nysub
    real(kind=plflt) :: r, theta, rmax, x0, &
        y0, xmin, xmax, eps, q1, d1, &
        ymin, ymax, &
        q1i, d1i, q2, d2, q2i, d2i, div1, div1i, div2, div2i, &
        zmin, zmax, dz, xpmin, xpmax, ypmin, ypmax, &
        xtick, ytick, delta

!    xdim and ydim are the absolute static size of the 2D arrays.
!    NCX and NCY are associated with the part of the
!    2D arrays that are defined.
    integer, parameter :: xdim=99, NCX=40, ydim=100, NCY=64, NPLT=100

    real(kind=plflt)   :: z(xdim, ydim), ztmp(xdim, ydim+1)
    real(kind=plflt)   :: xg(xdim, ydim+1), yg(xdim, ydim+1), xtm(NPLT), ytm(NPLT)

    real(kind=plflt)   :: clevel(20)
    character(len=8)   :: xopt, yopt

    nx = NCX
    ny = NCY

    kx = 1
    lx = nx
    ky = 1
    ly = ny

!    Set up r-theta grids
!    Tack on extra cell in theta to handle periodicity.

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

    xmax = maxval( xg(1:nx,1:ny) )
    xmin = minval( xg(1:nx,1:ny) )
    ymax = maxval( yg(1:nx,1:ny) )
    ymin = minval( yg(1:nx,1:ny) )

    rmax = r
    x0 = (xmin + xmax)/2._plflt
    y0 = (ymin + ymax)/2._plflt

!    Potential inside a conducting cylinder (or sphere) by method of images.
!    Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
!    Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
!    Also put in smoothing term at small distances.

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
            div1  = sqrt((xg(i,j)-d1)**2  + (yg(i,j)-d1)**2  + eps**2)
            div1i = sqrt((xg(i,j)-d1i)**2 + (yg(i,j)-d1i)**2 + eps**2)

            div2  = sqrt((xg(i,j)-d2)**2  + (yg(i,j)+d2)**2  + eps**2)
            div2i = sqrt((xg(i,j)-d2i)**2 + (yg(i,j)+d2i)**2 + eps**2)

            z(i,j) = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
        enddo
    enddo

!   Tack on extra cell in theta to handle periodicity.

    ztmp(:,1:ny) = z
    ztmp(:,ny+1:ny+1) = z(:,1:1)

    zmax = maxval( z(1:nx,1:ny) )
    zmin = minval( z(1:nx,1:ny) )

!   Set up contour levels.

    nlevel = 20
    dz = abs(zmax - zmin)/dble (nlevel)
    clevel(1:nlevel) = zmin + (arange(1,nlevel+1) - 0.5_plflt) * dz

!   Split contours into two parts, z > 0, and z < 0.
!   Dashed contours will be at levels 'ilevlt' through 'ilevlt+nlevlt'.
!   Solid  contours will be at levels 'ilevgt' through 'ilevgt+nlevgt'.
!
!   Since the array clevel is ordered, we can find the level
!   where the values become positive by counting the non-positive
!   elements

    ilevlt = 1
    nlevlt = count( clevel(1:nlevel) <= 0.0_plflt )
    ilevgt = ilevlt + nlevlt
    nlevgt = nlevel - nlevlt

!   Advance graphics frame and get ready to plot.

    ncollin = 11
    ncolbox = 1
    ncollab = 2

    call pladv(0)
    call plcol0(ncolbox)

!   Scale window to user coordinates.
!   Make a bit larger so the boundary doesn't get clipped.

    eps = 0.05_plflt
    xpmin = xmin - abs(xmin)*eps
    xpmax = xmax + abs(xmax)*eps
    ypmin = ymin - abs(ymin)*eps
    ypmax = ymax + abs(ymax)*eps

    call plvpas(0.1_plflt, 0.9_plflt, 0.1_plflt, 0.9_plflt, 1.0_plflt )
    call plwind(xpmin, xpmax, ypmin, ypmax)

    xopt = ' '
    yopt = ' '
    xtick = 0._plflt
    nxsub = 0
    ytick = 0._plflt
    nysub = 0

    call plbox(xopt, xtick, nxsub, yopt, ytick, nysub)

!   Call plotter once for z < 0 (dashed), once for z > 0 (solid lines).

    call plcol0(ncollin)
    if (nlevlt > 0) then
        call pllsty(2)
        call plcont(ztmp, kx, lx, ky, ly+1, &
                 clevel(ilevlt:nlevlt), xg, yg)
    endif
    if (nlevgt > 0) then
        call pllsty(1)
        call plcont(ztmp, kx, lx, ky, ly+1, &
                 clevel(ilevgt:ilevgt-1+nlevgt), xg, yg)
    endif

!   Draw boundary.

    delta = TWOPI/(NPLT-1)
    xtm = x0 + rmax * cos(delta*arange(0,NPLT))
    ytm = y0 + rmax * sin(delta*arange(0,NPLT))

    call plcol0(ncolbox)
    call plline(xtm, ytm)

    call plcol0(ncollab)
    call pllab('', '', 'Shielded potential of charges in a conducting sphere')
end subroutine potential
end program x09f95
