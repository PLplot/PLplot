/* $Id$
 * $Log$
 * Revision 1.11  1995/06/30 13:47:35  furnish
 * Took out yesterday's hack, which didn't work, and put in a new one
 * which seems to.
 *
 * Revision 1.10  1995/06/28  14:48:25  furnish
 * Little hack to try to cope with the insidious trailing space which
 * screws up Tcl expr something awfull.  This needs to be exercised quite
 * a bit before the next release, though, to be sure it works ok.
 *
 * Revision 1.9  1995/06/09  22:47:45  mjl
 * Eliminated practice of setting local variable (used for tracing) to matrix
 * name.  Was originally used for backward compatibility with old tclMatrix
 * declaration syntax.  The old way will no longer work -- time to upgrade.
 *
 * Revision 1.8  1995/06/02  15:04:22  mjl
 * Support for N-d initializer lists.  Contributed by Martin L. Smith.
 *
 * Revision 1.7  1995/03/21  19:43:53  mjl
 * Added the optional -persist flag to the matrix command line.  If present
 * (can be anywhere on the line), the matrix is not automatically deleted
 * when the scope of the current proc (method) ends.  Instead, you must
 * explicitly clean up by using either the 'delete' matrix command or renaming
 * the matrix command name to {}.  Should now work correctly from within itcl.
 * Copyright message fixed.
 *
 * Revision 1.6  1994/08/09  08:33:35  mjl
 * Changed the tclMatrix API once again.  The 'matrix' operator now sets up a
 * command for the actual name used to declare the matrix.  This is faster
 * than the old way, more intuitive, and less robust (the created matrix is
 * global).  The last problem can presumably fixed by modifications to the
 * Tcl kernel to support a matrix type, and is worth sacrificing for now
 * in favor of a better syntax.  The matrix declarator sets a local variable
 * of the same name as the created (matrix) command in order to automatically
 * free locally-declared matrix resources at the end of a proc.  So previous
 * scripts will still work.
 *
 * Revision 1.5  1994/06/30  18:55:02  mjl
 * Now accepts an initializer when matrix is declared, for example:
 * matrix base i 4 = {0, 200, 500, 600}.  Only works for 1-d arrays so far.
 *
 * Revision 1.4  1994/06/30  05:45:24  furnish
 * Cobbled together an extension facility which allows a user to define
 * their own subcommands for tclMatricies.  The idea is that you can use
 * this to implement your own matrix processing commands entirely on the
 * compiled side.  fft's, matrix multiplication, inversion, etc.
 *
 * Revision 1.3  1994/06/25  20:38:59  mjl
 * Added support for integer matrix data.  Substantially rewrote and improved
 * code in the process.  Extension to handle other types or higher
 * dimensionality arrays should now be straightforward.
 *
 * Revision 1.2  1994/06/24  20:38:23  mjl
 * Changed name of struct to tclMatrix to avoid conflicts with C++ Matrix
 * classes.
 *
 * Revision 1.1  1994/06/23  22:42:53  mjl
 * The Tcl Matrix class moved here, since it is useful independent of the
 * Tk driver.
 *
 * Revision 1.2  1994/06/16  21:55:54  mjl
 * Changed to new declaration syntax.  Now declaration must specify variable
 * that holds matrix operator name.  This allows matrix command to place a
 * trace on any unset of the variable, in which case it can clean up after
 * itself.  I.e. when a matrix is declared locally, it goes away
 * automatically when the proc exits (note to itcl users: since itcl does not
 * let you place traces on class data, you will need to either make it global
 * or create a destructor for it).
 *
 * Revision 1.1  1994/06/16  19:28:35  mjl
 * New Tcl matrix command.  Supplies a much-needed new capability to Tcl.
 * Arrays are stored as binary, column-dominant, and contiguous, so that
 * interaction with scientific code API's as well as Tcl is trivial.
 */

