#include <string.h>

// ----------------------------------------------------------------------------------
// Visibility of this C routine treated with an adaptation of the PLplot method.

#ifdef USINGDLL
  #if defined ( _WIN32 )
// Visual C/C++, Borland, MinGW and Watcom
    #if defined ( __VISUALC__ ) || defined ( _MSC_VER ) || defined ( __BORLANDC__ ) || defined ( __GNUC__ ) || defined ( __WATCOMC__ )
      #define HWDLLEXPORT    __declspec( dllexport )
      #define HWDLLIMPORT    __declspec( dllimport )
    #else
      #define HWDLLEXPORT
      #define HWDLLIMPORT
    #endif
  #elif defined ( __CYGWIN__ )
    #define HWDLLEXPORT    __declspec( dllexport )
    #define HWDLLIMPORT    __declspec( dllimport )
  #elif defined ( __GNUC__ ) && __GNUC__ > 3
// Follow ideas in http://gcc.gnu.org/wiki/Visibility for GCC version 4.x
// The following forces exported symbols specifically designated with
// HWDLLEXPORT to be visible.
    #define HWDLLEXPORT    __attribute__ ( ( visibility( "default" ) ) )
    #define HWDLLIMPORT
  #endif
#endif

// For an unknown compiler or static built we clear the macros
#ifndef HWDLLEXPORT
  #define HWDLLEXPORT
  #define HWDLLIMPORT
#endif

// The IMPEXP macros will always be set to DLLIMPORT (even for
// the static library, but DLLIMPORT is empty in this case), if
// cmake didn't set the corresponding macro xxxx_EXPORTS when the
// corresponding library is built (DLLIMPEXP is set to DLLEXPORT
// then)
#if defined ( hello_world_c_EXPORTS )
  #define HWDLLIMPEXP    HWDLLEXPORT
#else
  #define HWDLLIMPEXP    HWDLLIMPORT
#endif


// ----------------------------------------------------------------------------------


HWDLLIMPEXP void hello_world( char *p_ver );

void hello_world( char *p_ver )
{
    strcpy( p_ver, "hello, world" );
}
