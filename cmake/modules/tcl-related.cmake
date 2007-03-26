# cmake/modules/tcl-related.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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

# Module for determining all tcl-related configuration options not directly
# related to the tk device driver.

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_tcl "Enable Tcl bindings" OFF)
  option(ENABLE_itcl "Enable incr Tcl interface code" OFF)
  option(ENABLE_tk "Enable Tk interface code" OFF)
  option(ENABLE_itk "Enable incr TK interface code" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_tcl "Enable Tcl bindings" ON)
  option(ENABLE_itcl "Enable incr Tcl interface code" ON)
  option(ENABLE_tk "Enable Tk interface code" ON)
  option(ENABLE_itk "Enable incr TK interface code" ON)
endif(DEFAULT_NO_BINDINGS)

# Depending on these above options and system resources may also determine
# the following variables which are largely self-explantory unless documented
# further.

#TCL_INCLUDE_PATH
#TCL_LIBRARY
#ITCL_INCLUDE_PATH
#ITCL_LIBRARY
#HAVE_ITCL   		(On when itcl header and library has been found.
#			 Otherwise, undefined.
#			 Used for source file configuration.)
#HAVE_ITCLDECLS_H 	(defined [actually as a path, but we don't use that]
#			 when itclDecls.h.  Otherwise, undefined.
#			 Used for source file configuration.)
#TK_INCLUDE_PATH
#TK_LIBRARY
#ITK_INCLUDE_PATH
#ITK_LIBRARY
#HAVE_ITK   		(On when itcl header and library has been found.
#			 Otherwise, undefined.
#			 Used for source file configuration.)