/*
    tclMatrix.c

    Copyright 1994, 1995
    Maurice LeBrun			mjl@dino.ph.utexas.edu
    Institute for Fusion Studies	University of Texas at Austin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	
    This file contains routines that implement Tcl matrices.
    These are operators that are used to store, return, and modify
    numeric data stored in binary array format.  The emphasis is
    on high performance and low overhead, something that Tcl lists
    or associative arrays aren't so good at.
*/

/*
#define DEBUG
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tclMatrix.h"

/* For the truly desperate debugging task */

#ifdef DEBUG_ENTER
#define dbug_enter(a) \
    fprintf(stderr, "%s: Entered %s\n", __FILE__, a);

#else
#define dbug_enter(a)
#endif

/* Internal data */

static int matTable_initted;	/* Hash table initialization flag */
static Tcl_HashTable matTable;	/* Hash table for external access to data */

/* Function prototypes */

/* Handles matrix initialization lists */

static int
matrixInitialize(Tcl_Interp* interp, tclMatrix* m,
		 int dim, int offs, int nargs, char** args);

/* Invoked to process the "matrix" Tcl command. */

static  int
MatrixCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

/* Causes matrix command to be deleted.  */

static char *
DeleteMatrixVar(ClientData clientData,
		Tcl_Interp *interp, char *name1, char *name2, int flags);

/* Releases all the resources allocated to the matrix command. */

static void
DeleteMatrixCmd(ClientData clientData);

/* These do the put/get operations for each supported type */

static void
MatrixPut_f(ClientData clientData, int index, char *string);

static void
MatrixGet_f(ClientData clientData, int index, char *string);

static void
MatrixPut_i(ClientData clientData, int index, char *string);

static void
MatrixGet_i(ClientData clientData, int index, char *string);

/*--------------------------------------------------------------------------*\
 *
 * Tcl_MatCmd --
 *
 *	Invoked to process the "matrix" Tcl command.  Creates a multiply
 *	dimensioned array (matrix) of floats or ints.  The number of
 *	arguments determines the dimensionality.
 *
 * Results:
 *	Returns the name of the new matrix.
 *
 * Side effects:
 *	A new matrix (operator) gets created.
 *
\*--------------------------------------------------------------------------*/

int
Tcl_MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	      int argc, char **argv)
{
    register tclMatrix *matPtr;
    int i, j, length, new, index, persist = 0, initializer = 0;
    Tcl_HashEntry *hPtr;
    Tcl_CmdInfo infoPtr;
    char c, *value, *varName;

    dbug_enter("Tcl_MatrixCmd");

    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
	    " ?-persist? var type dim1 ?dim2? ?dim3? ...\"", (char *) NULL);
	return TCL_ERROR;
    }

/* Create hash table on first call */

    if ( ! matTable_initted) {
	matTable_initted = 1;
	Tcl_InitHashTable(&matTable, TCL_STRING_KEYS);
    }

/* Check for -persist flag */

    for (i = 1; i < argc; i++) {
	c = argv[i][0];
	length = strlen(argv[i]);

    /* If found, set persist variable and compress argv-list */

	if ((c == '-') && (strncmp(argv[i], "-persist", length) == 0)) {
	    persist = 1;
	    argc --;
	    for (j = i; j < argc; j++)
		argv[j] = argv[j+1];
	    break;
	}
    }

/* Create matrix data structure */

    matPtr = (tclMatrix *) ckalloc(sizeof(tclMatrix));
    matPtr->fdata = NULL;
    matPtr->idata = NULL;
    matPtr->name = NULL;
    matPtr->dim = 0;
    matPtr->len = 1;
    matPtr->tracing = 0;
    for (i = 0; i < MAX_ARRAY_DIM; i++)
	matPtr->n[i] = 1;

