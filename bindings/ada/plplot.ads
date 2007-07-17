with
    PLplot_Thin,
    PLplot_Auxiliary,
    System,
    Interfaces.C.Pointers,
    Ada.Strings.Unbounded,
    Ada.Strings.Maps;
use
    PLplot_Thin,
    PLplot_Auxiliary,
    Ada.Strings.Unbounded;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;

package PLplot is

--------------------------------------------------------------------------------
--        Types and constants for thick binding                               --
--------------------------------------------------------------------------------

    -- Default dummy arrays for various plotters which take multiple inputs.
    subtype Length_One_Real_Vector is Real_Vector(1..1);
    Dont_Plot_This : Length_One_Real_Vector := (1..1 => 0.0);
    
    -- Default colors for Color Map 0.
    -- These are hard-wired to the current colors of color map 0; if that 
    -- color map has been changed, then these colors will return surprising 
    -- results. Color map 0 can always be restored to its default state using 
    -- Restore_Default_Snapshot_Of_Color_Map_0.
    subtype Plot_Color_Type is Integer;
    Black      : constant Plot_Color_Type := 0;
    Red        : constant Plot_Color_Type := 1;
    Yellow     : constant Plot_Color_Type := 2;
    Green      : constant Plot_Color_Type := 3;
    Aquamarine : constant Plot_Color_Type := 4;
    Pink       : constant Plot_Color_Type := 5;
    Wheat      : constant Plot_Color_Type := 6;
    Grey       : constant Plot_Color_Type := 7;
    Brown      : constant Plot_Color_Type := 8;
    Blue       : constant Plot_Color_Type := 9;
    BlueViolet : constant Plot_Color_Type := 10;
    Cyan       : constant Plot_Color_Type := 11;
    Turquoise  : constant Plot_Color_Type := 12;
    Magenta    : constant Plot_Color_Type := 13;
    Salmon     : constant Plot_Color_Type := 14;
    White      : constant Plot_Color_Type := 15;


    Max_Lines_For_Multiplot : constant := 5;
    
    -- Pre-defined line styles, e.g., continuous, dashed, etc.
    subtype Line_Style_Type is Integer range 1..8;

    -- Arrays for various line appearances.
    type Color_Array_Type      is array (1..Max_Lines_For_Multiplot) of Plot_Color_Type;
    Default_Color_Array : Color_Array_Type := (Red, Blue, Green, Salmon, BlueViolet);

    type Line_Width_Array_Type is array (1..Max_Lines_For_Multiplot) of Integer;
    Default_Line_Width_Array : Line_Width_Array_Type := (1, 1, 1, 1, 1);

    type Line_Style_Array_Type is array (1..Max_Lines_For_Multiplot) of Line_Style_Type;
    Default_Line_Style_Array : Line_Style_Array_Type := (1, 1, 1, 1, 1);
    
    -- Arrays of label strings
    Default_Label_String : constant Unbounded_String := To_Unbounded_String("");
    type Label_String_Array_Type is array (1..Max_Lines_For_Multiplot) of Unbounded_String;
    Default_Label_String_Array : Label_String_Array_Type := (others => Default_Label_String);
    type Stripchart_Label_String_Array_Type is array (1..4) of Unbounded_String;

    -- Justification for plots
    subtype Justification_Type is Integer range -1..2;
    User_Justified       : constant Justification_Type := -1;
    Not_Justified        : constant Justification_Type := 0;
    Justified            : constant Justification_Type := 1;
    Justified_Square_Box : constant Justification_Type := 2;

    -- Axis styles
    subtype Axis_Style_Type is Integer range -2..33; -- Not all values are used.

    No_Box            : constant Axis_Style_Type := -2;
    Box               : constant Axis_Style_Type := -1;

    Linear_Box_Plus   : constant Axis_Style_Type := 0;
    Linear_Zero_Axes  : constant Axis_Style_Type := 1;
    Linear_Major_Grid : constant Axis_Style_Type := 2;
    Linear_Minor_Grid : constant Axis_Style_Type := 3;

    Log_X_Box_Plus    : constant Axis_Style_Type := 10;
    Log_X_Zero_Axes   : constant Axis_Style_Type := 11;
    Log_X_Major_Grid  : constant Axis_Style_Type := 12;
    Log_X_Minor_Grid  : constant Axis_Style_Type := 13;
    
    Log_Y_Box_Plus    : constant Axis_Style_Type := 20;
    Log_Y_Zero_Axes   : constant Axis_Style_Type := 21;
    Log_Y_Major_Grid  : constant Axis_Style_Type := 22;
    Log_Y_Minor_Grid  : constant Axis_Style_Type := 23;
    
    Log_XY_Box_Plus   : constant Axis_Style_Type := 30;
    Log_XY_Zero_Axes  : constant Axis_Style_Type := 31;
    Log_XY_Major_Grid : constant Axis_Style_Type := 32;
    Log_XY_Minor_Grid : constant Axis_Style_Type := 33;

    -- Integer constrained to 0..255
    subtype Integer_0_255_Type is Integer range 0 .. 255;
    type Integer_0_255_Array is array (Integer range <>) of Integer_0_255_Type;

    -- Long_Float constrained to 0.0 .. 1.0.
    subtype Long_Float_0_1_Type is Long_Float range 0.0 .. 1.0;
    
    -- Escape characters for text strings
    Escape_Character_Set : Ada.Strings.Maps.Character_Set;
    
    -- Generic font styles
    subtype Font_Style_Type is Integer range 1..4;
    Normal_Font : constant Font_Style_Type := 1;
    Roman_Font  : constant Font_Style_Type := 2;
    Italic_Font : constant Font_Style_Type := 3;
    Script_Font : constant Font_Style_Type := 4;
    -- Bold_Font?
    
    -- Character sets
    subtype Character_Set_Type is Integer range 0..1;
    Standard_Character_Set : constant Character_Set_Type := 0;
    Extended_Character_Set : constant Character_Set_Type := 1;
    
    -- Plot orientation
    type Orientation_Type is (Landscape, Portrait);
    
    -- Constant for several procedures which don't change a parameter if used
    Unchanged : constant Integer := 0;
    
    -- Pre-defined fill patterns
    subtype Fill_Pattern_Type is Integer range 0..7; -- Guessing; not documented
    
    -- Modes for parsing command line arguments.
    Parse_Partial    : constant Parse_Mode_Type := 0;   -- For backward compatibility
    Parse_Full       : constant Parse_Mode_Type := 1;   -- Process fully & exit if error
    Parse_Quiet      : constant Parse_Mode_Type := 2;   -- Don't issue messages
    Parse_No_Delete  : constant Parse_Mode_Type := 4;   -- Don't delete options after processing
    Parse_Show_All   : constant Parse_Mode_Type := 8;   -- Show invisible options
    Parse_Override   : constant Parse_Mode_Type := 16;  -- Obsolete
    Parse_No_Program : constant Parse_Mode_Type := 32;  -- Program name NOT in *argv[0]..
    Parse_No_Dash    : constant Parse_Mode_Type := 64;  -- Set if leading dash NOT required
    Parse_Skip       : constant Parse_Mode_Type := 128; -- Skip over unrecognized args

    -- Descriptions of map outlines for continents, countries, and US states.
    type Map_Type is (Continents, USA_and_States, Continents_and_Countries, USA_States_and_Continents);

    -- definitions for the opt argument in plot3dc() and plsurf3d()
    -- DRAW_LINEX *must* be 1 and DRAW_LINEY *must* be 2, because of legacy code!
    No_3D_Options             : constant Integer := 0;   -- None of the options
    Lines_Parallel_To_X       : constant Integer := 1;   -- draw lines parallel to the X axis
    Lines_Parallel_To_Y       : constant Integer := 2;   -- draw lines parallel to the Y axis
    Lines_Parallel_To_X_And_Y : constant Integer := 3;   -- draw lines parallel to both the X and Y axis
    Magnitude_Color           : constant Integer := 4;   -- draw the mesh with a color dependent of the magnitude
    Base_Contour              : constant Integer := 8;   -- draw contour plot at bottom xy plane
    Top_Contour               : constant Integer := 16;  -- draw contour plot at top xy plane
    Surface_Contour           : constant Integer := 32;  -- draw contour plot at surface
    Sides                     : constant Integer := 64;  -- draw sides
    Facets                    : constant Integer := 128; -- draw outline for each square that makes up the surface
    Meshed                    : constant Integer := 256; -- draw mesh

    subtype Gridding_Algorithm_Type is Integer range 1..6;

    -- Type of gridding algorithm for plgriddata()
    -- "Long form" gridding algorithm names
    Grid_Bivariate_Cubic_Spline_Approximation               : constant Gridding_Algorithm_Type := 1; -- GRID_CSA
    Grid_Delaunay_Triangulation_Linear_Interpolation        : constant Gridding_Algorithm_Type := 2; -- GRID_DTLI
    Grid_Natural_Neighbors_Interpolation                    : constant Gridding_Algorithm_Type := 3; -- GRID_NNI
    Grid_Nearest_Neighbors_Inverse_Distance_Weighted        : constant Gridding_Algorithm_Type := 4; -- GRID_NNIDW
    Grid_Nearest_Neighbors_Linear_Interpolation             : constant Gridding_Algorithm_Type := 5; -- GRID_NNLI
    Grid_Nearest_Neighbors_Around_Inverse_Distance_Weighted : constant Gridding_Algorithm_Type := 6; -- GRID_NNAIDW


