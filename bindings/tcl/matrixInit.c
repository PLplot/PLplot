/* $Id$

    Tcl Matrix initializer.
    Vince Darley
*/

#include "plplot/tclMatrix.h"

int Matrix_Init( Tcl_Interp *interp ) {

    /* matrix -- matrix	support	command	*/
    Tcl_CreateCommand(interp, "matrix",	Tcl_MatrixCmd,
		      (ClientData) NULL, (void (*)(ClientData))	NULL);

    Tcl_PkgProvide(interp, "Matrix", "0.1");
    return TCL_OK;
}