/* Create name */
/* It should be unique */

    argc--; argv++;

    if (Tcl_GetCommandInfo(interp, argv[0], &infoPtr)) {
	Tcl_AppendResult(interp, "Matrix operator \"", argv[0], 
	    "\" already in use", (char *) NULL);
	ckfree((char *) matPtr);
	return TCL_ERROR;
    }

    if (Tcl_GetVar(interp, argv[0], 0) != NULL) {
	Tcl_AppendResult(interp, "Illegal name for Matrix operator \"", 
	    argv[0], "\": local variable of same name is active", 
	    (char *) NULL);
	ckfree((char *) matPtr);
	return TCL_ERROR;
    }

    matPtr->name = (char *) ckalloc(strlen(argv[0])+1);
    strcpy(matPtr->name, argv[0]);

/* Initialize type */

    argc--; argv++;
    c = argv[0][0];
    length = strlen(argv[0]);

    if ((c == 'f') && (strncmp(argv[0], "float", length) == 0)) {
	matPtr->type = TYPE_FLOAT;
	matPtr->put = MatrixPut_f;
	matPtr->get = MatrixGet_f;
    }
    else if ((c == 'i') && (strncmp(argv[0], "int", length) == 0)) {
	matPtr->type = TYPE_INT;
	matPtr->put = MatrixPut_i;
	matPtr->get = MatrixGet_i;
    }
    else {
	Tcl_AppendResult(interp, "Matrix type \"", argv[0],
	    "\" not supported, should be \"float\" or \"int\"", 
	    (char *) NULL);

	DeleteMatrixCmd((ClientData) matPtr);
	return TCL_ERROR;
    }

/* Initialize dimensions */

    argc--; argv++;
    for (; argc > 0; argc--, argv++) {

    /* Check for initializer */

	if (strcmp(argv[0], "=") == 0) {
	    argc--; argv++;
	    initializer = 1;
	    break;
	}

    /* Must be a dimensional parameter.  Increment number of dimensions. */

	matPtr->dim++;
	if (matPtr->dim > MAX_ARRAY_DIM) {
	    Tcl_AppendResult(interp, 
		"too many dimensions specified for Matrix operator \"",
		matPtr->name, "\"", (char *) NULL);

	    DeleteMatrixCmd((ClientData) matPtr);
	    return TCL_ERROR;
	}

    /* Check to see if dimension is valid and store */

	index = matPtr->dim - 1;
	matPtr->n[index] = atoi(argv[0]);
	if (matPtr->n[index] < 1) {
	    Tcl_AppendResult(interp, "invalid matrix dimension \"", argv[0],
		"\" for Matrix operator \"", matPtr->name, "\"", 
		(char *) NULL);

	    DeleteMatrixCmd((ClientData) matPtr);
	    return TCL_ERROR;
	}
	matPtr->len *= matPtr->n[index];
    }

    if (matPtr->dim < 1) {
	Tcl_AppendResult(interp, 
	    "insufficient dimensions given for Matrix operator \"",
	     matPtr->name, "\"", (char *) NULL);
	DeleteMatrixCmd((ClientData) matPtr);
	return TCL_ERROR;
    }

/* Allocate space for data */

    switch (matPtr->type) {
    case TYPE_FLOAT:
	matPtr->fdata = (Mat_float *) ckalloc(matPtr->len * sizeof(Mat_float));
	for (i = 0; i < matPtr->len; i++)
	    matPtr->fdata[i] = 0.0;
	break;

    case TYPE_INT:
	matPtr->idata = (Mat_int *) ckalloc(matPtr->len * sizeof(Mat_int));
	for (i = 0; i < matPtr->len; i++)
	    matPtr->idata[i] = 0;
	break;
    }

/* Process the initializer, if present */

    if (initializer) 
	matrixInitialize(interp, matPtr, 0, 0, 1, &argv[0]);

