#ifndef __PL_DLL_H
#define __PL_DLL_H

#ifdef USINGDLL
  #define USINGPLDLL
#endif

#if defined(WIN32)
  /* Visual C/C++, Borland, MinGW and Watcom */
  #if defined(__VISUALC__) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
    #define PLDLLEXPORT __declspec(dllexport)
    #define PLDLLIMPORT __declspec(dllimport)
  #else
    #define PLDLLEXPORT
    #define PLDLLIMPORT
  #endif
#elif defined(__CYGWIN__)
  #define PLDLLEXPORT __declspec(dllexport)
  #define PLDLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ > 3
  /* Follow ideas in http://gcc.gnu.org/wiki/Visibility for GCC version 4.x
   * The following forces exported symbols specifically designated with 
   * PLDLLEXPORT to be visible.  */
  #define PLDLLEXPORT __attribute__ ((visibility("default")))
  #define PLDLLIMPORT
#endif

#ifndef PLDLLEXPORT
#    define PLDLLEXPORT
#    define PLDLLIMPORT
#endif

#if defined(MAKINGPLDLL)
  #define PLDLLIMPEXP PLDLLEXPORT
  #define PLDLLIMPEXP_DATA(type) PLDLLEXPORT type
#elif defined(USINGPLDLL)
  #define PLDLLIMPEXP PLDLLIMPORT
  #define PLDLLIMPEXP_DATA(type) PLDLLIMPORT type
#else
  #define PLDLLIMPEXP
  #define PLDLLIMPEXP_DATA(type) type
  /* Enforce consistency for this case. */
  #define PLDLLEXPORT
  #define PLDLLIMPORT
#endif

#endif /* __PL_DLL_H */
