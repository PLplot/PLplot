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
open Printf

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

let plot4 () =
  (* TAI-UTC (seconds) as a function of time.
     Use Besselian epochs as the continuous time interval just to prove
     this does not introduce any issues. *)

  (* Use the definition given in http://en.wikipedia.org/wiki/Besselian_epoch
   * B = 1900. + (JD -2415020.31352)/365.242198781
   * ==> (as calculated with aid of "bc -l" command)
   * B = (MJD + 678940.364163900)/365.242198781
   * ==>
   * MJD = B*365.24219878 - 678940.364163900 *)
  let scale = 365.242198781 in
  let offset1 = -678940.0 in
  let offset2 = -0.3641639 in
  plconfigtime scale offset1 offset2 0x0 false 0 0 0 0 0 0.0;

  let xmin = ref 0.0 in
  let xmax = ref 0.0 in
  let npts = ref 0 in
  let ymin = ref 0.0 in
  let ymax = ref 0.0 in
  let time_format = ref "" in
  let if_TAI_time_format = ref false in
  let title_suffix = ref "" in
  let xtitle = ref "" in
  let xlabel_step = ref 0.0 in

  for kind = 0 to 6 do
    let () =
      match kind with
      | 0 ->
          xmin := plctime 1950 0 2 0 0 0.0;
          xmax := plctime 2020 0 2 0 0 0.0;
          npts := 70 * 12 + 1;
          ymin := 0.0;
          ymax := 36.0;
          time_format := "%Y%";
          if_TAI_time_format := true;
          title_suffix := "from 1950 to 2020";
          xtitle := "Year";
          xlabel_step := 10.0;
      | 1
      | 2 ->
          xmin := plctime 1961 7 1 0 0 (1.64757 -. 0.20);
          xmax := plctime 1961 7 1 0 0 (1.64757 +. 0.20);
          npts := 1001;
          ymin := 1.625;
          ymax := 1.725;
          time_format := "%S%2%";
          title_suffix := "near 1961-08-01 (TAI)";
          xlabel_step := 0.05 /. (scale *. 86400.0);
          if kind = 1 then (
            if_TAI_time_format := true;
            xtitle := "Seconds (TAI)";
          )
          else (
            if_TAI_time_format := false;
            xtitle := "Seconds (TAI) labelled with corresponding UTC";
          )
      | 3
      | 4 ->
          xmin := plctime 1963 10 1 0 0 (2.6972788 -. 0.20);
          xmax := plctime 1963 10 1 0 0 (2.6972788 +. 0.20);
          npts := 1001;
          ymin := 2.55;
          ymax := 2.75;
          time_format := "%S%2%";
          title_suffix := "near 1963-11-01 (TAI)";
          xlabel_step := 0.05 /. (scale *. 86400.0);
          if kind = 3 then (
            if_TAI_time_format := true;
            xtitle := "Seconds (TAI)";
          )
          else (
            if_TAI_time_format := false;
            xtitle := "Seconds (TAI) labelled with corresponding UTC";
          )
      | 5
      | 6 ->
          xmin := plctime 2009 0 1 0 0 (34.0 -. 5.0);
          xmax := plctime 2009 0 1 0 0 (34.0 +. 5.0);
          npts := 1001;
          ymin := 32.5;
          ymax := 34.5;
          time_format := "%S%2%";
          title_suffix := "near 2009-01-01 (TAI)";
          xlabel_step := 1.0 /. (scale *. 86400.0);
          if kind = 5 then (
            if_TAI_time_format := true;
            xtitle := "Seconds (TAI)";
          )
          else (
            if_TAI_time_format := false;
            xtitle := "Seconds (TAI) labelled with corresponding UTC";
          )
      | _ -> failwith "Bad plot kind"
    in

    let x = Array.make 1001 0.0 in
    let y = Array.make 1001 0.0 in

    for i = 0 to !npts - 1 do
      x.(i) <-
        !xmin +. float_of_int i *. (!xmax -. !xmin) /. (float_of_int (!npts - 1));
      plconfigtime scale offset1 offset2 0x0 false 0 0 0 0 0 0.0;
      let tai = x.(i) in
      (* The "full" calls to plbtime are commented out to avoid OCaml
         compilation warnings. *)
      (*
      let tai_year, tai_month, tai_day, tai_hour, tai_min, tai_sec =
        plbtime tai
      in
      *)
      ignore (plbtime tai);
      plconfigtime scale offset1 offset2 0x2 false 0 0 0 0 0 0.0;
      let utc_year, utc_month, utc_day, utc_hour, utc_min, utc_sec =
        plbtime tai
      in
      plconfigtime scale offset1 offset2 0x0 false 0 0 0 0 0 0.0;
      let utc = plctime utc_year utc_month utc_day utc_hour utc_min utc_sec in
      y.(i) <- (tai -. utc) *. scale *. 86400.0;
    done;

    pladv 0;
    plvsta ();
    plwind !xmin !xmax !ymin !ymax;
    plcol0 1;
    plconfigtime scale offset1 offset2
      (if !if_TAI_time_format then 0x0 else 0x2)
      false 0 0 0 0 0 0.0;
    pltimefmt !time_format;
    plbox "bcnstd" !xlabel_step 0 "bcnstv" 0.0 0;
    plcol0 3;
    let title = sprintf "#frPLplot Example 29 - TAI-UTC %s" !title_suffix in
    pllab !xtitle "TAI-UTC (sec)" title;

    plcol0 4;

    plline (Array.sub x 0 !npts) (Array.sub y 0 !npts);
  done;
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

  plot4 ();

  (* Don't forget to call plend() to finish off! *)
  plend();
  ()