/* Delete matrix when it goes out of scope unless -persist specified */
/* Use local variable of same name as matrix and trace it for unsets */

    if ( ! persist) {
	if (Tcl_SetVar(interp, matPtr->name,
		       "old_bogus_syntax_please_upgrade", 0) == NULL) {
	    Tcl_AppendResult(interp, "unable to schedule Matrix operator \"", 
		matPtr->name, "\" for automatic deletion", (char *) NULL);
	    DeleteMatrixCmd((ClientData) matPtr);
	    return TCL_ERROR;
	}
	matPtr->tracing = 1;
	Tcl_TraceVar(interp, matPtr->name, TCL_TRACE_UNSETS, DeleteMatrixVar,
		     (ClientData) matPtr);
    }

/* Create matrix operator */

#ifdef DEBUG
    fprintf(stderr, "Creating Matrix operator of name %s\n", matPtr->name);
#endif
    Tcl_CreateCommand(interp, matPtr->name, MatrixCmd, (ClientData) matPtr,
		      DeleteMatrixCmd);

/* Store pointer to interpreter to handle bizarre uses of multiple */
/* interpreters (e.g. as in [incr Tcl]) */ 

    matPtr->interp = interp;

/* Create hash table entry for this matrix operator's data */
/* This should never fail */

    hPtr = Tcl_CreateHashEntry(&matTable, matPtr->name, &new);
    if ( ! new) {
	Tcl_AppendResult(interp, 
	    "Unable to create hash table entry for Matrix operator \"",
	    matPtr->name, "\"", (char *) NULL);
	return TCL_ERROR;
    }
    Tcl_SetHashValue(hPtr, matPtr);

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 *
 * Tcl_GetMatrixPtr --
 *
 *	Returns a pointer to the specified matrix operator's data.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
\*--------------------------------------------------------------------------*/

tclMatrix *
Tcl_GetMatrixPtr(Tcl_Interp *interp, char *matName)
{
    Tcl_HashEntry *hPtr;

    dbug_enter("Tcl_GetMatrixPtr");

    hPtr = Tcl_FindHashEntry(&matTable, matName);
    if (hPtr == NULL) {
	Tcl_AppendResult(interp, "No matrix operator named \"",
			 matName, "\"", (char *) NULL);
	return NULL;
    }
    return (tclMatrix *) Tcl_GetHashValue(hPtr);
}

/*--------------------------------------------------------------------------*\
 *
 *  Tcl_MatrixInstallXtnsn --
 *
 *	Install a tclMatrix extension subcommand.
 *
 * Results:
 *	Should be 1.  Have to think about error results.
 *
 * Side effects:
 *	Enables you to install special purpose compiled code to handle 
 *	custom operations on a tclMatrix.
 *
\*--------------------------------------------------------------------------*/

static tclMatrixXtnsnDescr *head = (tclMatrixXtnsnDescr *) NULL;
static tclMatrixXtnsnDescr *tail = (tclMatrixXtnsnDescr *) NULL;

int
Tcl_MatrixInstallXtnsn( char *cmd, tclMatrixXtnsnProc proc )
{
/*
 * My goodness how I hate primitive/pathetic C.  With C++ this
 * could've been as easy as:
 *     List<TclMatrixXtnsnDescr> xtnlist;
 *     xtnlist.append( tclMatrixXtnsnDescr(cmd,proc) );
 * grrrrr.
 */

    tclMatrixXtnsnDescr *new =
	(tclMatrixXtnsnDescr *) malloc(sizeof(tclMatrixXtnsnDescr));

    dbug_enter("Tcl_MatrixInstallXtnsn");

#ifdef DEBUG
    fprintf(stderr, "Installing a tclMatrix extension -> %s\n", cmd );
#endif

    new->cmd = malloc( strlen(cmd)+1 );
    strcpy( new->cmd, cmd );
    new->cmdproc = proc;
    new->next = (tclMatrixXtnsnDescr *) NULL;

    if (!head) {
	tail = head = new;
	return 1;
    } else {
	tail = tail->next = new;
	return 1;
    }
	
}
    
