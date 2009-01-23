-- $Id$

-- 3-d plot demo.

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
    PLplot_Traditional,
    PLplot_Auxiliary;
use
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot_Traditional,
    PLplot_Auxiliary;

@Ada_Is_2007_With_and_Use_Numerics@

procedure x08a is

    XPTS : Integer := 35;
    YPTS : Integer := 46;
    x : Real_Vector(0 .. XPTS - 1);
    y : Real_Vector(0 .. YPTS - 1);
    z : Real_Matrix(0 .. XPTS - 1, 0 .. YPTS - 1);
    xx, yy, r : Long_Float;
    zmin, zmax, step : Long_Float;
    LEVELS : Integer := 10;
    clevel: Real_Vector(0 .. LEVELS - 1);
    nlevel : Integer := LEVELS;
    rosen : Boolean := True;
    sombrero : Boolean := False;
    alt : Real_Vector(0 .. 1) := (60.0, 20.0);
    az  : Real_Vector(0 .. 1) := (30.0, 60.0);
    title : array(0 .. 1) of String(1 .. 35) := 
        ("#frPLplot Example 8 - Alt=60, Az=30", 
         "#frPLplot Example 8 - Alt=20, Az=60");


    ------------------------------------------------------------------------------
    -- cmap1_init1

    -- Initializes color map 1 in HLS space.
    -- Basic grayscale variation from half-dark (which makes more interesting
    -- looking plot compared to dark) to light.
    -- An interesting variation on this:
    -- s[1] = 1.0
    ----------------------------------------------------------------------------

    procedure cmap1_init(gray : Boolean) is
        i, h, l, s : Real_Vector(0 .. 1);
    begin
        i(0) := 0.0;        -- left boundary
        i(1) := 1.0;        -- right boundary

        if gray then
            h(0) := 0.0;	-- hue -- low: red (arbitrary if s=0)
            h(1) := 0.0;	-- hue -- high: red (arbitrary if s=0)

            l(0) := 0.5;	-- lightness -- low: half-dark
            l(1) := 1.0;	-- lightness -- high: light

            s(0) := 0.0;	-- minimum saturation
            s(1) := 0.0;	-- minimum saturation
         else
            h(0) := 240.0;  -- blue -> green -> yellow ->
            h(1) := 0.0;    -- -> red

            l(0) := 0.6;
            l(1) := 0.6;

            s(0) := 0.8;
            s(1) := 0.8;
        end if;

        plscmap1n(256);
        plscmap1l(HLS, i, h, l, s, Reverse_Hue_None);

    end cmap1_init;

begin

    -- Parse and process command line arguments
    plparseopts(PL_PARSE_FULL);
    if sombrero then
        rosen := False;
    end if;

    -- Initialize plplot
    plinit;

    for i in x'range loop
        x(i) := Long_Float(i - XPTS / 2) / Long_Float(XPTS / 2);
        if rosen then
            x(i) :=  x(i) * 1.5;
        end if;
    end loop;

    for i in y'range loop
        y(i) := Long_Float(i - YPTS / 2) / Long_Float(YPTS / 2);
        if rosen then
            y(i) := y(i) + 0.5;
        end if;
    end loop;

    for i in x'range loop
        xx := x(i);
        for j in y'range loop
            yy := y(j);
            if rosen then
                z(i, j) := (1.0 - xx) * (1.0 - xx) + 100.0 * (yy - (xx * xx)) * (yy - (xx * xx));
                -- The log argument may be zero for just the right grid. 
                if z(i, j) > 0.0 then
                    z(i, j) := log(z(i, j));
                else
                    z(i, j) := -5.0; -- -MAXFLOAT would mess-up up the scale
                end if;
            else -- Sombrero is True.
                r := sqrt(xx * xx + yy * yy);
                z(i, j) := exp(-r * r) * cos(2.0 * pi * r);
            end if;
        end loop; -- j
    end loop; -- i

    zmin := Matrix_Min(z);
    zmax := Matrix_Max(z);
    step := (zmax - zmin) / Long_Float(nlevel + 1);
    for i in clevel'range loop
        clevel(i) := zmin + step + step * Long_Float(i);
    end loop;

    pllightsource(1.0, 1.0, 1.0);

    for k in alt'range loop
        for ifshade in 0 .. 3 loop
            pladv(0);
            plvpor(0.0, 1.0, 0.0, 0.9);
            plwind(-1.0, 1.0, -0.9, 1.1);
            plcol0(3);
            plmtex("t", 1.0, 0.5, 0.5, title(k));
            plcol0(1);
            if rosen then
                plw3d(1.0, 1.0, 1.0, -1.5, 1.5, -0.5, 1.5, zmin, zmax, alt(k), az(k));
            else
                plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, zmin, zmax, alt(k), az(k));
            end if;
        
            plbox3("bnstu", "x axis", 0.0, 0,
                "bnstu", "y axis", 0.0, 0,
                "bcdmnstuv", "z axis", 0.0, 0);
            plcol0(2);

            if ifshade = 0 then -- diffuse light surface plot
                cmap1_init(True);
                plsurf3d(x, y, z, 0, clevel); -- clevel is not used here
            elsif ifshade = 1 then -- magnitude colored plot
                cmap1_init(False);
                plsurf3d(x, y, z, MAG_COLOR, clevel);
            elsif ifshade = 2 then --  magnitude colored plot with faceted squares
                cmap1_init(False);
                plsurf3d(x, y, z, MAG_COLOR + FACETED, clevel);
            else                    -- magnitude colored plot with contours
                cmap1_init(False);
                plsurf3d(x, y, z, MAG_COLOR + SURF_CONT + BASE_CONT, clevel);
            end if;
        end loop; -- ifshade
    end loop; -- k

    plend;

end x08a;
