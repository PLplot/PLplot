/* $Id$
 * $Log$
 * Revision 1.2  1994/06/16 21:55:54  mjl
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

/*----------------------------------------------------------------------*\
 *
 * tclMatrix.c --
 *
 *	This file contains routines that implement Tcl matrices.
 *	These are operators that are used to store, return, and modify
 *	numeric data stored in binary array format.  The emphasis is
 *	on high performance and low overhead, something that Tcl lists
 *	or associative arrays aren't so good at.
 *
 * Maurice LeBrun
 * IFS, University of Texas at Austin
 * 10-Jun-1994
\*----------------------------------------------------------------------*/

/*
#define DEBUG
*/

#include <stdlib.h>
#include <tcl.h>
#include "tclMatrix.h"

/* Internal data */

static int matNum;		/* Counter used in simple naming scheme */
static int matTable_initted;	/* Hash table initialization flag */
static Tcl_HashTable matTable;	/* Hash table for external access to data */

/* Function prototypes */

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

/*----------------------------------------------------------------------*\
 *
 * Tcl_MatCmd --
 *
 *	Invoked to process the "matrix" Tcl command.
 *	Creates a multiply dimensioned array (matrix) of floats (other
 *	types will be supported eventually).  The number of arguments
 *	determines the dimensionality.
 *
 * Results:
 *	Returns the name of the new matrix.
 *
 * Side effects:
 *	A new matrix (operator) gets created.
 *
\*----------------------------------------------------------------------*/

int
Tcl_MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	      int argc, char **argv)
{
    register Matrix *matPtr;
    int i, new;
    Tcl_HashEntry *hPtr;
    char *varName;

    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" var type dim1 ?dim2? ?dim3? ...\"", (char *) NULL);
	return TCL_ERROR;
    }

    if (argc > MAX_ARRAY_DIM + 4) {
	Tcl_AppendResult(interp, "too many dimensions specified",
			 (char *) NULL);
	return TCL_ERROR;
    }

/* Create hash table on first call */

    if ( ! matTable_initted) {
	matTable_initted = 1;
	Tcl_InitHashTable(&matTable, TCL_STRING_KEYS);
    }

/* Create matrix data structure */

    matPtr = (Matrix *) ckalloc(sizeof(Matrix));
    for (i = 0; i < MAX_ARRAY_DIM; i++)
	matPtr->n[i] = 0;

/* Create name */

    matNum++;
    sprintf(matPtr->name, "matrix_%d", matNum);

    argc--; argv++;
    varName = argv[0];

    if (Tcl_GetVar(interp, varName, 0) != NULL) {
	Tcl_AppendResult(interp, "Local variable of name \"", varName,
			 "\" already exists", (char *) NULL);
	ckfree((char *) matPtr);
	return TCL_ERROR;
    }

    if (Tcl_SetVar(interp, varName, matPtr->name, 0) == NULL) {
	Tcl_AppendResult(interp, "unable to set variable to matrix name",
			 (char *) NULL);
	ckfree((char *) matPtr);
	return TCL_ERROR;
    }
    Tcl_TraceVar(interp, varName, TCL_TRACE_UNSETS, DeleteMatrixVar,
		 (ClientData) matPtr);

/* Initialize type */

    argc--; argv++;

    if (strcmp(argv[0], "float") == 0) {
	matPtr->type = TYPE_FLOAT;
    }
    else {
	Tcl_AppendResult(interp, "Matrix type \"", argv[0],
		" not supported, should be \"float\"", (char *) NULL);

	ckfree((char *) matPtr);
	return TCL_ERROR;
    }

/* Initialize dimensions */

    argc--; argv++;

    matPtr->dim = argc;
    matPtr->len = 1;

    for (i = 0; i < matPtr->dim; i++) {
	matPtr->n[i] = atoi(argv[i]);

	if (matPtr->n[i] <= 0) {
	    Tcl_AppendResult(interp, "invalid matrix dimension \"", argv[i],
			     "\"", (char *) NULL);

	    ckfree((char *) matPtr);
	    return TCL_ERROR;
	}
	matPtr->len *= matPtr->n[i];
    }

/* Allocate space for data */

    switch (matPtr->type) {
    case TYPE_FLOAT:
	matPtr->fdata = (Mat_float *) ckalloc(matPtr->len * sizeof(Mat_float));
	for (i = 0; i < matPtr->len; i++)
	    matPtr->fdata[i] = 0.0;
	break;
    }

/* Create matrix operator */

    Tcl_CreateCommand(interp, matPtr->name, MatrixCmd, (ClientData) matPtr,
		      DeleteMatrixCmd);

/* Create hash table entry for this matrix operator's data */
/* This should never fail */

    hPtr = Tcl_CreateHashEntry(&matTable, matPtr->name, &new);
    if ( ! new) {
	Tcl_AppendResult(interp, "Unable to create hash table entry",
			 (char *) NULL);
	return TCL_ERROR;
    }
    Tcl_SetHashValue(hPtr, matPtr);

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
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
\*----------------------------------------------------------------------*/

