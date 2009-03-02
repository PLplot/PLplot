(* $Id$

  Copyright (C) 2008 Alan W. Irwin
  Copyright (C) 2008 Andrew Ross
  Copyright (C) 2008 Hezekiah M. Carty

  set/get tester

  This file is part of PLplot.
  
  PLplot is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with PLplot; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  
*)

open Printf
open Plplot

let () =
  let r1 = [|0; 255|] in
  let g1 = [|255; 0|] in
  let b1 = [|0; 0|] in
  let a1 = [|1.0; 1.0|] in
  let status = ref 0 in

  let failed_if t s =
    if t then (
      eprintf s;
      status := 1;
    );
  in

  (* Parse and process command line arguments *)
  ignore (plparseopts Sys.argv [PL_PARSE_FULL]);

  (* Test setting / getting familying parameters before plinit *)
  (* Save values set by plparseopts to be restored later. *)
  let fam0, num0, bmax0 = plgfam () in
  let fam1 = 0 in
  let num1 = 10 in
  let bmax1 = 1000 in
  plsfam fam1 num1 bmax1;

  (* Retrieve the same values? *)
  let fam2, num2, bmax2 = plgfam () in
  printf "family parameters: fam, num, bmax = %d %d %d\n" fam2 num2 bmax2;
  failed_if
    (fam2 <> fam1 || num2 <> num1 || bmax2 <> bmax1)
    "plgfam test failed\n";
  (* Restore values set initially by plparseopts. *)
  plsfam fam0 num0 bmax0;

  (* Test setting / getting page parameters before plinit *)
  (* Save values set by plparseopts to be restored later. *)
  let xp0, yp0, xleng0, yleng0, xoff0, yoff0 = plgpage () in

  let xp1 = 200. in
  let yp1 = 200. in
  let xleng1 = 400 in
  let yleng1 = 200 in
  let xoff1 = 10 in
  let yoff1 = 20 in
  plspage xp1 yp1 xleng1 yleng1 xoff1 yoff1;

  (* Retrieve the same values? *)
  let xp2, yp2, xleng2, yleng2, xoff2, yoff2 = plgpage () in
  printf
    "page parameters: xp, yp, xleng, yleng, xoff, yoff = %f %f %d %d %d %d\n"
    xp2 yp2 xleng2 yleng2 xoff2 yoff2;
  failed_if
    (xp2 <> xp1 || yp2 <> yp1 || xleng2 <> xleng1 || yleng2 <> yleng1 || 
     xoff2 <> xoff1 || yoff2 <> yoff1)
    "plgpage test failed\n";
  (* Restore values set initially by plparseopts. *)
  plspage xp0 yp0 xleng0 yleng0 xoff0 yoff0;

  (* Test setting / getting compression parameter across plinit. *)
  let compression1 = 95 in
  plscompression compression1;

  (* Initialize plplot *)
  plinit ();

  (* Test if device initialization screwed around with the preset
     compression parameter. *)
  let compression2 = plgcompression () in
  printf "Output various PLplot parameters\n";
  printf "compression parameter = %d\n" compression2;
  failed_if
    (compression2 <> compression1)
    "plgcompression test failed\n";

  (* Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
     they work without any obvious error messages. *)
  plscolor 1;
  plscol0 1 255 0 0;
  plscmap1 r1 g1 b1;
  plscmap1a r1 g1 b1 a1;
  
  let level2 = plglevel () in
  printf "level parameter = %d\n" level2;
  failed_if (level2 != 1) "plglevel test failed.\n";

  pladv 0;
  plvpor 0.01 0.99 0.02 0.49;
  let xmin, xmax, ymin, ymax = plgvpd () in
  printf
    "plvpor: xmin, xmax, ymin, ymax = %f %f %f %f\n"
    xmin xmax ymin ymax;
  failed_if
    (xmin <> 0.01 || xmax <> 0.99 || ymin <> 0.02 || ymax <> 0.49)
    "plgvpd test failed\n";

  let xmid = 0.5 *. (xmin +. xmax) in
  let ymid = 0.5 *. (ymin +. ymax) in

  plwind 0.2 0.3 0.4 0.5;
  let xmin, xmax, ymin, ymax = plgvpw () in
  printf
    "plwind: xmin, xmax, ymin, ymax = %f %f %f %f\n"
    xmin xmax ymin ymax;
  failed_if
    (xmin <> 0.2 || xmax <> 0.3 || ymin <> 0.4 || ymax <> 0.5)
    "plgvpw test failed\n";

  (* Get world coordinates for middle of viewport *)
  let wx, wy, win = plcalc_world xmid ymid in
  printf "world parameters: wx, wy, win = %f %f %d\n" wx wy win;
  failed_if
    (
      abs_float (wx -. 0.5 *. (xmin +. xmax)) > 1.0E-5 ||
      abs_float (wy -. 0.5 *. (ymin +. ymax)) > 1.0E-5
    )
    "plcalc_world test failed\n";

  (* Retrieve and print the name of the output file (if any).
     This goes to stderr not stdout since it will vary between tests and 
     we want stdout to be identical for compare test. *)
  let fnam = plgfnam () in
  if String.length fnam = 0 then
    printf "No output file name is set\n"
  else
    printf"Output file name read\n";
  eprintf "Output file name is %s\n" fnam;

  (* Set and get the number of digits used to display axis labels *)
  (* Note digits is currently ignored in pls[xyz]ax and 
     therefore it does not make sense to test the returned 
     value *)
  plsxax 3 0;
  let digmax, digits = plgxax () in
  printf "x axis parameters: digmax, digits = %d %d\n" digmax digits;
  failed_if (digmax <> 3) "plgxax test failed\n";

  plsyax 4 0;
  let digmax, digits = plgyax () in
  printf "y axis parameters: digmax, digits = %d %d\n" digmax digits;
  failed_if (digmax <> 4) "plgyax test failed\n";

  plszax 5 0;
  let digmax, digits = plgzax () in
  printf "z axis parameters: digmax, digits = %d %d\n" digmax digits;
  failed_if (digmax <> 5) "plgzax test failed\n";

  plsdidev 0.05 (-42.0) 0.1 0.2;
  let mar, aspect, jx, jy = plgdidev () in
  printf
    "device-space window parameters: mar, aspect, jx, jy = %f %f %f %f\n"
    mar aspect jx jy;
  failed_if
    (mar <> 0.05 || jx <> 0.1 || jy <> 0.2)
    "plgdidev test failed\n";

  plsdiori 1.0;
  let ori = plgdiori () in
  printf "ori parameter = %f\n" ori;
  failed_if (ori <> 1.0) "plgdiori test failed\n";

  plsdiplt 0.1 0.2 0.9 0.8;
  let xmin, ymin, xmax, ymax = plgdiplt () in
  printf
    "plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f\n"
    xmin ymin xmax ymax;
  failed_if
    (xmin <> 0.1 || xmax <> 0.9 || ymin <> 0.2 || ymax <> 0.8)
    "plgdiplt test failed\n";

  plsdiplz 0.1 0.1 0.9 0.9;
  let zxmin, zymin, zxmax, zymax = plgdiplt () in
  printf
    "zoomed plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f\n"
    zxmin zymin zxmax zymax;
  failed_if (
    abs_float (zxmin -. (xmin +. (xmax -. xmin) *. 0.1)) > 1.0E-5 || 
    abs_float (zxmax -. (xmin +. (xmax -. xmin) *. 0.9)) > 1.0E-5 || 
    abs_float (zymin -. (ymin +. (ymax -. ymin) *. 0.1)) > 1.0E-5 || 
    abs_float (zymax -. (ymin +. (ymax -. ymin) *. 0.9)) > 1.0E-5
  ) "plsdiplz test failed\n";

  plscolbg 10 20 30;
  let r, g, b = plgcolbg () in
  printf "background colour parameters: r, g, b = %d %d %d\n" r g b;
  failed_if (r <> 10 || g <> 20 || b <> 30) "plgcolbg test failed\n";

  plscolbga 20 30 40 0.5;
  let r, g, b, a = plgcolbga () in
  printf
    "background/transparency colour parameters: r, g, b, a = %d %d %d %f\n"
    r g b a;
  failed_if
    (r <> 20 || g <> 30 || b <> 40 || a <> 0.5)
    "plgcolbga test failed\n";

  plend ();

  exit !status;
  ()
