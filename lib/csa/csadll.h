#ifndef __CSA_DLL_H
#define __CSA_DLL_H

#ifdef USINGDLL
  #define USINGCSADLL
#endif

#if defined(WIN32)
  /* Visual C/C++, Borland, MinGW and Watcom */
  #if defined(__VISUALC__) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
    #define CSADLLEXPORT __declspec(dllexport)
    #define CSADLLIMPORT __declspec(dllimport)
  #else
    #define CSADLLEXPORT
    #define CSADLLIMPORT
  #endif
#elif defined(__CYGWIN__)
  #define CSADLLEXPORT __declspec(dllexport)
  #define CSADLLIMPORT __declspec(dllimport)
#endif

#ifndef CSADLLEXPORT
#    define CSADLLEXPORT
#    define CSADLLIMPORT
#endif

#if defined(MAKINGCSADLL)
  #define CSADLLIMPEXP CSADLLEXPORT
  #define CSADLLIMPEXP_DATA(type) CSADLLEXPORT type
#elif defined(USINGCSADLL)
  #define CSADLLIMPEXP CSADLLIMPORT
  #define CSADLLIMPEXP_DATA(type) CSADLLIMPORT type
#else
  #define CSADLLIMPEXP
  #define CSADLLIMPEXP_DATA(type) type
#endif

#endif /* __CSA_DLL_H */
