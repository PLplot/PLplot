open Ocamlbuild_plugin
open Command

(* The camlidl command to use (default: first one found in $PATH) *)
let camlidl = S([A"camlidl"; A"-header"])

(* Files included in the main idl descriptor *)
let idl_includes = ["plplot_h.inc"]
;;

dispatch begin function
  | After_rules ->
      (* Handle *.idl files properly... I think *)
      rule "camlidl processing"
        ~prods:["%.mli"; "%.ml"; "%_stubs.c"]
        ~deps:["%.idl"]
        begin fun env _build ->
          let idl = env "%.idl" in
          let tags = tags_of_pathname idl++"compile"++"camlidl" in
          let cmd = Cmd(S[camlidl; T tags; P idl]) in
          Seq [cmd]
        end;

      (* Include the plplot and camlidl compiler options for ocamlmklib *)
      flag ["ocamlmklib"; "c"]
        (S[A"-L@BUILD_DIR@/src"; A"-lplplot@LIB_TAG@";
           A"-L@CAMLIDL_LIB_DIR@"; A"-lcamlidl"]);

      (* gcc needs to know where to find the needed #includes *)
      flag ["c"; "compile"]
        (S[A"-ccopt"; A"-I@SOURCE_DIR@/include";
           A"-ccopt"; A"-I@BUILD_DIR@/include"] );

      (* Custom tag for OCaml bytecode *)
      flag ["ocaml"; "link"; "byte"]
        (A"-custom");

      (* Use the proper extras when compiling the OCaml library *)
      flag ["ocaml"; "link"; "library"; "byte"]
        (S[A"-dllib"; A"-lplplot_stubs"; A"-cclib"; A"-lplplot_stubs";
           A"-cclib"; A"-lplplot@LIB_TAG@"; A"-cclib"; A"-lcamlidl"]);

      flag ["ocaml"; "link"; "library"; "native"]
        (S[A"-cclib"; A"-lplplot_stubs"; 
           A"-cclib"; A"-lplplot@LIB_TAG@"; 
           A"-cclib"; A"-lcamlidl"]);

      (* Make sure the C pieces and built... *)
      dep ["ocaml"; "compile"] ["libplplot_stubs.a"];

      (* Any camlidl work must be updated if the camlidl inputs are changed *)
      dep ["compile"; "camlidl"] idl_includes;

  | _ -> ()
end
