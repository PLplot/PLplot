#include "plplot/tclMatrix.h"

int Matrix_Init	(Tcl_Interp*);

int Matrix_Init( Tcl_Interp *interp ) {

    /* matrix -- matrix	support	command	*/
    Tcl_CreateCommand(interp, "matrix",	Tcl_MatrixCmd,
		      (ClientData) NULL, (void (*)(ClientData))	NULL);

#if ( TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION > 4 )	// jc:
    Tcl_PkgProvide(interp, "Matrix", "0.1");
#endif

    return TCL_OK;
}
