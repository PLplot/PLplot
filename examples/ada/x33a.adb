-- -*- coding: utf-8; -*-
--
-- $Id$
--
-- Demonstrate most pllegend capability including unicode symbols.
--
-- Copyright (C) 2011 Jerry Bauck
--
-- This file is part of PLplot.
--
-- PLplot is free software; you can redistribute it and/or modify
-- it under the terms of the GNU Library General Public License as published
-- by the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- PLplot is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Library General Public License for more details.
--
-- You should have received a copy of the GNU Library General Public License
-- along with PLplot; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
--

-- This example designed just for devices (e.g., the cairo-related and
-- qt-related devices) where the best choice of glyph is automatically
-- selected by the related libraries (pango/cairo or Qt4) for each
-- unicode character depending on what system fonts are installed.  Of
-- course, you must have the appropriate TrueType fonts installed to
-- have access to all the required glyphs.

-- Ada note: The function TUB is Ada.Strings.Unbounded.To_Unbounded_String
-- renamed. See PLplot_Traditional.ads or PLplot.ads.

with
    Ada.Strings.Unbounded,
    Ada.Strings,
    Ada.Strings.Fixed,
    PLplot_Auxiliary,
    PLplot_Traditional;
use
    Ada.Strings.Unbounded,
    Ada.Strings,
    Ada.Strings.Fixed,
    PLplot_Auxiliary,
    PLplot_Traditional;

procedure x33a is
    position_options : Integer_Array_1D(0 .. 15) := 
       (PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_OUTSIDE,
        PL_POSITION_TOP + PL_POSITION_OUTSIDE,
        PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_OUTSIDE,
        PL_POSITION_RIGHT + PL_POSITION_OUTSIDE,
        PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE,
        PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE,
        PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE,
        PL_POSITION_LEFT + PL_POSITION_OUTSIDE,
        PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE,
        PL_POSITION_TOP + PL_POSITION_INSIDE,
        PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE,
        PL_POSITION_RIGHT + PL_POSITION_INSIDE,
        PL_POSITION_RIGHT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE,
        PL_POSITION_BOTTOM + PL_POSITION_INSIDE,
        PL_POSITION_LEFT + PL_POSITION_BOTTOM + PL_POSITION_INSIDE,
        PL_POSITION_LEFT + PL_POSITION_INSIDE);

    -- Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
    special_symbols : Legend_String_Array_Type(0 .. 4) := 
       (TUB("✰"), TUB("✴"), TUB("✱"), TUB("✽"), TUB("✦"));

    colorbar : Boolean := False; -- By default do not plot plcolorbar pages
                                   -- for now while we are working out the API.

-- static PLOptionTable options() := {
--     {
--         "colorbar",              -- Turns on pages showing colorbars
--         NULL,
--         NULL,
--         &colorbar,
--         PL_OPT_BOOL,
--         "-colorbar",
--         "Plot the \"color bar\" pages."
--     },
--     {
--         NULL,                   -- option
--         NULL,                   -- handler
--         NULL,                   -- client data
--         NULL,                   -- address of variable to set
--         0,                      -- mode flag
--         NULL,                   -- short syntax
--         NULL
--     }                           -- long syntax
-- };

