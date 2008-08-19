#  -*- coding: utf-8; -*-
#
#  $Id$
#
#  Multi-lingual version of the first page of example 4.
#
#  Copyright (C) 2006 Alan Irwin
#  Copyright (C) 2006,2008 Andrew Ross
#  Copyright (C) 2008 Arjen Markus
#
#  Thanks to the following for providing translated strings for this example:
#  Valery Pipin (Russian)
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Library Public License as published
#  by the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  PLplot is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public License
#  along with PLplot; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
#

#
# This example designed just for devices (e.g., psttfc and the
# cairo-related devices) that utilise the pango and fontconfig libraries. The
# best choice of glyph is selected by fontconfig and automatically rendered
# by pango in way that is sensitive to complex text layout (CTL) language
# issues for each unicode character in this example. Of course, you must
# have the appropriate TrueType fonts installed to have access to all the
# required glyphs.
#
# Translation instructions: The strings to be translated are given by
# x_label, y_label, alty_label, title_label, and line_label below.  The
# encoding used must be UTF-8.
#
# The following strings to be translated involve some scientific/mathematical
# jargon which is now discussed further to help translators.
#
# (1) dB is a decibel unit, see http://en.wikipedia.org/wiki/Decibel .
# (2) degrees is an angular measure, see
#     http://en.wikipedia.org/wiki/Degree_(angle) .
# (3) low-pass filter is one that transmits (passes) low frequencies.
# (4) pole is in the mathematical sense, see
#     http://en.wikipedia.org/wiki/Pole_(complex_analysis) .  "Single Pole"
#     means a particular mathematical transformation of the filter function has
#     a single pole, see
#     http://ccrma.stanford.edu/~jos/filters/Pole_Zero_Analysis_I.html .
#     Furthermore, a single-pole filter must have an inverse square decline
#     (or -20 db/decade). Since the filter plotted here does have that
#     characteristic, it must by definition be a single-pole filter, see also
#     http://www-k.ext.ti.com/SRVS/Data/ti/KnowledgeBases/analog/document/faqs/1p.htm
# (5) decade represents a factor of 10, see
#     http://en.wikipedia.org/wiki/Decade_(log_scale) .
#
#
#

# --------------------------------------------------------------------------
#  main
#
#  Illustration of logarithmic axes, and redefinition of window.
# --------------------------------------------------------------------------

proc x26 {{w loopback}} {

  set x_label {
       "Frequency"
       "Частота"
  }

  set y_label {
       "Amplitude (dB)"
       "Амплитуда (dB)"
  }

  set alty_label {
       "Phase shift (degrees)"
       "Фазовый сдвиг (градусы)"
  }

  set title_label {
      "Single Pole Low-Pass Filter"
      "Однополюсный Низко-Частотный Фильтр"
  }

  set line_label {
       "-20 dB/decade"
       "-20 dB/десяток"
  }

  #  Parse and process command line arguments

  #  AM: TODO
  #  plparseopts?

  #  Initialize plplot

  $w cmd plfont 2

  #  Make log plots using two different styles.

  foreach xl $x_label yl $y_label altyl $alty_label title $title_label linel $line_label {
      plot261 $w 0 $xl $yl $altyl $title $linel
  }

}

# --------------------------------------------------------------------------
#  plot1
#
#  Log-linear plot.
# --------------------------------------------------------------------------

proc plot261 { w type x_label y_label alty_label title_label line_label } {

  set PI [expr {4.0*atan(1.0)}]

  matrix freql f 101
  matrix ampl  f 101
  matrix phase f 101

  $w cmd pladv 0

  #  Set up data for log plot

  set f0 1.0
  for {set i 0} {$i < 101} {incr i} {
     freql $i = [expr {-2.0 + $i / 20.0}]
     set freq [expr {pow(10.0,[freql $i])}]
     ampl $i  = [expr {20.0 * log10(1.0 / sqrt(1.0 + pow($freq/$f0,2)))}]
     phase $i = [expr {-(180.0 / $PI) * atan($freq / $f0)}]
  }
  $w cmd plvpor 0.15 0.85 0.1 0.9
  $w cmd plwind -2.0 3.0 -80.0 0.0

  #  Try different axis and labelling styles.

  $w cmd plcol0 1
  if { $type == 0 } {
     $w cmd plbox "bclnst" 0.0 0 "bnstv" 0.0 0
  }
  if { $type == 1 } {
     $w cmd plbox "bcfghlnst" 0.0 0 "bcghnstv" 0.0 0
  }

  #  Plot ampl vs freq

  $w cmd plcol0 2
  $w cmd plline 101 freql ampl
  $w cmd plcol0 1
  $w cmd plptex 1.6 -30.0 1.0 -20.0 0.5 $line_label

  #  Put labels on

  $w cmd plcol0 1
  $w cmd plmtex "b" 3.2 0.5 0.5 $x_label
  $w cmd plmtex "t" 2.0 0.5 0.5 $title_label
  $w cmd plcol0 2
  $w cmd plmtex "l" 5.0 0.5 0.5 $y_label

  #  For the gridless case, put phase vs freq on same plot

  if {$type == 0} {
     $w cmd plcol0 1
     $w cmd plwind -2.0 3.0 -100.0 0.0
     $w cmd plbox "" 0.0 0 "cmstv" 30.0 3
     $w cmd plcol0 3
     $w cmd plline 101 freql phase
     $w cmd plcol0 3
     $w cmd plmtex "r" 5.0 0.5 0.5 $alty_label
  }
}
