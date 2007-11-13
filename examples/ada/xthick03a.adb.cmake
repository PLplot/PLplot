-- $Id$

-- Polar plot demo.
-- Generates polar plot, with 1-1 scaling.

-- Copyright (C) 2007 Jerry Bauck

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
    Ada.Strings,
    Ada.Strings.Fixed,
    PLplot,
    PLplot_Auxiliary;
use
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Strings,
    Ada.Strings.Fixed,
    PLplot,
    PLplot_Auxiliary;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;
@Ada_Is_2007_With_and_Use_Numerics@

procedure xthick03a is
    dtr, theta, dx, dy, r : Long_Float;
    x0, y0 : Real_Vector(0..360);
    x,  y  : Real_Vector(0..360);

begin
    dtr := pi / 180.0;
    for i in x0'range loop
        x0(i) := cos(dtr * Long_Float(i));
        y0(i) := sin(dtr * Long_Float(i));
    end loop;

    -- Parse and process command line arguments. 
    Parse_Command_Line_Arguments(Parse_Full);

    -- Initialize plplot 
    Initialize_PLplot;

    -- Set up viewport and window, but do not draw box. 
    Set_Environment(-1.3, 1.3, -1.3, 1.3, 1, -2);
    for i in 1..10 loop
        for j in x'range loop
            x(j) := 0.1 * Long_Float(i) * x0(j);
            y(j) := 0.1 * Long_Float(i) * y0(j);
        end loop;

        -- Draw circles for polar grid. 
        Draw_Curve(x, y);
    end loop;

    Set_Pen_Color(2);
    for i in 0..11 loop
        theta := 30.0 * Long_Float(i);
        dx := cos(dtr * theta);
        dy := sin(dtr * theta);

        -- Draw radial spokes for polar grid. 
        Draw_Line(0.0, 0.0, dx, dy);

        -- Write labels for angle. 
        -- Slightly off zero to avoid floating point logic flips at 90 and 270 deg
        if dx >= -0.00001 then
            Write_Text_World(dx, dy, dx, dy, -0.15, Trim(Integer'image(Integer(theta)), Left));
        else
            Write_Text_World(dx, dy, -dx, -dy, 1.15, Trim(Integer'image(Integer(theta)), Left));
        end if;
    end loop;

    -- Draw the graph.
    for i in x'range loop
        r := sin(dtr * Long_Float(5 * i));
        x(i) := x0(i) * r;
        y(i) := y0(i) * r;
    end loop;
    Set_Pen_Color(Green);
    Draw_Curve(x, y);

    Set_Pen_Color(Aquamarine);
    Write_Text_Viewport("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh");

    -- Close the plot at end. 
    End_PLplot;
end xthick03a;