--const char           *notes() := { "Make sure you get it right!", NULL };

    --procedure plcolorbar_example_1(PLINT bar_type, PLINT cont_color, PLINT cont_width, PLFLT ticks, PLINT sub_ticks, PLINT n, PLFLT *values, const char *title)


    -- procedure plcolorbar_example_1
    --    (bar_type, cont_color, cont_width : Integer;
    --     ticks : Long_Float;
    --     sub_ticks, n : Integer;
    --     values : Real_Vector;
    --     title : Unbounded_String)
    -- is
    --     colors : Real_Vector(0 .. n - 1);
    --     i : Integer;
    --     color_step, color_offset : Long_Float;
    --     opt, position : Integer;
    --     axis_opts_1, axis_opts_2 : Unbounded_String;
    -- begin
    --     pladv(0);
    --     
    --     -- Set up color palette 1.
    --     plspal1("cmap1_blue_red.pal", True);
    -- 
    --     color_step := 1.0 / Long_Float(n - 1);
    --     for i in 0 .. n - 1 loop
    --         colors(i) := color_offset + color_step * Long_Float(i); -- COLOR_OFFSET NOT DEFINED!
    --     end loop;
    -- 
    --     position := PL_POSITION_LEFT;
    --     opt      := bar_type + PL_COLORBAR_LABEL_LEFT + PL_COLORBAR_CAP_HIGH;
    -- 
    --     if bar_type and PL_COLORBAR_SHADE_LABEL then
    --         axis_opts_1 := To_Unbounded_String("iv");
    --         axis_opts_2 := To_Unbounded_String("i");
    --     else
    --         if sub_ticks /= 0 then
    --             axis_opts_1 := To_Unbounded_String("stv");
    --             axis_opts_2 := To_Unbounded_String("st");
    --         else
    --             axis_opts_1 := To_Unbounded_String("tv");
    --             axis_opts_2 := To_Unbounded_String("t");
    --         end if;
    --     end if;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.1, 0.5, 0.1, -- fixme
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_1, "Test label - Left, High Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_RIGHT;
    --     opt      := bar_type + PL_COLORBAR_LABEL_RIGHT + PL_COLORBAR_CAP_LOW;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.4, 0.5, 0.1, -- fixme etc.
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_1, "Test label - Right, Low Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_TOP;
    --     opt      := bar_type + PL_COLORBAR_LABEL_TOP + PL_COLORBAR_CAP_HIGH;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.1, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_2, "Test label - Top, High Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_BOTTOM;
    --     opt      := bar_type + PL_COLORBAR_LABEL_BOTTOM + PL_COLORBAR_CAP_LOW;
    -- 
    --     plcolorbar(opt, position, 0.4, 0.1, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_2, "Test label - Bottom, Low Cap",
    --         n, colors, values);
    -- 
    --     plvpor(0.0, 1.0, 0.0, 1.0);
    --     plwind(0.0, 1.0, 0.0, 1.0);
    --     plptex(0.5, 0.5, 0.0, 0.0, 0.5, To_String(title));
    -- end plcolorbar_example_1;
    -- 
    -- 
    -- procedure plcolorbar_example_2
    --    (bar_type, cont_color, cont_width : Integer;
    --     ticks, sub_ticks : Long_Float;
    --     n : Integer; 
    --     values : Real_Vector;
    --     title : Unbounded_String)
    -- is
    --     colors : Long_Float(0 .. n - 1);
    --     i : Integer;
    --     color_step, color_offset : Long_Float;
    --     opt, position : Integer;
    --     axis_opts_1, axis_opts_2 : Unbounded_String;
    -- begin
    --     pladv(0);
    --     
    --     -- Set up color palette 1.
    --     plspal1("cmap1_blue_yellow.pal", True);
    -- 
    --     color_step := 1.0 / Long_Float(n - 1);
    --     for i in 0 .. n - 1 loop
    --         if i = 0 then
    --             color_offset := 0.01;
    --         elsif i = n - 1 then
    --             color_offset := -0.01;
    --         else
    --             color_offset := 0.0;
    --         end if;
    --         colors(i) := color_offset + color_step * Long_Float(i);
    --     end loop;
    -- 
    --     position := PL_POSITION_LEFT;
    --     opt      := bar_type + PL_COLORBAR_LABEL_LEFT + PL_COLORBAR_CAP_LOW;
    -- 
    --     if bar_type = PL_COLORBAR_SHADE_LABEL then
    --         axis_opts_1 := "";
    --         axis_opts_2 := "";
    --     else
    --         if sub_ticks /= 0 then
    --             axis_opts_1 := "stv";
    --             axis_opts_2 := "st";
    --         else
    --             axis_opts_1 := "tv";
    --             axis_opts_2 := "t";
    --         end if;
    --     end if;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.1, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_1, "Test label - Left, Low Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_RIGHT;
    --     opt      := bar_type + PL_COLORBAR_LABEL_RIGHT + PL_COLORBAR_CAP_HIGH;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.4, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_1, "Test label - Right, High Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_TOP;
    --     opt      := bar_type + PL_COLORBAR_LABEL_TOP + PL_COLORBAR_CAP_LOW;
    -- 
    --     plcolorbar(opt, position, 0.1, 0.1, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_2, "Test label - Top, Low Cap",
    --         n, colors, values);
    -- 
    --     position := PL_POSITION_BOTTOM;
    --     opt      := bar_type + PL_COLORBAR_LABEL_BOTTOM + PL_COLORBAR_CAP_HIGH;
    -- 
    --     plcolorbar(opt, position, 0.4, 0.1, 0.5, 0.1,
    --         cont_color, cont_width,
    --         ticks, sub_ticks,
    --         axis_opts_2, "Test label - Bottom, High Cap",
    --         n, colors, values);
    -- 
    --     plvpor(0.0, 1.0, 0.0, 1.0);
    --     plwind(0.0, 1.0, 0.0, 1.0);
    --     plptex(0.5, 0.5, 0.0, 0.0, 0.5, To_String(title));
    -- end plcolorbar_example_2;

