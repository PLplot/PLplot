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
#endif

#endif /* __NN_DLL_H */
