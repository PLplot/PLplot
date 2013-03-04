// Get device info from PLplot driver module
//
// Copyright (C) 2003  Rafael Laboissiere
// Copyright (C) 2004  Joao Cardoso
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify it under
// the terms of the GNU Library General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// PLplot is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library
// General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the GNU C Library; see the file COPYING.LIB.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//

#include "plplotP.h"
#ifndef LTDL_WIN32
  #include <ltdl.h>
#else
  #include "ltdl_win32.h"
#endif
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define SYM_LEN        300
#define DRVSPEC_LEN    400

// function prototype
RETSIGTYPE catch_segv( int sig );

// SEGV signal handler
RETSIGTYPE
catch_segv( int PL_UNUSED( sig ) )
{
    fprintf( stderr, "libltdl error: %s\n", lt_dlerror() );
    exit( 1 );
}

int
main( int PL_UNUSED( argc ), char* argv[] )
{
    lt_dlhandle dlhand;
    char        sym[SYM_LEN];
    char        * drvnam = argv[1];
    char        drvspec[ DRVSPEC_LEN ];
    char        ** info;

    // Establish a handler for SIGSEGV signals.
    signal( SIGSEGV, catch_segv );

    lt_dlinit();
#if defined ( LTDL_WIN32 ) || defined ( __CYGWIN__ )
    snprintf( drvspec, DRVSPEC_LEN, "%s", drvnam );
#else
    snprintf( drvspec, DRVSPEC_LEN, "%s/%s", plGetDrvDir(), drvnam );
#endif // LTDL_WIN32
    dlhand = lt_dlopenext( drvspec );
    if ( dlhand == NULL )
    {
        fprintf( stderr, "Could not open driver module %s\n"
            "libltdl error: %s\n", drvspec, lt_dlerror() );
        exit( 1 );
    }
    snprintf( sym, SYM_LEN, "plD_DEVICE_INFO_%s", drvnam );
    info = (char **) lt_dlsym( dlhand, sym );
    if ( info != NULL )
    {
        printf( "%s", *info );
        exit( 0 );
    }
    else
    {
        fprintf( stderr, "Could not read symbol %s in driver module %s\n"
            "libltdl error: %s\n", sym, drvspec, lt_dlerror() );
        exit( 1 );
    }
}
