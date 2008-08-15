(* $Id$

        Drawing "spirograph" curves - epitrochoids, cycolids, roulettes

   Copyright (C) 2007  Arjen Markus
   Copyright (C) 2008  Hezekiah M. Carty

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

open Plplot

let cycloid () = () (* TODO *)

let spiro params =
  let npnt = 20000 in
  let xcoord = Array.make (npnt + 1) 0.0 in
  let ycoord = Array.make (npnt + 1) 0.0 in

  (* Fill the coordinates *)

  let windings = int_of_float params.(3) in
  let steps = npnt / windings in
  let dphi = 8.0 *. acos (-1.0) /. float_of_int steps in

  (* This initialisation is safe! *)
  let xmin = ref 0.0 in
  let xmax = ref 0.0 in
  let ymin = ref 0.0 in
  let ymax = ref 0.0 in

  for i = 0 to windings * steps do
    let phi = float_of_int i *. dphi in
    let phiw = (params.(0) -. params.(1)) /. params.(1) *. phi in
    xcoord.(i) <- (params.(0) -. params.(1)) *. cos phi +. params.(2) *. cos phiw;
    ycoord.(i) <- (params.(0) -. params.(1)) *. sin phi -. params.(2) *. sin phiw;

    if !xmin > xcoord.(i) then xmin := xcoord.(i) else ();
    if !xmax < xcoord.(i) then xmax := xcoord.(i) else ();
    if !ymin > ycoord.(i) then ymin := ycoord.(i) else ();
    if !ymax < ycoord.(i) then ymax := ycoord.(i) else ();
  done;

  let scale = max (!xmax -. !xmin) (!ymax -. !ymin) in
  let xmin = -0.65 *. scale in
  let xmax = 0.65 *. scale in
  let ymin = -0.65 *. scale in
  let ymax = 0.65 *. scale in

  plwind xmin xmax ymin ymax;

  plcol0 1;
  let xcoord' = Array.sub xcoord 0 (1 + steps * windings) in
  let ycoord' = Array.sub ycoord 0 (1 + steps * windings) in
  plline xcoord' ycoord';
  ()

(*--------------------------------------------------------------------------*\
 * Generates two kinds of plots:
 *   - construction of a cycloid (animated)
 *   - series of epitrochoids and hypotrochoids
\*--------------------------------------------------------------------------*)
let () =
  (* R, r, p, N *)
  let params =
    [|
      [|21.0;  7.0;  7.0;  3.0|];  (* Deltoid *)
      [|21.0;  7.0; 10.0;  3.0|];
      [|21.0; -7.0; 10.0;  3.0|];
      [|20.0;  3.0;  7.0; 20.0|];
      [|20.0;  3.0; 10.0; 20.0|];
      [|20.0; -3.0; 10.0; 20.0|];
      [|20.0; 13.0;  7.0; 20.0|];
      [|20.0; 13.0; 20.0; 20.0|];
      [|20.0;-13.0; 20.0; 20.0|];
    |]
  in

  (* plplot initialization *)
  (* Parse and process command line arguments *)
  ignore (plparseopts Sys.argv [|PL_PARSE_FULL|]);

  (* Initialize plplot *)
  plinit ();

  (* Illustrate the construction of a cycloid *)
  cycloid ();

  (* Loop over the various curves
     First an overview, then all curves one by one *)
  (* Three by three window *)
  plssub 3 3;

  for i = 0 to 8 do
    pladv 0;
    plvpor 0.0 1.0 0.0 1.0;
    spiro params.(i);
  done;

  pladv 0;
  (* One window per curve *)
  plssub 1 1;

  for i = 0 to 8 do
    pladv 0;
    plvpor 0.0 1.0 0.0 1.0;
    spiro params.(i);
  done;

  (* Don't forget to call plend() to finish off! *)
  plend ();
  ()

