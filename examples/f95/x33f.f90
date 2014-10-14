!     Demonstrate most pllegend capability including unicode symbols.
!
!     Copyright (C) 2010 Alan Irwin
!
!     This file is part of PLplot.
!
!     PLplot is free software; you can redistribute it and/or modify
!     it under the terms of the GNU Library General Public License as published
!     by the Free Software Foundation; either version 2 of the License, or
!     (at your option) any later version.
!
!     PLplot is distributed in the hope that it will be useful,
!     but WITHOUT ANY WARRANTY; without even the implied warranty of
!     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!     GNU Library General Public License for more details.
!
!     You should have received a copy of the GNU Library General Public License
!     along with PLplot; if not, write to the Free Software
!     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!
!     This example designed just for devices (e.g., the cairo-related and
!     qt-related devices) where the best choice of glyph is automatically
!     selected by the related libraries (pango/cairo or Qt4) for each
!     unicode character depending on what system fonts are installed.  Of
!     course, you must have the appropriate TrueType fonts installed to
!     have access to all the required glyphs.


!--------------------------------------------------------------------------
!  main
!
!  Demonstrate most pllegend capability including unicode symbols.
! --------------------------------------------------------------------------

      use plplot

      implicit none

      integer(kind=plint), parameter :: MAX_NLEGEND = 7_plint

      integer(kind=plint)            :: i, k
      integer(kind=plint)            :: opt, position
      integer(kind=plint)            :: nlegend, nturn
      integer(kind=plint)            :: opt_array(MAX_NLEGEND)
      integer(kind=plint)            :: text_colors(MAX_NLEGEND)
      integer(kind=plint)            :: box_colors(MAX_NLEGEND)
      integer(kind=plint)            :: box_patterns(MAX_NLEGEND)
      real(kind=plflt)   :: box_scales(MAX_NLEGEND)
      real(kind=plflt)   :: box_line_widths(MAX_NLEGEND)
      integer(kind=plint)            :: line_colors(MAX_NLEGEND)
      integer(kind=plint)            :: line_styles(MAX_NLEGEND)
      real(kind=plflt)   :: line_widths(MAX_NLEGEND)
      integer(kind=plint)            :: symbol_numbers(MAX_NLEGEND), symbol_colors(MAX_NLEGEND)
      real(kind=plflt)   :: symbol_scales(MAX_NLEGEND)
      character(len=200) :: text(MAX_NLEGEND)
      character(len=20)  :: symbols(MAX_NLEGEND)
      real(kind=plflt)   :: legend_width, legend_height, x, y, xstart, ystart
      real(kind=plflt)   :: max_height, text_scale
      integer(kind=plint)            :: opt_base, nrow, ncolumn

      integer(kind=plint)            :: position_options(16_plint)
      character(len=3)   :: special_symbols(5_plint)

      real(kind=plflt)   :: values_small(2_plint)
      real(kind=plflt)   :: values_uneven(9_plint)
      real(kind=plflt)   :: values_even(9_plint)

      integer(kind=plint), parameter :: COLORBAR_KINDS = 4_plint
      integer(kind=plint)            :: colorbar_option_kinds(COLORBAR_KINDS)
      character(len=100) :: colorbar_option_kind_labels(COLORBAR_KINDS)

      integer(kind=plint), parameter :: COLORBAR_POSITIONS = 4_plint
      integer(kind=plint)            :: colorbar_position_options(COLORBAR_POSITIONS)
      character(len=100) :: colorbar_position_option_labels(COLORBAR_POSITIONS)

      integer(kind=plint), parameter :: COLORBAR_LABELS = 4_plint
      integer(kind=plint)            :: colorbar_label_options(COLORBAR_LABELS)
      character(len=100) :: colorbar_label_option_labels(COLORBAR_LABELS)

      integer(kind=plint), parameter :: COLORBAR_CAPS = 4_plint
      integer(kind=plint)            :: colorbar_cap_options(COLORBAR_CAPS)
      character(len=100) :: colorbar_cap_option_labels(COLORBAR_CAPS)

