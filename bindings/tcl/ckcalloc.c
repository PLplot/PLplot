
#include "plplotP.h"

/* 
 * This function is a replacement calloc for use when Tcl's allocators are
 * used to over-ride the standard ones.  This is useful when running
 * Plplot under MacOS or Windows when using the Tcl toolkit.
 */

void *	ckcalloc(size_t nmemb, size_t size) {
	long *ptr;
	long *p;
	size *= nmemb;
	ptr = (long*) ckalloc(size);
	if (!ptr)
		return(0);
#if !__POWERPC__
	
	for (size = (size / sizeof(long)) + 1, p = ptr; --size;)
		*p++ = 0;

#else
	
	for (size = (size / sizeof(long)) + 1, p = ptr - 1; --size;)
		*++p = 0;

#endif
	
	return(ptr);
}

