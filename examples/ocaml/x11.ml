(*
Copyright 2007  Hezekiah M. Carty

This file is part of PLplot.

PLplot is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

PLplot is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with PLplot.  If not, see <http://www.gnu.org/licenses/>.
*)

(* A more or less direct port of the PLplot example 11 *)
open Plplot

let xpts = 35 (* Data points in x *)
let ypts = 46 (* Data points in y *)
let nlevel = 10 (* Contouring levels *)


let alt = [|33.0; 17.0|]
let az = [|24.0; 115.0|]
let opt = [|3; 3|]
let title = [|"#frPLplot Example 11 - Alt=33, Az=24, Opt=3";
              "#frPLplot Example 11 - Alt=17, Az=115, Opt=3"|]

let cmap1_init () =
  let i = [|0.0; 1.0|] in (* left+right bounds *)
  let h = [|240.0; 0.0|] in
  let l = [|0.6; 0.6|] in
  let s = [|0.8; 0.8|] in
  plscmap1n 256;
  plscmap1l 0 i h l s None;
  ()

(*--------------------------------------------------------------------------*\
 * main
 *
 * Does a series of mesh plots for a given data set, with different
 * viewing options in each plot.
\*--------------------------------------------------------------------------*)

let () =
  (* Initialize plplot *)
  plinit ();

  (* Convenience, because I'm lazy *)
  let (!.) = float_of_int in

  let x =
    Array.init xpts (
      fun i ->
        3. *. (!.i -. (!.xpts /. 2.)) /. (!.xpts /. 2.)
    )
  in
  let y =
    Array.init ypts (
      fun i ->
        3. *. (!.i -. (!.ypts /. 2.)) /. (!.ypts /. 2.)
    )
  in

  let z = Array.make_matrix xpts ypts 0.0 in

  for i = 0 to xpts - 1 do
    let xx = x.(i) in
    for j = 0 to ypts - 1 do
      let yy = y.(j) in
      z.(i).(j) <-
        3. *. (1. -. xx) *. (1. -. xx) *.
        exp (-.(xx *. xx) -. (yy +. 1.) *. (yy +. 1.)) -.
        10. *. (xx /. 5. -. xx**3. -. yy**5.) *.
        exp (-.(xx *. xx) -. (yy *. yy)) -.
        1. /. 3. *. exp (-.(xx +. 1.) *. (xx +. 1.) -. (yy *. yy));
    done
  done;

  let zmin, zmax = plMinMax2dGrid z in
  let step = (zmax -. zmin) /. !.(nlevel + 1) in
  let clevel = Array.init nlevel (fun i -> zmin +. step +. step *. !.i) in

  cmap1_init();
  for k = 0 to 1 do
    for i = 0 to 3 do
      pladv 0;
      plcol0 1;
      plvpor 0.0 1.0 0.0 0.9;
      plwind (-1.0) 1.0 (-1.0) 1.5;
      plw3d 1.0 1.0 1.2 (-3.0) 3.0 (-3.0) 3.0 zmin zmax alt.(k) az.(k);
      plbox3 "bnstu" "x axis" 0.0 0
             "bnstu" "y axis" 0.0 0
             "bcdmnstuv" "z axis" 0.0 4;

      plcol0 2;

      let () =
        match i with
            0 ->
              (* wireframe plot *)
              plmesh x y z opt.(k);
          | 1 ->
              (* magnitude colored wireframe plot *)
              plmesh x y z (opt.(k) lor 4);
          | 2 ->
              (* magnitude colored wireframe plot with sides *)
              plot3d x y z (opt.(k) lor 4) 1;
          | 3 ->
              (* magnitude colored wireframe plot with base contour *)
              plmeshc x y z (opt.(k) lor 4 lor 8)
                clevel;
          | _ -> raise (Failure "You let i get too big!")
      in

      plcol0 3;
      plmtex "t" 1.0 0.5 0.5 title.(k);
    done
  done;

  (* Clean up *)
  
  plend ();
  ()