/*--------------------------------------------------------------------------*\
 *
 * matrixInitialize --
 *
 *	Handles matrix initialization lists.
 *	Written by Martin L. Smith.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
\*--------------------------------------------------------------------------*/

static int matrixInitialize(Tcl_Interp* interp, tclMatrix* m,
			    int dim, int offs, int nargs, char** args)
{
    static int verbose = 0;

    char** newargs;
    int numnewargs;
    int newoffs;
    int i;

    if (verbose)
	fprintf(stderr, "level %d  offset %d  args %d\n", dim, offs, nargs);

    if (dim < m->dim) {
	for (i = 0; i < nargs; i++) {
	    if (Tcl_SplitList(interp, args[i], &numnewargs, &newargs) 
		!= TCL_OK) {
		Tcl_AppendResult(interp, "bad matrix initializer list form: ",
				 args[i], (char *) NULL);
		return TCL_ERROR;
	    }
	    if (dim > 0)
		newoffs = offs * m->n[dim - 1] + i;
	    else
		newoffs = 0;

	    matrixInitialize(interp, m, dim + 1, newoffs, numnewargs, newargs);
	    free((char *) newargs);
	}
	return TCL_OK;
    }
    
    for (i = 0; i < nargs; i++) {
	newoffs = offs * m->n[dim - 1] + i;
	(m->put)((ClientData) m, newoffs, args[i]);
	if (verbose)
	    fprintf(stderr, "\ta[%d] = %s\n",  newoffs, args[i]);
    }
    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 *
 * MatrixCmd --
 *
 *	When a Tcl matrix command is invoked, this routine is called.
 *
 * Results:
 *	A standard Tcl result value, usually TCL_OK.
 *	On matrix get commands, one or a number of matrix elements are
 *	printed. 
 *
 * Side effects:
 *	Depends on the matrix command.
 *
\*--------------------------------------------------------------------------*/

static int
MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    register tclMatrix *matPtr = (tclMatrix *) clientData;
    int length, put = 0;
    char c, tmp[80];
    char *name = argv[0];
    int nmin[MAX_ARRAY_DIM], nmax[MAX_ARRAY_DIM];
    int i, j, k;

    for (i = 0; i < MAX_ARRAY_DIM; i++) {
	nmin[i] = 0;
	nmax[i] = matPtr->n[i]-1;
    }

/* First check for a matrix command */

    argc--; argv++;
    c = argv[0][0];
    length = strlen(argv[0]);

/* dump -- send a nicely formatted listing of the array contents to stdout */
/* (very helpful for debugging) */

    if ((c == 'd') && (strncmp(argv[0], "dump", length) == 0)) {
	for (i = nmin[0]; i <= nmax[0]; i++) {
	    for (j = nmin[1]; j <= nmax[1]; j++) {
		for (k = nmin[2]; k <= nmax[2]; k++) {
		    (*matPtr->get)((ClientData) matPtr, I3D(i,j,k), tmp);
		    printf(tmp);
		}
		if (matPtr->dim > 2)
		    printf("\n");
	    }
	    if (matPtr->dim > 1)
		printf("\n");
	}
	printf("\n");
	return TCL_OK;
    }

/* delete -- delete the array */

    else if ((c == 'd') && (strncmp(argv[0], "delete", length) == 0)) {
#ifdef DEBUG
	fprintf(stderr, "Deleting array %s\n", name);
#endif
	Tcl_DeleteCommand(interp, name);
	return TCL_OK;
    }

/* info */

    else if ((c == 'i') && (strncmp(argv[0], "info", length) == 0)) {
	for (i = 0; i < matPtr->dim; i++) {
	    sprintf(tmp, "%d ", matPtr->n[i]);
	    Tcl_AppendResult(interp, tmp, (char *) NULL);
	}
	return TCL_OK;
    }

/* redim */

    else if ((c == 'r') && (strncmp(argv[0], "redim", length) == 0)) {
	fprintf(stderr, "Redimensioning array %s... NOT!\n", name);
	return TCL_OK;
    }

/* Not a "standard" command, check the extension commands. */

    {
	tclMatrixXtnsnDescr *p = head;
	for( ; p; p=p->next ) {
	    if ((c == p->cmd[0]) && (strncmp(argv[0],p->cmd,length) == 0)) {
#ifdef DEBUG
		printf( "found a match, invoking %s\n", p->cmd );
#endif
		return (*(p->cmdproc))( matPtr, interp, --argc, ++argv );
	    }
	}
    }

/* Must be a put or get.  Get array indices.  */

    if (argc < matPtr->dim) {
	Tcl_AppendResult(interp, "not enough dimensions specified for \"",
			 name, (char *) NULL);
	return TCL_ERROR;
    }
    for (i = 0; i < matPtr->dim; i++) {
	if (strcmp(argv[0], "*") == 0) {
	    nmin[i] = 0;
	    nmax[i] = matPtr->n[i]-1;
	}
	else {
	    nmin[i] = atoi(argv[0]);
	    nmax[i] = nmin[i];
	}
	if (nmin[i] < 0 || nmax[i] > matPtr->n[i]-1) {
	    sprintf(tmp, "Array index %d out of bounds: %s; max: %d\n",
		    i, argv[0], matPtr->n[i]-1);
	    Tcl_AppendResult(interp, tmp, (char *) NULL);
	    return TCL_ERROR;
	}
	argc--; argv++;
    }

/* If there is an "=" after indicies, it's a put.  Do error checking. */

    if (argc > 0) {
	put = 1;
	if (strcmp(argv[0], "=") == 0) {
	    argc--; argv++;
	    if (argc == 0) {
		Tcl_AppendResult(interp, "no value specified",
				 (char *) NULL);
		return TCL_ERROR;
	    }
	    else if (argc > 1) {
		Tcl_AppendResult(interp, "extra characters after value: \"",
				 argv[1], "\"", (char *) NULL);
		return TCL_ERROR;
	    }
	}
	else {
	    Tcl_AppendResult(interp, "extra characters after indices: \"",
			     argv[0], "\"", (char *) NULL);
	    return TCL_ERROR;
	}
    }

/* Do the get/put. */
/* The loop over all elements takes care of the multi-element cases. */

    for (i = nmin[0]; i <= nmax[0]; i++) {
	for (j = nmin[1]; j <= nmax[1]; j++) {
	    for (k = nmin[2]; k <= nmax[2]; k++) {
		if (put) 
		    (*matPtr->put)((ClientData) matPtr, I3D(i,j,k), argv[0]);
		else {
		    (*matPtr->get)((ClientData) matPtr, I3D(i,j,k), tmp);
		    if (i == nmax[0] && j == nmax[1] && k == nmax[2])
			Tcl_AppendResult(interp, tmp, (char *) NULL);
		    else
			Tcl_AppendResult(interp, tmp, " ", (char *) NULL);
		}
	    }
	}
    }

    return TCL_OK;
}

