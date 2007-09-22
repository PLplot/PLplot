-- $Id$

-- Auxiliary types and subprograms to be with-ed and by all the Ada 
-- bindings to PLplot

-- Copyright (C) 2006-2007  Jerry Bauck

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
    Ada.Strings.Bounded,
    Ada.Strings.Unbounded;

use 
    Ada.Strings.Bounded,
    Ada.Strings.Unbounded;

package PLplot_Auxiliary is

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;

--------------------------------------------------------------------------------
--           Utility type declarations used by the bindings                   --
--------------------------------------------------------------------------------

    -- UN-COMMENT THE NEXT TWO LINES IF YOUR COMPILER DOES NOT INCLUDE THESE 
    -- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.

    -- COMMENT THESE TWO LINES IF YOUR COMPILER DECLARES SIMILAR TYPES (E.G. IT 
    -- IS ADA 2005 WITH ANNEX G.3)
    -- ALSO, YOU WILL NEED TO MAKE A SINGLE-LINE EDIT TO THESE BINDING FILES: 
    -- plplot.ads, plplot.adb, plplot_traditional.ads, plplot_traditional.adb, 
    -- and plplot_thin.ads.
    type Real_Vector is array (Integer range <>) of Long_Float;
    type Real_Matrix is array (Integer range <>, Integer range <>) of Long_Float;

    -- Other array types for the bindings.
    type Boolean_Array_1D is array (Integer range <>) of Boolean;
    type Integer_Array_1D is array (Integer range <>) of Integer;


--------------------------------------------------------------------------------
--            Utility procedures useful in compiling the examples             --
--------------------------------------------------------------------------------

    -- Find minimum in a 1D array.
    function Vector_Min(x : Real_Vector) return Long_Float;

    -- Find minimum and its location in a 1D array.
    procedure Vector_Min(x               : Real_Vector;
                         The_Minimum     : out Long_Float;
                         Location_Of_Min : out Integer);

    -- Find maximum in a 1D array.
    function Vector_Max(x : Real_Vector) return Long_Float;

    -- Find maximum and its location in a 1D array.
    procedure Vector_Max(x               : Real_Vector;
                         The_Maximum     : out Long_Float;
                         Location_Of_Max : out Integer);
    
    -- Find minimum in a 2D array.
    function Matrix_Min(x : Real_Matrix) return Long_Float;
    
    -- Find maximum in a 2D array.
    function Matrix_Max(x : Real_Matrix) return Long_Float;


--------------------------------------------------------------------------------
--             String things useful in compiling the examples                 --
--------------------------------------------------------------------------------

    -- Convenient string of length 80
    package Bounded_String_80 is
        new Ada.Strings.Bounded.Generic_Bounded_Length(80);
    subtype String_80 is Bounded_String_80.Bounded_String;

end PLplot_Auxiliary;