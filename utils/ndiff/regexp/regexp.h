/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#define NSUBEXP  10
typedef struct regexp {
	const char *startp[NSUBEXP];
	const char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#define REGEXP_ARGS(plist)	plist
#else
#define REGEXP_ARGS(plist)	()
#define const
#endif

extern regexp	*regcomp REGEXP_ARGS((const char *expr_));
extern int	regexec REGEXP_ARGS((regexp *prog_, const char *string_));
extern void	regsub REGEXP_ARGS((regexp *prog_, const char *source_, 
				    char *dest_));
extern void	regerror REGEXP_ARGS((const char *msg_));

#undef REGEXP_ARGS
