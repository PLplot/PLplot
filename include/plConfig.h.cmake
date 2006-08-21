/* -*-C-*- */
/* $Id$

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Copyright (C) 2004, 2006  Alan W. Irwin
    Copyright (C) 2004  Rafael Laboissiere
    Copyright (C) 2004  Joao Cardoso

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Library Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


    Contains macro definitions that determine miscellaneous PLplot library
    configuration defaults, such as macros for bin, font, lib, and tcl
    install directories, and various system dependencies.  On a Unix
    system, typically the configure script builds plConfig.h from
    plConfig.h.in.  Elsewhere, it's best to hand-configure a plConfig.h
    file and keep it with the system-specific files.
*/

#ifndef __PLCONFIG_H__
#define __PLCONFIG_H__

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/* Define if dynamic drivers are enabled.*/
#cmakedefine ENABLE_DYNDRIVERS

/* Define if [incr Tcl] is available */
#cmakedefine HAVE_ITCL

/* Define if [incr Tk] is available */
#cmakedefine HAVE_ITK

/* Define if [freetype] is available */
#cmakedefine HAVE_FREETYPE

/* Define if finite is available */
#cmakedefine HAVE_FINITE

/* Define if isinf is available */
#cmakedefine HAVE_ISINF

/* Define if isnan is available */
#cmakedefine HAVE_ISNAN

/* Define if you have c++ accessible stdint.h */
#cmakedefine HAVE_CXX_STDINT_H

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine HAVE_UNISTD_H 1

/* Define if usleep is available */
#cmakedefine HAVE_USLEEP

/* Define if you want PLplot's float type to be double */
#cmakedefine PL_DOUBLE

/* Define if C++ compiler accepts using namespace */
#cmakedefine USE_NAMESPACE

/* Define to 1 if <cmath> header file is available */
#cmakedefine HAVE_CMATH 1

/* Define if isnan is present in <math.h> but not in <cmath> 
 * - broken Mac OSX systems */
#cmakedefine BROKEN_ISNAN_CXX

#endif	/* __PLCONFIG_H__ */
