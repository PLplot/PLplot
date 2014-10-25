!   Displays the plotter symbols for PLPOIN
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

program x06f95
    use plplot
    implicit none
    integer :: i, j, k, font, kind_font, maxfont
    real(kind=plflt), dimension(1:1) :: x, y

    character (len=3) :: text

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

!   Full sized page for display
    call plinit()

    do kind_font = 1,2
        call plfontld(kind_font-1)
        maxfont = merge( 1, 4, kind_font==1 )

        do font=1,maxfont
            call plfont(font)

            call pladv(0)

            call plcol0(2)

!           Set up viewport and window

            call plvpor(0.1_plflt, 1.0_plflt, 0.1_plflt, 0.9_plflt)
            call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.3_plflt)

!           Draw the grid using plbox

            call plbox('bcg', 0.1_plflt, 0, 'bcg', 0.1_plflt, 0)
            call plcol0(15)

!           Write the digits below the frame

            do i=0,9
                write (text,'(i1)') i
                call plmtex('b', 1.5_plflt, (0.1_plflt*i+0.05_plflt), 0.5_plflt, text)
            enddo

            k=0
            do i=0,12

!               Write the digits to the left of the frame

                write (text,'(i0)') 10*i

                call plmtex('lv', 1.0_plflt, (1.0_plflt-(2*i+1)/26.0_plflt), &
                   1.0_plflt, text)

                do j=0,9
                    x=0.1_plflt*j+0.05_plflt
                    y=1.25_plflt-0.1_plflt*i

!                   Display the symbols
                    if (k < 128) call plpoin(x,y,k)

                    k=k+1

                enddo
            enddo

            if (kind_font==1) then
               call plmtex('t', 1.5_plflt, 0.5_plflt, 0.5_plflt, &
                  'PLplot Example 6 - plpoin symbols (compact)')
            else
               call plmtex('t', 1.5_plflt, 0.5_plflt, 0.5_plflt, &
                  'PLplot Example 6 - plpoin symbols (extended)')
            endif
        enddo
    enddo

    call plend
end program x06f95
