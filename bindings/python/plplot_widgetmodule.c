// C code to create dynamically loaded library to implement plplot_widget module

#define NPY_NO_DEPRECATED_API    NPY_1_7_API_VERSION
#include <Python.h>
#include <arrayobject.h>
#include "plplot.h"
#include "plplotP.h"

#ifdef ENABLE_tkX
#include <tcl.h>
#include "pltk.h"
#endif

void initplplot_widget( void );

#define TRY( E )    if ( !( E ) ) return NULL

#ifdef ENABLE_tkX
static char doc_Pltk_init[] = "Initialize the Pltk Tcl extension.";

//--------------------------------------------------------------------------
// A python module method for initializing the PLtk extension.  This method
// must be called from python with a single argument, which is the address of
// the Tcl interpreter into which the Pltk extension is to be injected.
//--------------------------------------------------------------------------

static PyObject *pl_Pltk_init( PyObject * PL_UNUSED( self ), PyObject *args )
{
    //printf( "in pl_Pltk_init()\n" );
    long x = 0;

    TRY( PyArg_ParseTuple( args, "l", &x ) );

    if ( !x )
    {
        printf( "Something went wrong...\n" );
        Py_INCREF( Py_None );
        return Py_None;
    }

    Tcl_Interp *interp = (Tcl_Interp *) x;

    //printf( "Tcl_Interp * = %ld \n", x );

    if ( Pltk_Init( interp ) == TCL_ERROR )
    {
        printf( "Initialization of Pltk Tcl extension failed!\n" );
        return NULL;
    }

    //printf( "plframe has been installed into the Tcl interpreter.\n" );

    Py_INCREF( Py_None );
    return Py_None;
}

#endif

//--------------------------------------------------------------------------

static PyMethodDef plplot_widget_methods[] = {
#ifdef ENABLE_tkX
    { "Pltk_init", pl_Pltk_init, METH_VARARGS, doc_Pltk_init },
#endif

    { NULL,        NULL,                    0, NULL          }
};

PLDLLIMPEXP_PLPLOT_WIDGETMODULE void initplplot_widget( void )
{
    PyObject *m;
    //PyObject *d;

    import_array();

    // Create the module and add the functions
    m = Py_InitModule( "plplot_widget", plplot_widget_methods );
    PyModule_GetDict( m );

    // Check for errors
    if ( PyErr_Occurred() )
        Py_FatalError( "plplot_widget module initialization failed" );
}