Matrix *
Tcl_GetMatrixPtr(Tcl_Interp *interp, char *matName)
{
    Tcl_HashEntry *hPtr;

    hPtr = Tcl_FindHashEntry(&matTable, matName);
    if (hPtr == NULL) {
	Tcl_AppendResult(interp, "No matrix operator named \"",
			 matName, "\"", (char *) NULL);
	return NULL;
    }
    return (Matrix *) Tcl_GetHashValue(hPtr);
}

/*----------------------------------------------------------------------*\
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
\*----------------------------------------------------------------------*/

static int
MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	  int argc, char **argv)
{
    register Matrix *matPtr = (Matrix *) clientData;
    int length, put = 0;
    char c, tmp[80];
    char* name = argv[0];
    int nmin[MAX_ARRAY_DIM], nmax[MAX_ARRAY_DIM];
    Mat_float fvalue;
    int i, j, k;

    for (i = 0; i < MAX_ARRAY_DIM; i++) {
	nmin[i] = 0;
	nmax[i] = 0;
    }

/* First check for a matrix command */

    argc--; argv++;
    c = argv[0][0];
    length = strlen(argv[0]);

/* delete -- delete the array */

    if ((c == 'd') && (strncmp(argv[0], "delete", length) == 0)) {
	fprintf(stderr, "Deleting array %s\n", name);
	Tcl_DeleteCommand(interp, name);
	return TCL_OK;
    }

/* dump -- send a nicely formatted listing of the array contents to stdout */
/* (mostly for debugging) */

    else if ((c == 'd') && (strncmp(argv[0], "dump", length) == 0)) {
	if (matPtr->dim == 1) {
	    for (i = 0; i < matPtr->n[0]; i++) {
		printf("%f ", M1D(i));
	    }
	    printf("\n");
	}
	else if (matPtr->dim == 2) {
	    for (i = 0; i < matPtr->n[0]; i++) {
		for (j = 0; j < matPtr->n[1]; j++) {
		    printf("%f ", M2D(i,j));
		}
		printf("\n");
	    }
	    printf("\n");
	}
	else if (matPtr->dim == 3) {
	    for (i = 0; i < matPtr->n[0]; i++) {
		for (j = 0; j < matPtr->n[1]; j++) {
		    for (k = 0; k < matPtr->n[2]; k++) {
			printf("%f ", M3D(i,j,k));
		    }
		    printf("\n");
		}
		printf("\n");
	    }
	    printf("\n");
	}
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

/* If there is an "=" after indicies, it's a put.  Get value to put. */

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
	    else {
		fvalue = atof(argv[0]);
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

    if (matPtr->dim == 1) {
	for (i = nmin[0]; i <= nmax[0]; i++) {
	    if (put) {
		M1D(i) = fvalue;
	    }
	    else {
		sprintf(tmp, "%f ", M1D(i));
		Tcl_AppendResult(interp, tmp, (char *) NULL);
	    }
	}
    }
    else if (matPtr->dim == 2) {
	for (i = nmin[0]; i <= nmax[0]; i++) {
	    for (j = nmin[1]; j <= nmax[1]; j++) {
		if (put) {
		    M2D(i,j) = fvalue;
		}
		else {
		    sprintf(tmp, "%f ", M2D(i,j));
		    Tcl_AppendResult(interp, tmp, (char *) NULL);
		}
	    }
	}
    }
    else if (matPtr->dim == 3) {
	for (i = nmin[0]; i <= nmax[0]; i++) {
	    for (j = nmin[1]; j <= nmax[1]; j++) {
		for (k = nmin[2]; k <= nmax[2]; k++) {
		    if (put) {
			M3D(i,j,k) = fvalue;
		    }
		    else {
			sprintf(tmp, "%f ", M3D(i,j,k));
			Tcl_AppendResult(interp, tmp, (char *) NULL);
		    }
		}
	    }
	}
    }

    return TCL_OK;
}

/*----------------------------------------------------------------------*\
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
\*----------------------------------------------------------------------*/

static char *
DeleteMatrixVar(ClientData clientData,
		Tcl_Interp *interp, char *name1, char *name2, int flags)
{
    Matrix *matPtr = (Matrix *) clientData;

    if (Tcl_DeleteCommand(interp, matPtr->name) != TCL_OK) 
	fprintf(stderr, "Unable to delete command %s\n", matPtr->name);
#ifdef DEBUG
    else
	fprintf(stderr, "Deleted command %s\n", matPtr->name);
#endif
    return (char *) NULL;
}

/*----------------------------------------------------------------------*\
 *
 * DeleteMatrixCmd --
 *
 *	Releases all the resources allocated to the matrix command.
 *	Invoked just before a matrix command is removed from an
 *	interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	All memory associated with the matrix operator is freed.
 *
\*----------------------------------------------------------------------*/

static void
DeleteMatrixCmd(ClientData clientData)
{
    Matrix *matPtr = (Matrix *) clientData;

#ifdef DEBUG
    fprintf(stderr, "Freeing space associated with matrix %s\n", matPtr->name);
#endif
    if (matPtr->fdata != NULL) 
	ckfree((char *) matPtr->fdata);
    ckfree((char *) matPtr);
}

