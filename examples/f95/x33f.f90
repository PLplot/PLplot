!     -*- coding: utf-8; -*-
!
!     $Id$
!
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

      integer, parameter :: MAX_NLEGEND = 7

      integer            :: i, k
      integer            :: opt, position
      integer            :: nlegend, nturn
      integer            :: opt_array(MAX_NLEGEND)
      integer            :: text_colors(MAX_NLEGEND)
      integer            :: box_colors(MAX_NLEGEND)
      integer            :: box_patterns(MAX_NLEGEND)
      real(kind=plflt)   :: box_scales(MAX_NLEGEND)
      integer            :: box_line_widths(MAX_NLEGEND)
      integer            :: line_colors(MAX_NLEGEND)
      integer            :: line_styles(MAX_NLEGEND)
      integer            :: line_widths(MAX_NLEGEND)
      integer            :: symbol_numbers(MAX_NLEGEND), symbol_colors(MAX_NLEGEND)
      real(kind=plflt)   :: symbol_scales(MAX_NLEGEND)
      character(len=200) :: text(MAX_NLEGEND)
      character(len=3)   :: symbols(MAX_NLEGEND)
      real(kind=plflt)   :: legend_width, legend_height, x, y, xstart, ystart
      real(kind=plflt)   :: max_height, text_scale
      integer            :: opt_base, nrow, ncolumn

      integer            :: position_options(16)
      character(len=3)   :: special_symbols(5)

      real(kind=plflt)   :: values_small(2)
      real(kind=plflt)   :: values_uneven(9)
      real(kind=plflt)   :: values_even(9)


!     Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
      data special_symbols / &
          '✰',             &
          '✴',             &
          '✱',             &
          '✽',             &
          '✦'              /

      data values_small   / 0.0_plflt, 1.0_plflt /
      data values_uneven &
      / 0.0_plflt, 2.0_plflt, 2.6_plflt, 3.4_plflt, 6.0_plflt, 7.0_plflt, 8.0_plflt, 9.0_plflt, 10.0_plflt /
      data values_even &
      / 0.0_plflt, 1.0_plflt, 2.0_plflt, 3.0_plflt, 4.0_plflt, 5.0_plflt, 6.0_plflt, 7.0_plflt, 8.0_plflt /

      position_options(1) = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(2) = PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(3) = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_OUTSIDE
      position_options(4) = PL_POSITION_RIGHT + PL_POSITION_OUTSIDE
      position_options(5) = PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(6) = PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(7) = PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      position_options(8) = PL_POSITION_LEFT + PL_POSITION_OUTSIDE
      position_options(9) = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(10) = PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(11) = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE
      position_options(12) = PL_POSITION_RIGHT + PL_POSITION_INSIDE
      position_options(13) = PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(14) = PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(15) = PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      position_options(16) = PL_POSITION_LEFT + PL_POSITION_INSIDE

!     Parse and process command line arguments

      call plparseopts(PL_PARSE_FULL)

!     Initialize plplot
      call plinit

!     First page illustrating the 16 standard positions.
      call pladv( 0 )
      call plvpor( 0.25_plflt, 0.75_plflt, 0.25_plflt, 0.75_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plbox( 'bc', 0.0_plflt, 0, 'bc', 0.0_plflt, 0 )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 8.0_plflt, 0.5_plflt, 0.5_plflt, &
               'The 16 standard legend positions with' )
      call plmtex( 't', 6.0_plflt, 0.5_plflt, 0.5_plflt, &
               'the same (0.05) offset in x and y' )

      nlegend = 1
!     Only specify legend data that are required according to the
!     value of opt_array for that entry.
      opt_base          = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      opt_array(1)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
      line_styles(1)    = 1
      line_widths(1)    = 1
      symbol_scales(1)  = 1._plflt
      symbol_numbers(1) = 4
      symbols(1)        = '*'