--------------------------------------------------------------------------------
-- Constants copied from PLplot_Thin.ads so that it doesn't have to be seen.  --
-- These are replicated herein with other names. Either name may be used,     --
-- e.g., Parse_Full is the same as PL_PARSE_FULL.    --
--------------------------------------------------------------------------------

    -- Modes for parsing command line arguments (redux).
    PL_PARSE_PARTIAL   : constant Parse_Mode_Type := 16#0000#; -- For backward compatibility
    PL_PARSE_FULL      : constant Parse_Mode_Type := 16#0001#; -- Process fully & exit if error
    PL_PARSE_QUIET     : constant Parse_Mode_Type := 16#0002#; -- Don't issue messages
    PL_PARSE_NODELETE  : constant Parse_Mode_Type := 16#0004#; -- Don't delete options after processing
    PL_PARSE_SHOWALL   : constant Parse_Mode_Type := 16#0008#; -- Show invisible options
    PL_PARSE_OVERRIDE  : constant Parse_Mode_Type := 16#0010#; -- Obsolete
    PL_PARSE_NOPROGRAM : constant Parse_Mode_Type := 16#0020#; -- Program name NOT in *argv[0]..
    PL_PARSE_NODASH    : constant Parse_Mode_Type := 16#0040#; -- Set if leading dash NOT required
    PL_PARSE_SKIP      : constant Parse_Mode_Type := 16#0080#; -- Skip over unrecognized args

    -- definitions for the opt argument in plot3dc() and plsurf3d()
    DRAW_LINEX  : constant Integer := 1;   -- draw lines parallel to the X axis
    DRAW_LINEY  : constant Integer := 2;   -- draw lines parallel to the Y axis
    DRAW_LINEXY : constant Integer := 3;   -- draw lines parallel to both the X and Y axis
    MAG_COLOR   : constant Integer := 4;   -- draw the mesh with a color dependent of the magnitude
    BASE_CONT   : constant Integer := 8;   -- draw contour plot at bottom xy plane
    TOP_CONT    : constant Integer := 16;  -- draw contour plot at top xy plane
    SURF_CONT   : constant Integer := 32;  -- draw contour plot at surface
    DRAW_SIDES  : constant Integer := 64;  -- draw sides
    FACETED     : constant Integer := 128; -- draw outline for each square that makes up the surface
    MESH        : constant Integer := 256; -- draw mesh

    -- Type of gridding algorithm for plgriddata()
    GRID_CSA    : constant Gridding_Algorithm_Type := 1;
    GRID_DTLI   : constant Gridding_Algorithm_Type := 2;
    GRID_NNI    : constant Gridding_Algorithm_Type := 3;
    GRID_NNIDW  : constant Gridding_Algorithm_Type := 4;
    GRID_NNLI   : constant Gridding_Algorithm_Type := 5;
    GRID_NNAIDW : constant Gridding_Algorithm_Type := 6;


--------------------------------------------------------------------------------
-- A convenient string function                                               --
--------------------------------------------------------------------------------

    -- Short name for To_Unbounded_String
    function TUB(arg : String) return Ada.Strings.Unbounded.Unbounded_String renames Ada.Strings.Unbounded.To_Unbounded_String;


