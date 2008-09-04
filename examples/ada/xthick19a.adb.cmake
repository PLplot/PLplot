-- $Id$

--	Illustrates backdrop plotting of world, US maps.
--	Contributed by Wesley Ebisuzaki.

-- Copyright (C) 2008 Jerry Bauck

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
    PLplot;
use
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;
@Ada_Is_2007_With_and_Use_Numerics@

-- Shows two views of the world map.
procedure xthick19a is
    minx, maxx, miny, maxy : Long_Float;

    -- This spec is necessary in order to enforce C calling conventions, used 
    -- in the callback by intervening C code.
    procedure mapform19(n : Integer; x, y : in out Map_Form_Constrained_Array); 
    pragma Convention(C, mapform19);

    -- Defines specific coordinate transformation for example 19.
    -- Not to be confused with mapform in src/Draw_Map.c.
    -- x(), y() are the coordinates to be plotted.

    -- Ada note: Passing the array length as the first argument is the easiest 
    -- way (for the developer of the bindings) and maintain a C-compatible 
    -- argument list. It might be possible to instead pass a pointer to something 
    -- with an argument list (x, y : in out Real_Vector) instead, and write a
    -- wrapper function inside Draw_Map and Draw_Latitude_Longitude that has the "correct" C 
    -- argument list, and then pass a pointer to _that_ when calling Draw_Map and
    -- plmeridian.
    procedure mapform19(n : Integer; x, y : in out Map_Form_Constrained_Array) is 
        xp, yp, radius : Long_Float;
    begin
         -- DO NOT use x'range for this loop because the C function which calls 
         -- this function WILL NOT SEE IT AND YOU WILL GET A SEGFAULT. Simply 
         -- use 0 .. n - 1 explicitly.
        for i in 0 .. n - 1 loop
            radius := 90.0 - y(i);
            xp := radius * cos(x(i) * pi / 180.0);
            yp := radius * sin(x(i) * pi / 180.0);
            x(i) := xp;
            y(i) := yp;
        end loop;
    end mapform19;

begin

    -- Parse and process command line arguments 
    Parse_Command_Line_Arguments(Parse_Full);

    Initialize_PLplot;

    -- Longitude (x) and latitude (y) 
    miny := -70.0;
    maxy := 80.0;

    -- Cartesian plots 
    -- Most of world 
    minx := 190.0;
    maxx := 190.0 + 360.0;

    Set_Pen_Color(Red);
    Set_Environment(minx, maxx, miny, maxy, Justified, Box);
    Draw_Map(null, USA_States_and_Continents, minx, maxx, miny, maxy);

    -- The Americas 
    minx := 190.0;
    maxx := 340.0;

    Set_Pen_Color(Red);
    Set_Environment(minx, maxx, miny, maxy, Justified, Box);
    Draw_Map(null, USA_States_and_Continents, minx, maxx, miny, maxy);

    -- Polar, Northern hemisphere 
    minx := 0.0;
    maxx := 360.0;

    Set_Environment(-75.0, 75.0, -75.0, 75.0, Justified, Box);
    Draw_Map(mapform19'Unrestricted_Access, Continents, minx, maxx, miny, maxy);

    Select_Line_Style(Line_Style => 2);
    Draw_Latitude_Longitude(mapform19'Unrestricted_Access, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0);
    End_PLplot;
end xthick19a;
