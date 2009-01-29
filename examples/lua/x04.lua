--[[ $Id$

	Log plot demo.
	Simple line plot and multiple windows demo.

  Copyright (C) 2008  Werner Smekal

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

-- initialise Lua bindings for PLplot examples.
dofile("plplot_examples.lua")

----------------------------------------------------------------------------
-- plot1
--
-- Log-linear plot.
----------------------------------------------------------------------------

function plot1(type)
  freql = {}
  ampl = {}
  phase = {}

  pl.adv(0)

  -- Set up data for log plot 
  f0 = 1
  for i=1, 101 do
    freql[i] = -2 + (i-1) / 20
    freq = 10^freql[i]
    ampl[i] = 20 * math.log10(1 / math.sqrt(1 + (freq / f0)^2))
    phase[i] = -(180 / math.pi) * math.atan(freq / f0)
  end

  pl.vpor(0.15, 0.85, 0.1, 0.9)
  pl.wind(-2, 3, -80, 0)

  -- Try different axis and labelling styles. 
  pl.col0(1)
  if type == 0 then
    pl.box("bclnst", 0, 0, "bnstv", 0, 0)
  end
  
  if type == 1 then
    pl.box("bcfghlnst", 0, 0, "bcghnstv", 0, 0)
  end
  
  -- Plot ampl vs freq 
  pl.col0(2)
  pl.line(freql, ampl)
  pl.col0(1)
  pl.ptex(1.6, -30, 1, -20, 0.5, "-20 dB/decade")

  -- Put labels on 
  pl.col0(1)
  pl.mtex("b", 3.2, 0.5, 0.5, "Frequency")
  pl.mtex("t", 2, 0.5, 0.5, "Single Pole Low-Pass Filter")
  pl.col0(2)
  pl.mtex("l", 5, 0.5, 0.5, "Amplitude (dB)")

  -- For the gridless case, put phase vs freq on same plot 
  if type == 0 then
    pl.col0(1)
    pl.wind(-2, 3, -100, 0)
    pl.box("", 0, 0, "cmstv", 30, 3)
    pl.col0(3)
    pl.line(freql, phase)
    pl.col0(3)
    pl.mtex("r", 5, 0.5, 0.5, "Phase shift (degrees)")
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
plot1(0)
plot1(1)

pl.plend()
