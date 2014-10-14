!  Multi-lingual version of the first page of example 4.
!
!  Copyright (C) 2006 Alan Irwin
!  Copyright (C) 2006,2008 Andrew Ross
!
!  Thanks to the following for providing translated strings for this example:
!  Valery Pipin (Russian)
!
!  This file is part of PLplot.
!
!  PLplot is free software; you can redistribute it and/or modify
!  it under the terms of the GNU Library General Public License as published
!  by the Free Software Foundation; either version 2 of the License, or
!  (at your option) any later version.
!
!  PLplot is distributed in the hope that it will be useful,
!  but WITHOUT ANY WARRANTY; without even the implied warranty of
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!  GNU Library General Public License for more details.
!
!  You should have received a copy of the GNU Library General Public License
!  along with PLplot; if not, write to the Free Software
!  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!

!
! This example designed just for devices (e.g., psttfc and the
! cairo-related devices) that utilise the pango and fontconfig libraries. The
! best choice of glyph is selected by fontconfig and automatically rendered
! by pango in way that is sensitive to complex text layout (CTL) language
! issues for each unicode character in this example. Of course, you must
! have the appropriate TrueType fonts installed to have access to all the
! required glyphs.
!
! Translation instructions: The strings to be translated are given by
! x_label, y_label, alty_label, title_label, and line_label below.  The
! encoding used must be UTF-8.
!
! The following strings to be translated involve some scientific/mathematical
! jargon which is now discussed further to help translators.
!
! (1) dB is a decibel unit, see http://en.wikipedia.org/wiki/Decibel .
! (2) degrees is an angular measure, see
!     http://en.wikipedia.org/wiki/Degree_(angle) .
! (3) low-pass filter is one that transmits (passes) low frequencies.
! (4) pole is in the mathematical sense, see
!     http://en.wikipedia.org/wiki/Pole_(complex_analysis) .  "Single Pole"
!     means a particular mathematical transformation of the filter function has
!     a single pole, see
!     http://ccrma.stanford.edu/~jos/filters/Pole_Zero_Analysis_I.html .
!     Furthermore, a single-pole filter must have an inverse square decline
!     (or -20 db/decade). Since the filter plotted here does have that
!     characteristic, it must by definition be a single-pole filter, see also
!     http://www-k.ext.ti.com/SRVS/Data/ti/KnowledgeBases/analog/document/faqs/1p.htm
! (5) decade represents a factor of 10, see
!     http://en.wikipedia.org/wiki/Decade_(log_scale) .
!
!
!

! --------------------------------------------------------------------------
!  main
!
!  Illustration of logarithmic axes, and redefinition of window.
! --------------------------------------------------------------------------

program x26f

  use plplot, PI => PL_PI
  implicit none

  integer(kind=plint), parameter :: nolangs = 2_plint
  integer(kind=plint), parameter :: nlegend = 2_plint

  character(len=80) x_label(nolangs)
  character(len=80) y_label(nolangs)
  character(len=80) legend_text(2_plint,nolangs)
  character(len=80) alty_label(nolangs)
  character(len=80) title_label(nolangs)
  character(len=80) line_label(nolangs)

  integer(kind=plint)      i, j

  data (x_label(i) ,i=1,nolangs) / &
       'Frequency', &
       'Частота'/

  data (y_label(i) ,i=1,nolangs) / &
       'Amplitude (dB)', &
       'Амплитуда (dB)'/

  data (alty_label(i) ,i=1,nolangs) / &
       'Phase shift (degrees)', &
       'Фазовый сдвиг (градусы)'/

  ! Short rearranged versions of y_label and alty_label.
  data ((legend_text(j,i), j=1, nlegend), i=1,nolangs) / &
      'Amplitude',          'Phase shift'              , &
      'Амплитуда', 'Фазовый сдвиг' /

  data (title_label(i) ,i=1,nolangs) / &
      'Single Pole Low-Pass Filter', &
      'Однополюсный Низко-Частотный Фильтр'/

  data (line_label(i) ,i=1,nolangs) / &
       '-20 dB/decade', &
       '-20 dB/десяток'/

  !  Parse and process command line arguments

  call plparseopts(PL_PARSE_FULL)

  !  Initialize plplot

  call plinit()
  call plfont(2_plint)

  !  Make log plots using two different styles.

  do i = 1_plint,nolangs
     call plot1(nlegend, 0_plint, x_label(i), y_label(i), alty_label(i), &
          legend_text(:,i), title_label(i), line_label(i))
  end do
  call plend()

