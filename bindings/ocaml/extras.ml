open Plplot

let draw_legend x y names colors =
  (*
  let () =
    if x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0 then
      failwith "Legend location must be between 0.0 and 1.0"
  in
  *)

  let character_width = 0.2 in
  let character_height = 0.2 in
  let x_buffer = 0.1 in
  let y_buffer = 0.1 in

  let max_name_length =
    List.fold_left (
      fun l n2 ->
        max l (String.length n2)
    ) 0 names
  in
  let width =
    float_of_int max_name_length *. character_width +. 2.0 *. x_buffer
  in
  let height =
    character_height *. float_of_int (List.length names)
  in

  let tx = x +. 0.6 in
  let ty = ref (y +. 0.2) in

  List.iter2 (
    fun n c ->
      plcol0 c;
      plline [|tx -. 0.4; tx -. 0.1|] [|!ty; !ty|];
      plptex tx !ty 0.0 0.0 0.0 n;
      ty := !ty -. character_height -. 0.2;
      ()
  ) names colors;
  ()

(** [plshades0 a xmin xmax ymin ymax clevels base_color] acts in the same
    manner as plshades, but uses color map 0 rather than color map 1.
    [base_color] is the index of the color to use for the first shaded
    interval.  The next interval uses color index [base_color] + 1 and so on.
    A separate call to {plcont} is 
*)
let plshades0 a xmin xmax ymin ymax clevels base_color color_step =
  (* Save the current color and pencil width values *)
  let initial_color = plg_current_col0 () in
  let initial_width = plgwid () in

  (* Use color map 0, and iterate through colors from base_color in increments
     of 1. *)
  let color_map = 0 in
  Array.iteri (
    fun i c0 ->
      let c1 = clevels.(i + 1) in
      let color = float_of_int (base_color + i * color_step) in
      plshade a xmin xmax ymin ymax c0 c1 color_map color 0 0 0 0 0 0;
  ) (Array.sub clevels 0 (Array.length clevels - 1));

  (* Restore the initial color and width values *)
  plcol0 initial_color;
  plwid initial_width;
  ()

(** [color_bar ?tick_interval ?step contours] draws a vertical color bar on the
    right side end of the plot canvas, using the information in [contours] to
    decide which colors to draw.
    If [step] is provided, then every [step] units there will
    be a new color.  If [step] is omitted, then the smallest difference
    between two consecutive values in [contours] is used.
    [tick_level] can be used to set the interval between axis numbers if
    something other than [step] is desired.
*)
let color_bar ?(tick_interval) ?(step) contours =
  (* Get the current character size to restore later *)
  let (old_default, old_scale) = plgchr () in

  (* A wee, tiny little font for the color scale *)
  plschr 0.0 0.50;

  (* Vertical bar on the right side of the plot canvas *)
  plvpor 0.93 0.96 0.15 0.85;

  (* Minimum and maximum values *)
  let cmin = Array.fold_left min infinity contours in
  let cmax = Array.fold_left max neg_infinity contours in

  (* Find the smallest step between contours, if none is provided *)
  let step =
    match step with
        Some s -> s
      | None ->
          let accum = ref 0.0 in
          for i = 1 to Array.length contours - 1 do
            let delta = contours.(i) -. contours.(i - 1) in
            if delta > !accum then
              accum := delta
            else
              ()
          done;
          !accum
  in
  let tick_interval =
    match tick_interval with
        Some t -> t
      | None -> step
  in

  (* Setup the color bar shades according to the given step size *)
  let shades =
    Array.init
      (int_of_float ((cmax -. cmin) /. step) + 1)
      (fun i -> float_of_int i *. step)
  in
  (* Unit x-axis, y-axis scaled to contour values *)
  plwind 0.0 1.0 cmin cmax;
  (* Small ticks on the vertical axis *)
  plsmaj 1.0 1.0;
  (* Show each contour *)
  plshades [| shades; shades |] 0.0 1.0 cmin cmax shades 0 0 0 0;
  plcol0 15;
  plbox "bc" 0.0 0 "bcmtv" tick_interval 0;

  (* Set the character size to the old values *)
  plschr old_default old_scale;
  ()

(** [color_bar0 ?tick_interval ?step contours base_color color_step] --
    See [color_bar].  This does the same thing, except that it uses color map 0
    for the colors.  [base_color] is the first map 0 index to use, and
    [color_step] is the index increment.  So the colors used are:
    cmap0.(base_color + n * color_step) for n in 0 .. (Array.length contours)
*)
let color_bar0 ?(tick_interval) ?(step) contours base_color color_step =
  (* Save some plot parameters to restore at the end of the function *)
  let (old_default, old_scaled) = plgchr () in
  let old_col0 = plg_current_col0 () in
  let (old_vxmin, old_vxmax, old_vymin, old_vymax) = plgvpd () in
  let (old_wxmin, old_wxmax, old_wymin, old_wymax) = plgvpw () in

  (* A wee, tiny little font for the color scale *)
  plschr 0.0 0.50;

  (* Vertical bar on the right side of the plot canvas *)
  plvpor 0.93 0.96 0.15 0.85;

  (* Minimum and maximum values *)
  let cmin = Array.fold_left min infinity contours in
  let cmax = Array.fold_left max neg_infinity contours in

  (* Find the smallest step between contours, if none is provided *)
  let step =
    match step with
        Some s -> s
      | None ->
          let accum = ref 0.0 in
          for i = 1 to Array.length contours - 1 do
            let delta = contours.(i) -. contours.(i - 1) in
            if delta > !accum then
              accum := delta
            else
              ()
          done;
          !accum
  in
  let tick_interval =
    match tick_interval with
        Some t -> t
      | None -> step
  in

  (* Setup the color bar shades according to the given step size *)
  let shades =
    Array.init
      ((int_of_float (cmax -. cmin) + 1) * int_of_float step)
      (fun i -> float_of_int i *. step)
  in
  (* Unit x-axis, y-axis scaled to contour values *)
  plwind 0.0 1.0 cmin cmax;
  (* Small ticks on the vertical axis *)
  plsmaj 0.0 0.5;
  (* Show each contour *)
  plshades0 [| contours; contours |] 0.0 1.0 cmin cmax contours base_color color_step;
  plcol0 15;
  plbox "bc" 0.0 0 "bcmtv" tick_interval 0;

  (* Set things back to their old/default values *)
  plschr old_scaled 1.0;
  plcol0 old_col0;
  plsmaj 0.0 1.0;
  plvpor old_vxmin old_vxmax old_vymin old_vymax;
  plwind old_wxmin old_wxmax old_wymin old_wymax;
  ()

