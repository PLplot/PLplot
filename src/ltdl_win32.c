/* $Id$

	Contains all prototypes for driver functions.

    Copyright (C) 2008  Werner Smekal

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/

#include <windows.h>

#include "ltdl_win32.h"

lt_dlhandle lastHandle=NULL;

void lt_dlinit( void )
{
  lastHandle=NULL;
}

void lt_dlexit( void )
{
  lt_dlhandle prev;
  
  while( lastHandle!=NULL ) {
    if( lastHandle->hinstLib )
      FreeLibrary( lastHandle->hinstLib );
    prev=lastHandle->previousHandle;
    free( lastHandle );
    lastHandle=prev;
  }
}

lt_dlhandle lt_dlopenext( char* dllname )
{
  lt_dlhandle dlhandle=malloc( sizeof(lt_dlhandle) );
  memset( dlhandle, '\0', sizeof(lt_dlhandle) );
  
  dlhandle->hinstLib = LoadLibrary( dllname );
  if( !dlhandle->hinstLib ) {
    free( dlhandle );
    return NULL;
  }

  dlhandle->previousHandle = lastHandle;
  lastHandle = dlhandle;
  
  return dlhandle;
}

const char* lt_dlerror()
{
  static char* text="No error information";
  
  return text;
}

void* lt_dlsym( lt_dlhandle dlhandle, const char* symbol )
{
  return GetProcAddress( dlhandle->hinstLib, symbol );
}
