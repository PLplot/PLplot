/* 
 * matrixStubLib.c --
 *
 *	Stub object that will be statically linked into extensions that wish
 *	to access Matrix.
 */


/*
 * We need to ensure that we use the stub macros so that this file contains
 * no references to any of the stub functions.  This will make it possible
 * to build an extension that references Matrix_InitStubs but doesn't end up
 * including the rest of the stub functions.
 */

#ifndef USE_TCL_STUBS
#define USE_TCL_STUBS
#endif
#undef  USE_TCL_STUB_PROCS

#ifndef USE_MATRIX_STUBS
#define USE_MATRIX_STUBS
#endif
#undef  USE_MATRIX_STUB_PROCS

#include "tclMatrix.h"
#include "matrixIntDecls.h"

/*
 * Ensure that Matrix_InitStubs is built as an exported symbol.  The other stub
 * functions should be built as non-exported symbols.
 */

#undef  TCL_STORAGE_CLASS
#define TCL_STORAGE_CLASS DLLEXPORT

MatrixStubs *matrixStubsPtr;
MatrixIntStubs *matrixIntStubsPtr;


/*
 *----------------------------------------------------------------------
 *
 * Matrix_InitStubs --
 *
 *	Checks that the correct version of Matrix is loaded and that it
 *	supports stubs. It then initialises the stub table pointers.
 *
 * Results:
 *	The actual version of Matrix that satisfies the request, or
 *	NULL to indicate that an error occurred.
 *
 * Side effects:
 *	Sets the stub table pointers.
 *
 *----------------------------------------------------------------------
 */

#ifdef Matrix_InitStubs
#undef Matrix_InitStubs
#endif

char *
Matrix_InitStubs(interp, version, exact)
    Tcl_Interp *interp;
    CONST char *version;
    int exact;
{
    char *actualVersion;

    actualVersion = Tcl_PkgRequireEx(interp, "Matrix", (char *) version, exact,
		(ClientData *) &matrixStubsPtr);

    if (!actualVersion) {
	return NULL;
    }

    if (!matrixStubsPtr) {
	Tcl_SetResult(interp,
		"This implementation of Matrix does not support stubs",
		TCL_STATIC);
	return NULL;
    }
    
    matrixIntStubsPtr = matrixStubsPtr->hooks->matrixIntStubs;
    
    return actualVersion;
}