!     Use monotype fonts so that all legends are the same size.
      call plsfont( PL_FCI_MONO, -1, -1 )
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      do k = 1,16
          position = position_options(k)
          opt = opt_base
          text_colors(1)   = 1 + mod( k-1, 8 )
          line_colors(1)   = 1 + mod( k-1, 8 )
          symbol_colors(1) = 1 + mod( k-1, 8 )
          write( text(1), '(i2.2)' ) k-1

          call pllegend( legend_width, legend_height, opt, position, &
              0.05_plflt, 0.05_plflt,                                &
              0.1_plflt, 15, 1, 1, 0, 0,                             &
              opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt, &
              1._plflt, text_colors, text,                           &
              box_colors, box_patterns, box_scales, box_line_widths, &
              line_colors, line_styles, line_widths,                 &
              symbol_colors, symbol_scales, symbol_numbers, symbols  )
      enddo

!     Second page illustrating effect of nrow, ncolumn for the same legend
!     data.

      call pladv( 0 )
      call plvpor( 0.25_plflt, 0.75_plflt, 0.25_plflt, 0.75_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plbox( 'bc', 0.0_plflt, 0, 'bc', 0.0_plflt, 0 )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 8.0_plflt, 0.5_plflt, 0.5_plflt, &
               'The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,' )
      call plmtex( 't', 6.0_plflt, 0.5_plflt, 0.5_plflt, &
               'and position for the same legend data' )

      nlegend = 7

!     Only specify legend data that are required according to the
!     value of opt_array for that entry.

      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      do 120 k = 1,nlegend
          opt_array(k)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
          line_styles(k)    = 1
          line_widths(k)    = 1
          symbol_scales(k)  = 1._plflt
          symbol_numbers(k) = 2
          symbols(k)        = '*'
          write( text(k), '(i2.2)' ) k-1
          text_colors(k)   = 1 + mod( k-1, 8 )
          line_colors(k)   = 1 + mod( k-1, 8 )
          symbol_colors(k) = 1 + mod( k-1, 8 )
  120 continue

!     Use monotype fonts so that all legends are the same size.

      call plsfont( PL_FCI_MONO, -1, -1 )
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      position = PL_POSITION_TOP + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0.1_plflt
      nrow    = 1
      ncolumn = nlegend
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0.1_plflt
      nrow    = 1
      ncolumn = nlegend
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
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
      ncolumn = 1
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
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
      ncolumn = 1
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE
      opt     = opt_base
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 6
      ncolumn = 2
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE
      opt     = opt_base + PL_LEGEND_ROW_MAJOR
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 6
      ncolumn = 2
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

      position = PL_POSITION_BOTTOM + PL_POSITION_INSIDE
      opt     = opt_base + PL_LEGEND_ROW_MAJOR
      x       = 0._plflt
      y       = 0._plflt
      nrow    = 3
      ncolumn = 3
      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.05_plflt, 15, 1, 1, nrow, ncolumn,                         &
          opt_array(1:nlegend), 1.0_plflt, 1.0_plflt, 2.0_plflt,       &
          1._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )

!     Third page demonstrating legend alignment

      call pladv( 0 )
      call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 0.9_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
               'Demonstrate legend alignment' )

      x        = 0.1_plflt
      y        = 0.1_plflt
      nturn    = 5
      nlegend  = 0
      position = PL_POSITION_TOP + PL_POSITION_LEFT + PL_POSITION_SUBPAGE
      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX
      opt      = opt_base
      do i = 1,9

!         Set up legend arrays with the correct size, type.

          if ( i .le. nturn ) then
              nlegend = nlegend + 1
          else
              nlegend = nlegend - 1
          endif
          nlegend = max( 1, nlegend )

!         nly specify legend data that are required according to the
!         value of opt_array for that entry.

          do k = 1,nlegend
              opt_array(k)      = PL_LEGEND_LINE + PL_LEGEND_SYMBOL
              line_styles(k)    = 1
              line_widths(k)    = 1
              symbol_scales(k)  = 1._plflt
              symbol_numbers(k) = 2
              symbols(k)        = '*'
              write( text(k), '(i2.2)' ) k-1
              text_colors(k)   = 1 + mod( k-1, 8 )
              line_colors(k)   = 1 + mod( k-1, 8 )
              symbol_colors(k) = 1 + mod( k-1, 8 )
          enddo

