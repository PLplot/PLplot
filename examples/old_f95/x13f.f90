!   Pie chart demo.
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

program x13f95
    use plplot, PI => PL_PI
    implicit none
    real(kind=plflt)   :: just, dx, dy, x(500), y(500)
    integer            ::  i, j, dthet, theta0, theta1, theta
    character(len=20), dimension(5) :: text = &
         (/ 'Maurice ', 'Geoffrey', 'Alan    ', 'Rafael  ', 'Vince   '/)
    real(kind=plflt)   :: per(5) = &
         (/ 10._plflt , 32._plflt , 12._plflt , 30._plflt , 16._plflt /)

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

!   Ask user to specify the output device.
    call plinit()

    call pladv(0)
!   Ensure window has aspect ratio of one so circle is
!   plotted as a circle.
    call plvasp(1.0_plflt)
    call plwind(0._plflt, 10._plflt, 0._plflt, 10._plflt)
!   call plenv( 0._plflt, 10._plflt, 0._plflt, 10._plflt, 1, -2 )
    call plcol0(2)

!   n.b. all theta quantities scaled by 2*pi/500 to be integers to avoid
!   floating point logic problems.
    theta0 = 0
    dthet = 1

    do i = 0, 4
        j = 0
        x(j+1) = 5._plflt
        y(j+1) = 5._plflt
        j = j + 1
!       n.b. the theta quantities multiplied by 2*pi/500 afterward so
!       in fact per is interpreted as a percentage.
        theta1 = int(theta0 + 5*per(i+1))
        if (i .eq. 4) theta1 = 500
        do theta = theta0, theta1, dthet
            x(j+1) = 5 + 3*cos((2._plflt*pi/500._plflt)*theta)
            y(j+1) = 5 + 3*sin((2._plflt*pi/500._plflt)*theta)
            j = j + 1
        enddo

        call plcol0(i+1)
        call plpsty( mod(i+3, 8) + 1 )
        call plfill(x(:j), y(:j))
        call plcol0(1)
        call plline(x(:j), y(:j))

        just = (2._plflt*pi/500._plflt)*(theta0 + theta1) / 2._plflt
        dx = 0.25_plflt * cos(just)
        dy = 0.25_plflt * sin(just)
        if ((theta0  + theta1) .lt. 250 .or. &
            (theta0 + theta1) .gt. 750) then
            just = 0._plflt
        else
            just = 1._plflt
        endif
        call plptex( x(j/2+1)+dx, y(j/2+1)+dy, 1._plflt, 0._plflt, just, text(i+1) )
        theta0 = theta - dthet
    enddo

    call plfont(2)
    call plschr( 0._plflt, 1.3_plflt)
    call plptex( 5._plflt, 9._plflt, 1._plflt, 0._plflt, 0.5_plflt, 'Percentage of Sales' )

    call plend
end program x13f95
