-- $Id$

-- set/get tester

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
    Ada.Text_IO,
    Ada.Long_Float_Text_IO,
    Ada.Strings,
    Ada.Strings.Fixed,
    Ada.Strings.Unbounded,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot_Auxiliary,
    PLplot_Traditional;
use
    Ada.Text_IO,
    Ada.Long_Float_Text_IO,
    Ada.Strings,
    Ada.Strings.Fixed,
    Ada.Strings.Unbounded,
    Ada.Numerics,
    Ada.Numerics.Long_Elementary_Functions,
    PLplot_Auxiliary,
    PLplot_Traditional;

-- COMMENT THIS LINE IF YOUR COMPILER DOES NOT INCLUDE THESE 
-- DEFINITIONS, FOR EXAMPLE, IF IT IS NOT ADA 2005 WITH ANNEX G.3 COMPLIANCE.
--with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;
@Ada_Is_2007_With_and_Use_Numerics@

-- This example mostly outputs text. As part of the PLplot testing regime, we 
-- require that the text output match that of the C version, x31c.c, exactly.
-- Therefore, certain extra measures were made in this Ada version to assure 
-- that matching text is created.

procedure x31a is
    xmin, xmax, ymin, ymax, zxmin, zxmax, zymin, zymax : Long_Float;
    xmid, ymid, wx, wy : Long_Float;
    mar, aspect, jx, jy, ori : Long_Float;  
    win, level2, digmax, digit, compression1, compression2 : Integer;
    xp1, yp1, xp2, yp2 : Long_Float;
    xleng1, yleng1, xoff1, yoff1, xleng2, yleng2, xoff2, yoff2 : Integer;
    num1, bmax1, num2, bmax2, r, g, b : Integer;
    fam1, fam2 : Boolean;
    a : Long_Float;
    r1 : Integer_0_255_Array(0 .. 1) := (0, 255);
    g1 : Integer_0_255_Array(0 .. 1) := (0, 255);
    b1 : Integer_0_255_Array(0 .. 1) := (0,   0);
    a1 : Real_Vector(0 .. 1) := (1.0, 1.0);
    status : Integer;
    fnam : Unbounded_String;
    
    -- Convert a boolean to integers 0 or 1.
    function Boolean_To_Integer(B : Boolean) return Integer is
    begin
        if B then
            return 1;
        else
            return 0;
        end if;
    end Boolean_To_Integer;
    
    -- Function to return a formated float that looks like what C does.
    function PF(x : Long_Float) return String is
        A_String : String(1 .. 30);
    begin
        Put(A_String, x, Aft => 6, Exp => 0);
        return Trim(A_String, Left);
    end PF;

