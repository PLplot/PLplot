(* $Id$

       Sample plots using date / time formatting for axes

  Copyright (C) 2007 Andrew Ross
  Copyright (C) 2008 Hezekiah M. Carty

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

let pi = atan 1.0 *. 4.0

(* Plot a model diurnal cycle of temperature *)
let plot1 () =
  (* Data points every 10 minutes for 1 day *)
  let npts = 73 in

  let xmin = 0.0 in
  let xmax = 60.0 *. 60.0 *. 24.0 in (* Number of seconds in a day *)
  let ymin = 10.0 in
  let ymax = 20.0 in

  let x =
    Array.init npts (fun i -> xmax *. float_of_int i /. float_of_int npts)
  in
  let y =
    Array.init npts (
      fun i ->
        15.0 -. 5.0 *. cos (2.0 *. pi *. (float_of_int i /. float_of_int npts))
    )
  in

  (* Set x error bars to +/- 5 minute *)
  let xerr1 = Array.map (fun xi -> xi -. 60.0 *. 5.0) x in
  let xerr2 = Array.map (fun xi -> xi +. 60.0 *. 5.0) x in
  (* Set y error bars to +/- 0.1 deg C *)
  let yerr1 = Array.map (fun yi -> yi -. 0.1) y in
  let yerr2 = Array.map (fun yi -> yi +. 0.1) y in

  pladv 0;

  (* Rescale major ticks marks by 0.5 *)
  plsmaj 0.0 0.5;
  (* Rescale minor ticks and error bar marks by 0.5 *)
  plsmin 0.0 0.5;

  plvsta ();
  plwind xmin xmax ymin ymax;

  (* Draw a box with ticks spaced every 3 hour in X and 1 degree C in Y. *)
  plcol0 1;
  (* Set time format to be hours:minutes *)
  pltimefmt "%H:%M";
  plbox "bcnstd" (3.0 *. 60.0 *. 60.0) 3 "bcnstv" 1.0 5;

  plcol0 3;
  pllab "Time (hours:mins)" "Temperature (degC)" "#frPLplot Example 29 - Daily temperature";

  plcol0 4;

  plline x y;

  plcol0 2;
  plerrx xerr1 xerr2 y;
  plcol0 3;
  plerry x yerr1 yerr2;

  (* Rescale major / minor tick marks back to default *)
  plsmin 0.0 1.0;
  plsmaj 0.0 1.0; 
  ()

(* Plot the number of hours of daylight as a function of day for a year *)
let plot2 () =
  (* Latitude for London *)
  let lat = 51.5 in

  let npts = 365 in

  let xmin = 0.0 in
  let xmax = float_of_int npts *. 60.0 *. 60.0 *. 24.0 in
  let ymin = 0.0 in
  let ymax = 24.0 in

  let x = Array.make npts 0.0 in
  let y = Array.make npts 0.0 in
  (* Formula for hours of daylight from
   * "A Model Comparison for Daylength as a Function of Latitude and
   * Day of the Year", 1995, Ecological Modelling, 80, pp 87-95. *)
  for j = 0 to npts - 1 do
    x.(j) <- float_of_int j *. 60.0 *. 60.0 *. 24.0;
    let p =
      asin (0.39795 *. cos (0.2163108 +. 2.0 *.
        atan (0.9671396 *. tan (0.00860 *. (float_of_int j -. 186.0))))
      )
    in
    let d =
      24.0 -. (24.0 /. pi) *. acos (( sin (0.8333 *. pi /. 180.0) +.
        sin (lat *. pi /. 180.0) *. sin p) /.
        (cos (lat *. pi /. 180.0) *. cos p)
      )
    in
    y.(j) <- d;
  done;

  plcol0 1;
  (* Set time format to be abbreviated month name followed by day of month *)
  pltimefmt "%b %d";
  plprec 1 1;
  plenv xmin xmax ymin ymax 0 40;


  plcol0 3;
  pllab "Date" "Hours of daylight" "#frPLplot Example 29 - Hours of daylight at 51.5N";

  plcol0 4;

  plline x y;

  plprec 0 0;
  ()

let plot3 () =
  (* Calculate seconds since the Unix epoch for 2005-12-01 UTC. *)
  (* Adopt the known result for POSIX-compliant systems. *)
  let tstart = 1133395200.0 in

  let npts = 62 in

  let xmin = tstart in
  let xmax = xmin +. float_of_int npts *. 60.0 *. 60.0 *. 24.0 in
  let ymin = 0.0 in
  let ymax = 5.0 in

  let x =
    Array.init npts (fun i -> xmin +. float_of_int i *. 60.0 *. 60.0 *. 24.0)
  in
  let y =
    Array.init npts (
      fun i ->
        1.0 +. sin (2.0 *. pi *. float_of_int i /. 7.0) +.
        exp (float_of_int (min i (npts - i)) /. 31.0)
    )
  in

  pladv 0;

  plvsta ();
  plwind xmin xmax ymin ymax;

  plcol0 1;
  (* Set time format to be ISO 8601 standard YYYY-MM-DD. Note that this is
     equivalent to %f for C99 compliant implementations of strftime. *)
  pltimefmt "%Y-%m-%d";
  (* Draw a box with ticks spaced every 14 days in X and 1 hour in Y. *)
  plbox "bcnstd" (14.0 *. 24.0 *. 60.0 *. 60.0) 14 "bcnstv" 1.0 4;

  plcol0 3;
  pllab "Date" "Hours of television watched" "#frPLplot Example 29 - Hours of television watched in Dec 2005 / Jan 2006";

  plcol0 4;

  (* Rescale symbol size (used by plpoin) by 0.5 *)
  plssym 0.0 0.5;
  plpoin x y 2;
  plline x y;
  ()

(*--------------------------------------------------------------------------*\
 * Draws several plots which demonstrate the use of date / time formats for
 * the axis labels.
 * Time formatting is done using the system strftime routine. See the
 * documentation of this for full details of the available formats.
 *
 * 1) Plotting temperature over a day (using hours / minutes)
 * 2) Plotting
 *
 * Note: Times are stored as seconds since the epoch (usually 1st Jan 1970).
\*--------------------------------------------------------------------------*)
let () =
  (* Parse command line arguments *)
  ignore (plparseopts Sys.argv [PL_PARSE_FULL]);

  (* Initialize plplot *)
  plinit ();

  plot1 ();

  plot2 ();

  plot3 ();

  (* Don't forget to call plend() to finish off! *)
  plend();
  ()

