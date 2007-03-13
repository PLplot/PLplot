-- $Id$

-- Simple line plot and multiple windows demo.

-- Copyright (C) 2007  Werner Smekal

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
    Interfaces.C,
    Ada.Text_IO,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Strings.Bounded,
    PLplot,
    PlplotThin;
use
    Interfaces.C,
    Ada.Text_IO,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    Ada.Strings.Bounded,
    PLplot,
    PlplotThin;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;

procedure x12a is
    string : char_array(0 .. 20);
    y0 : PL_Float_Array (0 .. 9);
    procedure Sprintf( buffer : out char_array; format : in char_array; variable: in PLFLT );
    pragma Import(C, Sprintf, "sprintf" );    

    procedure plfbox(x0 : PLFLT; y0 : PLFLT) is
        x, y : PL_Float_Array (0 .. 3);
    begin
			  x(0) := x0;
			  y(0) := 0.0;
        x(1) := x0;
        y(1) := y0;
        x(2) := x0 + 1.0;
        y(2) := y0;
        x(3) := x0 + 1.0;
        y(3) := 0.0;
        plfill(4, x, y);
        plcol0(1);
        pllsty(1);
        plline(4, x, y);
		end;

begin
			-- plplot initialization
			
			-- plparseopts(PL_PARSE_FULL);
			Parse_Command_Line_Arguments(PL_PARSE_FULL);
	
			plinit;
	
			pladv(0);
			plvsta;
			plwind(1980.0, 1990.0, 0.0, 35.0);
			plbox("bc" & Nul, 1.0, 0, "bcnv" & Nul, 10.0, 0);
			plcol0(2);
			pllab("Year" & Nul, "Widget Sales (millions)" & Nul, "#frPLplot Example 12" & Nul);
	
			y0(0) := 5.0;
			y0(1) := 15.0;
			y0(2) := 12.0;
			y0(3) := 24.0;
			y0(4) := 28.0;
			y0(5) := 30.0;
			y0(6) := 20.0;
			y0(7) := 8.0;
			y0(8) := 12.0;
			y0(9) := 3.0;

			for i in y0'Range loop
          plcol0(i + 1);
          plpsty(0);
          plfbox(Long_Float(1980 + i), y0(i));
          sprintf(string, "%.0f" & Nul, y0(i));
          plptex(Long_Float(1980 + i) + 0.5, (y0(i) + 1.0), 1.0, 0.0, 0.5, string);
          sprintf(string, "%.0f" & Nul, Long_Float(1980 + i));
          plmtex("b" & Nul, 1.0, (Long_Float(i + 1) * 0.1 - 0.05), 0.5, string);
			end loop;

			-- Don't forget to call plend() to finish off!
	    plend;
	end x12a;
