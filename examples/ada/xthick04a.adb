-- $Id: xthick04c.c xxxxxx $
--
--	Log plot demo.

-- Copyright (C) 2007  Jerry Bauck

-- This file is part of PLplot.

-- PLplot is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Library Public License as published
-- by the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.

-- PLplot is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Library General Public License for more details.

-- You should have received a copy of the GNU Library General Public License
-- along with PLplot; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

with
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot,
    PLplot_Auxiliary;
use
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot,
    PLplot_Auxiliary;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;

procedure xthick04a is

    procedure plot1(Plot_Type : Integer) is
        freql, ampl, phase : Real_Vector(0 .. 100);
        f0, freq : Long_Float;
    begin
        Advance_To_Subpage(Next_Subpage);
        f0 := 1.0;
        for i in freql'range loop
            freql(i) := -2.0 + Long_Float(i) / 20.0;
            freq := 10.0 ** freql(i);
            ampl(i) := 20.0 * log(1.0 / sqrt(1.0 + (freq / f0) ** 2.0), 10.0);
            phase(i) := -(180.0 / pi) * arctan(freq / f0);
        end loop;

        Set_Viewport_Normalized(0.15, 0.85, 0.1, 0.9);
        Set_Viewport_World(-2.0, 3.0, -80.0, 0.0);
        
        -- Try different axis and labelling styles.
        Set_Pen_Color(Red);
        
        if Plot_Type = 0 then
            Box_Around_Viewport("bclnst", 0.0, 0, "bnstv", 0.0, 0);
        elsif Plot_Type = 1 then
            Box_Around_Viewport("bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0);
        end if;

        -- Plot ampl vs freq
        Set_Pen_Color(Yellow);
        Draw_Curve(freql, ampl);
        Set_Pen_Color(Red);
        Write_Text_World(1.6, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade");

        -- Put labels on
        Set_Pen_Color(Red);
        Write_Text_Viewport("b", 3.2, 0.5, 0.5, "Frequency");
        Write_Text_Viewport("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter");
        Set_Pen_Color(Yellow);
        Write_Text_Viewport("l", 5.0, 0.5, 0.5, "Amplitude (dB)");

        -- For the gridless case, put phase vs freq on same plot
        if Plot_Type = 0 then
            Set_Pen_Color(Red);
            Set_Viewport_World(-2.0, 3.0, -100.0, 0.0);
            Box_Around_Viewport("", 0.0, 0, "cmstv", 30.0, 3);
            Set_Pen_Color(Green);
            Draw_Curve(freql, phase);
            Set_Pen_Color(Green);
            Write_Text_Viewport("r", 5.0, 0.5, 0.5, "Phase shift (degrees)");
        end if;
    end plot1;

begin
    -- Parse and process command line arguments
    Parse_Command_Line_Arguments(PL_PARSE_FULL); 

    -- Initialize plplot
    Initialize_PLplot;
    Set_Font_Style(Roman_Font);

    -- Make log plots using two different styles.
    plot1(0);
    plot1(1);

    End_PLplot;
end xthick04a;
