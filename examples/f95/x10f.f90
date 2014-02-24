!   Demonstration program for PLplot illustrating absolute positioning
!   of graphs on a page
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

program x10f95
    use plplot
    implicit none

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

    call plinit()

    call pladv(0)
    call plvpor(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
    call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
    call plbox('bc', 0.0_plflt, 0, 'bc', 0.0_plflt, 0 )

    call plsvpa(50.0_plflt, 150.0_plflt, 50.0_plflt, 100.0_plflt )
    call plwind(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
    call plbox('bc', 0.0_plflt, 0, 'bc', 0.0_plflt, 0 )
    call plptex(0.5_plflt, 0.5_plflt, 1.0_plflt, 0.0_plflt, &
             0.5_plflt, 'BOX at (50,150,50,100)' )

    call plend

end program x10f95