/*--------------------------------------------------------------------------*\
 *
 * Routines to handle Matrix get/put dependent on type:
 *
 * MatrixPut_f	MatrixGet_f
 * MatrixPut_i	MatrixGet_i
 *
 * A "put" converts from string format to the intrinsic type, storing into
 * the array.
 *
 * A "get" converts from the intrinsic type to string format, storing into
 * a string buffer.
 *
\*--------------------------------------------------------------------------*/

static void
MatrixPut_f(ClientData clientData, int index, char *string)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;

    matPtr->fdata[index] = atof(string);
}

static void
MatrixGet_f(ClientData clientData, int index, char *string)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;

    sprintf(string, "%f", matPtr->fdata[index]);
}

static void
MatrixPut_i(ClientData clientData, int index, char *string)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;

    matPtr->idata[index] = atoi(string);
}

static void
MatrixGet_i(ClientData clientData, int index, char *string)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;

    sprintf(string, "%d", matPtr->idata[index]);
}

/*--------------------------------------------------------------------------*\
 *
 * DeleteMatrixVar --
 *
 *	Causes matrix command to be deleted.  Invoked when variable
 *	associated with matrix command is unset.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	See DeleteMatrixCmd.
 *
\*--------------------------------------------------------------------------*/

static char *
DeleteMatrixVar(ClientData clientData,
		Tcl_Interp *interp, char *name1, char *name2, int flags)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;
    Tcl_CmdInfo infoPtr;
    char *name;

    dbug_enter("DeleteMatrixVar");

    if (matPtr->tracing != 0) {
	matPtr->tracing = 0;
	name = (char *) malloc(strlen(matPtr->name)+1);
	strcpy(name, matPtr->name);

#ifdef DEBUG
	if (Tcl_GetCommandInfo(matPtr->interp, matPtr->name, &infoPtr)) {
	    if (Tcl_DeleteCommand(matPtr->interp, matPtr->name) == TCL_OK) 
		fprintf(stderr, "Deleted command %s\n", name);
	    else
		fprintf(stderr, "Unable to delete command %s\n", name);
	}
#else
	if (Tcl_GetCommandInfo(matPtr->interp, matPtr->name, &infoPtr))
	    Tcl_DeleteCommand(matPtr->interp, matPtr->name);
#endif
	free((void *) name);
    }
    return (char *) NULL;
}