--------------------------------------------------------------------------------
--        High-Level subroutines for thick binding                            --
--------------------------------------------------------------------------------

    -- When asked to draw white lines on black background, do it.
    -- This is the default.
    procedure Draw_White_On_Black;
    
    
    -- When asked to draw black lines on white background, reverse black and white.
    -- This might look better on anti-aliased displays.
    procedure Draw_Black_On_White;


    -- Plotter for up to five x-y pairs and settable axis style, plot 
    -- line colors, widths, and styles, justification, zoom, and labels.
    -- Can be used directly or as part of a "simple" plotter 
    -- such as those that follow or are made by the user.
    procedure Multiplot_Pairs
       (x1            : Real_Vector     := Dont_Plot_This;
        y1            : Real_Vector     := Dont_Plot_This;
        x2            : Real_Vector     := Dont_Plot_This;
        y2            : Real_Vector     := Dont_Plot_This;
        x3            : Real_Vector     := Dont_Plot_This;
        y3            : Real_Vector     := Dont_Plot_This;
        x4            : Real_Vector     := Dont_Plot_This;
        y4            : Real_Vector     := Dont_Plot_This;
        x5            : Real_Vector     := Dont_Plot_This;
        y5            : Real_Vector     := Dont_Plot_This;
        X_Labels      : Label_String_Array_Type := Default_Label_String_Array;
        Y_Labels      : Label_String_Array_Type := Default_Label_String_Array;
        Title_Labels  : Label_String_Array_Type := Default_Label_String_Array;
        Axis_Style    : Axis_Style_Type         := Linear_Box_Plus;
        Colors        : Color_Array_Type        := Default_Color_Array;
        Line_Widths   : Line_Width_Array_Type   := Default_Line_Width_Array;
        Line_Styles   : Line_Style_Array_Type   := Default_Line_Style_Array;
        Justification : Justification_Type      := Not_Justified;
        x_Min_Zoom    : Long_Float              := Long_Float'small;
        x_Max_Zoom    : Long_Float              := Long_Float'large;
        y_Min_Zoom    : Long_Float              := Long_Float'small;
        y_Max_Zoom    : Long_Float              := Long_Float'large);


--------- Simple plotters requiring minimal arguments -----
    
    -- fix this Add x, y, title labels; make default " " or see predefined default.
    -- Simple plotter for single x array and multiple y arrays
    procedure Simple_Plot 
       (x  : Real_Vector;
        y1 : Real_Vector := Dont_Plot_This;
        y2 : Real_Vector := Dont_Plot_This;
        y3 : Real_Vector := Dont_Plot_This;
        y4 : Real_Vector := Dont_Plot_This;
        y5 : Real_Vector := Dont_Plot_This;
        X_Label     : String := To_String(Default_Label_String);
        Y_Label     : String := To_String(Default_Label_String);
        Title_Label : String := To_String(Default_Label_String));
    
    
    -- Simple log x plotter for single x array and multiple y arrays
    procedure Simple_Plot_Log_X
       (x  : Real_Vector;
        y1 : Real_Vector := Dont_Plot_This;
        y2 : Real_Vector := Dont_Plot_This;
        y3 : Real_Vector := Dont_Plot_This;
        y4 : Real_Vector := Dont_Plot_This;
        y5 : Real_Vector := Dont_Plot_This;
        X_Label     : String := To_String(Default_Label_String);
        Y_Label     : String := To_String(Default_Label_String);
        Title_Label : String := To_String(Default_Label_String));
    
    
    -- Simple log y plotter for multiple x arrays and single y array
    procedure Simple_Plot_Log_Y
       (x1 : Real_Vector := Dont_Plot_This;
        y  : Real_Vector := Dont_Plot_This; -- Beware of argument order.
        x2 : Real_Vector := Dont_Plot_This;
        x3 : Real_Vector := Dont_Plot_This;
        x4 : Real_Vector := Dont_Plot_This;
        x5 : Real_Vector := Dont_Plot_This;
        X_Label     : String := To_String(Default_Label_String);
        Y_Label     : String := To_String(Default_Label_String);
        Title_Label : String := To_String(Default_Label_String));
    
    
    -- Simple log x - log y plotter
    procedure Simple_Plot_Log_XY(x, y : Real_Vector;
        X_Label     : String := To_String(Default_Label_String);
        Y_Label     : String := To_String(Default_Label_String);
        Title_Label : String := To_String(Default_Label_String));


    -- Simple plotter for multiple x-y arrays specified pairwise.
    procedure Simple_Plot_Pairs
       (x1 : Real_Vector := Dont_Plot_This;
        y1 : Real_Vector := Dont_Plot_This;
        x2 : Real_Vector := Dont_Plot_This;
        y2 : Real_Vector := Dont_Plot_This;
        x3 : Real_Vector := Dont_Plot_This;
        y3 : Real_Vector := Dont_Plot_This;
        x4 : Real_Vector := Dont_Plot_This;
        y4 : Real_Vector := Dont_Plot_This;
        x5 : Real_Vector := Dont_Plot_This;
        y5 : Real_Vector := Dont_Plot_This;
        X_Label     : String := To_String(Default_Label_String);
        Y_Label     : String := To_String(Default_Label_String);
        Title_Label : String := To_String(Default_Label_String));


--------- Plotter requiring somewhat more arguments ------

    -- Single plotter with flexible attributes
    -- Similar to Multiplot_Pairs except single trace and no attribute arrays.
    procedure Single_Plot
       (x, y          : Real_Vector;
        X_Label       : String             := To_String(Default_Label_String);
        Y_Label       : String             := To_String(Default_Label_String);
        Title_Label   : String             := To_String(Default_Label_String);
        Axis_Style    : Axis_Style_Type    := Linear_Major_Grid;
        Color         : Plot_Color_Type    := Red;
        Line_Width    : Integer            := 1;
        Line_Style    : Line_Style_Type    := 1;
        Justification : Justification_Type := Not_Justified;
        x_Min_Zoom    : Long_Float         := Long_Float'small;
        x_Max_Zoom    : Long_Float         := Long_Float'large;
        y_Min_Zoom    : Long_Float         := Long_Float'small;
        y_Max_Zoom    : Long_Float         := Long_Float'large);


--------- Simple Contour Plotter ------

    procedure Simple_Contour
       (z             : Real_Matrix;
        Number_Levels : Integer := 10;
        X_Label       : String  := To_String(Default_Label_String);
        Y_Label       : String  := To_String(Default_Label_String);
        Title_Label   : String  := To_String(Default_Label_String));


--------- Simple 3D Mesh Plotter ------

    procedure Simple_Mesh_3D
       (x, y     : Real_Vector; -- data definition points 
        z        : Real_Matrix; -- z(x, y) = z(x(i), y(j))
        x_Min    : Long_Float := 0.0; -- user coordinate limits
        x_Max    : Long_Float := 0.0;
        y_Min    : Long_Float := 0.0;
        y_Max    : Long_Float := 0.0;
        Altitude : Long_Float := 30.0; -- viewing elevation angle in degrees
        Azimuth  : Long_Float := 30.0; -- viewing azimuth in degrees
        X_Label  : String := "x";
        Y_Label  : String := "y";
        Z_Label  : String := "z");


--------- Simple 3D Surface Plotter ------

    procedure Simple_Surface_3D
       (x, y     : Real_Vector; -- data definition points 
        z        : Real_Matrix; -- z(x, y) = z(x(i), y(j))
        x_Min    : Long_Float := 0.0; -- user coordinate limits
        x_Max    : Long_Float := 0.0;
        y_Min    : Long_Float := 0.0;
        y_Max    : Long_Float := 0.0;
        Altitude : Long_Float := 30.0; -- viewing elevation angle in degrees
        Azimuth  : Long_Float := 30.0; -- viewing azimuth in degrees
        X_Label  : String := "x";
        Y_Label  : String := "y";
        Z_Label  : String := "z");


--------- Simple color table manipulatons -----

    -- Things for manipulating color map 0 --

    -- Current default number of colors provided by PLplot. There is no way to 
    -- get this number under program control. The actual number can be set by 
    -- the user with Set_Number_Of_Colors_Map_0.
    Number_Of_Default_Colors : constant Integer := 16;

    -- The default color map 0 is captured at initialization of PLplot.adb with 
    -- a call to Make_Snapshot_Of_Color_Map_0 stored here.
    Default_Red_Components   : Integer_Array_1D(0 .. Number_Of_Default_Colors - 1);
    Default_Green_Components : Integer_Array_1D(0 .. Number_Of_Default_Colors - 1);
    Default_Blue_Components  : Integer_Array_1D(0 .. Number_Of_Default_Colors - 1);


    -- Make a snapshot of color map 0 for possible later full or partial restoration.
    -- This is automatically called at package initialization with results stored
    -- in Default_Red_Components, Default_Green_Components, Default_Blue_Components.
    procedure Make_Snapshot_Of_Color_Map_0 
       (Reds, Greens, Blues : out Integer_Array_1D);

 
    -- Restore an arbitray snapshot of color map 0.
    procedure Restore_Snapshot_Of_Color_Map_0
       (Reds, Greens, Blues : Integer_Array_1D);


    -- Restore the default colors of color map 0 taken as a snapshot at initialization.
    procedure Restore_Default_Snapshot_Of_Color_Map_0;


    -- Functions which correspond to the default colors of color map 0. Calling
    -- one of these (1) resets the corresponding slot in color map 0 to its 
    -- default value, and (2) returns the correct integer value for the default 
    -- color specified. Thus, using Set_Pen_Color(Reset_Red) instead of 
    -- Set_Pen_Color(Red) guarantees that the color will be set to Red even if 
    -- there have been prior manipulations of color 1.
    
    function Reset_Black return Integer;
    
    function Reset_Red return Integer;
    
    function Reset_Yellow return Integer;
    
    function Reset_Green return Integer;
    
    function Reset_Aquamarine return Integer;
    
    function Reset_Pink return Integer;
    
    function Reset_Wheat return Integer;
    
    function Reset_Grey return Integer;
    
    function Reset_Brown return Integer;
    
    function Reset_Blue return Integer;
    
    function Reset_BlueViolet return Integer;
    
    function Reset_Cyan return Integer;
    
    function Reset_Turquoise return Integer;
    
    function Reset_Magenta return Integer;
    
    function Reset_Salmon return Integer;
    
    function Reset_White return Integer;


    -- Things for manipulating color map 1 --

    type Color_Themes_For_Map_1_Type is (Gray, Blue_Green_Red, Red_Green_Blue, 
        Red_Cyan_Blue, Blue_Black_Red, Red_Blue_Green, Red_Yellow);


    -- Quick application of pre-fabricated color schemes to color map 1.
    procedure Quick_Set_Color_Map_1(Color_Theme : Color_Themes_For_Map_1_Type);


--------------------------------------------------------------------------------
--        Auxiliary things                                                    --
--------------------------------------------------------------------------------

    -- This is a mask function for Shade_Regions (aka plshades) et al that always
    -- returns 1 so that all points are plotted. Can be used as a template 
    -- for other user-written mask functions. This should be the same as 
    -- passing null for the second argument in Shade_Regions.
    function Mask_Function_No_Mask(x, y : Long_Float) return Integer;


    -- Given an array to hold contour levels and function minimum and maximum,
    -- fill it and return. Useful for contour and shade plots.
    procedure Calculate_Contour_Levels
       (Contour_Levels : in out Real_Vector;
        z_Min, z_Max : Long_Float);


--------------------------------------------------------------------------------
--        Re-define PLplot procedures using Ada style.                        --
--------------------------------------------------------------------------------

-- These correspond to the section in plot.h called "Function Prototypes".

    -- set the format of the contour labels
    -- pl_setcontlabelformat
    procedure Set_Contour_Label_Format
       (Limit_Exponent     : Integer := 4;
        Significant_Digits : Integer := 2);


    -- set offset and spacing of contour labels
    -- pl_setcontlabelparam
    procedure Set_Contour_Label_Parameters
       (Label_Offset       : Long_Float := 0.006; -- Units are ???
        Label_Font_Height  : Long_Float := 0.3;   -- Units are ???
        Label_Spacing      : Long_Float := 0.1;  -- Units are???
        Labels_Active      : Boolean := False);


    Next_Subpage : constant Integer := 0;

    -- Advance to subpage "page", or to the next one if "page" = 0.
    -- pladv
    procedure Advance_To_Subpage(Page : Natural);


    -- simple arrow plotter.
    -- plarrows
    procedure Arrow_Plotter;


    -- Draw a 2D vector plot.
    -- plvect
    procedure Vector_Plot
       (u, v                             : Real_Matrix;
        Scale                            : Long_Float;
        Transformation_Procedure_Pointer : Transformation_Procedure_Pointer_Type;
        Transformation_Data              : Transformation_Data_Type);

    -- Set the style for the arrow used by plvect to plot vectors.
    -- plsvect
    procedure Set_Arrow_Style_For_Vector_Plots
       (X_Vertices, Y_Vertices : Real_Vector;
        Fill_Arrow             : Boolean);
    

    -- This functions similarly to plbox() except that the origin of the axes
    -- is placed at the user-specified point (x0, y0).
    -- plaxes
    procedure Box_Around_Viewport_With_Origin
       (X_Origin, Y_Origin       : Long_Float;
        X_Option_String          : String;
        X_Major_Tick_Interval    : Long_Float;
        X_Number_Of_Subintervals : Natural;
        Y_Option_String          : String;
        Y_Major_Tick_Interval    : Long_Float;
        Y_Number_Of_Subintervals : Natural);


    -- Plot a histogram using x to store data values and y to store frequencies
    -- plbin
    procedure Histogram_Binned
       (Bin_Values     : Real_Vector; -- "x"
        Bin_Counts     : Real_Vector; -- "y"
        Options        : Integer); -- Options are not defined in plplot.h.


    -- Start new page. Should only be used with pleop().
    -- plbop
    procedure Begin_New_Page;


    -- This draws a box around the current viewport.
    -- plbox
    procedure Box_Around_Viewport
       (X_Option_String          : String;
        X_Major_Tick_Interval    : Long_Float;
        X_Number_Of_Subintervals : Natural := 0;
        Y_Option_String          : String;
        Y_Major_Tick_Interval    : Long_Float;
        Y_Number_Of_Subintervals : Natural);

    Auto_Subintervals : Integer := 0; -- Intervals for minor ticks is automatic.


    -- This is the 3-d analogue of plbox().
    -- plbox3
    procedure Box_Around_Viewport_3D
       (X_Option_String          : String;
        X_Label                  : String := To_String(Default_Label_String);
        X_Major_Tick_Interval    : Long_Float := 0.0;
        X_Number_Of_Subintervals : Natural := 0;

        Y_Option_String          : String;
        Y_Label                  : String := To_String(Default_Label_String);
        Y_Major_Tick_Interval    : Long_Float := 0.0;
        Y_Number_Of_Subintervals : Natural := 0;

        Z_Option_String          : String;
        Z_Label                  : String := To_String(Default_Label_String);
        Z_Major_Tick_Interval    : Long_Float := 0.0;
        Z_Number_Of_Subintervals : Natural := 0);


    -- Calculate world coordinates and subpage from relative device coordinates.
    -- plcalc_world
    procedure World_From_Relative_Coordinates
       (x_Relative, y_Relative : Long_Float_0_1_Type;
        x_World,    y_World    : out Long_Float;
        Last_Window_Index      : out Integer);


    -- Clear current subpage.
    -- plclear
    procedure Clear_Current_Subpage;


    -- Set color, map 0. Argument is integer between 0 and 15.
    -- plcol0
    procedure Set_Pen_Color(A_Color : Plot_Color_Type);


    -- Set color, map 1. Argument is a float between 0. and 1.
    -- plcol1
    procedure Set_Color_Map_1(Color : Long_Float_0_1_Type);


    -- Draws a contour plot from data in f(nx,ny). Is just a front-end to plfcont,
    -- with a particular choice for f2eval and f2eval_data.
    -- plcont
    procedure Contour_Plot
       (z                                : Real_Matrix;
        x_Min_Index, x_Max_Index         : Integer;
        y_Min_Index, y_Max_Index         : Integer;
        Contour_Levels                   : Real_Vector;
        Transformation_Procedure_Pointer : Transformation_Procedure_Pointer_Type;
        Transformation_Data              : Transformation_Data_Type);


    -- The procedure plfcont is not documented and is not part of the API. 
    -- However, it is a very useful capability to have available.
    -- I have tried to implement it as I think was intended but this may be incorrect.
    -- It appears as though the intent is to pass the arbitrarily organized 
    -- data (pointed to by Irregular_Data_Pointer) as a (single) pointer to a 
    -- 2D C-style array. Thus, for examaple, it is not possible to pass the data  
    -- as triples.
    
    -- Draws a contour plot using the function evaluator f2eval and data stored
    -- by way of the f2eval_data pointer. This allows arbitrary organizations
    -- of 2d array data to be used.

    -- plfcont
    procedure Contour_Plot_Irregular_Data
       (Function_Evaluator_Pointer       : Function_Evaluator_Pointer_Type;
        Irregular_Data                   : Real_Matrix;
        x_Min_Index, x_Max_Index         : Integer;
        y_Min_Index, y_Max_Index         : Integer;
        Contour_Levels                   : Real_Vector;
        Transformation_Procedure_Pointer : Transformation_Procedure_Pointer_Type;
        Transformation_Data              : Transformation_Data_Type);


    -- Copies state parameters from the reference stream to the current stream.
    -- plcpstrm
    procedure Copy_State_Parameters
       (Stream_ID                      : Integer;
        Do_Not_Copy_Device_Coordinates : Boolean);


    -- Converts input values from relative device coordinates to relative plot
    -- coordinates.
    -- pldid2pc
    procedure pldid2pc_Placeholder;


    -- Converts input values from relative plot coordinates to relative
    -- device coordinates.
    -- pldip2dc
    procedure pldip2dc_Placeholder;


    -- End a plotting session for all open streams.
    -- plend
    procedure End_PLplot;


    -- End a plotting session for the current stream only.
    -- plend1
    procedure End_PLplot_Current_Stream;


    -- Simple interface for defining viewport and window.
    -- plenv
    procedure Set_Environment
       (x_Min, x_Max, y_Min, y_Max : Long_Float;
        Justification              : Justification_Type;
        Axis_Style                 : Axis_Style_Type);


    -- similar to plenv() above, but in multiplot mode does not advance the subpage,
    -- instead the current subpage is cleared
    -- plenv0
    procedure Set_Environment_Clear_Subpage
       (x_Min, x_Max, y_Min, y_Max : Long_Float;
        Justification              : Justification_Type;
        Axis_Style                 : Axis_Style_Type);


    -- End current page. Should only be used with plbop().
    -- pleop
    procedure Eject_Current_Page;
    

    -- Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i))
    -- plerrx
    procedure Draw_Error_Bars_X(x_Min, x_Max, y : Real_Vector);


    -- Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i))
    -- plerry
    procedure Draw_Error_Bars_Y(x, y_Min, y_Max : Real_Vector);


    -- Advance to the next family file on the next new page
    -- plfamadv
    procedure Next_Family_File_On_New_Page;


    -- Pattern fills the polygon bounded by the input points.
    -- plfill
    procedure Fill_Polygon(x, y : Real_Vector);


    -- Pattern fills the 3d polygon bounded by the input points.
    -- plfill3
    procedure Fill_Polygon_3D (x, y, z : Real_Vector);


    -- Flushes the output stream. Use sparingly, if at all.
    -- plflush
    procedure Flush_Output_Stream;


    -- Sets the global font flag to 'ifont'.
    -- plfont
    procedure Set_Font_Style(Font_Style : Font_Style_Type);


    -- Load specified font set.
    -- plfontld
    procedure Set_Characer_Set(Character_Set : Character_Set_Type);


    -- Get character default height and current (scaled) height
    -- plgchr
    procedure Get_Character_Height(Default_Height, Current_Height : out Long_Float);


    -- Returns 8 bit RGB values for given color from color map 0
    -- plgcol0
    procedure Get_Color_RGB
       (Color_Index      : Integer;
        Red_Component, Green_Component, Blue_Component : out Integer);


    -- Returns the background color by 8 bit RGB value
    -- plgcolbg
    procedure Get_Background_Color_RGB
       (Red_Component, Green_Component, Blue_Component : out Integer);


    -- Returns the current compression setting
    -- plgcompression
    procedure Get_Compression_Level(Compression_Level : out Integer);


    -- Get the current device (keyword) name
    -- plgdev
    procedure Get_Device_Name(Device_Name : out String);


    -- Retrieve current window into device space
    -- plgdidev
    procedure Get_Device_Window_Parameters
       (Relative_Margin_Width : out Long_Float;
        Aspect_Ratio          : out Long_Float;
        x_Justification       : out Long_Float;
        y_Justification       : out Long_Float);


    -- Get plot orientation
    -- plgdiori
    procedure Get_Plot_Orientation(Rotation : out Long_Float);


    -- Retrieve current window into plot space
    -- plgdiplt
    procedure Get_Device_Window_Extrema
       (x_Min : out Long_Float;
        y_Min : out Long_Float;
        x_Max : out Long_Float;
        y_Max : out Long_Float);


    -- Get FCI (font characterization integer)
    -- plgfci
    procedure Get_Font_Characterization_Integer(Font_Characterization_Integer : out PLUNICODE);


    -- Get family file parameters
    -- plgfam
    procedure Get_File_Family_Parameters
       (Family_Enabled : out Boolean;
        Family_File_Number : out Integer;
        Maximum_File_Size : out Integer);


    -- Get the (current) output file name. Must be preallocated to >80 bytes
    -- plgfnam
    procedure Get_Output_File_Name(Output_File_Name : out String);


    -- Get the (current) run level.
    -- plglevel
    procedure Get_Run_Level(Run_Level : out Integer);


    -- Get output device parameters.
    -- plgpage
    procedure Get_Page_Parameters
       (x_Pixels, y_Pixels           : out Long_Float;
        x_Page_Length, y_Page_Length : out Integer;
        x_Page_Offset, y_Page_Offset : out Integer);


    -- Switches to graphics screen.
    -- plgra
    procedure Use_Graphics_Mode;


    -- Grid irregularly sampled data.
    -- plgriddata
    procedure Grid_Data
       (x, y, z                : Real_Vector; -- ungridded x- and y-points; z is height
        x_Grid, y_Grid         : Real_Vector;
        z_Gridded              : in out Real_Matrix;
        Gridding_Algorithm     : Gridding_Algorithm_Type;
        Griding_Algorithm_Data : Long_Float);


    -- Get subpage boundaries in absolute coordinates
    -- Results are millimeters from the lower left corner.
    -- plgspa
    procedure Get_Subpage_Boundaries
       (Left_Edge, Right_Edge, Bottom_Edge, Top_Edge : out Long_Float);


    -- Get current stream number.
    -- plgstrm
    procedure Get_Stream_Number(Stream_Number : out Integer);


    -- Get the current library version number
    -- plgver
    procedure Get_Version_Number(Version_Number : out String);

    -- Function version of the procedure Get_Version_Number; not part of the PLplot API.
    -- plgver
    function Get_Version_Number return String;

    -- Get viewport boundaries in normalized device coordinates
    -- plgvpd
    procedure Get_Viewport_Normalized(x_Min, x_Max, y_Min, y_Max : out Long_Float);


    -- Get viewport boundaries in world coordinates
    -- plgvpw
    procedure Get_Viewport_World(x_Min, x_Max, y_Min, y_Max : out Long_Float);


    -- Get x axis labeling parameters
    -- plgxax
    procedure Get_X_Label_Parameters(Max_Digits, Actual_Digits : out Integer);


    -- Get y axis labeling parameters
    -- plgyax
    procedure Get_Y_Label_Parameters(Max_Digits, Actual_Digits : out Integer);
    

    -- Get z axis labeling parameters
    -- plgzax
    procedure Get_Z_Label_Parameters(Max_Digits, Actual_Digits : out Integer);


    -- Draws a histogram of n values of a variable in array data[0..n-1]
    -- plhist
    procedure Histogram_Unbinned
       (Data           : Real_Vector;
        Data_Min       : Long_Float; -- left  edge of left-most bin
        Data_Max       : Long_Float; -- right edge of right-most bin
        Number_Of_Bins : Positive; -- equal-sized, between Data_Min and Data_Max
        Options : Integer); -- Options are not defined in plplot.h.


    -- Set current color (map 0) by hue, lightness, and saturation.
    -- plhls
    procedure Set_Color_HLS
       (Hue_Component        : Long_Float; -- Not documented; presumably 0.0..360.0.
        Lightness_Component  : Long_Float_0_1_Type;
        Saturation_Component : Long_Float_0_1_Type);


    -- Functions for converting between HLS and RGB color space
    -- plhlsrgb
    procedure HLS_To_RGB
       (Hue_Component        : Long_Float;
        Lightness_Component  : Long_Float_0_1_Type;
        Saturation_Component : Long_Float_0_1_Type;
        Red_Component        : out Long_Float_0_1_Type;
        Green_Component      : out Long_Float_0_1_Type;
        Blue_Component       : out Long_Float_0_1_Type);


    -- Initialization. Must be called before starting plot.
    -- Certain other procedures, if used, must be called first.
    -- Initializes PLplot, using preset or default options
    -- plinit
    procedure Initialize_PLplot;


    -- Draws a line segment from (x1, y1) to (x2, y2).
    -- pljoin
    procedure Draw_Line(x1, y1, x2, y2 : Long_Float);


    -- Simple routine for labelling graphs.
    -- pllab
    procedure Write_Labels(X_Label, Y_Label, Title_Label : String := To_String(Default_Label_String));


    -- Sets position of the light source
    -- pllightsource
    procedure Set_Light_Source
       (x : Long_Float := 1.0;
        y: Long_Float := 1.0;
        z: Long_Float := 1.0);


    -- Draws line segments connecting a series of points.
    -- plline
    procedure Draw_Curve(x, y : Real_Vector);


    -- Draws a line in 3 space.
    -- plline3
    procedure Draw_Curve_3D(x, y, z : Real_Vector);


    -- Set line style.
    -- pllsty
    procedure Select_Line_Style(Line_Style : Line_Style_Type);


    -- plot continental outline in world coordinates
    -- plmap
    procedure Draw_Map
       (Map_Form_Function_Pointer            : Map_Form_Function_Pointer_Type;
        Map_Kind                             : Map_Type;
        Minimum_Longitude, Maximum_Longitude : Long_Float;
        Minimum_Latitude,  Maximum_Latitude  : Long_Float);


    -- Plot the latitudes and longitudes on the background.
    -- plmeridians
    procedure Draw_Latitude_Longitude
       (Map_Form_Function_Pointer            : Map_Form_Function_Pointer_Type;
        Delta_Longitude, Delta_Latitude      : Long_Float;
        Minimum_Longitude, Maximum_Longitude : Long_Float;
        Minimum_Latitude,  Maximum_Latitude  : Long_Float);


    -- Plots a mesh representation of the function z[x][y].
    -- plmesh
    procedure Mesh_3D
       (x, y    : Real_Vector; -- surface definition points
        z       : Real_Matrix; -- height of surface at definition points
        Options : Integer);

    -- Plots a mesh representation of the function z[x][y] with contour
    -- plmeshc
    procedure Mesh_3D_Base_Contour
       (x, y           : Real_Vector; -- surface definition points
        z              : in Real_Matrix; -- height of surface at definition points
        Options        : Integer;
        Contour_Levels : Real_Vector); -- levels at which to draw contours


    -- Creates a new stream and makes it the default.
    -- plmkstrm
    procedure Make_Stream(New_Stream_Number : out Integer);


    -- Prints out "text" at specified position relative to viewport
    -- plmtex
    procedure Write_Text_Viewport
       (Side                : String;
        Position_From_Edge  : Long_Float;
        Position_Along_Edge : Long_Float;
        Justification       : Long_Float_0_1_Type;
        The_Text            : String);


    -- Plots a 3-d representation of the function z[x][y].
    -- plot3d
    procedure Plot_3D
       (x, y    : Real_Vector; -- surface definition points
        z       : Real_Matrix; -- height of surface at definition points
        Options : Integer;
        Sides : Boolean); -- draw sides?


    -- Plots a 3-d representation of the function z[x][y] with contour.
    -- plot3dc
    procedure Plot_3D_Base_Contour
       (x, y           : Real_Vector; -- surface definition points
        z              : Real_Matrix; -- height of surface at definition points
        Options        : Integer;
        Contour_Levels : Real_Vector); -- levels at which to draw contours


    -- Plots a 3-d representation of the function z[x][y] with contour and
    -- y index limits.
    -- plot3dcl
    procedure Plot_3D_Base_Contour_Limits -- Lacks documentation in Chapter 17 of Ref. Man.
       (x, y           : Real_Vector; -- surface definition points
        z              : Real_Matrix; -- height of surface at definition points
        Options        : Integer;
        Contour_Levels : Real_Vector;
        ixstart, ixn   : Integer;
        indexymin, indexymax : Integer_Array_1D); -- levels at which to draw contours



       --  valid options for plot3dc():
       --
       --  DRAW_SIDES, BASE_CONT, TOP_CONT (not yet),
       --  MAG_COLOR, DRAW_LINEX, DRAW_LINEY, DRAW_LINEXY.
       --
       --  valid options for plsurf3d():
       --
       --  MAG_COLOR, BASE_CONT, SURF_CONT, FACETED, DRAW_SIDES.
 

    -- Set fill pattern directly.
    -- plpat
    procedure Set_Fill_Pattern
       (Inclinations : Integer_Array_1D;
        Spacings     : Integer_Array_1D);


    -- Plots array y against x for n points using ASCII code "code".
    -- plpoin
    procedure Draw_Points
       (x, y : Real_Vector;
        Symbol_As_Number : Integer);


    -- Draws a series of points in 3 space.
    -- plpoin3
    procedure Draw_Points_3D
       (x, y, z          : Real_Vector;
        Symbol_As_Number : Integer);


    -- Draws a polygon in 3 space.
    -- plpoly3
    procedure Draw_Polygon_3D
       (x, y, z : Real_Vector;
        Draw_Segments : Boolean_Array_1D;
        Draw_Counterclockwise : Boolean);


    -- Set the floating point precision (in number of places) in numeric labels.
    -- plprec
    procedure Set_Numeric_Label_Precision
       (Set_Automatically : Boolean := False;
        Number_Digits_After_Decimal_Point : Integer := 2);


    -- Set fill pattern, using one of the predefined patterns.
    -- plpsty
    procedure Select_Fill_Pattern(Fill_Pattern : Fill_Pattern_Type);


    -- Prints out "text" at world cooordinate (x,y).
    -- plptex
    procedure Write_Text_World
       (x, y             : Long_Float;
        Delta_X, Delta_Y : Long_Float;
        Justification    : Long_Float_0_1_Type;
        The_Text         : String);


    -- Replays contents of plot buffer to current device/file.
    -- plreplot
    procedure Replot;


    -- Set line color by red, green, blue from  0. to 1.
    -- plrgb
    procedure Set_Line_Color_RGB_0_1(Red_Component, Blue_Component, Green_Component : Long_Float_0_1_Type);


    -- Set line color by 8 bit RGB values.
    -- plrgb1
    procedure Set_Line_Color_RGB_0_255(Red_Component, Blue_Component, Green_Component : Integer);


    -- Functions for converting between HLS and RGB color space
    -- plrgbhls
    procedure RGB_To_HLS
       (Red_Component        : Long_Float_0_1_Type;
        Green_Component      : Long_Float_0_1_Type;
        Blue_Component       : Long_Float_0_1_Type;
        Hue_Component        : out Long_Float;
        Lightness_Component  : out Long_Float_0_1_Type;
        Saturation_Component : out Long_Float_0_1_Type);


    -- Set character height.
    -- plschr
    procedure Set_Character_Height(Default_Height, Scale_Factor : Long_Float);


    -- The PLplot docs say that the arguments to this procedure are arrays of 8-bit numbers
    -- but plplot.h says that they are arrays of 32-bit integers.
    -- Set color map 0 colors by 8 bit RGB values
    -- plscmap0
    procedure Set_Color_Map_0(Red_Components, Green_Components, Blue_Components : Integer_Array_1D);


    -- Set number of colors in cmap 0
    -- plscmap0n
    procedure Set_Number_Of_Colors_Map_0(Number_Of_Colors : Integer);


    -- Set color map 1 colors by 8 bit RGB values
    -- plscmap1
    procedure Set_Color_Map_1_RGB
       (Red_Component, Green_Component, Blue_Component : Integer_0_255_Array);


    -- Types for colors for color map 1.
    type Color_Model_Type is (HLS, RGB); -- Red, Green, Blue or Hue, Lightness, Saturation


    -- Set color map 1 colors using a piece-wise linear relationship between
    -- intensity [0,1] (cmap 1 index) and position in HLS or RGB color space.
    -- plscmap1l
    procedure Set_Color_Map_1_Piecewise
       (Color_Model    : Color_Model_Type;    -- HLS or RGB
        Control_Points : Real_Vector; -- range 0.0 .. 1.0; not checked here
        H_Or_R         : Real_Vector; -- range 0.0 .. 1.0; not checked here
        L_Or_G         : Real_Vector; -- range 0.0 .. 1.0; not checked here
        S_Or_B         : Real_Vector; -- range 0.0 .. 1.0; not checked here
        Reverse_Hue    : Boolean_Array_1D);   -- False means red<->green<->blue<->red, True reverses


    -- Set number of colors in cmap 1
    -- plscmap1n
    procedure Set_Number_Of_Colors_In_Color_Map_1(Number_Of_Colors : Integer);


    -- Set a given color from color map 0 by 8 bit RGB value
    -- plscol0
    procedure Set_One_Color_Map_0
       (Plot_Color : Plot_Color_Type;
        Red_Component, Green_Component, Blue_Component : Integer);

    -- Set the background color by 8 bit RGB value
    -- plscolbg
    procedure Set_Background_Color_RGB
       (Red_Component, Green_Component, Blue_Component : Integer);


    -- Used to globally turn color output on/off
    -- plscolor
    procedure Enable_Color_Output(Enable_Color : Boolean);


    -- Set the compression level
    -- plscompression
    procedure Set_Compression_Level(Compression_Level : Integer);


    -- Set the device (keyword) name
    -- plsdev
    procedure Set_Device_Name(Device_Name : String);


    -- Set window into device space using margin, aspect ratio, and
    -- justification
    -- plsdidev
    procedure Set_Device_Window_Parameters
       (Margin          : Long_Float;
        Aspect_Ratio    : Long_Float;
        x_Justification : Long_Float;
        y_Justification : Long_Float);


    -- Set up transformation from metafile coordinates.
    -- plsdimap
    procedure Set_Metafile_Transformation
       (dimxmin : Integer;
        dimxmax : Integer;
        dimymin : Integer;
        dimymax : Integer;
        dimxpmm : Long_Float;
        dimypmm : Long_Float);


    -- Set plot orientation, specifying rotation in units of pi/2.
    -- plsdiori
    procedure Set_Plot_Orientation(Rotation : Long_Float);


    -- Set window into plot space
    -- plsdiplt
    procedure Set_Device_Window_Extrema
       (x_Min : Long_Float := 0.0;
        y_Min : Long_Float := 0.0;
        x_Max : Long_Float := 1.0;
        y_Max : Long_Float := 1.0);


    -- Set window into plot space incrementally (zoom)
    -- plsdiplz
    procedure Set_Zoom
       (x_Min_Relative : Long_Float;
        y_Min_Relative : Long_Float;
        x_Max_Relative : Long_Float;
        y_Max_Relative : Long_Float);


    -- Set the escape character for text strings.
    -- plsesc
    procedure Set_Escape_Character(Escape_Character : Character);


    -- Set family file parameters
    -- plsfam
    procedure Set_File_Family_Parameters
       (Enable_Family : Boolean;
        Family_File_Number : Integer := 1;
        Maximum_File_Size : Integer := 1_000_000);


    -- Set FCI (font characterization integer)
    -- plsfci
    procedure Set_Font_Characterization_Integer(Font_Characterization_Integer : PLUNICODE);

    -- Set the output file name.
    -- plsfnam
    procedure Set_Output_File_Name(Output_File_Name : String);


    -- Shade region.
    -- plshade
    procedure Shade_Region
       (z                                        : Real_Matrix;
        Mask_Function_Pointer                    : Mask_Function_Pointer_Type;
        x_Min, x_Max, y_Min, y_Max               : Long_Float; -- world mins and maxes
        Shade_Min, Shade_Max                     : Long_Float;
        Select_Color_Map                         : Natural; -- should be 0 or 1
        Color                                    : Long_Float;
        Fill_Pattern_Pen_Width                   : Positive;
        Shade_Min_Pen_Color, Shade_Min_Pen_Width : Natural;
        Shade_Max_Pen_Color, Shade_Max_Pen_Width : Natural;
        Preserve_Rectangles                      : Boolean;
        Transformation_Procedure_Pointer          : Transformation_Procedure_Pointer_Type;
        Transformation_Data                      : Transformation_Data_Type);


    -- plshade1
    procedure Shade_Region_1
       (z                                        : Real_Matrix;
        Mask_Function_Pointer                    : Mask_Function_Pointer_Type;
        x_Min, x_Max, y_Min, y_Max               : Long_Float; -- world mins and maxes
        Shade_Min, Shade_Max                     : Long_Float;
        Select_Color_Map                         : Natural; -- should be 0 or 1
        Color                                    : Long_Float;
        Fill_Pattern_Pen_Width                   : Positive;
        Shade_Min_Pen_Color, Shade_Min_Pen_Width : Natural;
        Shade_Max_Pen_Color, Shade_Max_Pen_Width : Natural;
        Preserve_Rectangles                      : Boolean;
        Transformation_Procedure_Pointer          : Transformation_Procedure_Pointer_Type;
        Transformation_Data                      : Transformation_Data_Type);


    -- plshades
    procedure Shade_Regions
       (z                               : Real_Matrix;
        Mask_Function_Pointer           : Mask_Function_Pointer_Type;
        x_Min, x_Max, y_Min, y_Max      : Long_Float; -- world mins and maxes
        Contour_Levels                  : Real_Vector;
        Fill_Pattern_Pen_Width          : Positive;
        Contour_Pen_Color               : Natural; -- 0 for no contours
        Contour_Pen_Width               : Natural; -- 0 for no contours
        Preserve_Rectangles             : Boolean;
        Transformation_Procedure_Pointer : Transformation_Procedure_Pointer_Type;
        Transformation_Data             : Transformation_Data_Type);


    -- The procedure plfshade is not part of the API. If it should be necessary 
    -- to make it available to Ada programs, use the binding to plfcont in this 
    -- file as a guidline.

    -- fix this
    --    procedure
    --    plfshade(PLFLT (*f2eval) ( : PLINT;  : PLINT; PLPointer),
    --         PLPointer f2eval_data,
    --         PLFLT (*c2eval) ( : PLINT;  : PLINT; PLPointer),
    --         PLPointer c2eval_data,
    --         nx : PLINT; ny : PLINT;
    --         left : PLFLT; right : PLFLT; bottom : PLFLT; top : PLFLT;
    --         shade_min : PLFLT; shade_max : PLFLT;
    --         sh_cmap : PLINT; sh_color : PLFLT; sh_width : PLINT;
    --         min_color : PLINT; min_width : PLINT;
    --         max_color : PLINT; max_width : PLINT;
    --         void (*fill) ( : PLINT;  : PL_Float_Array;  : PL_Float_Array), rectangular : PLINT;
    --         void (*pltr) ( : PLFLT;  : PLFLT;  : PL_Float_Array;  : PL_Float_Array; PLPointer),
    --         PLPointer pltr_data);
    --

    -- Set up lengths of major tick marks.
    -- plsmaj
    procedure Set_Major_Tick_Length(Default_Length, Scale_Factor : Long_Float);


    -- Set the memory area to be plotted (with the 'mem' driver)
    -- plsmem
    procedure Plot_From_Memory
       (x_Size, y_Size : Integer;
        Plot_This      : System.Address);


    -- Set up lengths of minor tick marks.
    -- plsmin
    procedure Set_Minor_Tick_Length(Default_Length, Scale_Factor : Long_Float);


    -- Set orientation. Must be done before calling plinit.
    -- plsori
    procedure Set_Orientation(Orientation : Orientation_Type);


    -- Set output device parameters. Usually ignored by the driver.
    -- plspage
    procedure Set_Page_Parameters
       (x_Pixels, y_Pixels : Long_Float;
        x_Length, y_Length : Integer;
        x_Offset, y_Offset : Integer);


    -- Set the pause (on end-of-page) status
    -- plspause
    procedure Set_Pause(Pause : Boolean);


    -- Set stream number.
    -- plsstrm
    procedure Set_Stream_Number(Stream_Number : Integer);


    -- Set the number of subwindows in x and y
    -- plssub
    procedure Set_Number_Of_Subpages(x_Number, y_Number : Integer);


    -- Set symbol height.
    -- plssym
    procedure Set_Symbol_Size(Default_Height, Scale_Factor : Long_Float);


    -- Initialize PLplot, passing in the windows/page settings.
    -- plstar
    procedure Initialize_Query_Device
       (Number_Horizontal_Subpages, Number_Vertical_Subpages : Integer := 1);


    -- Initialize PLplot, passing the device name and windows/page settings.
    -- plstart
    procedure Initialize_Set_Device
       (Device_Name                                          : String;
        Number_Horizontal_Subpages, Number_Vertical_Subpages : Integer := 1);


    -- Add a point to a stripchart.
    -- plstripa
    procedure Update_Stripchart
       (ID         : Integer;
        Pen_Number : Integer;
        x, y       : Long_Float);


    -- Create 1d stripchart
    -- plstripc
    procedure Create_Stripchart
       (ID                                   : out Integer;
        X_Options, Y_Options                 : String;
        x_Min, x_Max                         : Long_Float;
        x_Jump                               : Long_Float;
        y_Min, y_Max                         : Long_Float;
        x_Legend_Position, y_Legend_Position : Long_Float_0_1_Type;
        Autoscale_Y                          : Boolean;
        Accumulate                           : Boolean; --Accumulate or slide
        Box_Color, Legend_Color              : Plot_Color_Type;
        Pen_Colors                           : Integer_Array_1D;
        Line_Styles                          : Integer_Array_1D;
        Pen_Labels                           : Stripchart_Label_String_Array_Type;
        X_Label, Y_Label, Title_Label        : String := To_String(Default_Label_String));


    -- Deletes and releases memory used by a stripchart.
    -- plstripd
    procedure Delete_Stripchart(ID : Integer);


    -- plots a 2d image (or a matrix too large for plshade() )
    -- plimage
    procedure Draw_Image -- No documentation in Chapter 17 of Programmer's Reference Manual
       (Data : Real_Matrix;
        x_Min, x_Max : Long_Float;
        y_Min, y_Max : Long_Float;
        z_Min, z_Max : Long_Float;
        Dxmin, Dxmax : Long_Float;
        Dymin, Dymax : Long_Float);


    -- Set up a new line style
    -- plstyl
    procedure Set_Line_Style(Marks, Spaces : Integer_Array_1D);


    Default_Continuous_Line : constant Integer := 0;

    -- This is an overloaded procedure equivalent to calling plstyl with its
    -- first argument zero.
    -- plstyl
    procedure Set_Line_Style(Default_Continuous_Line : Integer);


    -- Plots the 3d surface representation of the function z[x][y].
    -- plsurf3d
    procedure Shaded_Surface_3D
       (x, y           : Real_Vector; -- surface definition points
        z              : Real_Matrix; -- height of surface at definition points
        Options        : Integer;
        Contour_Levels : Real_Vector); -- levels at which to draw contours


    -- Plots the 3d surface representation of the function z[x][y] with y
    -- index limits.
    -- plsurf3dl


    -- Sets the edges of the viewport to the specified absolute coordinates
    -- plsvpa
    procedure Set_Viewport_Absolute
       (Left_Edge   : Long_Float;  -- millimeters from edge of subpage
        Right_Edge  : Long_Float;  -- millimeters from edge of subpage
        Bottom_Edge : Long_Float;  -- millimeters from edge of subpage
        Top_Edge    : Long_Float); -- millimeters from edge of subpage


    -- Set x axis labeling parameters
    -- "digits" changed to "field_digits".
    -- plsxax
    procedure Set_Floating_Point_Display_X(Max_Digits, Field_Digits : Integer);


    -- Set inferior X window
    -- plsxwin
    procedure Set_Inferior_Window(Window_ID : Integer);


    -- Set y axis labeling parameters
    -- "digits" changed to "field_digits".
    -- plsyax
    procedure Set_Floating_Point_Display_Y(Max_Digits, Field_Digits : Integer);


    -- Plots array y against x for n points using Hershey symbol "code"
    -- plsym
    procedure Draw_Hershey_Symbol
       (x, y : Real_Vector;
        Hershey_Code : Integer);


    -- Set z axis labeling parameters
    -- "digits" changed to "field_digits".
    -- plszax
    procedure Set_Floating_Point_Display_Z(Max_Digits, Field_Digits : Integer);


    -- Switches to text screen.
    -- pltext
    procedure Use_Text_Mode;


    -- Sets the edges of the viewport with the given aspect ratio, leaving
    -- room for labels.
    -- plvasp
    procedure Set_Viewport_Aspect_Ratio(Aspect_Ratio : Long_Float);


    -- Creates the largest viewport of the specified aspect ratio that fits
    -- within the specified normalized subpage coordinates.
    -- plvpas
    procedure Set_Viewport_Maximized_For_Aspect_Ratio
       (x_Min, x_Max : Long_Float;
        y_Min, y_Max : Long_Float;
        Aspect_Ratio : Long_Float);


    -- Creates a viewport with the specified normalized subpage coordinates.
    -- plvpor
    procedure Set_Viewport_Normalized
       (Left_Edge   : Long_Float := 0.0;
        Right_Edge  : Long_Float := 1.0;
        Bottom_Edge : Long_Float := 0.0;
        Top_Edge    : Long_Float := 1.0);

    -- Defines a "standard" viewport with seven character heights for
    -- the left margin and four character heights everywhere else.
    -- plvsta
    procedure Set_Viewport_Standard;


    -- Set up a window for three-dimensional plotting.
    -- plw3d
    procedure Set_Up_3D
       (X_Box, Y_Box, Z_Box                      : Long_Float; -- Extents of enclosing box; world coordinates
        X_Min, X_Max, Y_Min, Y_Max, Z_Min, Z_Max : Long_Float; -- Data limits; user coordinates
        Altitude, Azimuth                        : Long_Float); -- Viewing angles in world coordinates


    -- Set pen width.
    -- plwid
    procedure Set_Pen_Width(Pen_Width : Integer);


    -- Set up world coordinates of the viewport boundaries (2d plots).
    -- plwind
    procedure Set_Viewport_World
       (Left_Edge   : Long_Float;
        Right_Edge  : Long_Float;
        Bottom_Edge : Long_Float;
        Top_Edge    : Long_Float);

    -- set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device 
    -- plxormod
    procedure Set_XOR_Mode
       (Use_XOR : Boolean;
        Supports_XOR : out Boolean);


