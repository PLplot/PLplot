(* $Id$

        Log plot demo.
*)

open Plplot

let pi = atan 1.0 *. 4.0

(*--------------------------------------------------------------------------*\
 * plot1
 *
 * Log-linear plot.
\*--------------------------------------------------------------------------*)

let plot1 plot_type =
  (*
    int i;
    static PLFLT freql[101], ampl[101], phase[101];
    PLFLT f0, freq;
  *)
  pladv 0;

  (* Set up data for log plot *)
  let f0 = 1.0 in
  let freql = Array.init 101 (fun i -> -2.0 +. float_of_int i /. 20.0) in
  let freq = Array.map (fun x -> 10.0**x) freql in
  let ampl =
    Array.map (
      fun x -> 20.0 *. log10 (1.0 /. sqrt (1.0 +. (x /. f0)**2.0))
    ) freq
  in
  let phase =
    Array.map (fun x -> -.(180.0 /. pi) *. atan (x /. f0)) freq
  in

  plvpor 0.15 0.85 0.1 0.9;
  plwind (-2.0) 3.0 (-80.0) 0.0;

  (* Try different axis and labelling styles. *)
  plcol0 1;
  let () =
    match plot_type with
        0 ->
          plbox "bclnst" 0.0 0 "bnstv" 0.0 0;
      | 1 ->
          plbox "bcfghlnst" 0.0 0 "bcghnstv" 0.0 0;
      | _ -> failwith "Bad plot type specified"
  in

  (* Plot ampl vs freq *)
  plcol0 2;
  plline freql ampl;
  plcol0 1;
  plptex 1.6 (-30.0) 1.0 (-20.0) 0.5 "-20 dB/decade";

  (* Put labels on *)
  plcol0 1;
  plmtex "b" 3.2 0.5 0.5 "Frequency";
  plmtex "t" 2.0 0.5 0.5 "Single Pole Low-Pass Filter";
  plcol0 2;
  plmtex "l" 5.0 0.5 0.5 "Amplitude (dB)";

  (* For the gridless case, put phase vs freq on same plot *)
  if plot_type = 0 then (
    plcol0 1;
    plwind (-2.0) 3.0 (-100.0) 0.0;
    plbox "" 0.0 0 "cmstv" 30.0 3;
    plcol0 3;
    plline freql phase;
    plcol0 3;
    plmtex "r" 5.0 0.5 0.5 "Phase shift (degrees)";
    ()
  )
  else
    ()

(*--------------------------------------------------------------------------*\
 * Illustration of logarithmic axes, and redefinition of window.
\*--------------------------------------------------------------------------*)

let () =
  (* Parse and process command line arguments *)
  ignore (plparseopts Sys.argv [|PL_PARSE_FULL|]);

  (* Initialize plplot *)
  plinit();
  plfont 2;

  (* Make log plots using two different styles. *)
  plot1 0;
  plot1 1;

  plend ();
  ()