----------------------------------------------------------------------------
-- Demonstrate most pllegend capability including unicode symbols.
----------------------------------------------------------------------------

    opt : Integer;
    nlegend : Integer;
    nturn : Integer;
    legend_width, legend_height, x, y, xstart, ystart : Long_Float;
    max_height, text_scale : Long_Float;
    position, opt_base, nrow, ncolumn : Integer;

begin -- main
    plparseopts(PL_PARSE_FULL); 

    -- Initialize plplot
    plinit;

    -- First page illustrating the 16 standard positions.
    declare
        nlegend : constant Integer := 1;
        opt_array       :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        text_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_colors      :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_patterns    :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_scales      :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        box_line_widths :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        line_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_styles     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_widths     :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        symbol_numbers  :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_colors   :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_scales   :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        text            : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
        symbols         : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
    begin
        pladv(0);
        plvpor(0.25, 0.75, 0.25, 0.75);
        plwind(0.0, 1.0, 0.0, 1.0);
        plbox("bc", 0.0, 0, "bc", 0.0, 0);
        plsfont(PL_FCI_SANS, -1, -1);
        plmtex("t", 8.0, 0.5, 0.5, "The 16 standard legend positions with");
        plmtex("t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y");

        -- Only specify legend data that are required according to the
        -- value of opt_array for that entry.
        opt_base          := PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX;
        opt_array(0)      := PL_LEGEND_LINE + PL_LEGEND_SYMBOL;
        line_styles(0)    := 1;
        line_widths(0)    := 1.0;
        symbol_scales(0)  := 1.0;
        symbol_numbers(0) := 4;
        symbols(0)        := TUB("*");

        -- Use monotype fonts so that all legends are the same size.
        plsfont(PL_FCI_MONO, -1, -1);
        plscol0a(15, 32, 32, 32, 0.70);

        for k in 0 .. 15 loop
            position := position_options(k);
            opt      := opt_base;
            -- Make a 2-digit string with leading 0 for 0 .. 9.
            if k >= 10 then
                text(0) := To_Unbounded_String(Trim(Integer'image(k), Left));
            else
                text(0) := To_Unbounded_String("0" & Trim(Integer'image(k), Left));
            end if;
            text_colors(0)   := 1 + (k mod 8);
            line_colors(0)   := 1 + (k mod 8);
            symbol_colors(0) := 1 + (k mod 8);

            pllegend(legend_width, legend_height, opt, position, 0.05, 0.05,
                0.1, 15, 1, 1, 0, 0,
                opt_array, 1.0, 1.0, 2.0,
                1.0, text_colors, text,
                Box_Colors, Box_Patterns, 
                Box_Scales, Box_Line_Widths,
                line_colors, line_styles, line_widths,
                symbol_colors, symbol_scales, symbol_numbers, symbols);
        end loop;
    end; -- declare

    -- Second page illustrating effect of nrow, ncolumn for the same legend data.
    declare
        nlegend : constant Integer := 7;
        opt_array       :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        text_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_colors      :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_patterns    :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_scales      :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        box_line_widths :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        line_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_styles     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_widths     :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        symbol_numbers  :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_colors   :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_scales   :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        text            : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
        symbols         : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
    begin
        pladv(0);
        plvpor(0.25, 0.75, 0.25, 0.75);
        plwind(0.0, 1.0, 0.0, 1.0);
        plbox("bc", 0.0, 0, "bc", 0.0, 0);
        plsfont(PL_FCI_SANS, -1, -1);
        plmtex("t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,");
        plmtex("t", 6.0, 0.5, 0.5, "and position for the same legend data");

        -- Only specify legend data that are required according to the
        -- value of opt_array for that entry.
        opt_base := PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX;
        for k in 0 .. nlegend - 1 loop
            opt_array(k)      := PL_LEGEND_LINE + PL_LEGEND_SYMBOL;
            line_styles(k)    := 1;
            line_widths(k)    := 1.0;
            symbol_scales(k)  := 1.0;
            symbol_numbers(k) := 2;
            symbols(k)        := TUB("*");
            -- Make 2-digit strings with leading 0 for 0 .. 9.
            if k >= 10 then
                text(k) := To_Unbounded_String(Trim(Integer'image(k), Left));
            else
                text(k) := To_Unbounded_String("0" & Trim(Integer'image(k), Left));
            end if;
            text_colors(k)   := 1 + (k mod 8);
            line_colors(k)   := 1 + (k mod 8);
            symbol_colors(k) := 1 + (k mod 8);
        end loop;

        -- Use monotype fonts so that all legends are the same size.
        plsfont(PL_FCI_MONO, -1, -1);
        plscol0a(15, 32, 32, 32, 0.70);

        position := PL_POSITION_TOP + PL_POSITION_OUTSIDE;
        opt      := opt_base;
        x        := 0.0;
        y        := 0.1;
        nrow     := 1;
        ncolumn  := nlegend;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_BOTTOM + PL_POSITION_OUTSIDE;
        opt      := opt_base;
        x        := 0.0;
        y        := 0.1;
        nrow     := 1;
        ncolumn  := nlegend;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_LEFT + PL_POSITION_OUTSIDE;
        opt      := opt_base;
        x        := 0.1;
        y        := 0.0;
        nrow     := nlegend;
        ncolumn  := 1;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_RIGHT + PL_POSITION_OUTSIDE;
        opt      := opt_base;
        x        := 0.1;
        y        := 0.0;
        nrow     := nlegend;
        ncolumn  := 1;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_LEFT + PL_POSITION_TOP + PL_POSITION_INSIDE;
        opt      := opt_base;
        x        := 0.0;
        y        := 0.0;
        nrow     := 6;
        ncolumn  := 2;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_RIGHT + PL_POSITION_TOP + PL_POSITION_INSIDE;
        opt      := opt_base + PL_LEGEND_ROW_MAJOR;
        x        := 0.0;
        y        := 0.0;
        nrow     := 6;
        ncolumn  := 2;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);

        position := PL_POSITION_BOTTOM + PL_POSITION_INSIDE;
        opt      := opt_base + PL_LEGEND_ROW_MAJOR;
        x        := 0.0;
        y        := 0.0;
        nrow     := 3;
        ncolumn  := 3;
        pllegend(legend_width, legend_height, opt, position, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
    end; -- declare

    -- Third page demonstrating legend alignment
    pladv(0);
    plvpor(0.0, 1.0, 0.0, 0.9);
    plwind(0.0, 1.0, 0.0, 1.0);
    plsfont(PL_FCI_SANS, -1, -1);
    plmtex("t", 2.0, 0.5, 0.5, "Demonstrate legend alignment");

    x        := 0.1;
    y        := 0.1;
    nturn    := 4;
    nlegend  := 0;
    position := PL_POSITION_TOP + PL_POSITION_LEFT + PL_POSITION_SUBPAGE;
    opt_base := PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX;
    opt      := opt_base;
    for i in 0 .. 8 loop

        -- Set up legend arrays with the correct size, type.
        if i <= nturn then
            nlegend := nlegend + 1;
        else
            nlegend := nlegend - 1;
        end if;
        nlegend := Integer'max(1, nlegend);
        declare
            opt_array       :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            text_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            box_colors      :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            box_patterns    :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            box_scales      :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
            box_line_widths :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
            line_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            line_styles     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            line_widths     :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
            symbol_numbers  :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            symbol_colors   :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
            symbol_scales   :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
            text            : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
            symbols         : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
        begin
            -- Only specify legend data that are required according to the
            -- value of opt_array for that entry.
            for k in 0 .. nlegend - 1 loop
                opt_array(k)      := PL_LEGEND_LINE + PL_LEGEND_SYMBOL;
                line_styles(k)    := 1;
                line_widths(k)    := 1.0;
                symbol_scales(k)  := 1.0;
                symbol_numbers(k) := 2;
                symbols(k)        := TUB("*");
                -- Make 2-digit strings with leading 0 for 0 .. 9.
                if k >= 10 then
                    text(k) := To_Unbounded_String(Trim(Integer'image(k), Left));
                else
                    text(k) := To_Unbounded_String("0" & Trim(Integer'image(k), Left));
                end if;
                text_colors(k)   := 1 + (k mod 8);
                line_colors(k)   := 1 + (k mod 8);
                symbol_colors(k) := 1 + (k mod 8);
            end loop; -- k

            -- Use monotype fonts so that all legends are the same size.
            plsfont(PL_FCI_MONO, -1, -1);
            plscol0a(15, 32, 32, 32, 0.70);

            nrow    := Integer'min(3, nlegend);
            ncolumn := 0;

            pllegend(legend_width, legend_height, opt, position, x, y,
                0.025, 15, 1, 1, nrow, ncolumn,
                opt_array, 1.0, 1.0, 1.5,
                1.0, text_colors, text,
                Box_Colors, Box_Patterns, 
                Box_Scales, Box_Line_Widths,
                line_colors, line_styles, line_widths,
                symbol_colors, symbol_scales, symbol_numbers, symbols);

            if i = nturn then
                position := PL_POSITION_TOP + PL_POSITION_RIGHT + PL_POSITION_SUBPAGE;
                opt      := opt_base;
                x        := 1.0 - x;
                y        := y + legend_height;
            else
                x := x + legend_width;
                y := y + legend_height;
            end if;
        end; -- declare
    end loop; -- i

    -- Fourth page illustrating various kinds of legends
    declare
        nlegend : constant Integer := 5;
        opt_array       :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        text_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_colors      :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_patterns    :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        box_scales      :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        box_line_widths :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        line_colors     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_styles     :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        line_widths     :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        symbol_numbers  :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_colors   :         Integer_Array_1D(0 .. nlegend - 1) := (others => 0);
        symbol_scales   :         Real_Vector     (0 .. nlegend - 1) := (others => 0.0);
        text            : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
        symbols         : Legend_String_Array_Type(0 .. nlegend - 1) := (others => TUB(" "));
    begin
        max_height := 0.0;
        xstart     := 0.0;
        ystart     := 0.1;
        x          := xstart;
        y          := ystart;
        text_scale := 0.90;
        pladv(0);
        plvpor(0.0, 1.0, 0.0, 0.90);
        plwind(0.0, 1.0, 0.0, 1.0);
        plsfont(PL_FCI_SANS, -1, -1);
        plmtex("t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends");

        -- Only specify legend data that are required according to the
        -- value of opt_array for that entry.
        position := PL_POSITION_LEFT + PL_POSITION_TOP;
        opt_base := PL_LEGEND_BACKGROUND + PL_LEGEND_BOUNDING_BOX + PL_LEGEND_TEXT_LEFT;

        -- Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
        opt_array(0) := PL_LEGEND_NONE;
        text(0) := TUB("None");
        text_colors(0) := 1;

        opt_array(1) := PL_LEGEND_COLOR_BOX;
        text(1) := TUB("Box");
        text_colors(1)     := 2;
        box_colors(1)      := 2;
        box_patterns(1)    := 0;
        box_scales(1)      := 0.8;
        box_line_widths(1) := 1.0;

        opt_array(2) := PL_LEGEND_LINE;
        text(2) := TUB("Line");
        text_colors(2) := 3;
        line_colors(2) := 3;
        line_styles(2) := 1;
        line_widths(2) := 1.0;

        opt_array(3) := PL_LEGEND_SYMBOL;
        text(3) := TUB("Symbol");
        text_colors(3)    := 4;
        symbol_colors(3)  := 4;
        symbol_scales(3)  := text_scale;
        symbol_numbers(3) := 4;
        symbols(3)        := special_symbols(2);

        opt_array(4) := PL_LEGEND_SYMBOL + PL_LEGEND_LINE;
        text(4) := TUB("L & S");
        text_colors(4)    := 5;
        line_colors(4)    := 5;
        line_styles(4)    := 1;
        line_widths(4)    := 1.0;
        symbol_colors(4)  := 5;
        symbol_scales(4)  := text_scale;
        symbol_numbers(4) := 4;
        symbols(4)        := special_symbols(2);

        opt := opt_base;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up symbol legend entries with various symbols.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_SYMBOL;
            text(i) := TUB("Symbol ") & special_symbols(i);
            text_colors(i)    := i + 1;
            symbol_colors(i)  := i + 1;
            symbol_scales(i)  := text_scale;
            symbol_numbers(i) := 4;
            symbols(i)        := special_symbols(i);
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up symbol legend entries with various numbers of symbols.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_SYMBOL;
            text(i) := TUB("Symbol Number" & Integer'image(i + 2));
            text_colors(i)    := i + 1;
            symbol_colors(i)  := i + 1;
            symbol_scales(i)  := text_scale;
            symbol_numbers(i) := i + 2;
            symbols(i)        := special_symbols(2);
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            Box_Colors, Box_Patterns, 
            Box_Scales, Box_Line_Widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up box legend entries with various colours.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_COLOR_BOX;
            text(i) := TUB("Box Color" & Integer'image(i + 1));
            text_colors(i)     := i + 1;
            box_colors(i)      := i + 1;
            box_patterns(i)    := 0;
            box_scales(i)      := 0.8;
            box_line_widths(i) := 1.0;
        end loop;

        opt := opt_base;
        -- Use new origin
        x          := xstart;
        y         := y + max_height;
        max_height := 0.0;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up box legend entries with various patterns.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_COLOR_BOX;
            text(i) := TUB("Box Pattern" & Integer'image(i));
            text_colors(i)     := 2;
            box_colors(i)      := 2;
            box_patterns(i)    := i;
            box_scales(i)      := 0.8;
            box_line_widths(i) := 1.0;
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up box legend entries with various box pattern line widths.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_COLOR_BOX;
            text(i) := TUB("Box Line Width" & Integer'image(i + 1));
            text_colors(i)     := 2;
            box_colors(i)      := 2;
            box_patterns(i)    := 3;
            box_scales(i)      := 0.8;
            box_line_widths(i) := Long_Float(i) + 1.0;
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up line legend entries with various colours.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_LINE;
            text(i) := TUB("Line Color" & Integer'image(i + 1));
            text_colors(i) := i + 1;
            line_colors(i) := i + 1;
            line_styles(i) := 1;
            line_widths(i) := 1.0;
        end loop;

        opt := opt_base;
        -- Use new origin
        x          := xstart;
        y         := y + max_height;
        max_height := 0.0;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up line legend entries with various styles.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_LINE;
            text(i) := TUB("Line Style" & Integer'image(i + 1));
            text_colors(i) := 2;
            line_colors(i) := 2;
            line_styles(i) := i + 1;
            line_widths(i) := 1.0;
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);

        -- Set up line legend entries with various widths.
        for i in 0 .. nlegend - 1 loop
            opt_array(i) := PL_LEGEND_LINE;
            text(i) := TUB("Line Width" & Integer'image(i + 1));
            text_colors(i) := 2;
            line_colors(i) := 2;
            line_styles(i) := 1;
            line_widths(i) := Long_Float(i) + 1.0;
        end loop;

        opt := opt_base;
        x  := x + legend_width;
        plscol0a(15, 32, 32, 32, 0.70);

        pllegend(legend_width, legend_height, opt, position, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0.0, text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols);
        max_height := Long_Float'max(max_height, legend_height);
    end; -- declare

--    if colorbar then
        -- -- Color bar examples
        -- values_small : Real_Vector(0 .. 1)  := (0.0, 1.0);
        -- values_uneven : Real_Vector(0 .. 8) := (0.0, 2.0, 2.6, 3.4, 6.0, 7.0, 8.0, 9.0, 10.0);
        -- values_even : Real_Vector(0 .. 8)   := (0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
        -- plcolorbar_example_1(PL_COLORBAR_IMAGE, 0, 0, 0.0, 0, 2, values_small, "Image Color Bars");
        -- plcolorbar_example_2(PL_COLORBAR_IMAGE, 0, 0, 0.0, 0, 2, values_small, "Image Color Bars");
        -- plcolorbar_example_1(PL_COLORBAR_SHADE + PL_COLORBAR_SHADE_LABEL, 0, 0, 0.0, 0, 9, values_uneven,
        --     "Shade Color Bars - Uneven Steps");
        -- plcolorbar_example_2(PL_COLORBAR_SHADE, 0, 0, 3.0, 3, 9, values_even,
        --     "Shade Color Bars - Even Steps");
        -- plcolorbar_example_1(PL_COLORBAR_SHADE + PL_COLORBAR_SHADE_LABEL, 2, 1, 0.0, 0, 9, values_uneven,
        --     "Shade Color Bars - Uneven Steps, Contours");
        -- plcolorbar_example_2(PL_COLORBAR_SHADE, 2, 3, 3.0, 3, 9, values_even,
        --     "Shade Color Bars - Even Steps, Contours");
        -- plcolorbar_example_1(PL_COLORBAR_GRADIENT, 0, 0, 0.5, 5, 2, values_small,
        --     "Gradient Color Bars");
        -- plcolorbar_example_2(PL_COLORBAR_GRADIENT, 0, 0, 0.5, 5, 2, values_small,
        --     "Gradient Color Bars");
--    end if;

    plend;
end x33a;