!     Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
      data special_symbols / &
          '✰',             &
          '✴',             &
          '✱',             &
          '✽',             &
          '✦'              /

      data values_small   / -1.0e-200_plflt, 1.0e-200_plflt /
      data values_uneven &
      / -1.0e-200_plflt, 2.0e-200_plflt, 2.6e-200_plflt, 3.4e-200_plflt, &
      6.0e-200_plflt, 7.0e-200_plflt, 8.0e-200_plflt, 9.0e-200_plflt, &
      10.0e-200_plflt /
      data values_even &
      / -2.0e-200_plflt, -1.0e-200_plflt, 0.0e-200_plflt, 1.0e-200_plflt, &
      2.0e-200_plflt, 3.0e-200_plflt, 4.0e-200_plflt, 5.0e-200_plflt, &
      6.0e-200_plflt /

      position_options(1_plint) = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(2_plint) = PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(3_plint) = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(4_plint) = PL_POSITION_RIGHT + PL_POSITION_OUTSIDE
      position_options(5_plint) = PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(6_plint) = PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(7_plint) = PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(8_plint) = PL_POSITION_LEFT + PL_POSITION_OUTSIDE
      position_options(9_plint) = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(10_plint) = PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(11_plint) = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(12_plint) = PL_POSITION_RIGHT + PL_POSITION_INSIDE
      position_options(13_plint) = PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(14_plint) = PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(15_plint) = PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(16_plint) = PL_POSITION_LEFT + PL_POSITION_INSIDE

      ! plcolorbar options

      ! Colorbar type options
      colorbar_option_kinds(1_plint) = PL_COLORBAR_SHADE
      colorbar_option_kinds(2_plint) = PL_COLORBAR_SHADE + PL_COLORBAR_SHADE_LABEL
      colorbar_option_kinds(3_plint) = PL_COLORBAR_IMAGE
      colorbar_option_kinds(4_plint) = PL_COLORBAR_GRADIENT

      colorbar_option_kind_labels(1_plint) = "Shade colorbars"
      colorbar_option_kind_labels(2_plint) = "Shade colorbars with custom labels"
      colorbar_option_kind_labels(3_plint) = "Image colorbars"
      colorbar_option_kind_labels(4_plint) = "Gradient colorbars"

      ! Which side of the page are we positioned relative to?
      colorbar_position_options(1_plint) = PL_POSITION_LEFT
      colorbar_position_options(2_plint) = PL_POSITION_RIGHT
      colorbar_position_options(3_plint) = PL_POSITION_TOP
      colorbar_position_options(4_plint) = PL_POSITION_BOTTOM

      colorbar_position_option_labels(1_plint) = "Left"
      colorbar_position_option_labels(2_plint) = "Right"
      colorbar_position_option_labels(3_plint) = "Top"
      colorbar_position_option_labels(4_plint) = "Bottom"

      ! Colorbar label positioning options
      colorbar_label_options(1_plint) = PL_COLORBAR_LABEL_LEFT
      colorbar_label_options(2_plint) = PL_COLORBAR_LABEL_RIGHT
      colorbar_label_options(3_plint) = PL_COLORBAR_LABEL_TOP
      colorbar_label_options(4_plint) = PL_COLORBAR_LABEL_BOTTOM

      colorbar_label_option_labels(1_plint) = "Label left"
      colorbar_label_option_labels(2_plint) = "Label right"
      colorbar_label_option_labels(3_plint) = "Label top"
      colorbar_label_option_labels(4_plint) = "Label bottom"

      ! Colorbar cap options
      colorbar_cap_options(1_plint) = PL_COLORBAR_CAP_NONE
      colorbar_cap_options(2_plint) = PL_COLORBAR_CAP_LOW
      colorbar_cap_options(3_plint) = PL_COLORBAR_CAP_HIGH
      colorbar_cap_options(4_plint) = PL_COLORBAR_CAP_LOW + PL_COLORBAR_CAP_HIGH

      colorbar_cap_option_labels(1_plint) = "No caps"
      colorbar_cap_option_labels(2_plint) = "Low cap"
      colorbar_cap_option_labels(3_plint) = "High cap"
      colorbar_cap_option_labels(4_plint) = "Low and high caps"

!     Parse and process command line arguments

      call plparseopts(PL_PARSE_FULL)

!     Initialize plplot
      call plinit

!     First page illustrating the 16 standard positions.
      call pladv( 0_plint )
      call plvpor( 0.25_plflt, 0.75_plflt, 0.25_plflt, 0.75_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plbox( 'bc', 0.0_plflt, 0_plint, 'bc', 0.0_plflt, 0_plint )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 8.0_plflt, 0.5_plflt, 0.5_plflt, &
               'The 16 standard legend positions with' )
      call plmtex( 't', 6.0_plflt, 0.5_plflt, 0.5_plflt, &
               'the same (0.05) offset in x and y' )

      nlegend = 1_plint
