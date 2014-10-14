!   3-d plot demo
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

program x08f95
    use plplot, PI => PL_PI
    use plf95demolib

    implicit none

    integer(kind=plint) :: i, j, k, ifshade

!   xdim is the leading dimension of z, xpts <= xdim is the leading
!   dimension of z that is defined.
    integer(kind=plint), parameter :: xdim=99, ydim=100, xpts=35, ypts=46
    real(kind=plflt)   :: x(xdim), y(ydim), z(xdim,ypts), xx, yy, r

    character (len=80) :: title(2_plint) = &
        (/'#frPLplot Example 8 - Alt=60, Az=30', &
          '#frPLplot Example 8 - Alt=20, Az=60'/)
    real(kind=plflt)   :: alt(2_plint) = (/60.0_plflt, 20.0_plflt/)
    real(kind=plflt)   :: az(2_plint)  = (/30.0_plflt, 60.0_plflt/)
    integer(kind=plint)            :: rosen
    integer(kind=plint), parameter :: nlevel = 10_plint
    real(kind=plflt)   :: zmin, zmax, step, clevel(nlevel)

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

    rosen = 1_plint

    x = (arange(0_plint,xpts) - xpts/2) / dble(xpts/2)
    y = (arange(0_plint,ypts) - ypts/2) / dble(ypts/2)

    if ( rosen == 1_plint ) then
        x = 1.5_plflt * x
        y = y + 0.5_plflt
    endif

    do i=1,xpts
        xx = x(i)
        do j=1,ypts
            yy = y(j)
            if (rosen == 1_plint) then
                z(i,j) = (1._plflt - xx)**2 + 100._plflt*(yy - xx**2)**2

                ! The log argument may be zero for just the right grid.
                if (z(i,j) > 0._plflt) then
                    z(i,j) = log(z(i,j))
                else
                    z(i,j) = -5._plflt
                endif
            else
                ! Sombrero function
                r = sqrt(xx**2 + yy**2)
                z(i,j) = exp(-r**2) * cos(2.0_plflt*PI*r)
            endif
        enddo
    enddo

    zmin = minval( z(1:xpts,:) )
    zmax = maxval( z(1:xpts,:) )

    step = (zmax-zmin)/(nlevel+1)
    clevel = zmin + step * arange(1_plint,nlevel+1)

    call plinit()
    call pllightsource(1._plflt, 1._plflt, 1._plflt)
    do k=1,2
        do ifshade = 0_plint, 3_plint
            call pladv(0_plint)
            call plvpor(0.0_plflt, 1.0_plflt, 0.0_plflt, 0.9_plflt )
            call plwind(-1.0_plflt, 1.0_plflt, -0.9_plflt, 1.1_plflt )
            call plcol0(3_plint)
            call plmtex('t', 1.0_plflt, 0.5_plflt, 0.5_plflt, title(k))
            call plcol0(1_plint)
            if (rosen ==1) then
                call plw3d(1.0_plflt, 1.0_plflt, 1.0_plflt, -1.5_plflt, &
                         1.5_plflt, -0.5_plflt, 1.5_plflt, zmin, zmax, alt(k),az(k))
            else
                call plw3d(1.0_plflt, 1.0_plflt, 1.0_plflt, -1.0_plflt, &
                         1.0_plflt, -1.0_plflt, 1.0_plflt, zmin, zmax, alt(k),az(k))
            endif
            call plbox3('bnstu','x axis', 0.0_plflt, 0_plint, &
                     'bnstu', 'y axis', 0.0_plflt, 0_plint, &
                     'bcdmnstuv','z axis', 0.0_plflt, 0_plint)
            call plcol0(2_plint)

            select case (ifshade)
                case( 0_plint )
                    ! diffuse light surface plot
                    call cmap1_init(1_plint)
                    call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                             0_plint, clevel(nlevel:1))

                case( 1_plint )
                    ! magnitude colored plot
                    call cmap1_init(0_plint)
                    call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                             MAG_COLOR, clevel(nlevel:1))

                case( 2_plint )
                    ! magnitude colored plot with faceted squares
                    call cmap1_init(0_plint)
                    call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                             ior(MAG_COLOR, FACETED), clevel(nlevel:1))

                case( 3_plint )
                    ! magnitude colored plot with contours
                    call cmap1_init(0_plint)
                    call plsurf3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                             ior(MAG_COLOR, ior(SURF_CONT, BASE_CONT)), clevel)
                case default
                    stop 'x08f: bad logic'
            end select
         enddo
    enddo
    call plend
contains

!----------------------------------------------------------------------------
    subroutine cmap1_init(gray)

!   For gray.eq.1, basic grayscale variation from half-dark
!   to light.  Otherwise, hue variations around the front of the
!   colour wheel from blue to green to red with constant lightness
!   and saturation.

    integer(kind=plint)          :: gray
    real(kind=plflt) :: i(0:1), h(0:1), l(0:1), s(0:1)

!   left boundary
    i(0_plint) = 0._plflt
!   right boundary
    i(1_plint) = 1._plflt
    if (gray == 1_plint) then
!       hue -- low: red (arbitrary if s=0)
        h(0_plint) = 0.0_plflt
!       hue -- high: red (arbitrary if s=0)
        h(1_plint) = 0.0_plflt
!       lightness -- low: half-dark
        l(0_plint) = 0.5_plflt
!       lightness -- high: light
        l(1_plint) = 1.0_plflt
!       minimum saturation
        s(0_plint) = 0.0_plflt
!       minimum saturation
        s(1_plint) = 0.0_plflt
    else
!       This combination of hues ranges from blue to cyan to green to yellow
!       to red (front of colour wheel) with constant lightness = 0.6
!       and saturation = 0.8.

!       hue -- low: blue
        h(0_plint) = 240._plflt
!       hue -- high: red
        h(1_plint) = 0.0_plflt
!       lightness -- low:
        l(0_plint) = 0.6_plflt
!       lightness -- high:
        l(1_plint) = 0.6_plflt
!       saturation
        s(0_plint) = 0.8_plflt
!       minimum saturation
        s(1_plint) = 0.8_plflt
    endif
    call plscmap1n(256_plint)
    call plscmap1l(.false., i, h, l, s)
end subroutine cmap1_init
end program x08f95
