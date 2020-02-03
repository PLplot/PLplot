#if !defined(ARGS_H)
#define ARGS_H		1

#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#include <stdlib.h>
#include <stddef.h>
#define ARGS(plist)	plist
#define NEW_STYLE	1
#define VOID		void

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE	1
#endif

#else
#define ARGS(plist)	()
#define EXIT_FAILURE	1
#define EXIT_SUCCESS	0
#define NEW_STYLE	0
#define VOID
#define const
#endif

#endif /* ARGS_H */