!     Only specify legend data that are required according to the
!     value of opt_array for that entry.
      opt_base          = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      opt_array(1_plint)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
      line_styles(1_plint)    = 1_plint
      line_widths(1_plint)    = 1_plint
      symbol_scales(1_plint)  = 1._plflt
      symbol_numbers(1_plint) = 4_plint
      symbols(1_plint)        = "#(728)"

!     Use monotype fonts so that all legends are the same size.
      call plsfont( PL_FCI_MONO, -1, -1 )
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      do k = 1_plint,16
          position = position_options(k)
          opt = opt_base
          text_colors(1_plint)   = 1_plint + mod( k-1, 8_plint )
          line_colors(1_plint)   = 1_plint + mod( k-1, 8_plint )
          symbol_colors(1_plint) = 1_plint + mod( k-1, 8_plint )
          write( text(1_plint), '(i2.2)' ) k-1

          call pllegend( legend_width, legend_height, opt, position, &
              0.05_plflt, 0.05_plflt,                                &
              0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                             &
              opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt, &
              1._plflt, text_colors, text,                           &
              box_colors, box_patterns, box_scales, box_line_widths, &
              line_colors, line_styles, line_widths,                 &
              symbol_colors, symbol_scales, symbol_numbers, symbols  )
      enddo

!     Second page illustrating effect of nrow, ncolumn for the same legend
!     data.

      call pladv( 0_plint )
      call plvpor( 0.25_plflt, 0.75_plflt, 0.25_plflt, 0.75_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plbox( 'bc', 0.0_plflt, 0_plint, 'bc', 0.0_plflt, 0_plint )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 8.0_plflt, 0.5_plflt, 0.5_plflt, &
               'The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,' )
      call plmtex( 't', 6.0_plflt, 0.5_plflt, 0.5_plflt, &
               'and position for the same legend data' )

      nlegend = 7_plint

!     Only specify legend data that are required according to the
!     value of opt_array for that entry.

      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      do k = 1_plint,nlegend
          opt_array(k)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
          line_styles(k)    = 1_plint
          line_widths(k)    = 1_plint
          symbol_scales(k)  = 1._plflt
          symbol_numbers(k) = 2_plint
          symbols(k)        = "#(728)"
          write( text(k), '(i2.2)' ) k-1
          text_colors(k)   = 1_plint + mod( k-1, 8_plint )
          line_colors(k)   = 1_plint + mod( k-1, 8_plint )
          symbol_colors(k) = 1_plint + mod( k-1, 8_plint )
      enddo

!     Use monotype fonts so that all legends are the same size.

      call plsfont( PL_FCI_MONO, -1, -1 )
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      position = PL_POSITION_TOP + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0.1_plflt
      nrow    = 1_plint
      ncolumn = nlegend
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0.1_plflt
      nrow    = 1_plint
      ncolumn = nlegend
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_LEFT + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0.1_plflt
      y       = 0._plflt
      nrow    = nlegend
      ncolumn = 1_plint
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_RIGHT + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0.1_plflt
      y       = 0._plflt
      nrow    = nlegend
      ncolumn = 1_plint
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 6_plint
      ncolumn = 2_plint
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE
      opt     = opt_base + PL_LEGEND_ROW_MAJOR
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 6_plint
      ncolumn = 2_plint
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      opt     = opt_base + PL_LEGEND_ROW_MAJOR
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 3_plint
      ncolumn = 3_plint
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

!     Third page demonstrating legend alignment

      call pladv( 0_plint )
      call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 0.9_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
               'Demonstrate legend alignment' )

      x        = 0.1_plflt
      y        = 0.1_plflt
      nturn    = 5_plint
      nlegend  = 0_plint
      position = PL_POSITION_TOP + PL_POSITION_LEFT + PL_POSITION_SUBPAGE
      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      opt      = opt_base
      do i = 1_plint,9

!         Set up legend arrays with the correct size, type.

          if ( i .le. nturn ) then
              nlegend = nlegend + 1_plint
          else
              nlegend = nlegend - 1_plint
          endif
          nlegend = max( 1_plint, nlegend )

