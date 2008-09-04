-- $Id$

-- Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

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
    Ada.Numerics.Long_Elementary_Functions,
    PLplot;
use
    Ada.Numerics.Long_Elementary_Functions,
    PLplot;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;
@Ada_Is_2007_With_and_Use_Numerics@

------------------------------------------------------------------------------
-- Generates two kinds of plots:
--   - construction of a cycloid (animated)
--   - series of epitrochoids and hypotrochoids
------------------------------------------------------------------------------

procedure xthick27a is
    -- R, r, p, N
    params : Real_Matrix(0 .. 8, 0 .. 3) := 
      ((21.0,  7.0,  7.0,  3.0),
       (21.0,  7.0, 10.0,  3.0),
       (21.0, -7.0, 10.0,  3.0),
       (20.0,  3.0,  7.0, 20.0),
       (20.0,  3.0, 10.0, 20.0),
       (20.0, -3.0, 10.0, 20.0),
       (20.0, 13.0,  7.0, 20.0),
       (20.0, 13.0, 20.0, 20.0),
       (20.0,-13.0, 20.0, 20.0));

    procedure spiro(params : Real_Matrix; row : Integer) is
        NPNT : constant Integer := 20000;
        xcoord, ycoord : Real_Vector(0 .. 20000);
        windings : Integer;
        steps    : Integer;
        phi      : Long_Float;
        phiw     : Long_Float;
        dphi     : Long_Float;
        xmin     : Long_Float;
        xmax     : Long_Float;
        ymin     : Long_Float;
        ymax     : Long_Float;
        scale    : Long_Float;
    begin
        -- Fill the coordinates
        windings := Integer(params(row, 3));
        steps    := NPNT / windings;
        dphi     := 8.0 * arccos(-1.0) / Long_Float(steps);

        xmin := 0.0; -- This initialisation is safe!
        xmax := 0.0;
        ymin := 0.0;
        ymax := 0.0;

        for i in 0 .. windings * steps loop
            phi       := Long_Float(i) * dphi;
            phiw      := (params(row, 0) - params(row, 1)) / params(row, 1) * phi;
            xcoord(i) := (params(row, 0) - params(row, 1)) * cos(phi) + params(row, 2) * cos(phiw);
            ycoord(i) := (params(row, 0) - params(row, 1)) * sin(phi) - params(row, 2) * sin(phiw);

            if xmin > xcoord(i) then xmin := xcoord(i); end if;
            if xmax < xcoord(i) then xmax := xcoord(i); end if;
            if ymin > ycoord(i) then ymin := ycoord(i); end if;
            if ymax < ycoord(i) then ymax := ycoord(i); end if;
        end loop;

        if xmax - xmin > ymax - ymin then
            scale := xmax - xmin;
        else
            scale := ymax - ymin;
        end if;

        xmin := - 0.65 * scale;
        xmax :=   0.65 * scale;
        ymin := - 0.65 * scale;
        ymax :=   0.65 * scale;

        Set_Viewport_World(xmin, xmax, ymin, ymax);
        Set_Pen_Color(Red);
        declare
            xcoord_local, ycoord_local : Real_Vector(0 .. steps * windings);
        begin
            xcoord_local := xcoord(0 .. steps * windings);
            ycoord_local := ycoord(0 .. steps * windings);
            Draw_Curve(xcoord_local, ycoord_local);
        end;
    end spiro;


    procedure cycloid is
    begin
        null; -- TODO
    end cycloid;

begin
    -- Parse and process command line arguments
    Parse_Command_Line_Arguments(Parse_Full);

    -- Initialize plplot
    Initialize_PLplot;

    -- Illustrate the construction of a cycloid
    cycloid;

    -- Loop over the various curves.
    -- First an overview, then all curves one by one
    Set_Number_Of_Subpages(3, 3) ; -- Three by three window

    for i in params'range(1) loop
        Advance_To_Subpage(Next_Subpage);
        Set_Viewport_Normalized( 0.0, 1.0, 0.0, 1.0 );
        spiro(params, i);
    end loop;

    Advance_To_Subpage(Next_Subpage) ;
    Set_Number_Of_Subpages(1, 1) ; -- One window per curve

    for i in params'range(1) loop
        Advance_To_Subpage(Next_Subpage) ;
        Set_Viewport_Normalized( 0.0, 1.0, 0.0, 1.0 ) ;
        spiro(params, i);
    end loop;

    -- Don't forget to call End_PLplot to finish off!
    End_PLplot;
end xthick27a;
