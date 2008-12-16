(* $Id$

   set/get tester
*)

open Plplot
open Printf

(*--------------------------------------------------------------------------*\
 * Demonstrates absolute positioning of graphs on a page.
 \*--------------------------------------------------------------------------*)

let main () =
  let r1 = [|0; 255|] in
  let g1 = [|255; 0|] in
  let b1 = [|0; 0|] in
  let a1 = [|1.0; 1.0|] in

  (* Parse and process command line arguments *)
  ignore (plparseopts Sys.argv [PL_PARSE_FULL]);

  (* Test setting / getting page size *)
  let xp, yp, xleng, yleng, xoff, yoff = plgpage () in
  let xp2 = xp *. 0.9 in
  let yp2 = yp *. 0.9 in
  let xleng2 = int_of_float (float_of_int xleng *. 0.9) in
  let yleng2 = int_of_float (float_of_int yleng *. 0.9) in
  plspage xp2 yp2 xleng2 yleng2 xoff yoff;
  let xp, yp, xleng, yleng, xoff, yoff = plgpage () in
  if xp <> xp2 || yp <> yp2 || xleng <> xleng2 || yleng <> yleng2 then (
    failwith "plgpage test failed\n";
  );

  plscompression 1;
  let compression = plgcompression () in
  if (compression <> 1) then (
    failwith "plgcompression test failed\n"
  );

  let fam, num, bmax = plgfam () in
  plsfam 1 1 100000;
  let fam1, num1, bmax1 = plgfam () in
  if fam1 <> 1 || num1 <> 1 || bmax1 <> 100000 then (
    failwith "plgfam test failed\n";
  );
  plsfam fam num bmax;

  (* Initialize plplot *)
  plinit ();

  plscolor 1;

  plscol0 1 255 0 0;

  plscmap1 r1 g1 b1;
  plscmap1a r1 g1 b1 a1;
  
  let level = plglevel () in
  if level <> 1 then (
    let err =
      sprintf "plglevel test failed. Level is %d, but 1 expected.\n" level
    in
    failwith err;
  );

  pladv 0;
  plvpor 0.0 1.0 0.0 1.0;

  plwind 0.2 0.3 0.4 0.5;
  let xmin, xmax, ymin, ymax = plgvpw () in
  if xmin <> 0.2 || xmax <> 0.3 || ymin <> 0.4 || ymax <> 0.5 then (
    failwith "plgvpw test failed\n";
  );

  let xmin, xmax, ymin, ymax = plgvpd () in
  if xmin <> 0.0 || xmax <> 1.0 || ymin <> 0.0 || ymax <> 1.0 then (
    failwith "plgvpd test failed\n";
  );

  (* Get world coordinates for 0.5,0.5 which is in the middle of 
     the window *)
  let wx, wy, win = plcalc_world 0.5 0.5 in
  if abs_float (wx -. 0.25) > 1.0E-5 || abs_float (wy -. 0.45) > 1.0E-5 then (
    failwith "plcalc_world test failed\n";
  );

  (* Retrieve and print the name of the output file (if any) *)
  let fnam = plgfnam () in
  printf "Output file name is %s\n" fnam;

  (* Set and get the number of digits used to display axis labels *)
  (* Note digits is currently ignored in pls[xyz]ax and 
     therefore it does not make sense to test the returned 
     value *)
  plsxax 3 0;
  let digmax, _ = plgxax () in
  if digmax <> 3 then (
    failwith "plgxax test failed\n";
  );

  plsyax 3 0;
  let digmax, _ = plgyax () in
  if digmax <> 3 then (
    failwith "plgyax test failed\n";
  );

  plszax 3 0;
  let digmax, _ = plgzax () in
  if digmax <> 3 then (
    failwith "plgzax test failed\n";
  );

  (* TODO: Add PL_NOTSET definition to the OCaml bindings? *)
  plsdidev 0.05 (-42.0) 0.0 0.0;
  let mar, aspect, jx, jy = plgdidev () in
  if mar <> 0.05 || jx <> 0.0 || jy <> 0.0 then (
    failwith "plgdidev test failed\n";
  );

  plsdiori 1.0;
  let ori = plgdiori () in
  if ori <> 1.0 then (
    failwith "plgdiori test failed\n";
  );

  plsdiplt 0.1 0.1 0.9 0.9;
  let xmin, ymin, xmax, ymax = plgdiplt () in
  if xmin <> 0.1 || xmax <> 0.9 || ymin <> 0.1 || ymax <> 0.9 then (
    failwith "plgdiplt test failed\n";
  );

  plsdiplz 0.1 0.1 0.9 0.9;
  let xmin, ymin, xmax, ymax = plgdiplt () in
  if
    xmin <> 0.1 +. 0.8 *. 0.1 ||
    xmax <> 0.1 +. 0.8 *. 0.9 ||
    ymin <> 0.1 +. 0.8 *. 0.1 ||
    ymax <> 0.1 +. 0.8 *. 0.9
  then (
    failwith "plsdiplz test failed\n";
  );

  plscolbg 0 0 0;
  let r, g, b = plgcolbg () in
  if r <> 0 || g <> 0 || b <> 0 then (
    failwith "plgcolbg test failed\n";
  );

  plscolbga 0 0 0 1.0;
  let r, g, b, a = plgcolbga () in
  if r <> 0 || g <> 0 || b <> 0 || a <> 1.0 then (
    failwith "plgcolbga test failed\n";
  );

  ()

let () =
  let () =
    try
      main ()
    with
    | Failure err ->
        prerr_string err;
        plend ();
        exit 1;
        ()
  in
  plend ();
  ()
