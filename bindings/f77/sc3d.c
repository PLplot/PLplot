/* $Id$
   $Log$
   Revision 1.4  1992/10/22 17:05:44  mjl
   Fixed warnings, errors generated when compling with HP C++.

 * Revision 1.3  1992/09/30  18:25:59  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:46:34  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:35:06  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	sc3d.c

	Stub routines for 3d plots.
*/

#define PL_NEED_MALLOC
#define PL_NEED_SIZE_T

#include "plstubs.h"

void
PLMESH( PLFLT *x, PLFLT *y, PLFLT *z,
       PLINT *nx, PLINT *ny, PLINT *opt, PLINT *lx )
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *lx + i);

    c_plmesh(x, y, temp, *nx, *ny, *opt);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}


void
PLOT3D( PLFLT *x, PLFLT *y, PLFLT *z,
       PLINT *nx, PLINT *ny, PLINT *opt, PLINT *side, PLINT *lx )
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if (!(temp = (PLFLT **) malloc((size_t)*nx * sizeof(PLFLT *))))
	plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	if (!(temp[i] = (PLFLT *) malloc((size_t)*ny * sizeof(PLFLT))))
	    plexit("Out of memory");

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *lx + i);

    c_plot3d(x, y, temp, *nx, *ny, *opt, *side);

    for (i = 0; i < *nx; i++)
	free((char *) temp[i]);

    free((char *) temp);
}
