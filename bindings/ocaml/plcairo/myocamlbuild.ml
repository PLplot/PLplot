open Ocamlbuild_plugin
open Ocamlbuild_pack
open Command
open Ocaml_specific
open Outcome
open Printf

(* These functions are not really officially exported *)
let run_and_read = Ocamlbuild_pack.My_unix.run_and_read
let blank_sep_strings = Ocamlbuild_pack.Lexers.blank_sep_strings

(* This lists all supported packages *)
let find_packages () =
  blank_sep_strings &
    Lexing.from_string &
      run_and_read "ocamlfind list | cut -d' ' -f1"

(* ocamlfind command *)
let ocamlfind x = S[A"ocamlfind"; x]

(* C stuff *)
let cflags =
  try
    let fl = Sys.getenv "CFLAGS" in
    let flags = Lexers.comma_or_blank_sep_strings (Lexing.from_string fl) in
    S(List.concat (List.map (fun fl -> [A"-ccopt"; A fl]) flags))
  with
  | Not_found -> S[]

let pkg_config ?(notag = false) flag library =
  let flag, opt_or_lib =
    match flag with
    | `cflags -> "--cflags", `opt
    | `libs -> "--libs", `none
    | `libsl -> "--libs-only-l", `lib
    | `libsL -> "--libs-only-L", `opt
  in
  let opt_or_lib =
    if notag then
      N
    else
      match opt_or_lib with
      | `opt -> A "-ccopt"
      | `lib -> A "-cclib"
      | `none -> N
  in
  let cmd = "pkg-config " ^ flag ^ " " ^ library in
  let config =
    My_unix.run_and_open cmd (fun ic ->
      Log.dprintf 5 "Getting flags from command %s" cmd;
      input_line ic)
  in
  let flags = Lexers.comma_or_blank_sep_strings (Lexing.from_string config) in
  S(List.concat (List.map (fun fl -> [opt_or_lib; A fl]) flags))

let cairo_conf ?notag x = pkg_config ?notag x "cairo"
let plplot_conf ?notag x = pkg_config ?notag x "plplotd"
;;

let _ = dispatch begin function
   | Before_options ->
       (* by using Before_options one let command line options have an higher
          priority; on the contrary using After_options will guarantee to have
          the higher priority *)

       (* override default commands by ocamlfind ones *)
       Options.ocamlc     := ocamlfind & A"ocamlc";
       Options.ocamlopt   := ocamlfind & A"ocamlopt";
       Options.ocamldep   := ocamlfind & A"ocamldep";
       Options.ocamldoc   := ocamlfind & A"ocamldoc";
       Options.ocamlmktop := ocamlfind & A"ocamlmktop"
   | After_rules ->
       (* When one link an OCaml library/binary/package, one should use
          -linkpkg *)
       flag ["ocaml"; "link"] & A"-linkpkg";

       (* For each ocamlfind package one injects the -package option when
          compiling, computing dependencies, generating documentation and
          linking. *)
       List.iter begin fun pkg ->
         flag ["ocaml"; "compile";  "pkg_"^pkg] & S[A"-package"; A pkg];
         flag ["ocaml"; "ocamldep"; "pkg_"^pkg] & S[A"-package"; A pkg];
         flag ["ocaml"; "doc";      "pkg_"^pkg] & S[A"-package"; A pkg];
         flag ["ocaml"; "byte"; "link";     "pkg_"^pkg] & S[A"-package"; A pkg];
         flag ["ocaml"; "infer_interface"; "pkg_"^pkg] & S[A"-package"; A pkg];
       end (find_packages ());

       (* The default "thread" tag is not compatible with ocamlfind.
          Indeed, the default rules add the "threads.cma" or "threads.cmxa"
          options when using this tag. When using the "-linkpkg" option with
          ocamlfind, this module will then be added twice on the command line.

          To solve this, one approach is to add the "-thread" option when using
          the "threads" package using the previous plugin.
        *)
       flag ["ocaml"; "pkg_threads"; "compile"] (S[A "-thread"]);
       flag ["ocaml"; "pkg_threads"; "link"] (S[A "-thread"]);

      (* C-related rules *)
      (* Custom tag for OCaml bytecode *)
      flag ["ocaml"; "link"; "byte"]
        (A"-custom");

      (* Include compiler options for ocamlmklib *)
      flag ["ocamlmklib"; "c"]
        (S[plplot_conf `libs; cairo_conf `libs]);

      (* gcc needs to know where to find the needed #includes *)
      flag ["c"; "compile"] (S[plplot_conf `cflags; cairo_conf `cflags]);

      (* Use the proper extras when compiling the OCaml library *)
      flag ["ocaml"; "link"; "library"; "byte"]
        (S[A"-dllib"; A"-lplcairo_stubs"; A"-cclib"; A"-lplcairo_stubs";
           plplot_conf `libsL; plplot_conf `libsl;
           cairo_conf `libsL; cairo_conf `libsl]);

      flag ["ocaml"; "link"; "library"; "native"]
        (S[A"-cclib"; A"-lplcairo_stubs";
           plplot_conf `libsL; plplot_conf `libsl;
           cairo_conf `libsL; cairo_conf `libsl]);

      (* Make sure the C pieces are built... *)
      dep ["ocaml"; "compile"] ["libplcairo_stubs.a"];
   | _ -> ()
end
