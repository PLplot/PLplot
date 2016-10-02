!   Illustration of logarithmic axes, and redefinition of window.
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

program x04f95
    use plplot, PI => PL_PI
    use plf95demolib
    implicit none

    real(kind=plflt)  :: freql(0:100),ampl(0:100),phase(0:100)

!    Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

    call plinit
    call plfont(2)
!    Make log plots using two different styles.
    call plot1(0)
    call plot1(1)
    call plend
contains

subroutine plot1(type)
    integer           :: type
    integer           :: nlegend

    real(kind=plflt)  :: legend_width, legend_height
    integer           :: opt_array(2), text_colors(2), line_colors(2), &
                         line_styles(2), symbol_colors(2), symbol_numbers(2)
    real(kind=plflt)  :: line_widths(2), symbol_scales(2), box_scales(0)
    integer           :: box_colors(0), box_patterns(0)
    real(kind=plflt)  :: box_line_widths(0)
    character(len=20) :: text(2)
    character(len=20)  :: symbols(2)

    call pladv(0)
!    Set up data for log plot.
    call setdata

    call plvpor(0.15_plflt, 0.85_plflt, 0.1_plflt, 0.9_plflt)
    call plwind(-2.0_plflt, 3.0_plflt, -80.0_plflt, 0.0_plflt)
    call plcol0(1)
!    Try different axis and labelling styles.
    if (type.eq.0) then
      call plbox('bclnst', 0.0_plflt, 0, 'bnstv', 0.0_plflt, 0)
    elseif (type.eq.1) then
      call plbox('bcfghlnst', 0.0_plflt, 0, 'bcghnstv', 0.0_plflt, 0)
    else
      stop 'plot1: invalid type'
    endif
!    Plot ampl vs freq.
    call plcol0(2)
    call plline(freql,ampl)
    call plcol0(2)
    call plptex(1.6_plflt, -30.0_plflt, 1.0_plflt, -20.0_plflt, 0.5_plflt, &
                '-20 dB/decade')
!    Put labels on.
    call plcol0(1)
    call plmtex('b', 3.2_plflt, 0.5_plflt, 0.5_plflt, 'Frequency')
    call plmtex('t', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
                'Single Pole Low-Pass Filter')
    call plcol0(2)
    call plmtex('l', 5.0_plflt, 0.5_plflt, 0.5_plflt, 'Amplitude (dB)')
    nlegend = 1
!    For the gridless case, put phase vs freq on same plot.
    if(type == 0) then
      call plcol0(1)
      call plwind(-2.0_plflt, 3.0_plflt, -100.0_plflt, 0.0_plflt)
      call plbox(' ', 0.0_plflt, 0, 'cmstv', 30.0_plflt, 3)
      call plcol0(3)
      call plline(freql, phase)
      call plstring(freql, phase, "#(728)")
      call plcol0(3)
      call plmtex('r', 5.0_plflt, 0.5_plflt, 0.5_plflt, &
                  'Phase shift (degrees)')
      nlegend = 2
    endif

!   Draw a legend
!   First legend entry.
    opt_array(1)   = PL_LEGEND_LINE
    text_colors(1) = 2
    text(1)        = 'Amplitude'
    line_colors(1) = 2
    line_styles(1) = 1
    line_widths(1) = 1
!   note from the above opt_array the first symbol (and box) indices
!   do not have to be specified, at least in C. For Fortran we need
!   to set the symbols to be something, since the string is always
!   copied as part of the bindings.
    symbols(1) = ''

!   Second legend entry.
    opt_array(2)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
    text_colors(2)    = 3
    text(2)           = 'Phase shift'
    line_colors(2)    = 3
    line_styles(2)    = 1
    line_widths(2)    = 1
    symbol_colors(2)  = 3
    symbol_scales(2)  = 1.0
    symbol_numbers(2) = 4
    symbols(2)        = "#(728)"

!   from the above opt_arrays we can completely ignore everything
!   to do with boxes. (Hence the size 0 for the associated arrays)
!   (note: use the argument nlegend explicitly)

    call plscol0a( 15, 32, 32, 32, 0.70_plflt )
    call pllegend( legend_width, legend_height, &
        PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX, 0, &
        0.0_plflt, 0.0_plflt, 0.1_plflt, 15, &
        1, 1, 0, 0, &
        nlegend, opt_array, &
        1.0_plflt, 1.0_plflt, 2.0_plflt, &
        1.0_plflt, text_colors, text, &
        box_colors, box_patterns, box_scales, box_line_widths, &
        line_colors, line_styles, line_widths, &
        symbol_colors, symbol_scales, symbol_numbers, symbols )
end subroutine plot1

subroutine setdata
    real(kind=plflt), dimension(lbound(freql,1):ubound(freql,1)) :: freq
    real(kind=plflt) :: f0

    f0 = 1._plflt

    freql = -2.0_plflt + arange(lbound(freql,1),ubound(freql,1)+1) / 20.0_plflt
    freq  = 10.0_plflt ** freql
    ampl  = 20.0_plflt * log10(1.0_plflt/sqrt(1.0_plflt+(freq/f0)**2))
    phase = -(180.0_plflt/PI)*atan(freq/f0)
end subroutine setdata
end program x04f95