end program x26f

  ! --------------------------------------------------------------------------
  !  plot1
  !
  !  Log-linear plot.
  ! --------------------------------------------------------------------------

  subroutine plot1( nlegend, type, x_label, y_label, alty_label, &
       legend_text, title_label, line_label )

    use plplot, PI => PL_PI
    implicit none

    integer(kind=plint) type, nlegend
    character(*) x_label, y_label, alty_label, title_label, line_label

    integer(kind=plint) i
    real(kind=plflt)  freql(101_plint), ampl(101_plint), phase(101_plint)
    real(kind=plflt)  f0, freq

    character(*) legend_text(nlegend)

    integer(kind=plint)          opt_array(nlegend)
    integer(kind=plint)          text_colors(nlegend)
    integer(kind=plint)          line_colors(nlegend)
    integer(kind=plint)          line_styles(nlegend)
    real(kind=plflt) line_widths(nlegend)
    integer(kind=plint)          symbol_numbers(nlegend)
    integer(kind=plint)          symbol_colors(nlegend)
    real(kind=plflt) symbol_scales(nlegend)
    character(len=20) symbols(nlegend)
    integer(kind=plint)          box_colors(0_plint), box_patterns(0_plint)
    real(kind=plflt) box_scales(0_plint), box_line_widths(0_plint)
    real(kind=plflt) legend_width, legend_height

    call pladv(0_plint)

    !  Set up data for log plot

    f0 = 1.0
    do i = 1_plint,101
       freql(i) = -2.0_plflt + (i-1) / 20.0_plflt
       freq     = 10.0_plflt ** freql(i)
       ampl(i)  = 20.0_plflt * log10(1.0_plflt / sqrt(1.0_plflt + (freq/f0)**2))
       phase(i) = -(180.0_plflt / PI) * atan(freq / f0)
    end do
    call plvpor(0.15_plflt, 0.85_plflt, 0.1_plflt, 0.9_plflt)
    call plwind(-2.0_plflt, 3.0_plflt, -80.0_plflt, 0.0_plflt)

    !  Try different axis and labelling styles.

    call plcol0(1_plint)
    if ( type .eq. 0_plint ) then
       call plbox("bclnst", 0.0_plflt, 0_plint, "bnstv", 0.0_plflt, 0_plint)
    endif
    if ( type .eq. 1_plint ) then
       call plbox("bcfghlnst", 0.0_plflt, 0_plint, "bcghnstv", 0.0_plflt, 0_plint)
    endif

    !  Plot ampl vs freq

    call plcol0(2_plint)
    call plline(freql, ampl)
    call plcol0(2_plint)
    call plptex(1.6_plflt, -30.0_plflt, 1.0_plflt, -20.0_plflt, 0.5_plflt, line_label)

    !  Put labels on

    call plcol0(1_plint)
    call plmtex("b", 3.2_plflt, 0.5_plflt, 0.5_plflt, x_label)
    call plmtex("t", 2.0_plflt, 0.5_plflt, 0.5_plflt, title_label)
    call plcol0(2_plint)
    call plmtex("l", 5.0_plflt, 0.5_plflt, 0.5_plflt, y_label)

    !  For the gridless case, put phase vs freq on same plot

    if (type .eq. 0_plint) then
       call plcol0(1_plint)
       call plwind(-2.0_plflt, 3.0_plflt, -100.0_plflt, 0.0_plflt)
       call plbox("", 0.0_plflt, 0_plint, "cmstv", 30.0_plflt, 3_plint)
       call plcol0(3_plint)
       call plline(freql, phase)
       call plstring(freql, phase, "#(728)")
       call plcol0(3_plint)
       call plmtex("r", 5.0_plflt, 0.5_plflt, 0.5_plflt, alty_label)
    endif

    !  Draw a legend
    !     First legend entry.

    opt_array(1_plint)   = PL_LEGEND_LINE
    text_colors(1_plint) = 2_plint
    line_colors(1_plint) = 2_plint
    line_styles(1_plint) = 1_plint
    line_widths(1_plint) = 1_plint
    !     defining this makes the Fortran to C transformation of
    !     symbols(1_plint) work on defined values which makes valgrind
    !     happier (even if that transformed string is not used inside the
    !     C version of pllegend)
     symbols(1_plint) = ""

    !     note from the above opt_array the first symbol (and box) indices
    !     do not have to be specified

    !     Second legend entry.

    opt_array(2_plint)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
    text_colors(2_plint)    = 3_plint
    line_colors(2_plint)    = 3_plint
    line_styles(2_plint)    = 1_plint
    line_widths(2_plint)    = 1_plint
    symbol_colors(2_plint)  = 3_plint
    symbol_scales(2_plint)  = 1.
    symbol_numbers(2_plint) = 4_plint
    symbols(2_plint)        = "#(728)"

    !     from the above opt_arrays we can completely ignore everything
    !     to do with boxes.
    !     (note: number of legend entries determined from size of
    !     opt_array)

    call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70d0 )
    call pllegend( legend_width, legend_height, &
        PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX, 0_plint, &
        0.0_plflt, 0.0_plflt, 0.10_plflt, 15_plint, &
        1_plint, 1_plint, 0_plint, 0_plint, &
        opt_array, &
        1.0_plflt, 1.0_plflt, 2.0_plflt, &
        1.0_plflt, text_colors, legend_text, &
        box_colors, box_patterns, box_scales, box_line_widths, &
        line_colors, line_styles, line_widths, &
        symbol_colors, symbol_scales, symbol_numbers, symbols )

  end subroutine plot1
