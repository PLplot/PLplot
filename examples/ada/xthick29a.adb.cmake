-- $Id$

-- Sample plots using date / time formatting for axes

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
    Ada.Calendar,
    PLplot,
    PLplot_Auxiliary;
use
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Calendar,
    PLplot,
    PLplot_Auxiliary;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;
@Ada_Is_2007_With_and_Use_Numerics@

procedure xthick29a is
    -- Plot a model diurnal cycle of temperature
    procedure plot1 is
        x, y : Real_Vector(0 .. 144);

        -- Data points every 10 minutes for 1 day
        xmin, xmax, ymin, ymax : Long_Float;
    begin
        xmin := 0.0;
        xmax := 60.0 * 60.0 * 24.0; -- Number of seconds in a day
        ymin := 10.0;
        ymax := 20.0;

        for i in x'range loop
            x(i) := xmax * Long_Float(i) / Long_Float(x'length);
            y(i) := 15.0 - 5.0 * cos( 2.0 * pi * Long_Float(i) / Long_Float(x'length));
        end loop;

        Advance_To_Subpage(Next_Subpage);

        Set_Viewport_Standard;
        Set_Viewport_World(xmin, xmax, ymin, ymax);

        -- Draw a box with ticks spaced every 3 hours in X and 1 degree C in Y.
        Set_Pen_Color(Red);

        -- Set time format to be hours:minutes
        Set_Date_Time_Label_Format("%H:%M");
        Box_Around_Viewport("bcnstd", 3.0 * 60.0 * 60.0, 3, "bcnstv", 1.0, 5);

        Set_Pen_Color(Green);
        Write_Labels("Time (hours:mins)", "Temperature (degC)", "#frPLplot Example 29 - Daily temperature");
        Set_Pen_Color(Aquamarine);
        Draw_Curve(x, y);
    end plot1;


    -- Plot the number of hours of daylight as a function of day for a year
    procedure plot2 is
        xmin, xmax, ymin, ymax : Long_Float;
        lat, p, d : Long_Float;
        x, y : Real_Vector(0 .. 364);
    begin
        -- Latitude for London
        lat := 51.5;

        xmin := 0.0;
        xmax := Long_Float(x'length) * 60.0 * 60.0 * 24.0;
        ymin := 0.0;
        ymax := 24.0;

        -- Formula for hours of daylight from 
        -- "A Model Comparison for Daylength as a Function of Latitude and 
        -- Day of the Year", 1995, Ecological Modelling, 80, pp 87-95.
        for j in x'range loop
            x(j):= Long_Float(j) * 60.0 * 60.0 * 24.0;
            p := arcsin(0.39795 * cos(0.2163108 + 2.0 * arctan(0.9671396 * tan(0.00860 * Long_Float(j-186)))));
            d := 24.0 - (24.0 / pi) *
                arccos((sin(0.8333 * pi / 180.0) + sin(lat * pi / 180.0) * sin(p)) / 
                (cos(lat * pi / 180.0) * cos(p)));
            y(j):= d;
        end loop;

        Set_Pen_Color(Red);
        
        -- Set time format to be abbreviated month name followed by day of month
        Set_Date_Time_Label_Format("%b %d");
        Set_Environment(xmin, xmax, ymin, ymax, 0, 40);

        Set_Pen_Color(Green);
        Write_Labels("Date", "Hours of daylight", "#frPLplot Example 29 - Hours of daylight at 51.5N");

        Set_Pen_Color(Aquamarine);
        Draw_Curve(x, y);
    end plot2;


    procedure plot3 is
        xmin, xmax, ymin, ymax : Long_Float;
        x, y : Real_Vector(0 .. 61);
    begin
        -- Find the number of seconds since January 1, 1970 to December 12, 2005.
        -- Should be 1_133_395_200.0.
        xmin := Long_Float(Time_Of(2005, 12, 1, 0.0) - Time_Of(1970, 1,  1, 0.0));
        
        xmax := xmin + Long_Float(x'length) * 60.0 * 60.0 * 24.0;
        ymin := 0.0;
        ymax := 5.0;

        for i in x'range loop
            x(i) := xmin + Long_Float(i) * 60.0 * 60.0 * 24.0;
            y(i) := 1.0 + sin(2.0 * pi * Long_Float(i) / 7.0 ) + 
                exp((Long_Float(Integer'min(i, x'length - i))) / 31.0);
        end loop;
        
        Advance_To_Subpage(Next_Subpage);

        Set_Viewport_Standard;
        Set_Viewport_World(xmin, xmax, ymin, ymax);

        Set_Pen_Color(Red);

        -- Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
        -- equivalent to %f for C99 compliant implementations of strftime.
        Set_Date_Time_Label_Format("%Y-%m-%d");

        -- Draw a box with ticks spaced every 14 days in X and 1 hour in Y.
        Box_Around_Viewport("bcnstd", 14.0 * 24.0 * 60.0 * 60.0, 14, "bcnstv", 1.0, 4);

        Set_Pen_Color(Green);
        Write_Labels("Date", "Hours of television watched", "#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006");

        Set_Pen_Color(Aquamarine);

        Draw_Points(x, y, 2);
        Draw_Curve(x, y);
     
    end plot3;

begin
    -- Parse command line arguments
    Parse_Command_Line_Arguments(Parse_Full);

    -- Initialize plplot
    Initialize_PLplot;

    plot1;
    plot2;
    plot3;

    -- Don't forget to call End_PLplot to finish off!
    End_PLplot;
end xthick29a;
