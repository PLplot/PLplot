#use "topfind";;
#require "unix";;
#require "pcre";;

type attribute_spec = {
  function_name: string;
  function_attrs: string list option;
  parameter_attrs: (string * string list) list option;
}

let manual_function_attributes =
  [
    {
      function_name = "c_plhist";
      function_attrs = None;
      parameter_attrs = Some ["data", ["size_is(n)"]];
    };
    {
      function_name = "c_plimagefr";
      function_attrs = None;
      parameter_attrs = Some ["idata", ["in"; "size_is(nx, ny)"]];
    };
    {
      function_name = "c_plimage";
      function_attrs = None;
      parameter_attrs = Some ["idata", ["in"; "size_is(nx, ny)"]];
    };
    {
      function_name = "c_plstyl";
      function_attrs = None;
      parameter_attrs = Some ["mark", ["size_is(nms)"]; "space", ["size_is(nms)"]];
    };
    {
      function_name = "plMinMax2dGrid";
      function_attrs = None;
      parameter_attrs = Some ["f", ["size_is(nx, ny)"]; "fmax", ["out"]; "fmin", ["out"]];
    };
    {
      function_name = "c_plscmap1l";
      function_attrs = None;
      parameter_attrs = Some ["rev", ["in"; "size_is(npts)"; "unique"]];
    };
    {
      function_name = "c_plxormod";
      function_attrs = None;
      parameter_attrs = Some ["status", ["out"]];
    };
    {
      function_name = "c_plrgbhls";
      function_attrs = None;
      parameter_attrs = Some ["p_h", ["out"]; "p_l", ["out"]; "p_s", ["out"]];
    };
    {
      function_name = "c_plhlsrgb";
      function_attrs = None;
      parameter_attrs = Some ["p_r", ["out"]; "p_g", ["out"]; "p_b", ["out"]];
    };
    {
      function_name = "c_plmkstrm";
      function_attrs = None;
      parameter_attrs = Some ["p_strm", ["out"]];
    };
    {
      function_name = "c_plbin";
      function_attrs = None;
      parameter_attrs = Some ["x", ["in"; "size_is(nbin)"];
                              "y", ["in"; "size_is(nbin)"]];
    };
    {
      function_name = "c_plpat";
      function_attrs = None;
      parameter_attrs = Some ["inc", ["in"; "size_is(nlin)"];
                              "del", ["in"; "size_is(nlin)"]];
    };
    (* For now, this will be wrapped by hand...
    {
      function_name = "c_plgriddata";
      function_attrs = None;
      parameter_attrs = Some ["xg", ["in"; "size_is(nptsx)"]; "yg", ["in"; "size_is(nptsy)"]; "zg", ["out"; "size_is(nptsx,nptsy)"]];
    };
    *)
  ]

(* Length to allocate for output strings. *)
let max_string_length = "1024"

(* Functions to read in everything on STDOUT from a given command. *)
(* Many thanks to Richard M. Jones for the following two functions! *)
let rec input_all_lines chan =
  try
    let line = input_line chan in
    line :: input_all_lines chan
  with
      End_of_file -> []

let pget cmd =
  let chan = Unix.open_process_in cmd in
  let lines = input_all_lines chan in
  let stat = Unix.close_process_in chan in
  (match stat with
       Unix.WEXITED 0 -> ()
     | Unix.WEXITED i ->
         failwith ("command failed with code " ^ string_of_int i)
     | Unix.WSIGNALED i ->
         failwith ("command killed by signal " ^ string_of_int i)
     | Unix.WSTOPPED i ->
         failwith ("command stopped by signal " ^ string_of_int i));
  lines

let read_file filename =
  let preprocessed_text = pget ("cpp " ^ filename) in
  let l = List.map (fun l -> l ^ "\n") preprocessed_text in
  (*
  let text_blob =
    List.fold_left (^) "" l
  in
  print_endline text_blob;
  text_blob
  *)
  l

let (|>) x f = f x
let id x = x

let cleanup_lines l =
  (* Strip out #-started preprocessor lines, as well as lines with only
     whitespace. *)
  let blob =
    let filtered = 
      List.filter (
        fun line ->
          if Pcre.pmatch ~pat:"^#|^\\s+$" line then
            false
          else
            true
      ) l
    in
    List.fold_left (^) "" filtered
  in
  blob
    (* Compress lengths of whitespace down to a single character *)
    |> Pcre.replace ~pat:"\\s+" ~templ:" "
    (* Put newlines back in after each ; *)
    |> Pcre.replace ~pat:"; " ~templ:";\n"

