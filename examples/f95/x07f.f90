!   Displays the plotter symbols for PLSYM
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

program x07f95
    use plplot
    implicit none

    character (len=4) :: text

    integer(kind=plint) :: i, j, k, l
    integer(kind=plint), dimension(20) :: base = &
        (/    0_plint,  100_plint,    0_plint,  100_plint,  200_plint,  &
        500_plint,  600_plint,  700_plint,  800_plint,  900_plint, &
        2000_plint, 2100_plint, 2200_plint, 2300_plint, 2400_plint, &
        2500_plint, 2600_plint, 2700_plint, 2800_plint, 2900_plint  /)

    real(kind=plflt), dimension(1:1) ::  x, y

!   Process command-line arguments

    call plparseopts(PL_PARSE_FULL)

!   Full sized page for display

    call plinit()
    call plfontld(0_plint)
    do l=1,20
        if (l==3) call plfontld(1_plint)

        call pladv(0_plint)

        call plcol0(2_plint)

        ! Set up viewport and window

        call plvpor(0.15_plflt, 0.95_plflt, 0.1_plflt, 0.9_plflt)
        call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)

        ! Draw the grid using plbox

        call plbox('bcg', 0.1_plflt, 0_plint,'bcg', 0.1_plflt, 0_plint)
        call plcol0(15_plint)

        ! Write the digits below the frame

        do i=0,9
            write (text,'(i1)') i
            call plmtex('b', 1.5_plflt, (0.1_plflt*i+0.05_plflt), &
                     0.5_plflt, text)
        enddo

        k=0
        do i=0,9

            ! Write the digits to the left of the frame

            write (text,'(i0)') base(l)+10*i

            call plmtex('lv', 1.0_plflt, (0.95_plflt-0.1_plflt*i), &
                     1.0_plflt, text)
            do j=0,9
                x=0.1_plflt*j+0.05_plflt
                y=0.95_plflt-0.1_plflt*i

               ! Display the symbols

               call plsym(x,y,base(l)+k)
               k=k+1
            enddo
        enddo

        if (l<=2) then
            call plmtex('t', 1.5_plflt, 0.5_plflt, 0.5_plflt, &
                     'PLplot Example 7 - PLSYM symbols (compact)')
        else
            call plmtex('t', 1.5_plflt, 0.5_plflt, 0.5_plflt, &
                      'PLplot Example 7 - PLSYM symbols (extended)')
        endif
    enddo

    call plend
end program x07f95
