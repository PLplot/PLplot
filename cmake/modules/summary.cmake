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
Important CMake results:

CMAKE_SYSTEM_NAME:	${CMAKE_SYSTEM_NAME}
UNIX:			${UNIX}
WIN32:			${WIN32}
APPLE:			${APPLE}
MINGW:			${MINGW}
CYGWIN:			${CYGWIN}
BORLAND:		${BORLAND}

SWIG_FOUND:		${SWIG_FOUND}
PERL_FOUND:		${PERL_FOUND}
X11_FOUND:		${X11_FOUND}

CMAKE_INSTALL_PREFIX:	${CMAKE_INSTALL_PREFIX}
CMAKE_BUILD_TYPE:	${CMAKE_BUILD_TYPE}
CMAKE_C_COMPILER CMAKE_C_FLAGS:			${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")

if(ENABLE_cxx)
set(
_output_results
"${_output_results}
CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS:		${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS}")
endif(ENABLE_cxx)

if(ENABLE_f77 OR ENABLE_f95)
set(
_output_results
"${_output_results}
CMAKE_Fortran_COMPILER CMAKE_Fortran_FLAGS:	${CMAKE_Fortran_COMPILER} ${CMAKE_Fortran_FLAGS}")
endif(ENABLE_f77 OR ENABLE_f95)

set(
_output_results
"${_output_results}
LIB_TAG:		${LIB_TAG}

ENABLE_DYNDRIVERS:	${ENABLE_DYNDRIVERS}
DEVICES_LIST: ${DEVICES_LIST}
DRIVERS_LIST: ${DRIVERS_LIST}

Library options:
BUILD_SHARED_LIBS:	${BUILD_SHARED_LIBS}		PL_DOUBLE:	${PL_DOUBLE}

Optional libraries:
HAVE_QHULL:		${HAVE_QHULL}		WITH_CSA:	${WITH_CSA}
HAVE_FREETYPE:		${HAVE_FREETYPE}		HAVE_PTHREAD:	${HAVE_PTHREAD}

Language Bindings:
ENABLE_f77:		${ENABLE_f77}		ENABLE_f95:	${ENABLE_f95}
ENABLE_cxx:		${ENABLE_cxx}		ENABLE_java:	${ENABLE_java}
ENABLE_python:		${ENABLE_python}		ENABLE_octave:	${ENABLE_octave}
ENABLE_tcl:		${ENABLE_tcl}		ENABLE_itcl:	${ENABLE_itcl}
ENABLE_tk:		${ENABLE_tk}		ENABLE_itk:	${ENABLE_itk}
ENABLE_pdl:		${ENABLE_pdl}
")
message("${_output_results}")
endmacro(summary)
