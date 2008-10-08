#ifndef __NN_DLL_H
#define __NN_DLL_H

#ifdef USINGDLL
  #define USINGNNDLL
#endif

#if defined(WIN32)
  /* Visual C/C++, Borland, MinGW and Watcom */
  #if defined(__VISUALC__) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
    #define NNDLLEXPORT __declspec(dllexport)
    #define NNDLLIMPORT __declspec(dllimport)
  #else
    #define NNDLLEXPORT
    #define NNDLLIMPORT
  #endif
#elif defined(__CYGWIN__)
  #define NNDLLEXPORT __declspec(dllexport)
  #define NNDLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ > 3
  /* Follow ideas in http://gcc.gnu.org/wiki/Visibility for GCC version 4.x
   * The following forces exported symbols specifically designated with 
   * NNDLLEXPORT to be visible.  */
  #define NNDLLEXPORT __attribute__ ((visibility("default")))
  #define NNDLLIMPORT
#endif

#ifndef NNDLLEXPORT
  #define NNDLLEXPORT
  #define NNDLLIMPORT
#endif

#if defined(MAKINGNNDLL)
  #define NNDLLIMPEXP NNDLLEXPORT
  #define NNDLLIMPEXP_DATA(type) NNDLLEXPORT type
#elif defined(USINGNNDLL)
  #define NNDLLIMPEXP NNDLLIMPORT
  #define NNDLLIMPEXP_DATA(type) NNDLLIMPORT type
#else
  #define NNDLLIMPEXP
  #define NNDLLIMPEXP_DATA(type) type
  /* Enforce consistency for this case. */
  #define NNDLLEXPORT
  #define NNDLLIMPORT
#endif

#endif /* __NN_DLL_H */