let make_attribute_string attributes =
  match attributes with
      [] -> ""
    | a ->
        "[" ^ String.concat ", " a ^"]"

let minimize_whitespace s =
  s
    |> Pcre.replace ~pat:"^\\s+" ~templ:""
    |> Pcre.replace ~pat:"\\s$" ~templ:""
    |> Pcre.replace ~pat:"\\s+" ~templ:" "

let function_attributes return_type name =
  let check_re re =
    if Pcre.pmatch ~pat:re name then
      Pcre.extract ~pat:re ~full_match:false name
    else
      [||]
  in

  let name_checks =
    [
      (* OCaml values can not begin with a capital letter.  Translate a name
         like FOObar to foo_bar for OCaml. *)
      "^([A-Z]+)(.*)$",
      (
        fun a -> ["mlname(" ^ (
          match Array.length a with
              1 -> String.lowercase a.(0)
            | 2 ->
                String.lowercase a.(0) ^ "_" ^ a.(1)
            | _ -> raise (Failure "Bad result in function caps check")
        ) ^ ")"]
      );
      (* Plplot names many of their functions c_* to avoid clashes with certain
         language bindings.  There's no need to carry this over to OCaml.
         This turns c_foo in to foo. *)
      "^c_(\\w+)$", (fun a -> ["mlname(" ^ a.(0) ^ ")"]);
    ]
  in
  let type_checks =
    [
      (* Treat strings properly *)
      "char\\s*\\*",
      ["string"; "length_is(" ^ max_string_length ^ ")"]
    ]
  in

  (* Attributes based on the function name *)
  let name_attrs =
    List.map
      (fun (re,attrf) -> let a = check_re re in if Array.length a > 0 then attrf a else [])
      name_checks
    |> List.flatten
  in
  (* Attributes based on the function type *)
  let type_attrs =
    List.map
      (fun (re,attrs) -> if Pcre.pmatch ~pat:re return_type then attrs else [])
      type_checks
    |> List.flatten
  in
  (* Any other attributes, specified manually *)
  let manual_attrs =
    try
      let fa =
        List.find (fun fa -> fa.function_name = name) manual_function_attributes
      in
      match fa.function_attrs with
          Some a -> a
        | None -> []
    with
        Not_found -> []
  in
  name_attrs @ type_attrs @ manual_attrs