!         nly specify legend data that are required according to the
!         value of opt_array for that entry.

          do k = 1_plint,nlegend
              opt_array(k)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
              line_styles(k)    = 1_plint
              line_widths(k)    = 1_plint
              symbol_scales(k)  = 1._plflt
              symbol_numbers(k) = 2_plint
              symbols(k)        = "#(728)"
              write( text(k), '(i2.2)' ) k-1
              text_colors(k)   = 1_plint + mod( k-1, 8_plint )
              line_colors(k)   = 1_plint + mod( k-1, 8_plint )
              symbol_colors(k) = 1_plint + mod( k-1, 8_plint )
          enddo

!         Use monotype fonts so that all legends are the same size.

          call plsfont( PL_FCI_MONO, -1, -1 )
          call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

          nrow    = MIN( 3_plint, nlegend )
          ncolumn = 0_plint

          call pllegend( legend_width, legend_height, opt, position, &
              x, y,                                                  &
              0.025_plflt, 15_plint, 1_plint, 1_plint, nrow, ncolumn,                  &
              opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 1.5_plflt, &
              1._plflt, text_colors, text,                           &
              box_colors, box_patterns, box_scales, box_line_widths, &
              line_colors, line_styles, line_widths,                 &
              symbol_colors, symbol_scales, symbol_numbers, symbols  )

          if ( i .eq. nturn ) then
              position = PL_POSITION_TOP + PL_POSITION_RIGHT + PL_POSITION_SUBPAGE
              opt = opt_base
              x   = 1._plflt - x
              y   = y + legend_height
          else
              x = x + legend_width
              y = y + legend_height
          endif
      enddo

!     Fourth page illustrating various kinds of legends

      max_height = 0._plflt
      xstart     = 0.0_plflt
      ystart     = 0.1_plflt
      x          = xstart
      y          = ystart
      text_scale = 0.90_plflt
      call pladv( 0_plint )
      call plvpor( 0.0_plflt, 1._plflt, 0.0_plflt, 0.90_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!       call plbox('bc', 0.0, 0, 'bc', 0.0_plflt, 0)
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
               'Demonstrate Various Kinds of Legends' )

      nlegend = 5_plint

!     Only specify legend data that are required according to the
!     value of opt_array for that entry.

      position = PL_POSITION_LEFT + PL_POSITION_TOP
      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX + PL_LEGEND_TEXT_LEFT

!     Set up None, Box, Line, Symbol, and Line & Symbol legend entries.

      opt_array(1_plint) = PL_LEGEND_NONE
      text(1_plint) = 'None'
      text_colors(1_plint) = 1_plint

      opt_array(2_plint) = PL_LEGEND_COLOR_BOX
      text(2_plint) = 'Box'
      text_colors(2_plint)     = 2_plint
      box_colors(2_plint)      = 2_plint
      box_patterns(2_plint)    = 0_plint
      box_scales(2_plint)      = 0.8_plflt
      box_line_widths(2_plint) = 1_plint

      opt_array(3_plint) = PL_LEGEND_LINE
      text(3_plint) = 'Line'
      text_colors(3_plint) = 3_plint
      line_colors(3_plint) = 3_plint
      line_styles(3_plint) = 1_plint
      line_widths(3_plint) = 1_plint

      opt_array(4_plint) = PL_LEGEND_SYMBOL
      text(4_plint) = 'Symbol'
      text_colors(4_plint)    = 4_plint
      symbol_colors(4_plint)  = 4_plint
      symbol_scales(4_plint)  = text_scale
      symbol_numbers(4_plint) = 4_plint
      symbols(4_plint)        = special_symbols(3_plint)

      opt_array(5_plint) = PL_LEGEND_SYMBOL + PL_LEGEND_LINE
      text(5_plint) = 'L & S'
      text_colors(5_plint)    = 5_plint
      line_colors(5_plint)    = 5_plint
      line_styles(5_plint)    = 1_plint
      line_widths(5_plint)    = 1_plint
      symbol_colors(5_plint)  = 5_plint
      symbol_scales(5_plint)  = text_scale
      symbol_numbers(5_plint) = 4_plint
      symbols(5_plint)        = special_symbols(3_plint)

      opt = opt_base
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up symbol legend entries with various symbols.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_SYMBOL
          text(i) = 'Symbol ' // special_symbols(i)
          text_colors(i)    = i
          symbol_colors(i)  = i
          symbol_scales(i)  = text_scale
          symbol_numbers(i) = 4_plint
          symbols(i)        = special_symbols(i)
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up symbol legend entries with various numbers of symbols.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_SYMBOL
          write( text(i), '(a,i0)' ) 'Symbol Number ', i+1
          text_colors(i)    = i
          symbol_colors(i)  = i
          symbol_scales(i)  = text_scale
          symbol_numbers(i) = i + 1_plint
          symbols(i)        = special_symbols(3_plint)
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various colours.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Color ', i
          text_colors(i)     = i
          box_colors(i)      = i
          box_patterns(i)    = 0_plint
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = 1_plint
      enddo

      opt = opt_base

