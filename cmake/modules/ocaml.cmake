# cmake/modules/ocaml.cmake
#
# Copyright (C) 2008 Andrew Ross
# Copyright (C) 2009 Hezekiah M. Carty
# Copyright (C) 2009 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining Ada bindings configuration options

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_ocaml "Enable OCaml bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_ocaml "Enable OCaml bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_ocaml AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "OCaml requires shared libraries. Disabling ocaml bindings")
  set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
endif(ENABLE_ocaml AND NOT BUILD_SHARED_LIBS)

if(ENABLE_ocaml)
  find_program(OCAMLC NAMES ocamlc.opt ocamlc)
  if (OCAMLC)
    message(STATUS "OCAMLC = ${OCAMLC}")
  else (OCAMLC)
    message(STATUS "WARNING:"
      "ocamlc not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif (OCAMLC)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(CAMLIDL camlidl)
  if (CAMLIDL)
    message(STATUS "CAMLIDL = ${CAMLIDL}")
  else (CAMLIDL)
    message(STATUS "WARNING:"
      "camlidl not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif (CAMLIDL)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(OCAMLMKLIB ocamlmklib)
  if (OCAMLMKLIB)
    message(STATUS "OCAMLMKLIB = ${OCAMLMKLIB}")
  else (OCAMLMKLIB)
    message(STATUS "WARNING:"
      "ocamlmklib not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif (OCAMLMKLIB)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(OCAMLOPT NAMES ocamlopt.opt ocamlopt)
  if (OCAMLOPT)
    message(STATUS "OCAMLOPT = ${OCAMLOPT}")
  else (OCAMLOPT)
    message(STATUS "WARNING:"
      "ocamlopt not found. Disabling native code OCaml bindings")
  endif (OCAMLOPT)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(OCAMLFIND ocamlfind)
  if (OCAMLFIND)
    message(STATUS "OCAMLFIND = ${OCAMLFIND}")
  else (OCAMLFIND)
    message(STATUS "WARNING:"
      "ocamlfind not found.")
  endif (OCAMLFIND)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  execute_process(COMMAND ${OCAMLC} -version
    OUTPUT_VARIABLE OCAML_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  execute_process(COMMAND ${OCAMLC} -where
    OUTPUT_VARIABLE OCAML_LIB_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  message(STATUS "OCAML_LIB_PATH = ${OCAML_LIB_PATH}")
  find_path(CAMLIDL_LIB_DIR libcamlidl.a PATHS ${OCAML_LIB_PATH} )
  if(CAMLIDL_LIB_DIR)
    message(STATUS "CAMLIDL_LIB_DIR = ${CAMLIDL_LIB_DIR}")
  else(CAMLIDL_LIB_DIR)
    message(STATUS "WARNING:"
      "camlidl library not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif(CAMLIDL_LIB_DIR)

  # Installation follows the Debian ocaml policy for want of a better
  # standard.
  set(OCAML_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/ocaml/${OCAML_VERSION}
    CACHE PATH "install location for ocaml files"
    )

endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  option(GENERATE_PLPLOT_H_INC "Generate generated_plplot_h.inc" OFF)

  if(GENERATE_PLPLOT_H_INC)
    find_program(OCAML ocaml)
    if (OCAML)
      message(STATUS "OCAML = ${OCAML}")
    else (OCAML)
      message(STATUS "WARNING:"
	"ocaml not found. Disabling generation of generated_plplot_h.inc")
      set(GENERATE_PLPLOT_H_INC OFF CACHE BOOL "Generate generated_plplot_h.inc" FORCE)
    endif (OCAML)
  endif(GENERATE_PLPLOT_H_INC)

  # Test for availability of topfind and pcre (and unix).
  if(GENERATE_PLPLOT_H_INC)
    set(text
"#use \"topfind\";;
#require \"unix\";;
#require \"pcre\";;
"
      )
    file(WRITE ${CMAKE_BINARY_DIR}/test_ocaml.ml ${text})
    # For some reason the return code is not set if an error occurs
    # with test_ocaml.ml so look for any stdout or stderr output as a sign
    # of an error condition.
    execute_process(
      COMMAND ${OCAML} test_ocaml.ml
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      OUTPUT_VARIABLE output
      ERROR_VARIABLE error
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE
      )
    if(output OR error)
      message(STATUS "WARNING:"
	"One or all of topfind, unix, or pcre components of ocaml are not installed.  Disabling generation of generated_plplot_h.inc")
      set(GENERATE_PLPLOT_H_INC OFF CACHE BOOL "Generate generated_plplot_h.inc" FORCE)
    endif(output OR error)

  endif(GENERATE_PLPLOT_H_INC)

  # Test for the availability of Cairo and Gtk+ bindings
  if(OCAMLFIND)
    if(PLD_extcairo)
      set(text_cairo "module C = Cairo")
      file(WRITE ${CMAKE_BINARY_DIR}/test_cairo.ml ${text_cairo})
      execute_process(
        COMMAND ${OCAMLFIND} c -package cairo -linkpkg test_cairo.ml -o test_cairo
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE OCAML_HAS_CAIRO
	OUTPUT_QUIET
	ERROR_QUIET
        )
      # zero return code indicates no errors.  Thus, OCAML_HAS_CAIRO must have
      # truth inverted.
      if(OCAML_HAS_CAIRO)
	set(OCAML_HAS_CAIRO OFF)
      else(OCAML_HAS_CAIRO)
	set(OCAML_HAS_CAIRO ON)
      endif(OCAML_HAS_CAIRO)

      if(OCAML_HAS_CAIRO)
        message(STATUS "Cairo OCaml library found")
      else(OCAML_HAS_CAIRO)
        message(STATUS "WARNING:"
          "Cairo OCaml library not found.  Disabling Plcairo module")
      endif(OCAML_HAS_CAIRO)

      set(text_gtk
  "module G = Gtk
  module C = Cairo_lablgtk"
        )
      file (WRITE ${CMAKE_BINARY_DIR}/test_gtk.ml ${text_gtk})
      execute_process(
        COMMAND ${OCAMLFIND} c -package cairo.lablgtk2 -linkpkg test_gtk.ml -o test_gtk
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE OCAML_HAS_GTK
	OUTPUT_QUIET
	ERROR_QUIET
        )
      # zero return code indicates no errors.  Thus, OCAML_HAS_CAIRO must have
      # truth inverted.
      if(OCAML_HAS_GTK)
	set(OCAML_HAS_GTK OFF)
      else(OCAML_HAS_GTK)
	set(OCAML_HAS_GTK ON)
      endif(OCAML_HAS_GTK)

      if(OCAML_HAS_GTK)
        message(STATUS "lablgtk2 OCaml library found")
      else(OCAML_HAS_GTK)
        message(STATUS "WARNING: lablgtk2 OCaml library not found.")
      endif(OCAML_HAS_GTK)
    endif (PLD_extcairo)
  else(OCAMLFIND)
    message(STATUS "WARNING:"
      "ocamlfind not available.  Disabling Plcairo module and lablgtk support")
    set(OCAML_HAS_CAIRO OFF)
    set(OCAML_HAS_GTK OFF)
  endif(OCAMLFIND)
endif(ENABLE_ocaml)
