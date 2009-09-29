type ('a, 'b) t = {
  width : float;
  height : float;
  surface : 'a Cairo.surface;
  context : Cairo.t;
  file : 'b option;
  clear : bool;
  plstream : int;
}
type ('a, 'b) plplot_cairo_t = ('a, 'b) t
external set_cairo_context : Cairo.t -> unit = "ml_set_plplot_cairo_context"
val blit_to_cairo :
  dest:Cairo.t ->
  dim:[< `Both of float * float | `Height of float | `Width of float ] ->
  xoff:float -> yoff:float -> ([> `Any ], 'a) t -> unit
val rasterize : ?alpha:float -> ('a, 'b) t -> (unit -> 'c) -> unit
val rimage :
  ?alpha:float ->
  ('a, 'b) t ->
  float array array ->
  float ->
  float -> float -> float -> float -> float -> float -> float -> unit
val with_stream : ?stream:('a, 'b) t -> (unit -> 'c) -> 'c
val new_page : ('a, 'b) t -> unit
val pscairo :
  width:int ->
  height:int -> string option -> out_channel option * Cairo_ps.surface
val pdfcairo :
  width:int ->
  height:int -> string option -> out_channel option * Cairo_pdf.surface
val imagecairo :
  width:int ->
  height:int -> string option -> string option * Cairo.image_surface
val imagecairo_rgba :
  width:int ->
  height:int -> string option -> string option * Cairo.image_surface
val init_cairo :
  ?filename:'a ->
  clear:bool ->
  width:int ->
  height:int ->
  (width:int ->
   height:int -> 'a option -> 'b option * ([> `Any ] as 'c) Cairo.surface) ->
  ('c, 'b) t
val make_active : ('a, 'b) t -> unit
val finish : ([> `Any ], 'a) t -> unit
val save_image : ([> `Any ], string) t -> unit
val save_image_as : ([> `Any ], 'a) t -> string -> unit
val save_to_file : ([> `Any ], out_channel) t -> unit
val copy_plot : ('a, 'b) t -> string -> string -> unit
