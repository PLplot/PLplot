/* $Id$

	Stub routines for 3d plots.
*/

#include "plstubs.h"

void
PLMESH(PLFLT *x, PLFLT *y, PLFLT *z,
       PLINT *nx, PLINT *ny, PLINT *opt, PLINT *lx)
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if ( ! (temp = (PLFLT **) malloc((size_t) * nx * sizeof(PLFLT *)))) {
	plabort("PLMESH: Out of memory");
	return;
    }

    for (i = 0; i < *nx; i++) {
	if ( ! (temp[i] = (PLFLT *) malloc((size_t) * ny * sizeof(PLFLT)))) {
	    int ii;

	    for (ii = 0; ii < i-1; ii++)
		free((void *) temp[i]);
	    free((void *) temp);
	    plabort("PLMESH: Out of memory");
	    return;
	}
    }

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *lx + i);

    c_plmesh(x, y, temp, *nx, *ny, *opt);

    for (i = 0; i < *nx; i++)
	free((void *) temp[i]);

    free((void *) temp);
}


void
PLOT3D(PLFLT *x, PLFLT *y, PLFLT *z,
       PLINT *nx, PLINT *ny, PLINT *opt, PLINT *side, PLINT *lx)
{
    int i, j;
    PLFLT **temp;

    /* Create the vectored C matrix from the Fortran matrix */
    /* To make things easy we save a temporary copy of the transpose of the
       Fortran matrix, so that the first dimension of z corresponds to the x
       direction. */

    if ( ! (temp = (PLFLT **) malloc((size_t) * nx * sizeof(PLFLT *)))) {
	plabort("PLOT3D: Out of memory");
	return;
    }

    for (i = 0; i < *nx; i++) {
	if ( ! (temp[i] = (PLFLT *) malloc((size_t) * ny * sizeof(PLFLT)))) {
	    int ii;

	    for (ii = 0; ii < i-1; ii++)
		free((void *) temp[i]);
	    free((void *) temp);
	    plabort("PLOT3D: Out of memory");
	    return;
	}
    }

    for (i = 0; i < *nx; i++)
	for (j = 0; j < *ny; j++)
	    temp[i][j] = *(z + j * *lx + i);

    c_plot3d(x, y, temp, *nx, *ny, *opt, *side);

    for (i = 0; i < *nx; i++)
	free((void *) temp[i]);

    free((void *) temp);
}
