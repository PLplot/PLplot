--[[ -*- coding: utf-8 -*-
   
   $Id: $

   Multi-lingual version of the first page of example 4.

   Copyright (C) 2009 Werner Smekal

   Thanks to the following for providing translated strings for this example:
   Valery Pipin (Russian)
  
   This file is part of PLplot.
    
   PLplot is free software you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation either version 2 of the License, or
   (at your option) any later version.
   
   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.
    
   You should have received a copy of the GNU Library General Public License
   along with PLplot if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
--]]


--[[
  This example designed just for devices (e.g., psttfc and the
  cairo-related devices) that use the pango and fontconfig libraries. The
  best choice of glyph is selected by fontconfig and automatically rendered
  by pango in way that is sensitive to complex text layout (CTL) language
  issues for each unicode character in this example. Of course, you must
  have the appropriate TrueType fonts installed to have access to all the
  required glyphs.

  Translation instructions: The strings to be translated are given by
  x_label, y_label, alty_label, title_label, and line_label below.  The 
  encoding used must be UTF-8.

  The following strings to be translated involve some scientific/mathematical 
  jargon which is now discussed further to help translators.

  (1) dB is a decibel unit, see http://en.wikipedia.org/wiki/Decibel .
  (2) degrees is an angular measure, see 
      http://en.wikipedia.org/wiki/Degree_(angle) .
  (3) low-pass filter is one that transmits (passes) low frequencies.
  (4) pole is in the mathematical sense, see
      http://en.wikipedia.org/wiki/Pole_(complex_analysis) .  "Single Pole"
      means a particular mathematical transformation of the filter function has
      a single pole, see
      http://ccrma.stanford.edu/~jos/filters/Pole_Zero_Analysis_I.html .  
      Furthermore, a single-pole filter must have an inverse square decline 
      (or -20 db/decade). Since the filter plotted here does have that 
      characteristic, it must by definition be a single-pole filter, see also
      http://www-k.ext.ti.com/SRVS/Data/ti/KnowledgeBases/analog/document/faqs/1p.htm
  (5) decade represents a factor of 10, see
      http://en.wikipedia.org/wiki/Decade_(log_scale) .
--]]

-- initialise Lua bindings for PLplot examples.
dofile("plplot_examples.lua")

x_label = { "Frequency", "Частота" }
y_label = { "Amplitude (dB)", "Амплитуда (dB)" }
alty_label = { "Phase shift (degrees)", "Фазовый сдвиг (градусы)" }
title_label = { "Single Pole Low-Pass Filter", "Однополюсный Низко-Частотный Фильтр" }
line_label = { "-20 dB/decade", "-20 dB/десяток" }


----------------------------------------------------------------------------
-- plot1
--
-- Log-linear plot.
----------------------------------------------------------------------------

function plot1(typ, x_label, y_label, alty_label, title_label, line_label)
  freql = {}
  ampl = {}
  phase = {}

  pl.adv(0)

  -- Set up data for log plot 

  f0 = 1
  for i = 1, 101 do
    freql[i] = -2 + (i-1)/20
    freq = 10^freql[i]
    ampl[i] = 20 * math.log10(1/math.sqrt(1+(freq/f0)^2))
    phase[i] = -180/math.pi*math.atan(freq/f0)
  end

  pl.vpor(0.15, 0.85, 0.1, 0.9)
  pl.wind(-2, 3, -80, 0)

  -- Try different axis and labelling styles. 
  pl.col0(1)
  if typ==0 then
    pl.box("bclnst", 0, 0, "bnstv", 0, 0)
  else
    pl.box("bcfghlnst", 0, 0, "bcghnstv", 0, 0)
  end

  -- Plot ampl vs freq 
  pl.col0(2)
  pl.line(freql, ampl)
  pl.col0(1)
  pl.ptex(1.6, -30, 1, -20, 0.5, line_label)

  -- Put labels on 
  pl.col0(1)
  pl.mtex("b", 3.2, 0.5, 0.5, x_label)
  pl.mtex("t", 2, 0.5, 0.5, title_label)
  pl.col0(2)
  pl.mtex("l", 5, 0.5, 0.5, y_label)

  -- For the gridless case, put phase vs freq on same plot 
  if typ==0 then
    pl.col0(1)
    pl.wind(-2, 3, -100, 0)
    pl.box("", 0, 0, "cmstv", 30, 3)
    pl.col0(3)
    pl.line(freql, phase)
    pl.col0(3)
    pl.mtex("r", 5, 0.5, 0.5, alty_label)
  end
end


----------------------------------------------------------------------------
-- main
--
-- Illustration of logarithmic axes, and redefinition of window.
----------------------------------------------------------------------------

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()
pl.font(2)

-- Make log plots using two different styles. 
for i = 1, 2 do
  plot1(0, x_label[i], y_label[i], alty_label[i], title_label[i],  line_label[i])
end

pl.plend()
