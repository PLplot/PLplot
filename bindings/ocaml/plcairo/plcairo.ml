(*
Copyright 2008, 2009  Hezekiah M. Carty

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

(** Support for the extcairo PLplot device from OCaml. *)

open Plplot
open Printf

(** A record to keep track of the Cairo surface and context information *)
type ('a, 'b) t = {
  width : float;
  height : float;
  surface : 'a Cairo.surface;
  context : Cairo.t;
  file : 'b option;
  (* Should each new page be cleared? *)
  clear : bool;
  (* What PLplot stream number is associated with this plot? *)
  plstream : int;
}

(** An alias for {t}. *)
type ('a, 'b) plplot_cairo_t = ('a, 'b) t

(** Provide PLplot with a Cairo context to plot on. *)
external set_cairo_context : Cairo.t -> unit = "ml_set_plplot_cairo_context"

(** Blit the contents of the given {plplot_cairo_t} to the given {Cairo.t}.
    Scale according to [`Width w]. [`Height h] or [`Both (w, h)]. *)
let blit_to_cairo ~dest ~dim ~xoff ~yoff plcairo =
  let sx, sy =
    match dim with
    | `Width w -> let scale = w /. plcairo.width in scale, scale
    | `Height h -> let scale = h /. plcairo.height in scale, scale
    | `Both (w, h) -> w /. plcairo.width, h /. plcairo.height
  in
  Cairo.save dest;
  begin
    Cairo.scale ~sx ~sy dest;
    Cairo.set_source_surface dest plcairo.surface xoff yoff;
    Cairo.paint dest;
  end;
  Cairo.restore dest;
  ()

(** [rasterize ?alpha plot f] applies the plotting function [f ()] to [plot],
    with the caveat that the output will be rasterized for all plot
    output drivers, including vector-based output drivers such as PS, PDF and
    SVG.
    The [alpha] parameter may be provided to make the rasterized overlay
    transparent, even if the current color palette is not.
    Note that the plotting done by [f ()] will not be antialiased. *)
let rasterize ?alpha plot f =
  (* Create a Cairo image surface and context to plot the rasterized image
     on.  This will be a duplicate in size and shape of the current plot's
     surface.  Leave the background transparent, so only the plotted image
     is transfered over to the main plot surface. *)
  let img_sfc =
    Cairo.image_surface_create
      Cairo.FORMAT_ARGB32
      ~width:(int_of_float plot.width)
      ~height:(int_of_float plot.height)
  in
  let img_context = Cairo.create img_sfc in
  (* Assign the transformation matrix from the main plot context to maintain
     consistency.  It will also have to be applied to the main plot context
     again once back to it. *)
  let plot_matrix = Cairo.get_matrix plot.context in
  Cairo.set_matrix img_context plot_matrix;
  set_cairo_context img_context;
  (* Make sure antialiasing is turned OFF for the.  The output looks bad
     otherwise. *)
  Cairo.set_antialias img_context Cairo.ANTIALIAS_NONE;
  (* This will now be plotted on to the Cairo image surface. *)
  f ();
  (* Blit the raster image on to the main plot surface *)
  Cairo.set_source_surface plot.context img_sfc 0.0 0.0;
  let () =
    match alpha with
    | None -> Cairo.paint plot.context
    | Some a -> Cairo.paint_with_alpha plot.context a
  in
  (* Now set PLplot back to using the proper plot context. *)
  set_cairo_context plot.context;
  Cairo.set_matrix plot.context plot_matrix;
  (* Don't forget to do a [Cairo.surface_finish] when everything is done!
     That isn't done here because the plot may not be finished yet. *)
  ()

(** [rimage ?alpha plot] takes the same arguments as {Plplot.plimagefr} and
    works in the same way, except that the output will be rasterized for all
    plot output drivers, including vector-based output drivers such as PS, PDF
    and SVG.  {Plplot.plimagefr} is used internally, so image distortions and
    transformations are possible in the same way as for that function.  The
    [alpha] parameter may be provided to make the raster image overlay
    transparent even if the current color palette is not.
    This is somewhat deprecated, as [plimage] and [plimagefr] now act this way
    by default. *)
let rimage ?alpha plot m xmin xmax ymin ymax zmin zmax valuemin valuemax =
  rasterize ?alpha plot
    (fun () -> plimagefr m xmin xmax ymin ymax zmin zmax valuemin valuemax);
  ()

(*
(** FIXME TODO XXX : I don't think this works properly yet.
    [plxy_to_cairoxy ~x ~y context] will convert the plot world
    coordinates [x, y] to Cairo device coordinates. *)
let plxy_to_cairoxy context ~x ~y =
  (* Normalized device coordinates *)
  let nxmin, nxmax, nymin, nymax = plgvpd () in
  (* World (plot-space) coordinates *)
  let wxmin, wxmax, wymin, wymax = plgvpw () in
  (* Cairo device coordinates *)
  let xmin = context.width *. nxmin in
  let xmax = context.width *. nxmax in
  let ymin = context.height *. nymin in
  let ymax = context.height *. nymax in
  (* World coordinates -> Cairo coordinates *)
  xmin +. ((xmax -. xmin) *. (x /. (wxmax -. wxmin))),
  ymin +. ((ymax -. ymin) *. (y /. (wymax -. wymin)))
*)

(** [with_stream ?stream f] calls [f ()] with [stream] as the active
    plotting stream if [stream] is present.  Otherwise it just calls
    [f ()]. *)
let with_stream ?stream f =
  match stream with
  | None -> f ()
  | Some s ->
      let old_stream = plgstrm () in
      plsstrm s.plstream;
      let result = f () in
      plsstrm old_stream;
      result

(** [new_page p] will advance the Cairo context associted with [p] to a
    new page, for devices which support this. *)
let new_page p =
  Cairo.show_page p.context;
  if p.clear then with_stream ~stream:p (fun () -> pladv 0; plclear ()) else ()

(** The following 4 functions provide a relatively simple way to setup an
    appropriate Cairo surface for use with this library and the extcairo
    driver.  They should be passed as the [init] argument to the
    [plinit_cairo] function.*)
let pscairo ~width ~height filename =
  let outfile =
    match filename with
    | Some f -> open_out f
    | None -> raise (Invalid_argument "pscairo needs a filename")
  in
  Some outfile,
  Cairo_ps.surface_create_for_channel outfile
    ~width_in_points:(float_of_int width)
    ~height_in_points:(float_of_int height)

let pdfcairo ~width ~height filename =
  let outfile =
    match filename with
    | Some f -> open_out f
    | None -> raise (Invalid_argument "pdfcairo needs a filename")
  in
  Some outfile,
  Cairo_pdf.surface_create_for_channel outfile
    ~width_in_points:(float_of_int width)
    ~height_in_points:(float_of_int height)

let imagecairo ~width ~height (filename : string option) =
  filename,
  Cairo.image_surface_create Cairo.FORMAT_RGB24 ~width ~height

let imagecairo_rgba ~width ~height (filename : string option) =
  filename,
  Cairo.image_surface_create Cairo.FORMAT_ARGB32 ~width ~height

(** [init_cairo ?filename ~clear ~width ~height init] creates a Cairo context
    and associates it with a PLplot stream.  A new plot stream is created in the
    process.  The current plot stream is left active by this function. *)
let init_cairo ?filename ~clear ~width ~height init =
  let file, sfc = init ~width ~height filename in
  let context = Cairo.create sfc in
  (* Associate the Cairo context with PLplot, then initialize PLplot *)
  let old_stream = plgstrm () in
  let new_stream = plmkstrm () in
  plsdev "extcairo";
  ignore (plsetopt "geometry" (sprintf "%dx%d" width height));
  (* Make the plot have a white background. *)
  plscol0 0 255 255 255;
  plscol0 15 0 0 0;
  plinit ();
  (* Associate our new Cairo context with the PLplot stream. *)
  set_cairo_context context;
  if clear then (pladv 0; plclear ()) else ();
  (* Return the Cairo information so that the plot surface can be manipulated
     and/or saved late. *)
  plsstrm old_stream;
  {
    width = float_of_int width;
    height = float_of_int height;
    surface = sfc;
    context = context;
    file = file;
    clear = clear;
    plstream = new_stream;
  }

(** [make_active plcairo] sets PLplot to using the [plcairo] context
    and associated stream as its default plotting stream. *)
let make_active plcairo = plsstrm plcairo.plstream

(** [finish plcairo] calls [Cairo.surface_finish] on the Cairo surface
    associated with [plcairo]. *)
let finish plcairo =
  Cairo.surface_finish plcairo.surface;
  ()

(** [save_image plcairo] saves the plot surface in [plcairo]
    as a png to its associated file.  {Plplot.plend} should be called first or
    the plotting may not be complete! *)
let save_image plcairo =
  match plcairo.file with
  | Some filename ->
      Cairo_png.surface_write_to_file plcairo.surface filename;
      ()
  | None -> raise (Invalid_argument "No filename is associated with this plot")

(** [save_image_as plcairo filename] - Like [save_image], but save the plot to
    the specified file rather than to a filename defined at plot creation. *)
let save_image_as plcairo filename =
  Cairo_png.surface_write_to_file plcairo.surface filename;
  ()

(** [save_to_file plcairo] saves the plot surface in [plcairo] to its
    associated filename.
    Raises [Invalid_argument] if [plcairo] does not have a filename associated
    with it. *)
let save_to_file plcairo =
  match plcairo.file with
  | Some fout ->
      finish plcairo;
      close_out fout;
      ()
  | None -> raise (Invalid_argument "No file is associated with this plot")

(** NOTE XXX FIXME : This function will almost definitely change in a future
    revision.
    [copy_plot plcairo driver filename] copies the plot stream from
    [plcairo] to a new output stream, using the plot driver [driver], saving the
    output to [filename]. *)
let copy_plot plcairo driver filename =
  (* Get the current stream so we can restore it when done. *)
  let old_stream = plgstrm () in
  (* Make a new plot stream *)
  ignore (plmkstrm ());
  plsdev driver;
  plsfnam filename;
  let geometry = sprintf "%.0fx%.0f" plcairo.width plcairo.height in
  ignore (plsetopt "geometry" geometry);
  plscol0 0 255 255 255;
  plscol0 15 0 0 0;
  plinit ();
  plcpstrm plcairo.plstream true;
  plreplot ();
  plend1 ();
  plsstrm old_stream;
  ()