if(ENABLE_tcl)
  message(STATUS "Looking for include paths and libraries for Tcl/Tk")
  find_package(TCL)
  # Supplement those results with iTcl and iTk include paths and libraries.
  if(TCL_FOUND)
    message(STATUS
    "Looking for include paths and libraries for Tcl/Tk - found"
    )
    message(STATUS "Looking for tclsh")
    include(FindTclsh)
    if(TCL_TCLSH)
      message(STATUS "Looking for tclsh - found")
    else(TCL_TCLSH)
      message(STATUS "Looking for tclsh - not found")
    endif(TCL_TCLSH)
  else(TCL_FOUND)
    message(STATUS
    "Looking for include paths and libraries for Tcl/Tk - not found"
    )
  endif(TCL_FOUND)
  if(TCL_FOUND AND TCL_TCLSH)
    message(STATUS "TCL_INCLUDE_PATH = ${TCL_INCLUDE_PATH}")
    message(STATUS "TCL_LIBRARY = ${TCL_LIBRARY}")
    if(ENABLE_itcl)
      message(STATUS "Looking for itcl.h")
      find_path(ITCL_INCLUDE_PATH itcl.h ${TCL_INCLUDE_PATH})
      if(ITCL_INCLUDE_PATH)
        message(STATUS "Looking for itcl.h - found")
        message(STATUS "Looking for itcl library")
	get_filename_component(TCL_LIBRARY_PATH ${TCL_LIBRARY} PATH)
        set(itcl_library_versions 3.3 3.2 3.1 3.0 2.1 2.0)
        foreach(version ${itcl_library_versions})
          find_library(ITCL_LIBRARY itcl${version} PATHS ${TCL_LIBRARY_PATH}
                       PATH_SUFFIXES itcl${version})
        endforeach(version ${itcl_library_versions})
        if(ITCL_LIBRARY)
          message(STATUS "Looking for itcl library - found")
          set(HAVE_ITCL ON)
          find_path(HAVE_ITCLDECLS_H itclDecls.h ${TCL_INCLUDE_PATH})
        else(ITCL_LIBRARY)
          message(STATUS "Looking for itcl library - not found")
          message(STATUS "WARNING: Disabling incr Tcl interface code"
        )
        set(ENABLE_itcl OFF CACHE BOOL "Enable incr Tcl interface code" FORCE)
        endif(ITCL_LIBRARY)
      else(ITCL_INCLUDE_PATH)
        message(STATUS "Looking for itcl.h - not found")
        message(STATUS "WARNING: Disabling incr Tcl interface code"
        )
        set(ENABLE_itcl OFF CACHE BOOL "Enable incr Tcl interface code" FORCE)
      endif(ITCL_INCLUDE_PATH)
    endif(ENABLE_itcl)
    if(ENABLE_itcl)
      message(STATUS "ITCL_INCLUDE_PATH = ${ITCL_INCLUDE_PATH}")
      message(STATUS "ITCL_LIBRARY = ${ITCL_LIBRARY}")
    endif(ENABLE_itcl)
    # ------------------------------------------------------------------------
    # figure out how to build tclIndex
    # ------------------------------------------------------------------------
    set(MKTCLINDEX ${CMAKE_SOURCE_DIR}/scripts/mktclIndex)
    if(ENABLE_itcl)
      set(MKTCLINDEX_ARGS "-itcl")
    else(ENABLE_itcl)
      set(MKTCLINDEX_ARGS "-tcl")
    endif(ENABLE_itcl)
    if(ENABLE_tk AND NOT X11_FOUND)
      message(STATUS "WARNING: X11 not found, disabling Tk interface code")
      set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
    endif(ENABLE_tk AND NOT X11_FOUND)
    if(ENABLE_tk)
      set(TK_INCLUDE_PATH ${TK_INCLUDE_PATH} ${X11_INCLUDE_DIR})
      set(
      TK_LIBRARY
      ${TK_LIBRARY}
      -L${X11_LIBRARY_DIR}
      ${X11_LIBRARIES}
      )
      message(STATUS "TK_INCLUDE_PATH = ${TK_INCLUDE_PATH}")
      message(STATUS "TK_LIBRARY = ${TK_LIBRARY}")
    else(ENABLE_tk)
      message(STATUS
      "WARNING: Because Tk is disabled must disable incr Tk as well"
      )
      set(ENABLE_itk OFF CACHE BOOL "Enable incr Tk interface code" FORCE)
    endif(ENABLE_tk)
    if(ENABLE_itk)
      message(STATUS "Looking for itk.h")
      find_path(ITK_INCLUDE_PATH itk.h ${TK_INCLUDE_PATH})
      if(ITK_INCLUDE_PATH)
        message(STATUS "Looking for itk.h - found")
        message(STATUS "Looking for itk library")
        set(itk_library_versions 3.3 3.2 3.1 3.0 2.1 2.0)
        foreach(version ${itk_library_versions})
          find_library(ITK_LIBRARY itk${version} ${TK_LIBRARY}
                       PATH_SUFFIXES itk${version})
        endforeach(version ${itk_library_versions})
        if(ITK_LIBRARY)
          message(STATUS "Looking for itk library - found")
          set(HAVE_ITK ON)
        else(ITK_LIBRARY)
          message(STATUS "Looking for itk library - not found")
          message(STATUS "WARNING: Disabling incr Tk interface code"
        )
        set(ENABLE_itk OFF CACHE BOOL "Enable incr Tk interface code" FORCE)
        endif(ITK_LIBRARY)
      else(ITK_INCLUDE_PATH)
        message(STATUS "Looking for itk.h - not found")
        message(STATUS "WARNING: Disabling incr Tk interface code")
        set(ENABLE_itk OFF CACHE BOOL "Enable incr Tk interface code" FORCE)
      endif(ITK_INCLUDE_PATH)
    endif(ENABLE_itk)
    if(ENABLE_itk)
      message(STATUS "ITK_INCLUDE_PATH = ${ITK_INCLUDE_PATH}")
      message(STATUS "ITK_LIBRARY = ${ITK_LIBRARY}")
    endif(ENABLE_itk)
  else(TCL_FOUND AND TCL_TCLSH)
    message(STATUS "WARNING: Disabling everything that is Tcl/Tk related")
    set(ENABLE_tcl OFF CACHE BOOL "Enable Tcl bindings" FORCE)
    set(ENABLE_itcl OFF CACHE BOOL "Enable incr Tcl interface code" FORCE)
    set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
    set(ENABLE_itk OFF CACHE BOOL "Enable incr Tk interface code" FORCE)
  endif(TCL_FOUND AND TCL_TCLSH)
else(ENABLE_tcl)
  message(STATUS 
  "ENABLE_tcl is OFF so disabling everything else that "
  "is Tcl/Tk related"
  )
  set(ENABLE_itcl OFF CACHE BOOL "Enable incr Tcl interface code" FORCE)
  set(ENABLE_tk OFF CACHE BOOL "Enable Tk interface code" FORCE)
  set(ENABLE_itk OFF CACHE BOOL "Enable incr Tk interface code" FORCE)
endif(ENABLE_tcl)