/*--------------------------------------------------------------------------*\
 *
 * DeleteMatrixCmd --
 *
 *	Releases all the resources allocated to the matrix command.
 *	Invoked just before a matrix command is removed from an interpreter.
 *
 *	Note: If the matrix has tracing enabled, it means the user
 *	explicitly deleted a non-persistent matrix.  Not a good idea,
 *	because eventually the local variable that was being traced will
 *	become unset and the matrix data will be referenced in
 *	DeleteMatrixVar.  So I've massaged this so that at worst it only
 *	causes a minor memory leak instead of imminent program death. 
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	All memory associated with the matrix operator is freed (usually).
 *
\*--------------------------------------------------------------------------*/

static void
DeleteMatrixCmd(ClientData clientData)
{
    tclMatrix *matPtr = (tclMatrix *) clientData;
    Tcl_HashEntry *hPtr;

    dbug_enter("DeleteMatrixCmd");

#ifdef DEBUG
    fprintf(stderr, "Freeing space associated with matrix %s\n", matPtr->name);
#endif

/* Remove hash table entry */

    hPtr = Tcl_FindHashEntry(&matTable, matPtr->name);
    if (hPtr != NULL) 
	Tcl_DeleteHashEntry(hPtr);

/* Free data */

    if (matPtr->fdata != NULL) {
	ckfree((char *) matPtr->fdata);
	matPtr->fdata = NULL;
    }
    if (matPtr->idata != NULL) {
	ckfree((char *) matPtr->idata);
	matPtr->idata = NULL;
    }

/* Attempt to turn off tracing if possible. */

    if (matPtr->tracing) {
	if (Tcl_VarTraceInfo(matPtr->interp, matPtr->name, TCL_TRACE_UNSETS,
			     DeleteMatrixVar, NULL) != NULL) {
	    matPtr->tracing = 0;
	    Tcl_UntraceVar(matPtr->interp, matPtr->name, TCL_TRACE_UNSETS,
			   DeleteMatrixVar, (ClientData) matPtr);
	    Tcl_UnsetVar(matPtr->interp, matPtr->name, 0);
	}
    } 

/* Free name.  */

    if (matPtr->name != NULL) {
	ckfree((char *) matPtr->name);
	matPtr->name = NULL;
    }

/* Free tclMatrix */

    if ( ! matPtr->tracing)
	ckfree((char *) matPtr);
#ifdef DEBUG
    else
	fprintf(stderr, "OOPS!  You just lost %d bytes\n", sizeof(tclMatrix));
#endif
}