let parameter_attributes function_name types names =
  let pmatch re str = Pcre.pmatch ~pat:re str in
  let non_get_functions = ["c_plgriddata"; "c_plgra"] in

  (* If all of the pieces are true, then the attribute(s) is(are) appropriate
     for this parameter. *)
  let checks p_type p_name =
    [
      (* Order goes:
         function_name check
         type check
         attribute name check
         misc. check (anything, as long as it's a bool)
         attributes, if all of the above are true
      *)
      (* "PLUNICODE" parameters need at least unsigned int32 width ints,
         so use Int64.t values to be safe. *)
      true,
      pmatch "unsigned int" p_type,
      true,
      true,
      ["int64"];
      (* "get" functions *)
      pmatch "^c_plg" function_name,
      pmatch "\\*" p_type,
      true,
      not (List.mem function_name non_get_functions),
      ["out"] @
        if pmatch "char" p_type then ["length_is(" ^ max_string_length ^ ")"]
        else [];
      (* Strings *)
      true,
      pmatch "(?:const )?char\\s*\\*$" p_type,
      true,
      true,
      ["string"];
      (* Pointers to arrays of n elements *)
      true,
      pmatch "\\*" p_type,
      true,
      List.mem "n" names,
      ["in"; "size_is(n)"];
      (* Pointers to arrays of npts elements *)
      true,
      pmatch "\\*" p_type,
      not (pmatch "^[xyz]g$" p_name),
      List.mem "npts" names,
      ["in"; "size_is(npts)"];
      (* x and y dimensions *)
      true,
      pmatch "\\*" p_type,
      p_name = "x" || p_name = "y",
      List.mem ("n" ^ p_name) names,
      ["size_is(n" ^ p_name ^ ")"; "in"];
      (* z dimensions *)
      true,
      pmatch "\\*\\*" p_type,
      p_name = "z",
      List.mem "nx" names && List.mem "ny" names,
      ["size_is(nx, ny)"; "in"];
      (* Contouring levels *)
      true,
      true,
      p_name = "clevel",
      List.mem "nlevel" names,
      ["size_is(nlevel)"; "in"];
      (* Color maps *)
      true,
      pmatch "\\*" p_type,
      p_name = "r" || p_name = "g" || p_name = "b" || p_name = "a",
      List.mem "ncol0" names,
      ["size_is(ncol0)"; "in"];
      true,
      pmatch "\\*" p_type,
      p_name = "r" || p_name = "g" || p_name = "b" || p_name = "a",
      List.mem "ncol1" names,
      ["size_is(ncol1)"; "in"];
      (* Linear relationship color maps *)
      pmatch "c_plscmap1l" function_name,
      pmatch "\\*" p_type,
      List.mem p_name ["intensity"; "coord1"; "coord2"; "coord3"; "a"],
      true,
      ["size_is(npts)"];
      (* Relative to world coordinates *)
      function_name = "c_plcalc_world",
      pmatch "\\*" p_type,
      List.mem p_name ["wx"; "wy"; "window"],
      true,
      ["out"];
    ]
  in

  let attr_hash = Hashtbl.create 10 in

  let perform_check param_type param_name =
    (* Any other attributes, specified manually *)
    let manual_attrs =
      try
        let fa =
          List.find (fun fa -> fa.function_name = function_name) manual_function_attributes
        in
        match fa.parameter_attrs with
            Some a -> 
              List.assoc param_name a
          | None -> []
      with
          Not_found -> []
    in
    Hashtbl.add attr_hash param_name manual_attrs;
    (* Check for attributes, filter the ones we don't want, then add the rest
       to the attribute hash. *)
    checks param_type param_name
    |> List.filter (
        fun (function_check, type_check, name_check, other_check, _) ->
          List.for_all id [function_check; type_check; name_check; other_check]
       )
    |> List.iter (fun (_,_,_,_,attrs) -> Hashtbl.add attr_hash param_name attrs)
  in
  List.iter2 perform_check types names;
  attr_hash

let build_attribute_list l =
  List.map (
    fun (attrs, t, n) ->
      String.concat " " [make_attribute_string attrs; t; n]
  ) l

let process_prototype line =
  let pieces =
    line
    |> Pcre.extract ~pat:"^((?:(?:const|unsigned) )?\\w+ (?:\\*\\s*)?)(\\w+)\\s*\\(([\\w\\s\\*\\[\\],]*)\\)" ~full_match:false
    |> Array.map minimize_whitespace
  in
  (* Get the return type, name and arg list separately *)
  let return_type = pieces.(0) in
  let function_name = pieces.(1) in
  let params =
    pieces.(2)
    |> Pcre.split ~pat:","
    |> List.map minimize_whitespace
  in
  let param_types, param_names =
    params
    |> List.map (
         fun param ->
           let p = Pcre.extract ~pat:"(.*)?\\b(\\w+)" ~full_match:false param in
           minimize_whitespace p.(0), minimize_whitespace p.(1)
       )
    |> List.split
  in
  let f_attrs = function_attributes return_type function_name in
  let p_attrs = parameter_attributes function_name param_types param_names in
  let params_with_attrs =
    List.map2
      (fun t n -> Hashtbl.find_all p_attrs n |> List.flatten, t, n)
      param_types param_names
  in
  String.concat " " (
    [
      make_attribute_string f_attrs;
      return_type;
      function_name; "(";
    ]
    @ [String.concat ", " (build_attribute_list params_with_attrs)]
    @ [");"]
  )

let write_file filename lines =
  let fout = open_out filename in
  List.iter (output_string fout) lines;
  close_out fout;
  ()

let process_file filename =
  let lines = read_file filename in
  let lines' = cleanup_lines lines |> Pcre.split ~pat:"\n" in
  lines'
    |> List.map (
         fun l -> try process_prototype l with Not_found -> l
       )
    |> List.map minimize_whitespace
    |> List.map (fun l -> l ^ "\n")
    |> write_file (filename ^ ".inc")

let () =
  if !Sys.interactive then
    ()
  else
    process_file "plplot_h";
    ()
