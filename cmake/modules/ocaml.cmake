# cmake/modules/ocaml.cmake
#
# Copyright (C) 2008 Andrew Ross
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
  find_program(OCAMLBUILD ocamlbuild)
  if (OCAMLBUILD)
    message(STATUS "OCAMLBUILD = ${OCAMLBUILD}")
  else (OCAMLBUILD)
    message(STATUS "WARNING:"
      "ocamlbuild not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif (OCAMLBUILD)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(OCAMLFIND ocamlfind)
  if (OCAMLFIND)
    message(STATUS "OCAMLFIND = ${OCAMLFIND}")
  else (OCAMLFIND)
    message(STATUS "WARNING:"
      "ocamlfind not found. Disabling ocaml bindings")
    set(ENABLE_ocaml OFF CACHE BOOL "Enable OCaml bindings" FORCE)
  endif (OCAMLFIND)
endif(ENABLE_ocaml)

if(ENABLE_ocaml)
  find_program(OCAMLC ocamlc)
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
  
  # Subdirectory for ocamlbuild to use for build
  set(OCAML_BUILD_DIR _build CACHE PATH "Subdirectory to use for ocamlbuild")
  
  # Installation follows the Debian ocaml policy for want of a better
  # standard.
  set(OCAML_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/${OCAML_VERSION}
    CACHE PATH "install location for ocaml files"
    )

endif(ENABLE_ocaml)