!     Use new origin

      x          = xstart
      y          = y + max_height
      max_height = 0._plflt
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt)

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various patterns.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Pattern ', i-1
          text_colors(i)     = 2_plint
          box_colors(i)      = 2_plint
          box_patterns(i)    = i - 1_plint
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = 1_plint
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various box pattern line widths.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Line Width ', i
          text_colors(i)     = 2_plint
          box_colors(i)      = 2_plint
          box_patterns(i)    = 3_plint
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = i
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up line legend entries with various colours.

      do i =1,nlegend
          opt_array(i) = PL_LEGEND_LINE
          write( text(i), '(a,i0)' ) 'Line Color ', i
          text_colors(i) = i
          line_colors(i) = i
          line_styles(i) = 1_plint
          line_widths(i) = 1_plint
      enddo

      opt = opt_base

!     Use new origin

      x          = xstart
      y          = y + max_height
      max_height = 0._plflt
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up line legend entries with various styles.

      do i = 1_plint,nlegend
          opt_array(i) = PL_LEGEND_LINE
          write( text(i), '(a,i0)' ) 'Line Style ', i
          text_colors(i) = 2_plint
          line_colors(i) = 2_plint
          line_styles(i) = i
          line_widths(i) = 1_plint
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up line legend entries with various widths.

      do i =1,nlegend
          opt_array(i) = PL_LEGEND_LINE
          write( text(i), '(a,i0)' ) 'Line Width ', i
          text_colors(i) = 2_plint
          line_colors(i) = 2_plint
          line_styles(i) = 1_plint
          line_widths(i) = i
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15_plint, 32_plint, 32_plint, 32_plint, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15_plint, 1_plint, 1_plint, 0_plint, 0_plint,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Color bar examples

      ! Use unsaturated green background colour to contrast with black caps.
      call plscolbg( 70_plint, 185_plint, 70_plint )
      ! Cut out the greatest and smallest bits of the color spectrum to
      ! leave colors for the end caps.
      call plscmap1_range( 0.01_plflt, 0.99_plflt )

      ! We can only test image and gradient colorbars with two element arrays
      do i = 2_plint,COLORBAR_KINDS-1
         call plcolorbar_example( "cmap1_blue_yellow.pal", i, 0_plint, 0._plflt, 2_plint, values_small )
      enddo
      ! Test shade colorbars with larger arrays
      do i = 0_plint,1
         call plcolorbar_example( "cmap1_blue_yellow.pal", i, 4_plint, 2._plflt, 9_plint, values_even )
      enddo
      do i = 0_plint,1
         call plcolorbar_example( "cmap1_blue_yellow.pal", i, 0_plint, 0._plflt, 9_plint, values_uneven )
      enddo

      call plend()

     contains

       subroutine plcolorbar_example_page( kind_i, label_i, cap_i, cont_color, cont_width, n_values, values )

         use plplot
         
         implicit none

         integer(kind=plint)  :: kind_i, label_i, cap_i, cont_color, n_values
         real(kind=plflt)   :: cont_width
         real(kind=plflt), dimension(:)   :: values
         
         ! Parameters for the colorbars on this page
         integer(kind=plint)            :: position_i, position, opt
         real(kind=plflt)   :: x, y, x_length, y_length;
         real(kind=plflt)   :: ticks(1_plint)
         integer(kind=plint)            :: sub_ticks(1_plint)
         real(kind=plflt)   :: low_cap_color, high_cap_color
         logical            :: vertical, ifn
         character(len=20)  :: axis_opts(1_plint)
         integer(kind=plint)            :: label_opts(1_plint)
         character(len=200) :: labels(1_plint)
         character(len=200) :: title
         real(kind=plflt)   :: colorbar_width, colorbar_height
         integer(kind=plint)            :: n_values_array(1_plint);
         real(kind=plflt), allocatable, dimension(:,:) :: values_array

         ticks(1_plint) = 0.0_plflt
         sub_ticks(1_plint) = 0_plint
         label_opts(1_plint) = 0_plint

         n_values_array(1_plint) = n_values
         allocate(values_array(1_plint,n_values))
         values_array(1_plint,:) = values(:)

         low_cap_color  = 0.0_plflt;
         high_cap_color = 1.0_plflt;

         ! Start a new page
         call pladv( 0_plint )

         ! Draw one colorbar relative to each side of the page
         do position_i = 0_plint,COLORBAR_POSITIONS-1
            position = colorbar_position_options(position_i+1);
            opt      = ior( &
                 colorbar_option_kinds(kind_i+1), &
                 ior(colorbar_label_options(label_i+1), &
                 colorbar_cap_options(cap_i+1) ) )

            vertical = (iand(position, PL_POSITION_LEFT) > 0_plint .or. iand(position, PL_POSITION_RIGHT) > 0_plint )
            ifn      = (iand(position, PL_POSITION_LEFT) > 0_plint .or. iand(position, PL_POSITION_BOTTOM) > 0_plint )
            
            ! Set the offset position on the page
            if (vertical .eqv. .true.) then
               x        = 0.0_plflt
               y        = 0.0_plflt
               x_length = 0.05_plflt
               y_length = 0.5_plflt
            else
               x        = 0.0_plflt
               y        = 0.0_plflt
               x_length = 0.5_plflt
               y_length = 0.05_plflt
            endif

            ! Set appropriate labelling options.
            if (ifn .eqv. .true.) then
               if ( cont_color .eq. 0_plint .or. cont_width .eq. 0._plflt ) then
                  axis_opts(1_plint) = "uwtivn"
               else
                  axis_opts(1_plint) = "uwxvn"
               endif
            else
               if ( cont_color .eq. 0_plint .or. cont_width .eq. 0._plflt ) then
                  axis_opts(1_plint) = "uwtivm"
               else
                  axis_opts(1_plint) = "uwxvm"
               endif
            endif
            
            write(labels(1_plint), '(3A)') trim(colorbar_position_option_labels(position_i+1)), &
                 ', ', trim(colorbar_label_option_labels(label_i+1))

            ! Smaller text
            call plschr( 0.0_plflt, 0.75_plflt )
            ! Small ticks on the vertical axis
            call plsmaj( 0.0_plflt, 0.5_plflt )
            call plsmin( 0.0_plflt, 0.5_plflt )

            call plvpor( 0.20_plflt, 0.80_plflt, 0.20_plflt, 0.80_plflt )
            call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
            ! Set interesting background colour.
            call plscol0a( 15_plint, 0_plint, 0_plint, 0_plint, 0.20_plflt )
            call plcolorbar( colorbar_width, colorbar_height, &
                 ior(opt, ior(PL_COLORBAR_BOUNDING_BOX, PL_COLORBAR_BACKGROUND)), position, &
                 x, y, x_length, y_length, &
                 15_plint, 1_plint, 1_plint, &
                 low_cap_color, high_cap_color, &
                 cont_color, cont_width, &
                 label_opts, labels, &
                 axis_opts, ticks, sub_ticks, &
                 n_values_array, values_array )

            ! Reset text and tick sizes
            call plschr( 0.0_plflt, 1.0_plflt )
            call plsmaj( 0.0_plflt, 1.0_plflt )
            call plsmin( 0.0_plflt, 1.0_plflt )
         enddo
            

         ! Draw a page title
         write(title, '(3A)') trim(colorbar_option_kind_labels(kind_i+1)), ' - ', &
              trim(colorbar_cap_option_labels(cap_i+1))
         call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
         call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
         call plptex( 0.5_plflt, 0.5_plflt, 0.0_plflt, 0.0_plflt, 0.5_plflt, title )

         deallocate(values_array)
         
       end subroutine plcolorbar_example_page
       
       subroutine plcolorbar_example( palette, kind_i, cont_color, cont_width, n_values, values )
         character(*) :: palette
         integer(kind=plint)  :: kind_i, label_i, cap_i, cont_color, n_values
         real(kind=plflt)   :: cont_width
         real(kind=plflt), dimension(:)   :: values

         ! Load the color palette
         call plspal1( palette, 1_plint )

         do label_i = 0_plint,COLORBAR_LABELS-1
            do cap_i = 0_plint,COLORBAR_CAPS-1
               call plcolorbar_example_page( kind_i, label_i, cap_i, &
                    cont_color, cont_width, &
                    n_values, values )
            enddo
         enddo
       end subroutine plcolorbar_example

      end program
