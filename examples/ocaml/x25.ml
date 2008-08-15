(* $Id$
        Filling and clipping polygons.
*)

open Plplot

(*--------------------------------------------------------------------------*\
 * Test program for filling polygons and proper clipping
\*--------------------------------------------------------------------------*)

let () =
  (* Parse and process command line arguments *)
  ignore (plparseopts Sys.argv [|PL_PARSE_FULL|]);

  (* Initialize plplot *)
  plssub 3 3;
  plinit ();

  let xextreme =
    [|
      [|-120.0;  120.0|];
      [|-120.0;  120.0|];
      [|-120.0;  120.0|];
      [| -80.0;   80.0|];
      [|-220.0; -120.0|];
      [| -20.0;   20.0|];
      [| -20.0;   20.0|];
      [| -80.0;   80.0|];
      [|  20.0;  120.0|];
    |]
  in
  let yextreme =
    [|
      [|-120.0; 120.0|];
      [|  20.0; 120.0|];
      [| -20.0; 120.0|];
      [| -20.0; 120.0|];
      [|-120.0; 120.0|];
      [|-120.0; 120.0|];
      [| -20.0;  20.0|];
      [| -80.0;  80.0|];
      [|-120.0; 120.0|];
    |]
  in

  for j = 0 to 3 do
    let x, y =
      match j with
          0 ->
            (* Polygon 1: a diamond *)
            [|0.0; -100.0; 0.0; 100.0|],
            [|-100.0; 0.0; 100.0; 0.0|]
        | 1 ->
            (* Polygon 1: a diamond - reverse direction *)
            [|100.0; 0.0; -100.0; 0.0;|],
            [|0.0; 100.0; 0.0; -100.0|]
        | 2 ->
            (* Polygon 2: a square with punctures *)
            [|-100.0; -100.0; 80.0; -100.0; -100.0;
              -80.0; 0.0; 80.0; 100.0; 100.0|],
            [|-100.0; -80.0; 0.0; 80.0; 100.0;
              100.0; 80.0; 100.0; 100.0; -100.0|]
        | 3 ->
            (* Polygon 2: a square with punctures - reversed direction *)
            [|100.0; 100.0; 80.0; 0.0; -80.0;
              -100.0; -100.0; 80.0; -100.0; -100.0|],
            [|-100.0; 100.0; 100.0; 80.0; 100.0;
              100.0; 80.0; 0.0; -80.0; -100.0|]
        | _ -> raise (Failure "only 4 shapes are available")
    in

    for i = 0 to 8 do
      pladv 0;
      plvsta ();
      plwind xextreme.(i).(0) xextreme.(i).(1) yextreme.(i).(0) yextreme.(i).(1);
      plcol0 2;
      plbox "bc" 1.0 0 "bcnv" 10.0 0;
      plcol0 1;
      plpsty 0;
      plfill x y;
      plcol0 2;
      pllsty 1;
      plline x y;
    done
  done;

  (* Don't forget to call plend() to finish off! *)
  plend ();
  ()

