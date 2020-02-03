#if !defined(CONFIX_H_)

#include <config.h>

#if defined(__NeXT__)
#undef HAVE_LONG_DOUBLE	/* NeXT compilers recognize it, but library doesn't handle properly, sigh... */
#endif

#if defined(_IBMR2) || defined(_POWER)
#undef HAVE_LONG_DOUBLE			/* see README.QUAD for why */
#endif

#if defined(__ppc__) || defined(_ppc)
#undef HAVE_LONG_DOUBLE			/* see README.QUAD for why */
#endif

#endif /* !defined(CONFIX_H_) */