--------------------------------------------------------------------------------
--         Functions for use from C or C++ only                               --
--         (Not really ;).                                                    --
--------------------------------------------------------------------------------
--  THESE FUNCTIONS ^^^ ARE NOT IMPLEMENTED FOR THE ADA BINDING
--  EXCEPT FOR THE FOLLOWING.

    -- This procedure is currently GNAT-specific, importing Gnat_Argc and Gnat_Argv.
    -- Process options list using current options info.
    -- plparseopts
    procedure Parse_Command_Line_Arguments(Mode : Parse_Mode_Type);


    -- Process input strings, treating them as an option and argument pair.
    -- plsetopt
    procedure Set_Command_Line_Option(Option, Argument : String);


	-- Transformation routines

    -- These wrappers are necessary because Ada pointers don't like to point to 
    -- subroutines having non-Ada conventions (I suppose).

    -- Identity transformation.
    -- pltr0
    procedure Plot_Transformation_0
       (x_Grid, y_Grid   : Long_Float;
        x_World, y_World : out Long_Float;
        Data             : PLpointer);
        

    -- Does linear interpolation from singly dimensioned coord arrays.
    -- pltr1
    procedure Plot_Transformation_1
       (x_Grid, y_Grid   : Long_Float;
        x_World, y_World : out Long_Float;
        Data_Pointer     : PLpointer);
        

    -- Does linear interpolation from doubly dimensioned coord arrays
    -- (column dominant, as per normal C 2d arrays).
    -- pltr2
    procedure Plot_Transformation_2
       (x_Grid, y_Grid   : Long_Float;
        x_World, y_World : out Long_Float;
        Data_Pointer     : PLpointer);
        
        

end PLplot;
