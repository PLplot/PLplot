/*--------------------------------------------------------------------------*\
 * %cmd%Cmd
 *
 * Processes %cmd% Tcl command.
\*--------------------------------------------------------------------------*/

static int
%cmd%Cmd( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[] )
{
<argdecls>

    if ( ( %ndefs% && (argc < (1 + %nargs% - %ndefs%))) ||
         (!%ndefs% && (argc != (%nargs% + 1))) ||
         ((argc == 2) && (strcmp(argv[1],"-help") == 0)) ) {

	Tcl_AppendResult( interp, "wrong # args: should be \"",
			  "%cmd%%args%", "\"",
			  (char *) NULL);
	return TCL_ERROR;
    }

<getargs>

<plcmd>

<fetch_result>

    plflush();
    return TCL_OK;
}

