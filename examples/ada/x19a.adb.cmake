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
    Text_IO,
    Interfaces.C,
    System.Storage_Elements,
    Ada.Strings,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Strings.Unbounded,
    PLplot_Auxiliary,
    PLplot_Traditional;
use
    Text_IO,
    Interfaces.C,
    System.Storage_Elements,
    Ada.Strings,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Strings.Unbounded,
    PLplot_Auxiliary,
    PLplot_Traditional;

@Ada_Is_2007_With_and_Use_Numerics@

-- Shows two views of the world map.
procedure x19a is
    minx, maxx, miny, maxy : Long_Float;

    -- This spec is necessary in order to enforce C calling conventions, used 
    -- in the callback by intervening C code.
    procedure mapform19(n : Integer; x, y : in out Map_Form_Constrained_Array); 
    pragma Convention(C, mapform19);

    -- Defines specific coordinate transformation for example 19.
    -- Not to be confused with mapform in src/plmap.c.
    -- x(), y() are the coordinates to be plotted.

    -- Ada note: Passing the array length as the first argument is the easiest 
    -- way (for the developer of the bindings) and maintain a C-compatible 
    -- argument list. It might be possible to instead pass a pointer to something 
    -- with an argument list (x, y : in out Real_Vector) instead, and write a
    -- wrapper function inside plmap and plmeridians that has the "correct" C 
    -- argument list, and then pass a pointer to _that_ when calling plmap and
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


    -- This spec is necessary in order to enforce C calling conventions, used 
    -- in the callback by intervening C code.
    -- See Ada note above, for mapform19.
    -- a_length is used only for bounds checking against the C-allocated memory
    -- for label.
    procedure geolocation_labeler
       (axis     : Integer; 
        a_value  : Long_Float;
        label    : out Label_String_Type;
        a_length : size_t;
        data     : PLpointer);
    pragma Convention(C, geolocation_labeler);

    -- A custom axis labeling function for longitudes and latitudes.
    procedure geolocation_labeler
       (axis     : Integer; 
        a_value  : Long_Float;
        label    : out Label_String_Type;
        a_length : size_t;
        data     : PLpointer) 
    is
        direction_label : Unbounded_String;
        label_val : Long_Float;


        -- "Normalize" longitude values so that they always fall between -180.0 and 180.0.
        function normalize_longitude(lon : Long_Float) return Long_Float is
            times : Long_Float;
        begin -- normalize_longitude
            if lon >= -180.0 and lon <= 180.0 then
                return lon;
            else
                times := Long_Float'Floor((abs(lon) + 180.0) / 360.0);
                if lon < 0.0 then
                    return lon + 360.0 * times;
                else
                    return lon - 360.0 * times;
                end if;
            end if;
        end normalize_longitude;
        
        
        -- Function to convert an unbounded string to a fixed-length C string with the 
        -- null terminator somewhere in the middle and spaces after. The result, of type
        -- Label_String_Type, is fixed to a length by C, currently at 41, and is 
        -- indexed in Ada as 0 .. PLplot_Traditional.Max_Label_String_Length.
        function Unbounded_To_Weird_C
           (Item     : Unbounded_String;
            C_Length : size_t) return Label_String_Type 
        is
            Temp : Unbounded_String;
        begin
            -- Check length and adjust if necessary.
            if Length(Item) >= Integer(C_Length) then
                Put_Line("*** Warning: Custom label was truncated to" 
                    & Integer'Image(Integer(C_Length)) & " characters. ***");
                Temp := Head(Item, Integer(C_Length));
                return To_C(To_String(Temp), True);
            else
               return To_C(To_String(Item & ASCII.nul & 
                    (Max_Label_String_Length - Length(Item)) * " "), False);
            end if;
        end Unbounded_To_Weird_C;

     begin -- geolocation_labeler
        if axis = PL_Y_AXIS then
            label_val := a_value;
            if label_val > 0.0 then
                direction_label := To_Unbounded_String(" N");
            elsif label_val < 0.0 then
                direction_label := To_Unbounded_String(" S");
            else
                direction_label := To_Unbounded_String("Eq");
            end if;
        elsif axis = PL_X_AXIS then
            label_val := normalize_longitude(a_value);
            if label_val > 0.0 then
                direction_label := To_Unbounded_String(" E");
            elsif label_val < 0.0 then
                direction_label := To_Unbounded_String(" W");
            else
                direction_label := To_Unbounded_String("");
            end if;
        end if;
        
        if axis = PL_Y_AXIS and a_value = 0.0 then
            -- A special case for the equator
            null;
        else
            direction_label := Trim(Integer'Image(Integer(abs(label_val))) & direction_label, Left);
        end if;
        label := Unbounded_To_Weird_C(direction_label, a_length);
    end geolocation_labeler;

begin

    -- Parse and process command line arguments 
    plparseopts(PL_PARSE_FULL);

    plinit;

    -- Longitude (x) and latitude (y) 
    miny := -70.0;
    maxy := 80.0;

    -- Cartesian plots 
    -- Most of world 
    minx := 190.0;
    maxx := 190.0 + 360.0;

    -- Setup a custom latitude and longitude-based scaling function.
    plslabelfunc(geolocation_labeler'Unrestricted_Access, System.Null_Address);

    plcol0(1);
    plenv(minx, maxx, miny, maxy, Justified, Custom_Labels_Linear_Box_Plus);
    plmap(null, USA_States_and_Continents, minx, maxx, miny, maxy);

    -- The Americas 
    minx := 190.0;
    maxx := 340.0;

    plcol0(1);
    plenv(minx, maxx, miny, maxy, Justified, Custom_Labels_Linear_Box_Plus);
    plmap(null, USA_States_and_Continents, minx, maxx, miny, maxy);

    -- Clear the labeling function.
    Use_Default_Labels;
    -- or...
    -- plslabelfunc(Null, System.Null_Address);

    -- Polar, Northern hemisphere 
    minx := 0.0;
    maxx := 360.0;

    plenv(-75.0, 75.0, -75.0, 75.0, Justified, Box);
    plmap(mapform19'Unrestricted_Access, Continents, minx, maxx, miny, maxy);

    pllsty(2);
    plmeridians(mapform19'Unrestricted_Access, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0);
    plend;
end x19a;
