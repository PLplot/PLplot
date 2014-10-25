!   Mesh plot demo
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

program x11f95
    use plplot, PI => PL_PI
    use plf95demolib
    implicit none
    integer, parameter :: xpts=35, ypts=46
    integer            :: i, j, k, ifshade

    real(kind=plflt)   :: x(xpts), y(ypts), z(xpts,ypts), xx, yy

    character(len=80)  :: title(2) = &
               (/ '#frPLplot Example 11 - Alt=33, Az=24, Opt=3 ', &
                  '#frPLplot Example 11 - Alt=17, Az=115, Opt=3'  /)
    real(kind=plflt)   :: alt(2) = (/ 33.0_plflt,  17.0_plflt/)
    real(kind=plflt)   :: az(2)  = (/ 24.0_plflt, 115.0_plflt/)

    integer            :: opt(2) = (/ 3, 3 /)
    integer, parameter :: nlevel = 10
    real(kind=plflt)   :: zmin, zmax, step, clevel(nlevel)

!    Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

    x = 3._plflt * (arange(0,xpts) - (xpts/2)) / dble(xpts/2)
    y = 3._plflt * (arange(0,ypts) - (ypts/2)) / dble(ypts/2)

    do i=1,xpts
        xx = x(i)
        do j=1,ypts
            yy = y(j)
            z(i,j) = 3._plflt * (1._plflt-xx)*(1._plflt-xx) * &
                         exp(-(xx**2) - (yy+1._plflt)*(yy+1._plflt)) - &
                         10._plflt * (xx/5._plflt - xx**3 - yy**5) * exp(-xx**2-yy**2) - &
                         1._plflt/3._plflt * exp(-(xx+1._plflt)*(xx+1._plflt) - (yy**2))

        enddo
    enddo
    if (.false.) then
!       Jungfraujoch/Interlaken
        z = max(z, -1._plflt)
    endif

    zmin = minval(z)
    zmax = maxval(z)

    step = (zmax-zmin)/(nlevel+1)
    clevel = zmin + step * arange(1,nlevel+1)

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
            call plbox3('bnstu', 'x axis', 0.0_plflt, 0, &
                     'bnstu', 'y axis', 0.0_plflt, 0, &
                     'bcdmnstuv', 'z axis', 0.0_plflt, 0)
            call plcol0(2)

            select case (ifshade)
                case (0) ! wireframe plot
                    call plmesh(x(:xpts), y(:ypts), z(:xpts,:ypts), opt(k))

                case (1) ! magnitude colored wireframe plot
                    call plmesh(x(:xpts), y(:ypts), z(:xpts,:ypts), ior(opt(k), MAG_COLOR))

                case (2) ! magnitude colored wireframe plot with sides
                    call plot3d(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                             ior(opt(k), MAG_COLOR), .true.)

                case (3) ! magnitude colored wireframe plot with base contour
                    call plmeshc(x(:xpts), y(:ypts), z(:xpts,:ypts), &
                              ior(opt(k), ior(MAG_COLOR, BASE_CONT)), clevel)

                case default
                    stop 'x11f: bad logic'
            end select
            call plcol0(3)
            call plmtex('t', 1.0_plflt, 0.5_plflt, 0.5_plflt, title(k))
        enddo
    enddo
    call plend
contains

!----------------------------------------------------------------------------
subroutine cmap1_init(gray)
!    For gray.eq.1, basic grayscale variation from half-dark
!    to light.  Otherwise, hue variations around the front of the
!    colour wheel from blue to green to red with constant lightness
!    and saturation.

    integer gray
    real(kind=plflt) i(0:1), h(0:1), l(0:1), s(0:1)

!   left boundary
    i(0) = 0._plflt
!   right boundary
    i(1) = 1._plflt
    if (gray == 1) then
!       hue -- low: red (arbitrary if s=0)
        h(0) = 0.0_plflt
!       hue -- high: red (arbitrary if s=0)
        h(1) = 0.0_plflt
!       lightness -- low: half-dark
        l(0) = 0.5_plflt
!       lightness -- high: light
        l(1) = 1.0_plflt
!       minimum saturation
        s(0) = 0.0_plflt
!       minimum saturation
        s(1) = 0.0_plflt
    else
!       This combination of hues ranges from blue to cyan to green to yellow
!       to red (front of colour wheel) with constant lightness = 0.6
!       and saturation = 0.8.

!       hue -- low: blue
        h(0) = 240._plflt
!       hue -- high: red
        h(1) = 0.0_plflt
!       lightness -- low:
        l(0) = 0.6_plflt
!       lightness -- high:
        l(1) = 0.6_plflt
!       saturation
        s(0) = 0.8_plflt
!       minimum saturation
        s(1) = 0.8_plflt
    endif
    call plscmap1n(256)
    call plscmap1l(.false., i, h, l, s)
end subroutine cmap1_init

end program x11f95