!         Use monotype fonts so that all legends are the same size.

          call plsfont( PL_FCI_MONO, -1, -1 )
          call plscol0a( 15, 32, 32, 32, 0.70_plflt )

          nrow    = MIN( 3, nlegend )
          ncolumn = 0

          call pllegend( legend_width, legend_height, opt, position, &
              x, y,                                                  &
              0.025_plflt, 15, 1, 1, nrow, ncolumn,                  &
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
      call pladv( 0 )
      call plvpor( 0.0_plflt, 1._plflt, 0.0_plflt, 0.90_plflt )
      call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!       call plbox('bc', 0.0, 0, 'bc', 0.0_plflt, 0)
      call plsfont( PL_FCI_SANS, -1, -1 )
      call plmtex( 't', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
               'Demonstrate Various Kinds of Legends' )

      nlegend = 5

!     Only specify legend data that are required according to the
!     value of opt_array for that entry.

      position = PL_POSITION_LEFT + PL_POSITION_TOP
      opt_base = PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX + PL_LEGEND_TEXT_LEFT

!     Set up None, Box, Line, Symbol, and Line & Symbol legend entries.

      opt_array(1) = PL_LEGEND_NONE
      text(1) = 'None'
      text_colors(1) = 1

      opt_array(2) = PL_LEGEND_COLOR_BOX
      text(2) = 'Box'
      text_colors(2)     = 2
      box_colors(2)      = 2
      box_patterns(2)    = 0
      box_scales(2)      = 0.8_plflt
      box_line_widths(2) = 1

      opt_array(3) = PL_LEGEND_LINE
      text(3) = 'Line'
      text_colors(3) = 3
      line_colors(3) = 3
      line_styles(3) = 1
      line_widths(3) = 1

      opt_array(4) = PL_LEGEND_SYMBOL
      text(4) = 'Symbol'
      text_colors(4)    = 4
      symbol_colors(4)  = 4
      symbol_scales(4)  = text_scale
      symbol_numbers(4) = 4
      symbols(4)        = special_symbols(3)

      opt_array(5) = PL_LEGEND_SYMBOL + PL_LEGEND_LINE
      text(5) = 'L & S'
      text_colors(5)    = 5
      line_colors(5)    = 5
      line_styles(5)    = 1
      line_widths(5)    = 1
      symbol_colors(5)  = 5
      symbol_scales(5)  = text_scale
      symbol_numbers(5) = 4
      symbols(5)        = special_symbols(3)

      opt = opt_base
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up symbol legend entries with various symbols.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_SYMBOL
          text(i) = 'Symbol ' // special_symbols(i)
          text_colors(i)    = i
          symbol_colors(i)  = i
          symbol_scales(i)  = text_scale
          symbol_numbers(i) = 4
          symbols(i)        = special_symbols(i)
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up symbol legend entries with various numbers of symbols.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_SYMBOL
          write( text(i), '(a,i0)' ) 'Symbol Number ', i+1
          text_colors(i)    = i
          symbol_colors(i)  = i
          symbol_scales(i)  = text_scale
          symbol_numbers(i) = i + 1
          symbols(i)        = special_symbols(3)
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various colours.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Color ', i
          text_colors(i)     = i
          box_colors(i)      = i
          box_patterns(i)    = 0
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = 1
      enddo

      opt = opt_base

!     Use new origin

      x          = xstart
      y          = y + max_height
      max_height = 0._plflt
      call plscol0a( 15, 32, 32, 32, 0.70_plflt)

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various patterns.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Pattern ', i-1
          text_colors(i)     = 2
          box_colors(i)      = 2
          box_patterns(i)    = i - 1
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = 1
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up box legend entries with various box pattern line widths.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_COLOR_BOX
          write( text(i), '(a,i0)' ) 'Box Line Width ', i
          text_colors(i)     = 2
          box_colors(i)      = 2
          box_patterns(i)    = 3
          box_scales(i)      = 0.8_plflt
          box_line_widths(i) = i
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
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
          line_styles(i) = 1
          line_widths(i) = 1
      enddo

      opt = opt_base

!     Use new origin

      x          = xstart
      y          = y + max_height
      max_height = 0._plflt
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Set up line legend entries with various styles.

      do i = 1,nlegend
          opt_array(i) = PL_LEGEND_LINE
          write( text(i), '(a,i0)' ) 'Line Style ', i
          text_colors(i) = 2
          line_colors(i) = 2
          line_styles(i) = i
          line_widths(i) = 1
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
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
          text_colors(i) = 2
          line_colors(i) = 2
          line_styles(i) = 1
          line_widths(i) = i
      enddo

      opt = opt_base
      x   = x + legend_width
      call plscol0a( 15, 32, 32, 32, 0.70_plflt )

      call pllegend( legend_width, legend_height, opt, position, x, y, &
          0.1_plflt, 15, 1, 1, 0, 0,                                   &
          opt_array(1:nlegend), 1.0_plflt, text_scale, 2.0_plflt,      &
          0._plflt, text_colors, text,                                 &
          box_colors, box_patterns, box_scales, box_line_widths,       &
          line_colors, line_styles, line_widths,                       &
          symbol_colors, symbol_scales, symbol_numbers, symbols        )
      max_height = max( max_height, legend_height )

!     Color bar examples

!
!     Note: commented until plcolorbar is ready!
!
!     call plcolorbar_example_1( PL_COLORBAR_IMAGE, 0.0_plflt, 0, 2,
!    &         values_small, 'Image Color Bars' )
!     call plcolorbar_example_1(
!    &         PL_COLORBAR_SHADE + PL_COLORBAR_SHADE_LABEL,
!    &         0.0_plflt, 0, 9, values_uneven,
!    &         'Shade Color Bars - Uneven Steps' )
!     call plcolorbar_example_2( PL_COLORBAR_SHADE, 3.0_plflt, 3, 9,
!    &         values_even, 'Shade Color Bars - Even Steps' )
!     call plcolorbar_example_1( PL_COLORBAR_GRADIENT, 0.5_plflt, 5, 2,
!    &         values_small, 'Gradient Color Bars' )
!     call plcolorbar_example_2( PL_COLORBAR_GRADIENT, 0.5_plflt, 5, 2,
!    &         values_small, 'Gradient Color Bars' )

      call plend()

!     contains


! Color bar routines
!
!     subroutine plcolorbar_example_1( bar_type, ticks, sub_ticks, n,
!    &               values, title )
!
!     integer bar_type, position
!     real(kind=plflt) ticks
!     integer sub_ticks
!     integer n
!     real(kind=plflt) values(*)
!     character(*) title
!
!
!     FORTRAN 77: array fixed
!
!     real(kind=plflt) colors(20)
!     real(kind=plflt) color_step
!     integer i
!     integer opt
!     character(10) axis_opts_1, axis_opts_2
!
!     call pladv( 0 )
!
!     Setup color palette 1
!     call plspal1( 'cmap1_blue_red.pal', 1 )
!
!     color_step = 1.0_plflt / ( n - 1 )
!     do 110 i = 1,n
!         colors(i) = 0.0_plflt + color_step * i
! 110 continue
!
!     position = PL_POSITION_LEFT
!     opt      = bar_type + PL_COLORBAR_LABEL_LEFT +
!    &           PL_COLORBAR_CAP_HIGH
!
!     if ( mod( bar_type, 2*PL_COLORBAR_SHADE_LABEL ) /
!    &         PL_COLORBAR_SHADE_LABEL .eq. 1 ) then
!         axis_opts_1 = 'iv'
!         axis_opts_2 = 'i'
!     else
!             axis_opts_1 = 'stv'
!             axis_opts_2 = 'st'
!         else
!             axis_opts_1 = 'tv'
!             axis_opts_2 = 't'
!         endif
!     endif
!
!     call plcolorbar( opt, 0.1_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_1, 'Test label - Left, High Cap',
!
!     position = PL_POSITION_RIGHT
!     opt      = bar_type + PL_COLORBAR_LABEL_RIGHT +
!    &           PL_COLORBAR_CAP_LOW
!
!     call plcolorbar( opt, 0.1_plflt, 0.4_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    n, colors, values )
!
!     position = PL_POSITION_TOP
!     opt      = bar_type + PL_COLORBAR_LABEL_TOP +
!    &           PL_COLORBAR_CAP_HIGH
!
!     call plcolorbar( opt, 0.1_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_2, 'Test label - Upper, High Cap',
!    &    n, colors, values )
!
!     position = PL_POSITION_TOP
!     opt      = bar_type + PL_COLORBAR_LABEL_BOTTOM +
!    &           PL_COLORBAR_CAP_LOW
!
!     call plcolorbar( opt, 0.4_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_2, 'Test label - Lower, Low Cap',
!    &    n, colors, values )
!
!     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!     call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!     call plptex( 0.5_plflt, 0.5_plflt, 0.0_plflt, 0.0_plflt, 0.5_plflt, title )
!     end
!
!     subroutine plcolorbar_example_2( bar_type, ticks, sub_ticks, n,
!    &               values, title )
!
!     integer bar_type, position
!     real(kind=plflt) ticks
!     integer sub_ticks
!     integer n
!     real(kind=plflt) values(*)
!     character(*) title
!
!     real(kind=plflt) colors(20)
!     integer i
!     real(kind=plflt) color_step
!     integer opt
!     character(10) axis_opts_1, axis_opts_2
!
!     call pladv( 0 )
!     Setup color palette 1
!     call plspal1( 'cmap1_blue_yellow.pal', 1 )
!
!     color_step = 1.0_plflt / ( n - 1 )
!     do 110 i = 1,n
!         colors(i) = 0.0_plflt + color_step * i
! 110 continue
!
!     position = PL_POSITION_LEFT
!     opt      = bar_type + PL_COLORBAR_LABEL_LEFT +
!    &           PL_COLORBAR_CAP_LOW
!
!     if ( bar_type .eq. PL_COLORBAR_SHADE_LABEL ) then
!         axis_opts_1 = ''
!         axis_opts_2 = ''
!     else
!         if ( sub_ticks .ne. 0 ) then
!             axis_opts_1 = 'stv'
!             axis_opts_2 = 'st'
!         else
!             axis_opts_1 = 'tv'
!             axis_opts_2 = 't'
!         endif
!     endif
!
!     call plcolorbar( opt, 0.1_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_1, 'Test label - Left, Low Cap',
!    &    n, colors, values )
!
!     position = PL_POSITION_RIGHT
!     opt      = bar_type + PL_COLORBAR_LABEL_RIGHT +
!    &           PL_COLORBAR_CAP_HIGH
!
!     call plcolorbar( opt, 0.1_plflt, 0.4_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_1, 'Test label - Right, High Cap',
!    &    n, colors, values )
!
!     position = PL_POSITION_TOP
!     opt      = bar_type + PL_COLORBAR_LABEL_TOP +
!    &           PL_COLORBAR_CAP_LOW
!
!     call plcolorbar( opt, 0.1_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_2, 'Test label - Upper, Low Cap',
!    &    n, colors, values )
!
!     position = PL_POSITION_BOTTOM
!     opt      = bar_type + PL_COLORBAR_LABEL_BOTTOM +
!    &           PL_COLORBAR_CAP_HIGH
!
!     call plcolorbar( opt, 0.4_plflt, 0.1_plflt, 0.5_plflt, 0.1_plflt,
!    &    ticks, sub_ticks,
!    &    axis_opts_2, 'Test label - Lower, High Cap',
!    &    n, colors, values )
!
!     call plvpor( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!     call plwind( 0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt )
!     call plptex( 0.5_plflt, 0.5_plflt, 0.0_plflt, 0.0_plflt, 0.5_plflt, title )
!     end
      end program
