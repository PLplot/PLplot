# cmake/modules/summary.cmake
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# Macro for outputting all the most important CMake variables for PLplot.

macro(summary)
set(_output_results "
Summary of CMake build system results for PLplot

Install location variables which can be set by the user:
CMAKE_INSTALL_PREFIX:      ${CMAKE_INSTALL_PREFIX}
CMAKE_INSTALL_EXEC_PREFIX  ${CMAKE_INSTALL_EXEC_PREFIX}
CMAKE_INSTALL_BINDIR 	   ${CMAKE_INSTALL_BINDIR}
CMAKE_INSTALL_DATADIR 	   ${CMAKE_INSTALL_DATADIR}
CMAKE_INSTALL_LIBDIR 	   ${CMAKE_INSTALL_LIBDIR}
CMAKE_INSTALL_INCLUDEDIR   ${CMAKE_INSTALL_INCLUDEDIR}
CMAKE_INSTALL_INFODIR 	   ${CMAKE_INSTALL_INFODIR}
CMAKE_INSTALL_MANDIR 	   ${CMAKE_INSTALL_MANDIR}

Derived install location variables:
DATA_DIR	${DATA_DIR}
LIB_DIR		${LIB_DIR}
INCLUDE_DIR	${INCLUDE_DIR}
BIN_DIR		${BIN_DIR}
TCL_DIR		${TCL_DIR}
ADA_INCLUDE_DIR	${ADA_INCLUDE_DIR}
ADA_LIB_DIR	${ADA_LIB_DIR}
PYTHON_INSTDIR	${PYTHON_INSTDIR}
DRV_DIR		${DRV_DIR}
DOC_DIR		${DOC_DIR}
MAN_DIR		${MAN_DIR}
INFO_DIR	${INFO_DIR}

Other important CMake variables:

CMAKE_SYSTEM_NAME:	${CMAKE_SYSTEM_NAME}
UNIX:			${UNIX}
WIN32:			${WIN32}
APPLE:			${APPLE}
MSVC:			${MSVC}	(MSVC_VERSION:	${MSVC_VERSION})
MINGW:			${MINGW}
MSYS:			${MSYS}
CYGWIN:			${CYGWIN}
BORLAND:		${BORLAND}
WATCOM:		  ${WATCOM}

SWIG_FOUND:		${SWIG_FOUND}
PERL_FOUND:		${PERL_FOUND}
X11_FOUND:		${X11_FOUND}

CMAKE_BUILD_TYPE:	${CMAKE_BUILD_TYPE}
CMAKE_C_COMPILER CMAKE_C_FLAGS:			${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")

if(CMAKE_CXX_COMPILER_WORKS)
set(
_output_results
"${_output_results}
CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS:		${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
endif(CMAKE_CXX_COMPILER_WORKS)

if(ENABLE_f95)
set(
_output_results
"${_output_results}
CMAKE_Fortran_COMPILER CMAKE_Fortran_FLAGS:	${CMAKE_Fortran_COMPILER} ${CMAKE_Fortran_FLAGS}
Target Fortran:	${TARGET_FORTRAN}")
endif(ENABLE_f95)

if(ENABLE_python)
set(
_output_results
"${_output_results}
PYTHON_EXECUTABLE:				${PYTHON_EXECUTABLE}
PYTHON_INCLUDE_PATH:				${PYTHON_INCLUDE_PATH}
PYTHON_LIBRARIES:				${PYTHON_LIBRARIES}
NUMPY_INCLUDE_PATH:				${NUMPY_INCLUDE_PATH}")
endif(ENABLE_python)

set(
_output_results
"${_output_results}

ENABLE_DYNDRIVERS:	${ENABLE_DYNDRIVERS}
DRIVERS_LIST: ${DRIVERS_LIST}
DEVICES_LIST: ${DEVICES_LIST}

Library options:
BUILD_SHARED_LIBS:	${BUILD_SHARED_LIBS}		PL_DOUBLE:	${PL_DOUBLE}

Optional libraries:
PL_HAVE_QHULL:		${PL_HAVE_QHULL}		WITH_CSA:	${WITH_CSA}
PL_HAVE_FREETYPE:	${PL_HAVE_FREETYPE}		PL_HAVE_PTHREAD:	${PL_HAVE_PTHREAD}
HAVE_AGG:		${HAVE_AGG}		HAVE_SHAPELIB:	${HAVE_SHAPELIB}

Language Bindings:
ENABLE_ada:		${ENABLE_ada}
ENABLE_cxx:		${ENABLE_cxx}
ENABLE_d:		${ENABLE_d}
ENABLE_f95:		${ENABLE_f95}
ENABLE_java:		${ENABLE_java}
ENABLE_lua:		${ENABLE_lua}
ENABLE_ocaml:		${ENABLE_ocaml}
ENABLE_octave:		${ENABLE_octave}
ENABLE_pdl:		${ENABLE_pdl}
ENABLE_python:		${ENABLE_python}
ENABLE_qt:		${ENABLE_qt}
ENABLE_pyqt4:		${ENABLE_pyqt4}
ENABLE_pyqt5:		${ENABLE_pyqt5}
ENABLE_tcl:		${ENABLE_tcl}
ENABLE_itcl:		${ENABLE_itcl}
ENABLE_tk:		${ENABLE_tk}
ENABLE_itk:		${ENABLE_itk}
ENABLE_wxwidgets:	${ENABLE_wxwidgets}
")
message("${_output_results}")
endmacro(summary)
