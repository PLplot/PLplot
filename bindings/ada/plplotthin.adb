with Ada.Text_IO;
use  Ada.Text_IO;

package body PLplotThin is

    -- Find minimum in a 1D array.
    function Vector_Min(x : Real_Vector) return Long_Float is
        Result : Long_Float;
    begin
        Result := x(x'First);
        for i in x'First+1..x'Last loop
            Result := Long_Float'min(Result, x(i));
        end loop;
        return Result;
    end Vector_Min;
    

    -- Find minimum and its location in a 1D array.
    procedure Vector_Min(x               : Real_Vector;
                         The_Minimum     : out Long_Float;
                         Location_Of_Min : out Integer) is
    begin
        The_Minimum := x(x'First);
        Location_Of_Min := x'First;
        for i in x'First+1..x'Last loop
            if x(i) < x(i - 1) then -- Finds the first of repeated mins.
                The_Minimum := x(i);
                Location_Of_Min := i;
            end if;
        end loop;
    end Vector_Min;


    -- Find maximum in a 1D array.
    function Vector_Max(x : Real_Vector) return Long_Float  is
        Result : Long_Float;
    begin
        Result := x(x'First);
        for i in x'First+1..x'Last loop
            Result := Long_Float'max(Result, x(i));
        end loop;
        return Result;
    end Vector_Max;
    

    -- Find maximum and its location in a 1D array.
    procedure Vector_Max(x               : Real_Vector;
                         The_Maximum     : out Long_Float;
                         Location_Of_Max : out Integer) is
    begin
        The_Maximum := x(x'First);
        Location_Of_Max := x'First;
        for i in x'First+1..x'Last loop
            if x(i) > x(i - 1) then -- Finds the first of repeated maxes.
                The_Maximum := x(i);
                Location_Of_Max := i;
            end if;
        end loop;
    end Vector_Max;
    
    
    -- Find minimum in a 2D array.
    function Matrix_Min(x : Real_Matrix) return Long_Float is
        Result : Long_Float := Long_Float'large;
    begin
        for j in x'First(2)..x'Last(2) loop
            for i in x'First(1)..x'Last(1) loop
                Result := Long_Float'Min(Result, x(i, j));
            end loop;
        end loop;
        return Result;
    end Matrix_Min;    
    
    
    -- Find maximum in a 2D array.
    function Matrix_Max(x : Real_Matrix) return Long_Float is
        Result : Long_Float := Long_Float'small;
    begin
        for j in x'First(2)..x'Last(2) loop
            for i in x'First(1)..x'Last(1) loop
                Result := Long_Float'Max(Result, x(i, j));
            end loop;
        end loop;
        return Result;
    end Matrix_Max;
    

--------------------------------------------------------------------------------
-- Utility for passing matrices to C                                          --
--------------------------------------------------------------------------------

    -- Take a Real_Matrix as defined in Ada.Numerics.Generic_Real_Arrays 
    -- and its instances and produce a 1D array of access variables to the  
    -- element located at the first column of each row. This is then suitable  
    -- for passing to an external C subroutine which expects a "2D array" in the 
    -- form of an array of pointers to "1D arrays" which in turn are pointers to
    -- the first element of each row in C-land. This currently uses the GNAT 
    -- attribute Unrestricted_Access which makes it non-portable but allows the 
    -- accessing of the matrix elements without aliasing them, which is useful 
    -- because the Ada 2005 vector and matrix types are non-aliased.
    
    -- For more about the Unrestricted_Access attribute, see Implementation 
    -- Defined Attributes in the GNAT Reference Manual.

    -- TO-DO: Write a function which accepts x(Index, Index_Of_First_Column) 
    -- as an argument and returns a "proper" access variable using the method 
    -- discussed in "Ada as a Second Language," Second Edition, by Norman H. 
    -- Cohen, Section 19.3, for portability. This should remove GNAT dependence.

    function Matrix_To_Pointers(x : Real_Matrix) return Long_Float_Pointer_Array is
        Index_Of_First_Column : Integer := x'First(2);
        x_As_Pointers : Long_Float_Pointer_Array (x'range(1));
    begin
        for Index in x'range(1) loop
            x_As_Pointers(Index) := x(Index, Index_Of_First_Column)'Unrestricted_Access;
        end loop;
        return x_As_Pointers;
    end Matrix_To_Pointers;

end PLplotThin;
