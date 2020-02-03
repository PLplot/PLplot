#include <stdio.h>
#include "args.h"

void	regerror ARGS((const char *s_));

#if NEW_STYLE
void
regerror(const char *s)
#else
void
regerror(s)
const char *s;
#endif
{
#ifdef ERRAVAIL
	error("regexp: %s", s);
#else
	fprintf(stderr, "regexp(3): %s", s);
	exit(EXIT_FAILURE);
#endif
	/* NOTREACHED */
}
