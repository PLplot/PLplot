!   Demonstrates multiple windows and color map 0
!
!   Copyright (C) 2004  Alan W. Irwin
!   Copyright (C) 2005  Andrew Ross
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

program x02f95
    use plplot
    implicit none

!    Process command-line arguments
    call plparseopts(PL_PARSE_FULL)


!   Initialize plplot
    call plinit()

!   Run demos
    call demo1
    call demo2

    call plend

contains

!--------------------------------------------------------------------------
!     demo1
!
!     Demonstrates multiple windows and default color map 0 palette.
!--------------------------------------------------------------------------
subroutine demo1

    call plbop

!   Divide screen into 16 regions
    call plssub(4,4)

    call draw_windows(16, 0)

    call pleop

end subroutine demo1


!--------------------------------------------------------------------------
!     demo2
!
!     Demonstrates multiple windows, user-modified color map 0 palette,
!     and HLS -> RGB translation.
!--------------------------------------------------------------------------
subroutine demo2
    integer, parameter :: tablesize = 116
    integer, dimension(tablesize) :: r, g, b
    integer :: i

    real(kind=plflt), parameter :: lmin = 0.15_plflt, lmax = 0.85_plflt
    real(kind=plflt) h, l, s, r1, g1, b1

    call plbop

!   Divide screen into 100 regions
    call plssub(10,10)

    do i=0,99
!   Bounds on HLS, from plhlsrgb() commentary --
!   hue		[0., 360.]	degrees
!   lightness	        [0., 1.]	magnitude
!   saturation	[0., 1.]	magnitude

!   Vary hue uniformly from left to right
        h = (360._plflt/10._plflt)*mod(i,10)
!   Vary lightness uniformly from top to bottom, between min and max
        l = lmin + (lmax - lmin) * (i / 10) / 9._plflt
!   Use_ max saturation
        s = 1._plflt

        call plhlsrgb(h, l, s, r1, g1, b1)

        r(i+17) = int(r1*255.001)
        g(i+17) = int(g1*255.001)
        b(i+17) = int(b1*255.001)
    enddo

    do i=1,16
        call plgcol0(i-1,r(i),g(i),b(i))
    enddo

    call plscmap0(r, g, b)

    call draw_windows(100, 16)

    call pleop

end subroutine demo2

!--------------------------------------------------------------------------
!     draw_windows
!
!     Draws a set of numbered boxes with colors according to cmap0 entry.
!--------------------------------------------------------------------------
subroutine draw_windows( nw, cmap0_offset )

    integer :: nw, cmap0_offset
    integer :: i,j
    real(kind=plflt) :: vmin, vmax, xj
    character (len=3) :: text

    call plschr(0.0_plflt, 3.5_plflt)
    call plfont(4)

    do i=0,nw-1
        call plcol0(i+cmap0_offset)
        write (text,'(i0)') i

        call pladv(0)
        vmin = 0.1_plflt
        vmax = 0.9_plflt
        do j=1,3
            xj = j
            call plwidth(xj)
            call plvpor(vmin,vmax,vmin,vmax)
            call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
            call plbox('bc', 0.0_plflt, 0, 'bc', 0.0_plflt, 0)
            vmin = vmin + 0.1_plflt
            vmax = vmax - 0.1_plflt
        enddo
        call plwidth(1._plflt)
        call plptex(0.5_plflt, 0.5_plflt, 1.0_plflt, 0.0_plflt, 0.5_plflt, text)
    enddo

end subroutine draw_windows
end program x02f95
