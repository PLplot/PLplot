# matrix.decls --
#
#	This file contains the declarations for all supported public
#	functions that are exported by the Matrix library via the stubs table.
#	This file is used to generate the matrixDecls.h file.
#	

library matrix

# Define the tcl interface with several sub interfaces:
#     tclPlat	 - platform specific public
#     tclInt	 - generic private
#     tclPlatInt - platform specific private

interface matrix
hooks {matrixInt}

# Declare each of the functions in the public Matrix interface.  Note that
# every index should never be reused for a different function in order
# to preserve backwards compatibility.

declare 0 generic {
    tclMatrix* Tcl_GetMatrixPtr(Tcl_Interp *interp, char* matName)
}

declare 1 generic {
    int Tcl_MatrixInstallXtnsn(char *cmd, tclMatrixXtnsnProc proc)
}
