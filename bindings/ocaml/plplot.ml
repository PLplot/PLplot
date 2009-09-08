(*
Copyright 2009  Hezekiah M. Carty

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

include Plplot_core
open Printf

(** [plcalc_device x y] will give the device position, in normalized device
    coordinates, of the world coordinates (x, y). *)
let plcalc_device x y =
  let xmin, xmax, ymin, ymax = plgvpw () in
  let dev_xmin, dev_xmax, dev_ymin, dev_ymax = plgvpd () in
  let width = xmax -. xmin in
  let height = ymax -. ymin in
  let dev_width = dev_xmax -. dev_xmin in
  let dev_height = dev_ymax -. dev_ymin in
  ((x -. xmin) /. width) *. dev_width +. dev_xmin,
  ((y -. ymin) /. height) *. dev_height +. dev_ymin

(** [plfullcanvas ()] maximizes the plot viewport and window.  Dimensions are
    set to (0.0, 0.0) to (1.0, 1.0). *)
let plfullcanvas () =
  plvpor 0.0 1.0 0.0 1.0;
  plwind 0.0 1.0 0.0 1.0;
  ()

(** Draw an unfilled polygon.  The only functional difference between this and
    {plline} is that this function will close the given polygon, so there is no
    need to duplicate points to have a closed figure. *)
let plpolyline xs ys =
  plline xs ys;
  (* Close off the polygon. *)
  pljoin xs.(0) ys.(0) xs.(Array.length xs - 1) ys.(Array.length ys - 1)

(** Support modules for Plot and Quick_plot *)

module Option = struct
  let may f o =
    match o with
    | Some x -> f x
    | None -> ()

  let default x_default o =
    match o with
    | Some x -> x
    | None -> x_default

  let map_default f x_default o =
    match o with
    | Some x -> f x
    | None -> x_default
end

module Array_ext = struct
  let matrix_dims m =
    let ni = Array.length m in
    if ni = 0 then 0, 0 else (
      let nj = Array.length (Array.unsafe_get m 0) in
      for i = 1 to ni - 1 do
        if Array.length (Array.unsafe_get m i) <> nj
        then invalid_arg "Non-rectangular array"
      done;
      ni, nj
    )

  (** [range ~n a b] returns an array with elements ranging from [a] to [b]
      in [n] total elements. *)
  let range ~n a b =
    let step = (b -. a) /. (float n -. 1.0) in
    Array.init n (
      fun i ->
        a +. float i *. step
    )

  (** [reduce f a] performs a fold of [f] over [a], using only the elements of
      [a]. *)
  let reduce f a =
    if Array.length a = 0 then
      invalid_arg "reduce: empty array"
    else (
      let acc = ref a.(0) in
      for i = 1 to Array.length a - 1 do acc := f !acc a.(i) done;
      !acc
    )
end

(** Work-in-progress easy plotting library for PLplot.  The intention is to
    provide a PLplot interface which is closer in style to other OCaml
    libraries. *)
module Plot = struct
  let ( |? ) o x_default = Option.default x_default o

  (** [verify_arg x s] will raise [Invalid_arg s] if [x] is false.  Otherwise,
      it does nothing. *)
  let verify_arg x s = if x then () else (invalid_arg s)

  type 'a plot_side_t =
    | Top of 'a | Bottom of 'a
    | Left of 'a | Right of 'a

  type axis_options_t =
    | Axis
    | Frame0 | Frame1
    | Time
    | Fixed_point
    | Major_grid | Minor_grid
    | Invert_ticks
    | Log
    | Unconventional_label
    | Label
    | Custom_label
    | Minor_ticks | Major_ticks
    | Vertical_label

  type stream_t = {
    stream : int;
    x_axis : axis_options_t list;
    y_axis : axis_options_t list;
  }

  type color_t =
    | White
    | Red
    | Yellow
    | Green
    | Gray
    | Blue
    | Light_blue
    | Purple
    | Pink
    | Black
    | Brown
    | Index_color of int

  type map_t =
    | Globe_outline
    | USA_outline
    | Countries_outline
    | All_outline

  type pltr_t = float -> float -> float * float

  type plot_t =
    (* Standard plot elements *)
    | Arc of (color_t * float * float * float * float * float * float * bool)
    | Contours of (color_t * pltr_t * float array * float array array)
    | Image of image_t
    | Image_fr of (image_t * (float * float))
    | Join of (color_t * float * float * float * float)
    | Lines of (string option * color_t * float array * float array)
    | Map of (color_t * map_t * float * float * float * float)
    | Points of
      (string option * color_t * float array * float array * int * float)
    | Polygon of (color_t * float array * float array * bool)
    | Text of (color_t * string * float * float * float * float * float)
    | Text_outside of (color_t * string * float plot_side_t * float * float * bool)
    (* Set/clear coordinate transforms *)
    | Set_transform of pltr_t
    | Clear_transform
    (* Custom functions *)
    | Custom of (unit -> unit)
    (* Embedded list of plottable elements *)
    | List of plot_t list
  and image_t = (float * float) option * float * float * float * float * float array array

  type plot_device_family_t =
    | Cairo
    | Qt
    | Core
    | Wx
  type plot_device_t =
    | Pdf of plot_device_family_t
    | Png of plot_device_family_t
    | Ps of plot_device_family_t
    | Svg of plot_device_family_t
    | Window of plot_device_family_t
    | External of int
    | Other_device of string
  type plot_scaling_t = Preset | Greedy | Equal | Equal_square

  type color_palette_t =
    | Indexed of string
    | Continuous of (string * bool)

  let default_axis_options =
    [Frame0; Frame1; Major_ticks; Minor_ticks; Invert_ticks; Label]

  (** Convert a color to a (red, green, blue) triple *)
  let rgb_of_color = function
    | White -> 255, 255, 255
    | Red -> 255, 0, 0
    | Yellow -> 255, 255, 0
    | Green -> 0, 255, 0
    | Gray -> 200, 200, 200
    | Blue -> 0, 0, 255
    | Light_blue -> 0, 255, 255
    | Purple -> 160, 0, 213
    | Pink -> 255, 0, 255
    | Black -> 0, 0, 0
    | Brown -> 165, 42, 42
    | Index_color i -> plgcol0 i

  let string_of_map_t = function
    | Globe_outline -> "globe"
    | USA_outline -> "usa"
    | Countries_outline -> "cglobe"
    | All_outline -> "usaglobe"

  (** An internal function for converting a scaling variant value to the
      associated PLplot integer value. *)
  let int_of_scaling = function
    | Preset -> -1     (* Scaling must be set beforehand *)
    | Greedy -> 0      (* Use as much of the plot space as possible *)
    | Equal -> 1       (* Square aspect ratio *)
    | Equal_square -> 2 (* Square aspect ratio and square plot area *)


  (** Get the suffix string which matches the given device family *)
  let string_of_device_family = function
    | Cairo -> "cairo"
    | Qt -> "qt"
    | Core -> ""
    | Wx -> ""

  (** Returns the string to pass to plsdev and a boolean value indicating if
      the device is interactive or not. *)
  let devstring_of_plot_device = function
    | External _ -> "N/A", false
    | Other_device s -> s, false
    | Pdf family -> "pdf" ^ string_of_device_family family, false
    | Png family -> "png" ^ string_of_device_family family , false
    | Ps family -> "ps" ^ string_of_device_family family, false
    | Svg family -> "svg" ^ string_of_device_family family, false
    | Window family -> (
      match family with
      | Cairo -> "xcairo"
      | Qt -> "qtwidget"
      | Core -> "xwin"
      | Wx -> "wxwidgets"
      ), true

  let recommended_extension = function
    | Png _ -> ".png"
    | Ps _ -> ".ps"
    | Pdf _ -> ".pdf"
    | Svg _ -> ".svg"
    | Window _ -> invalid_arg "interactive plot device"
    | External _ -> invalid_arg "external plot device"
    | Other_device _ -> invalid_arg "other device, unknown extension"

  (** Make a new stream, without disturbing the current plot state. *)
  let make_stream ?stream () =
    let this_stream =
      match stream with
      | None ->
          let old_stream = plgstrm () in
          let new_stream = plmkstrm () in
          plsstrm old_stream;
          new_stream
      | Some s -> s
    in
    { stream = this_stream;
      x_axis = default_axis_options; y_axis = default_axis_options; }

  (** [with_stream ?stream f] calls [f ()] with [stream] as the active
      plotting stream if [stream] is present.  Otherwise it just calls
      [f ()]. *)
  let with_stream ?stream f =
    match stream with
    | None -> f ()
    | Some s ->
        let old_stream = plgstrm () in
        plsstrm s.stream;
        let result = f () in
        plsstrm old_stream;
        result

  (** Set the plotting color (color scale 0). NOTE that these are for the
      ALTERNATE color palette, not the DEFAULT color palette. *)
  let set_color ?stream c =
    let n =
      match c with
      | White -> 0
      | Red -> 3
      | Yellow -> 13
      | Green -> 12
      | Gray -> 10
      | Blue -> 2
      | Light_blue -> 11
      | Purple -> 15
      | Pink -> 14
      | Black -> 1
      | Brown -> 4
      | Index_color i -> i
    in
    with_stream ?stream (fun () -> plcol0 n)

  (** [set_color_scale ?stream rev colors] sets the color scale 1 (images and
      shade plots) using a linear interpolation between the given list of
      colors.  If [rev] is true then the scale goes in the reverse order. *)
  let set_color_scale ?stream rev colors =
    let cs = Array.map rgb_of_color colors in
    let r, g, b =
      Array.map (fun (r, _, _) -> float_of_int r /. 255.0) cs,
      Array.map (fun (_, g, _) -> float_of_int g /. 255.0) cs,
      Array.map (fun (_, _, b) -> float_of_int b /. 255.0) cs
    in
    let positions = Array_ext.range ~n:(Array.length cs) 0.0 1.0 in
    let rev =
      if rev then Some (Array.map (fun _ -> true) cs) else None
    in
    with_stream ?stream (fun () -> plscmap1l true positions r g b rev);
    ()

  (** Start a new page *)
  let start_page ?stream x0 x1 y0 y1 axis_scaling =
    with_stream ?stream (
      fun () ->
        (* Start with a black plotting color. *)
        set_color Black;
        plenv x0 x1 y0 y1 (int_of_scaling axis_scaling) (-2);
    )

  (** Load a color palette from a file on disk *)
  let load_palette ?stream which =
    with_stream ?stream (
      fun () ->
        match which with
        | Indexed file -> plspal0 file
        | Continuous (file, segmented) -> plspal1 file segmented
    )

  (** [init ?filename ?size ?pages x0 x1 y0 y1 axis_scaling device] creates a
    new plot instance.  [size] is the size of the plot device.  If a [filename]
    is provided then an appropriate file extension will be added, based on the
    given [device]. [pages] determines how many plots will be on a single
    device page. *)
  let init ?filename ?size ?pages ?pre x0 x1 y0 y1 axis_scaling device =
    (* Make a new plot stream. *)
    let stream, init =
      match device with
      | External stream -> make_stream ~stream (), false
      | _ -> make_stream (), true
    in
    (* If an external stream is provided, assume all initialization has been
       performed before we get here. *)
    if init then with_stream ~stream (
      fun () ->
        (* Set the output file name *)
        let dev, is_interactive = devstring_of_plot_device device in
        Option.may (
          fun name_base ->
            plsfnam (
              name_base ^ (
                if is_interactive then ""
                else (
                  (* Only append an extension if the filename does not have
                     one already. *)
                  let extension = recommended_extension device in
                  if Filename.check_suffix name_base extension then ""
                  else extension
                )
              )
            );
        ) filename;

        (* Set the physical page dimensions for the plot *)
        Option.may (
          fun (x, y) ->
            let dim_string = sprintf "%dx%d" x y in
            ignore (plsetopt "geometry" dim_string)
        ) size;
        plsdev dev;

        (* Run the requested pre-plot-initialization function and then
           initialize the new plot. *)
        Option.may (fun f -> f ()) pre;
        plinit ();
    );
    (* If requested, set up multiple sub-pages. *)
    Option.may (fun (x, y) -> with_stream ~stream (fun () -> plssub x y)) pages;
    start_page ~stream x0 x1 y0 y1 axis_scaling;
    stream

  (** [make_stream_active stream] makes [stream] the active plot stream.  If
      there is another active plot stream its identity is not saved. *)
  let make_stream_active stream = plsstrm stream.stream

  (** {3 Simplified plotting interface} *)

  (** [arc ?fill color x y a b angle1 angle2 rotation] *)
  let arc ?(fill = false) color x y a b angle1 angle2 =
    Arc (color, x, y, a, b, angle1, angle2, fill)

  (** [circle ?fill color x y r] - A special case of [arc]. *)
  let circle ?fill color x y r = arc ?fill color x y r r 0.0 360.0

  (** [contours color pltr contours data] *)
  let contours color pltr contours data =
    Contours (color, pltr, contours, data)

  (** [image ?range sw ne image] *)
  let image ?range (x0, y0) (x1, y1) data = Image (range, x0, y0, x1, y1, data)

  (** [imagefr ?range ~scale sw ne image] *)
  let imagefr ?range ~scale (x0, y0) (x1, y1) data =
    Image_fr ((range, x0, y0, x1, y1, data), scale)

  (** [join color x0 y0 x1 y1] *)
  let join color x0 y0 x1 y1 = Join (color, x0, y0, x1, y1)

  (** [lines ?label color xs ys] *)
  let lines ?label color xs ys = Lines (label, color, xs, ys)

  (** [map ?sw ?ne color outline] *)
  let map ?sw ?ne color outline =
    let x0, y0 = sw |? (-180.0, -90.0) in
    let x1, y1 = ne |? (180.0, 90.0) in
    Map (color, outline, x0, y0, x1, y1)

  (** [points ?label ?scale color xs ys symbol] *)
  let points ?label ?scale color xs ys symbol =
    Points (label, color, xs, ys, symbol, scale |? 1.0)

  (** [polygon ?fill color xs ys fill] *)
  let polygon ?(fill = false) color xs ys = Polygon (color, xs, ys, fill)

  (** [rectangle ?fill color (x0, y0) (x1, y1)] *)
  let rectangle ?(fill = false) color (x0, y0) (x1, y1) =
    polygon ~fill color [|x0; x1; x1; x0; x0|] [|y0; y0; y1; y1; y0|]

  (** [text ?dx ?dy ?just ?color s x y] *)
  let text ?(dx = 0.0) ?(dy = 0.0) ?(just = 0.5) ?(color = Black) s x y =
    Text (color, s, x, y, dx, dy, just)

  (** [text_outside ?just side displacement s] *)
  let text_outside ?(just = 0.5) ?(perp = false) ?(color = Black) side displacement s =
    Text_outside (color, s, side, displacement, just, perp)

  (** [func ?point ?step color f (min, max)] plots the function [f] from 
      [x = min] to [x = max].  [step] can be used to tighten or coarsen the
      sampling of plot points. *)
  let func ?point ?step color f (min, max) =
    let step =
      match step with
      | None -> (max -. min) /. 100.0
      | Some s -> s
    in
    let xs =
      Array_ext.range ~n:(int_of_float ((max -. min) /. step) + 1) min max
    in
    let ys = Array.map f xs in
    match point with
    | Some p -> points color xs ys p
    | None -> lines color xs ys

  (** [transform f] *)
  let transform f = Set_transform f

  (** [clear_transform] *)
  let clear_transform = Clear_transform

  (** [custom f] *)
  let custom f = Custom f

  (** [list l] *)
  let list l = List l

  (** Get the font character height in plot world coordinates *)
  let character_height ?stream () =
    with_stream ?stream (
      fun () ->
        let (_, char_height_mm) = plgchr () in
        (* Normalized viewport dims * dimensions in mm = world size in mm *)
        let (_, _, vymin, vymax) = plgvpd () in
        let vy = vymax -. vymin in
        let (_, _, mymin, mymax) = plgspa () in
        let mm_y = mymax -. mymin in
        let world_height_mm = (mm_y *. vy) in
        (* Character height (mm) / World height (mm) = Normalized char height *)
        let char_height_norm = char_height_mm /. world_height_mm in
        (* Normalized character height * World height (world) =
           Character height (world) *)
        let (_, _, wymin, wymax) = plgvpw () in
        let world_y = wymax -. wymin in
        char_height_norm *. world_y
    )

  (** Draw a legend with the upper-left corner at (x,y) *)
  let draw_legend ?stream ?line_length ?x ?y names colors =
    with_stream ?stream (
      fun () ->
      (* Save the current color to restore at the end *)
      let old_col0 = plg_current_col0 () in

      (* Get viewport world-coordinate limits *)
      let xmin, xmax, ymin, ymax = plgvpw () in
      let normalized_to_world nx ny =
        xmin +. nx *. (xmax -. xmin),
        ymin +. ny *. (ymax -. ymin)
      in

      (* Get world-coordinate positions of the start of the legend text *)
      let line_x = x |? 0.6 in
      let line_y = y |? 0.95 in
      let line_x_end = line_x +. 0.1 in
      let line_x_world, line_y_world = normalized_to_world line_x line_y in
      let line_x_end_world, _ = normalized_to_world line_x_end line_y in
      let text_x = line_x_end +. 0.01 in
      let text_y = line_y in
      let text_x_world, text_y_world = normalized_to_world text_x text_y in

      let character_height = character_height () in
      let ty = ref (text_y_world -. character_height) in

      (* Draw each line type with the appropriate label *)
      List.iter2 (
        fun n c ->
          set_color c;
          plline [|line_x_world; line_x_end_world|] [|!ty; !ty|];
          set_color Black;
          plptex text_x_world !ty 0.0 0.0 0.0 n;
          ty := !ty -. (1.5 *. character_height);
          ()
      ) names colors;

      (* Restore old color *)
      plcol0 old_col0;
      ()
    )

  (** [plot stream l] plots the data in [l] to the plot [stream]. *)
  let rec plot ?stream plottable_list =
    (* TODO: Add legend support. *)
    (*
    let dims ?(expand = true) l =
      let coef = 5.0e-5 in
      let one_dims = function
        | Pts (xs, ys, _)
        | Lines (xs, ys) ->
            (Array.reduce min xs, Array.reduce min ys),
            (Array.reduce max xs, Array.reduce max ys)
      in
      let all_dims = List.map one_dims l in
      let min_dims, max_dims = List.split all_dims in
      let xmins, ymins = List.split min_dims in
      let xmaxs, ymaxs = List.split max_dims in
      let (xmin, ymin), (xmax, ymax) =
        (List.reduce min xmins, List.reduce min ymins),
        (List.reduce max xmaxs, List.reduce max ymaxs)
      in
      let diminish n = n -. n *. coef in
      let extend n = n +. n *. coef in
      if expand then
        (diminish xmin, diminish ymin),
        (extend xmax, extend ymax)
      else
        (xmin, ymin), (xmax, ymax)
    in
    *)
    let set_color_in c f =
      let old_color = plg_current_col0 () in
      set_color c;
      f ();
      plcol0 old_color;
      ()
    in
    let plot_arc (color, x, y, a, b, angle1, angle2, fill) =
      set_color_in color (
        fun () -> plarc x y a b angle1 angle2 fill;
      )
    in
    let plot_contours (color, pltr, contours, data) =
      set_color_in color (
        fun () ->
          plset_pltr pltr;
          let ixmax, iymax = Array_ext.matrix_dims data in
          plcont data 1 ixmax 1 iymax contours;
          plunset_pltr ();
      )
    in
    let plot_image (range, x0, y0, x1, y1, image) =
      let range_min, range_max = range |? (0.0, 0.0) in
      plimage image
        x0 x1 y0 y1 range_min range_max x0 x1 y0 y1;
      ()
    in
    let plot_imagefr (range, x0, y0, x1, y1, image) scale =
      let range_min, range_max = range |? (0.0, 0.0) in
      let scale_min, scale_max = scale in
      plimagefr image
        x0 x1 y0 y1 range_min range_max scale_min scale_max;
      ()
    in
    let plot_join (color, x0, y0, x1, y1) =
      set_color_in color (fun () -> pljoin x0 y0 x1 y1)
    in
    let plot_lines (label, color, xs, ys) =
      set_color_in color (fun () -> plline xs ys)
    in
    let plot_map (color, outline, x0, y0, x1, y1) =
      set_color_in color (
        fun () ->
          plmap (string_of_map_t outline) x0 x1 y0 y1;
      )
    in
    let plot_points (label, color, xs, ys, symbol, scale) =
      set_color_in color (
        fun () ->
          plssym 0.0 scale;
          plpoin xs ys symbol;
          plssym 0.0 1.0;
      )
    in
    let plot_polygon (color, xs, ys, fill) =
      let x_len = Array.length xs in
      let y_len = Array.length ys in
      verify_arg (x_len = y_len)
        "plot_polygon: must provide same number of X and Y coordinates";
      set_color_in color (
        fun () ->
          if fill then (
            plfill xs ys;
          )
          else (
            (* Make sure the polygon is closed *)
            let xs' =
              Array.init (x_len + 1)
                (fun i -> if i < x_len - 1 then xs.(i) else xs.(0))
            in
            let ys' =
              Array.init (y_len + 1)
                (fun i -> if i < y_len - 1 then ys.(i) else ys.(0))
            in
            plline xs' ys';
          )
      )
    in
    let plot_text (color, s, x, y, dx, dy, just) =
      set_color_in color (fun () -> plptex x y dx dy just s)
    in
    let plot_text_outside (color, s, side, displacement, just, perp) =
      let side_string, position =
        match side with
        | Right p -> "r", p
        | Left p -> "l", p
        | Top p -> "t", p
        | Bottom p -> "b", p
      in
      let side_string = side_string ^ if perp then "v" else "" in
      set_color_in color
        (fun () -> plmtex side_string displacement position just s)
    in

    let one_plot p =
      match p with
      | Arc a -> plot_arc a
      | Contours c -> plot_contours c
      | Image i -> plot_image i
      | Image_fr (i, scale) -> plot_imagefr i scale
      | Join j -> plot_join j
      | Lines l -> plot_lines l
      | Map m -> plot_map m
      | Points p -> plot_points p
      | Polygon poly -> plot_polygon poly
      | Text t -> plot_text t
      | Text_outside t_o -> plot_text_outside t_o
      | Set_transform pltr -> plset_pltr pltr
      | Clear_transform -> plunset_pltr ()
      | Custom f -> f ()
      | List l -> plot l
    in
    List.iter (
      fun plottable -> with_stream ?stream (fun () -> one_plot plottable)
    ) plottable_list

  (** Label the axes and plot title *)
  let label ?stream x y title =
    with_stream ?stream (fun () -> pllab x y title)

  (** [colorbar ?label ?log ?pos values] draws a colorbar, using the given
      values for the color range. [label] gives the position and text of the
      colorbar label; if [log] is true then the scale is taken to be log rather
      than linear ; [pos] sets the position of the colorbar itself, both the
      side of the plot to put it on and the distance from the edge
      (normalized device units); [width] is the width of the colorbar (again in
      normalized device units).
      NOTE: This potentially wrecks the current viewport and
      window settings, among others, so it should be called AFTER the current
      plot page is otherwise complete. *)
  let colorbar ?label ?log ?(pos = Right 0.07) ?(width = 0.03) values =
    (* Save the state of the current plot window. *)
    let dxmin, dxmax, dymin, dymax = plgvpd () in
    let wxmin, wxmax, wymin, wymax = plgvpw () in
    (*let old_default, old_scale = plgchr () in*)

    let old_color = plg_current_col0 () in

    (* Small font *)
    plschr 0.0 0.75;
    (* Small ticks on the vertical axis *)
    plsmaj 0.0 0.5;
    plsmin 0.0 0.5;

    (* "Rotate" the image if we have a horizontal (Top or Bottom) colorbar.  If
       the colorbar is to the Right or Bottom of the image then count the
       distance as distance from that edge. *)
    let image, offset =
      match pos with
      | Right off -> [|values|], 1.0 -. off
      | Left off -> [|values|], off -. width
      | Top off ->
          Array.map (fun x -> [|x|]) values, 1.0 -. off
      | Bottom off ->
          Array.map (fun x -> [|x|]) values, off -. width
    in

    (* Find the min and max in the range of values, ignoring nan, infinity and
       neg_infinity values. *)
    let max_value, min_value = plMinMax2dGrid image in

    (* Put the bar on the proper side, with the proper offsets. *)
    (* Unit major-axis, minor-axis scaled to contour values. *)
    (* Draw the color bar as an image. *)
    let width_start = offset in
    let width_end = offset +. width in
    let () =
      match pos with
      | Right _
      | Left _ ->
          plvpor width_start width_end 0.15 0.85;
          plwind 0.0 1.0 min_value max_value;
          plimage
            image 0.0 1.0 min_value max_value 0.0 0.0
            0.0 1.0 min_value max_value;
      | Top _
      | Bottom _ ->
          plvpor 0.15 0.85 width_start width_end;
          plwind min_value max_value 0.0 1.0;
          plimage
            image min_value max_value 0.0 1.0 0.0 0.0
            min_value max_value 0.0 1.0;
    in

    (* Draw a box and tick marks around the color bar. *)
    set_color Black;
    (* Draw ticks and labels on the major axis.  Add other options as
       appropriate. *)
    let major_axis_opt =
      String.concat "" [
        "bct";
        (* Log? *)
        (match log with
        | None -> ""
        | Some b -> if b then "sl" else "");
        (* Which side gets the label *)
        (match pos with
        | Right _
        | Top _ -> "m"
        | Left _
        | Bottom _ -> "n");
        (* Perpendicular labeling? *)
        (match pos with
        | Right _
        | Left _ -> "v"
        | Top _
        | Bottom _ -> "");
      ]
    in
    (* Just draw the minor axis sides, no labels or ticks. *)
    let minor_axis_opt = "bc" in
    let x_opt, y_opt =
      match pos with
      | Top _
      | Bottom _ -> major_axis_opt, minor_axis_opt
      | Left _
      | Right _ -> minor_axis_opt, major_axis_opt
    in
    plbox x_opt 0.0 0 y_opt 0.0 0;

    (* Draw the label *)
    Option.may (
      fun l ->
        (* Which side to draw the label on and the offset from that side in units
           of character height. *)
        let label_string, label_pos_string, label_offset =
          match l with
          | Right s -> s, "r", 4.0
          | Left s -> s, "l", 4.0
          | Top s -> s, "t", 1.5
          | Bottom s -> s, "b", 1.5
        in
        plmtex label_pos_string label_offset 0.5 0.5 label_string
    ) label;

    (* TODO XXX FIXME - Make sure setting are all properly restored... *)
    (* Restore the old plot window settings. *)
    plvpor dxmin dxmax dymin dymax;
    plwind wxmin wxmax wymin wymax;
    plschr 0.0 1.0;
    plsmaj 0.0 1.0;
    plsmin 0.0 1.0;
    plcol0 old_color;
    ()

  (** Draw a colorbar, optionally log scaled and labeled. *)
  let colorbar ?stream ?label ?log ?pos ?width values =
    with_stream ?stream (fun () -> colorbar ?label ?log ?pos ?width values)

  (** An easier to deduce alternative to {Plplot.plbox} *)
  let plot_axes ~xtick ~xsub ~ytick ~ysub ~xoptions ~yoptions =
    let map_axis_options ol =
      List.map (
        function
        | Axis -> "a"
        | Frame0 -> "b"
        | Frame1 -> "c"
        | Time -> "d"
        | Fixed_point -> "f"
        | Major_grid -> "g"
        | Minor_grid -> "h"
        | Invert_ticks -> "i"
        | Log -> "l"
        | Unconventional_label -> "m"
        | Label -> "n"
        | Custom_label -> "o"
        | Minor_ticks -> "s"
        | Major_ticks -> "t"
        | Vertical_label -> "v"
      ) ol
    in
    let xopt = String.concat "" (map_axis_options xoptions) in
    let yopt = String.concat "" (map_axis_options yoptions) ^ "v" in
    plbox xopt xtick xsub yopt ytick ysub

  (** Default page ending steps *)
  let default_finish ?stream xstep ystep () =
    let xopt, yopt =
      match stream with
      | None -> default_axis_options, default_axis_options
      | Some s -> s.x_axis, s.y_axis
    in
    with_stream ?stream (
      fun () ->
        set_color Black;
        plot_axes
          ~xtick:xstep ~xsub:0 ~xoptions:xopt
          ~ytick:ystep ~ysub:0 ~yoptions:yopt;
    )

  (** Plot axes, but don't advance the page or end the session.  This is used
      internally by [finish]. *)
  let finish_page ?stream ?f ?post xstep ystep =
    with_stream ?stream (
      fun () ->
        let actual_finish =
          match f with
          | Some custom_finish -> custom_finish
          | None -> default_finish ?stream xstep ystep
        in
        actual_finish ();
        Option.may (fun f -> f ()) post;
    )

  (** Finish the current page, start a new one. *)
  let next_page ?stream ?f ?post x0 x1 y0 y1 axis_scaling =
    finish_page ?stream ?f ?post 0.0 0.0;
    start_page ?stream x0 x1 y0 y1 axis_scaling;
    ()

  (** Finish up the plot by plotting axes and ending the session.  This must
      be called after plotting is complete. *)
  let finish ?stream ?f ?post xstep ystep =
    finish_page ?stream ?f ?post xstep ystep;
    with_stream ?stream plend1
end

(** The [Quick_plot] module is intended to be used for quick, "throw-away"
    plots.  It's is likely to be most useful from the toplevel. *)
module Quick_plot = struct
  open Plot

  let special_float x =
    match classify_float x with
    | FP_normal
    | FP_subnormal
    | FP_zero -> false
    | FP_infinite
    | FP_nan -> true

  let safe_array_reduce f a =
    let n =
      Array_ext.reduce (
        fun accu x ->
          if special_float x then
            accu
          else if special_float accu then
            x
          else
            f accu x
      ) a
    in
    verify_arg (not (special_float n)) "No non-special values";
    n

  let extents xs_list ys_list =
    List.fold_left min infinity (List.map (safe_array_reduce min) xs_list),
    List.fold_left max neg_infinity (List.map (safe_array_reduce max) xs_list),
    List.fold_left min infinity (List.map (safe_array_reduce min) ys_list),
    List.fold_left max neg_infinity (List.map (safe_array_reduce max) ys_list)

  let maybe_log log p =
    Option.map_default (
      fun (x_log, y_log) ->
        {
          p with
          x_axis = if x_log then Log :: p.x_axis else p.x_axis;
          y_axis = if y_log then Log :: p.y_axis else p.y_axis;
        }
    ) p log

  (** [points xs ys] plots the points described by the coordinates [xs]
      and [ys]. *)
  let points ?filename ?(device = Window Cairo) ?labels ?log xs_list ys_list =
    let xmin, xmax, ymin, ymax = extents xs_list ys_list in
    let ys_array = Array.of_list ys_list in
    let p = init ?filename xmin xmax ymin ymax Greedy device in
    let plottable_points =
      Array.to_list (
        Array.mapi (
          fun i xs -> points (Index_color (i + 1)) xs ys_array.(i) i
        ) (Array.of_list xs_list)
      )
    in
    plot ~stream:p plottable_points;
    Option.may (fun (x, y, t) -> label ~stream:p x y t) labels;
    finish ~stream:(maybe_log log p) 0.0 0.0;
    ()

  (** [lines xs ys] plots the line segments described by the coordinates
      [xs] and [ys]. *)
  let lines
        ?filename ?(device = Window Cairo) ?labels ?names ?log
        xs_list ys_list =
    let xmin, xmax, ymin, ymax = extents xs_list ys_list in
    let ys_array = Array.of_list ys_list in
    let p = init ?filename xmin xmax ymin ymax Greedy device in
    let colors = Array.mapi (fun i _ -> Index_color (i + 1)) ys_array in
    let plottable_lines =
      Array.to_list (
        Array.mapi (
          fun i xs -> lines colors.(i) xs ys_array.(i)
        ) (Array.of_list xs_list)
      )
    in
    plot ~stream:p plottable_lines;
    Option.may (fun (x, y, t) -> label ~stream:p x y t) labels;
    Option.may (fun n -> draw_legend ~stream:p n (Array.to_list colors)) names;
    finish ~stream:(maybe_log log p) 0.0 0.0;
    ()

  (** [image ?log m] plots the image [m] with a matching colorbar.  If [log] is
      true then the data in [m] are assumed to be log10(x) values. *)
  let image ?filename ?(device = Window Cairo) ?labels ?log ?palette m =
    let m_max, m_min = plMinMax2dGrid m in
    let xmin, ymin = 0.0, 0.0 in
    let xmax, ymax = Array_ext.matrix_dims m in
    let xmax, ymax = float_of_int xmax, float_of_int ymax in
    let p = init ?filename xmin xmax ymin ymax Equal_square device in
    Option.may (load_palette ~stream:p) palette;
    plot ~stream:p [image (xmin, ymin) (xmax, ymax) m];
    Option.may (fun (x, y, t) -> label ~stream:p x y t) labels;
    colorbar ~stream:p ?log ~pos:(Right 0.12)
      (Array_ext.range ~n:100 m_min m_max);
    finish ~stream:p 0.0 0.0;
    ()

  (** [func ?point ?step fs (min, max)] plots the functions [fs] from [x = min]
      to [x = max].  [step] can be used to tighten or coarsen the sampling of
      plot points. *)
  let func
        ?filename ?(device = Window Cairo) ?labels ?names ?point ?step
        fs (xmin, xmax) =
    let fs_array = Array.of_list fs in
    let colors = Array.mapi (fun i _ -> Index_color (i + 1)) fs_array in
    let plot_content =
      Array.to_list (
        Array.mapi (
          fun i f ->
            func ?point ?step colors.(i) f (xmin, xmax)
        ) fs_array
      )
    in
    let ys =
      Array.of_list (
        List.map (
          function
            | Lines (_, _, _, y)
            | Points (_, _, _, y, _, _) -> y
            | _ -> invalid_arg "Invalid function output"
        ) plot_content
      )
    in
    let ymax, ymin = plMinMax2dGrid ys in
    let stream = init ?filename xmin xmax ymin ymax Greedy device in
    plot ~stream plot_content;
    Option.may (fun n -> draw_legend ~stream n (Array.to_list colors)) names;
    Option.may (fun (x, y, t) -> label ~stream x y t) labels;
    finish ~stream 0.0 0.0;
    ()
end