begin
    -- Parse and process command line arguments
    status := 0;

    plparseopts(PL_PARSE_FULL);

    -- Test setting / getting compression parameter across plinit.
    compression1 := 95;
    plscompression(compression1);

    -- Test setting / getting familying parameters across plinit.
    fam1 := False;
    num1 := 10;
    bmax1 := 1000;
    plsfam(fam1, num1, bmax1);

    -- Test setting / getting page parameters across plinit.
    xp1 := 200.0;
    yp1 := 200.0;
    xleng1 := 400;
    yleng1 := 200;
    xoff1 := 10;
    yoff1 := 20;
    plspage(xp1, yp1, xleng1, yleng1, xoff1, yoff1);

    -- Initialize plplot.
    plinit;

    -- Test if device initialization screwed around with the preset
    -- compression parameter.
    plgcompression(compression2);
    Put_Line("Output various PLplot parameters");
    Put_Line("compression parameter =" & Integer'image(compression2));
    if (compression2 /= compression1) then
        Put_Line(Standard_Error, "plgcompression test failed");
        status := 1;
    end if;

    -- Test if device initialization screwed around with any of the
    -- preset familying values.
    plgfam(fam2, num2, bmax2);
    Put_Line("family parameters: fam, num, bmax =" 
        & Integer'image(Boolean_To_Integer(fam2)) 
        & Integer'image(num2) & Integer'image(bmax2));
    if fam2 /= fam1 or num2 /= num1 or bmax2 /= bmax1 then
        Put_Line(Standard_Error, "plgfam test failed");
        status := 1;
    end if;

    -- Test if device initialization screwed around with any of the
    -- preset page values.
    plgpage(xp2, yp2, xleng2, yleng2, xoff2, yoff2);
    Put("page parameters: xp, yp, xleng, yleng, xoff, yoff = " 
        & PF(xp2) & " " & PF(yp2));
    Put_Line(Integer'image(xleng2) & Integer'image(yleng2) 
        & Integer'image(xoff2) & Integer'image(yoff2));
    if xp2 /= xp1 or yp2 /= yp1 or xleng2 /= xleng1 or yleng2 /= yleng1 or 
      xoff2 /= xoff1 or yoff2 /= yoff1 then
        Put_Line(Standard_Error, "plgpage test failed");
        status := 1;
    end if;

    -- Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
    -- they work without any obvious error messages.
    plscolor(True);
    plscol0(1, 255, 0, 0);
    plscmap1(r1, g1, b1);
    plscmap1a(r1, g1, b1, a1);

    plglevel(level2);
    Put_Line("level parameter =" & Integer'image(level2));
    if (level2 /= 1) then
        Put_Line(Standard_Error, "plglevel test failed.");
        status := 1;
    end if;

    pladv(0);
    plvpor(0.01, 0.99, 0.02, 0.49);
    plgvpd(xmin, xmax, ymin, ymax);
    Put_Line("plvpor: xmin, xmax, ymin, ymax = " 
        & PF(xmin) & " " & PF(xmax) & " " & PF(ymin) & " " & PF(ymax));
    if xmin /= 0.01 or xmax /= 0.99 or ymin /= 0.02 or ymax /= 0.49 then
        Put_Line(Standard_Error, "plgvpd test failed");
        status := 1;
    end if;
    xmid := 0.5 * (xmin + xmax);
    ymid := 0.5 * (ymin + ymax);

    plwind(0.2, 0.3, 0.4, 0.5);
    plgvpw(xmin, xmax, ymin, ymax);
    Put_Line("plwind: xmin, xmax, ymin, ymax = "  
        & PF(xmin) & " " & PF(xmax) & " " & PF(ymin) & " " & PF(ymax));
    if xmin /= 0.2 or xmax /= 0.3 or ymin /= 0.4 or ymax /= 0.5 then
        Put_Line(Standard_Error, "plgvpw test failed");
        status := 1;
    end if;

    -- Get world coordinates for middle of viewport.
    plcalc_world(xmid,ymid,wx,wy,win);
    Put_Line("world parameters: wx, wy, win = " 
        & PF(wx) & " " & PF(wy) & Integer'image(win));
    if abs(wx-0.5*(xmin+xmax))>1.0E-5 or abs(wy-0.5*(ymin+ymax))>1.0E-5 then
        Put_Line(Standard_Error, "plcalc_world test failed");
        status := 1;    
    end if;

    -- Retrieve and print the name of the output file (if any).
    -- This goes to stderr not stdout since it will vary between tests and 
    -- we want stdout to be identical for compare test.
    fnam := To_Unbounded_String(plgfnam);
    if fnam = Null_Unbounded_String then
        Put_Line("No output file name is set");
    else
        Put_Line("Output file name read");
    end if;
    Put_Line(Standard_Error, "Output file name is " & plgfnam);

    -- Set and get the number of digits used to display axis labels.
    -- Note digit is currently ignored in pls[xyz]ax and 
    -- therefore it does not make sense to test the returned value.
    plsxax(3, 0);
    plgxax(digmax, digit);
    Put_Line("x axis parameters: digmax, digits =" 
        & Integer'image(digmax) & Integer'image(digit));
    if digmax /= 3 then
        Put_Line(Standard_Error, "plgxax test failed");
        status := 1;
    end if;

    plsyax(4, 0);
    plgyax(digmax, digit);
    Put_Line("y axis parameters: digmax, digits ="
        & Integer'image(digmax) & Integer'image(digit));
    if digmax /= 4 then
        Put_Line(Standard_Error, "plgyax test failed");
        status := 1;
    end if;

    plszax(5, 0);
    plgzax(digmax, digit);
    Put_Line("z axis parameters: digmax, digits =" 
        & Integer'image(digmax) & Integer'image(digit));
    if digmax /= 5 then
        Put_Line(Standard_Error, "plgzax test failed");
        status := 1;
    end if;

    plsdidev(0.05, PL_NOTSET, 0.1, 0.2);
    plgdidev(mar, aspect, jx, jy);
    Put_Line("device-space window parameters: mar, aspect, jx, jy = " 
        & PF(mar) & " " & PF(aspect) & " " & PF(jx) & " " & PF(jy));
    if mar /= 0.05 or jx /= 0.1 or jy /= 0.2 then
        Put_Line(Standard_Error, "plgdidev test failed");
        status := 1;
    end if;

    plsdiori(1.0);
    plgdiori(ori);
    Put_Line("ori parameter = " & PF(ori));
    if ori /= 1.0 then
        Put_Line(Standard_Error, "plgdiori test failed");
        status := 1;
    end if;

    plsdiplt(0.1, 0.2, 0.9, 0.8);
    plgdiplt(xmin, ymin, xmax, ymax);
    Put_Line("plot-space window parameters: xmin, ymin, xmax, ymax = " 
        & PF(xmin) & " " & PF(ymin) & " " & PF(xmax) & " " & PF(ymax));
    if xmin /= 0.1 or xmax /= 0.9 or ymin /= 0.2 or ymax /= 0.8 then
        Put_Line(Standard_Error, "plgdiplt test failed");
        status := 1;
    end if;

    plsdiplz(0.1, 0.1, 0.9, 0.9);
    plgdiplt(zxmin, zymin, zxmax, zymax);
    Put_Line("zoomed plot-space window parameters: xmin, ymin, xmax, ymax = " 
        & PF(zxmin) & " " & PF(zymin) & " " & PF(zxmax) & " " & PF(zymax));
    if
        abs(zxmin - (xmin + (xmax - xmin) * 0.1)) > 1.0E-5 or 
        abs(zxmax - (xmin + (xmax - xmin) * 0.9)) > 1.0E-5 or 
        abs(zymin - (ymin + (ymax - ymin) * 0.1)) > 1.0E-5 or 
        abs(zymax - (ymin + (ymax - ymin) * 0.9)) > 1.0E-5
    then
        Put_Line(Standard_Error, "plsdiplz test failed");
        status := 1;
    end if;

    plscolbg(10, 20, 30);
    plgcolbg(r, g, b);
    Put_Line("background colour parameters: r, g, b ="
        & Integer'image(r) & Integer'image(g) & Integer'image(b));
    if r /= 10 or g /= 20 or b /= 30 then
        Put_Line(Standard_Error, "plgcolbg test failed");
        status := 1;
    end if;

    plscolbga(20, 30, 40, 0.5);
    plgcolbga(r, g, b, a);
    Put_Line("background/transparency colour parameters: r, g, b, a ="
        & Integer'image(r) & Integer'image(g) & Integer'image(b) & " " & PF(a));
    if r /= 20 or g /= 30 or b /= 40 or a /= 0.5 then
        Put_Line(Standard_Error, "plgcolbga test failed");
        status := 1;
    end if;

    plend;
end x31a;
