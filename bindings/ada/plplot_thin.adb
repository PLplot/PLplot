with
    PLplot_Auxiliary;
use
    PLplot_Auxiliary;

package body PLplot_Thin is

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


--------------------------------------------------------------------------------
--         Functions for use from C or C++ only                               --
--         (Not really ;).                                                    --
--------------------------------------------------------------------------------
-- THESE FUNCTIONS ^^^ ARE NOT IMPLEMENTED FOR THE ADA BINDING
-- EXCEPT FOR THE FOLLOWING.

    -- plparseopts here is an exact copy (exept for the name) of 
    -- Parse_Command_Line_Arguments in the thick binding. The reason for
    -- departing from the usual method of simply pragma Import-ing as in
    -- most or all of the other interfaces to C is because of the need to 
    -- figure out what the command lines arguments are by also pragma 
    -- Import-ing Gnat_Argc and Gnat_Argv. A single-argment version is made 
    -- at the request of the development team rather than the three-argument 
    -- version of the documetation. The caller specifies only the parse mode.
    
    -- Process options list using current options info.
    procedure plparseopts(Mode : Parse_Mode_Type) is

        Gnat_Argc : aliased Integer;
        pragma Import (C, Gnat_Argc);

        Gnat_Argv : System.Address;
        pragma Import (C, Gnat_Argv);

        type Gnat_Argc_Access_Type is access all Integer;
        Gnat_Argc_Access : Gnat_Argc_Access_Type;

        procedure
        plparseopts_local(argc : Gnat_Argc_Access_Type;
                          argv : System.Address;
                          mode : Parse_Mode_Type);
        pragma Import(C, plparseopts_local, "c_plparseopts");

    begin
        Gnat_Argc_Access := Gnat_Argc'access;
        plparseopts_local(Gnat_Argc_Access, Gnat_Argv, Mode);
    end plparseopts;


    -- This is a three-argument version of plparseopts as indicated in the
    -- documentation.

    -- Process options list using current options info.
    procedure plparseopts
       (Gnat_Argc : Integer;
        Gnat_Argv : System.Address;
        Mode      : Parse_Mode_Type) is

        Gnat_Argc_Dummy : aliased Integer;
        
        type Gnat_Argc_Access_Type is access all Integer;
        Gnat_Argc_Access : Gnat_Argc_Access_Type;

        procedure
        plparseopts_local(argc : Gnat_Argc_Access_Type;
                          argv : System.Address;
                          mode : Parse_Mode_Type);
        pragma Import(C, plparseopts_local, "c_plparseopts");

    begin
        Gnat_Argc_Dummy := Gnat_Argc;
        Gnat_Argc_Access := Gnat_Argc_Dummy'access;
        plparseopts_local(Gnat_Argc_Access, Gnat_Argv, Mode);
    end plparseopts;

end PLplot_Thin;
