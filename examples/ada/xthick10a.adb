-- $Id: x10a.adb 7703 2007-06-02 02:37:18Z airwin $

-- 	Window positioning demo

-- Copyright (C) 2007 Alan W. Irwin

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
    Ada.Text_IO,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot;
use
    Ada.Text_IO,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;

procedure xthick10a is
begin   
    -- Parse and process command line arguments    
    Parse_Command_Line_Arguments(Parse_Full);
    
    -- Initialize plplot
    Initialize_PLplot;
    
    Advance_To_Subpage(Next_Subpage);
    Set_Viewport_Normalized(0.0, 1.0, 0.0, 1.0);
    Set_Viewport_World(0.0, 1.0, 0.0, 1.0);
    Box_Around_Viewport("bc", 0.0, 0, "bc", 0.0, 0);
    
    Set_Viewport_Absolute(50.0, 150.0, 50.0, 100.0);
    Set_Viewport_World(0.0, 1.0, 0.0, 1.0);
    Box_Around_Viewport("bc", 0.0, 0, "bc", 0.0, 0);
    Write_Text_World(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,50,100)");

    End_PLplot;
end xthick10a;
