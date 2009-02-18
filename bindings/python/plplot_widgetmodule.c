/* C code to create dynamically loaded library to implement plplot_widget module*/

#include <Python.h>
/* Change this to the recommended
 * #include <Numeric/arrayobject.h> 
 *  once we no longer support python1.5 */
#include <arrayobject.h>
#include "plplot.h"
#include "plplotP.h"
#include "pltk.h"

#include <tcl.h>

#define TRY(E) if(! (E)) return NULL

/* ##############################################################################*/
static char doc_partialInitXw[]="Partially init a new device (X Window) ";

#ifndef WIN32
#include "plxwd.h"
static PyObject * pl_partialInitXw(PyObject *self, PyObject *args)
{
    PLINT ipls;
    PLStream *pls;  
    TRY (PyArg_ParseTuple(args, ":pl_partialInitXw"));
    ipls=0;

    plmkstrm(&ipls);
    printf(" ipls=%d\n",ipls); 
    plgpls(&pls);   /* pls points directly to a structure PLStream */
    printf(" pls OK \n"); 

    /* Partially initialize X driver. */

    pllib_init();

    plsdev("xwin");
    pllib_devinit();
    plP_esc(PLESC_DEVINIT, NULL);

    printf("devinit  OK \n"); 

    return Py_BuildValue("i", ipls);
}
#endif 


static char doc_resize[]="resize the window";

static PyObject * pl_resize(PyObject *self, PyObject *args)
{
    int width, height ;
    PLDisplay pldis;
    TRY (PyArg_ParseTuple(args, "ii:resize", &width, &height));

    pldis.width=width;
    pldis.height=height;
    pl_cmd(PLESC_RESIZE, (void *) &(pldis));     
    pl_cmd(PLESC_EXPOSE, (void *) NULL);     

    Py_INCREF(Py_None);
    return Py_None;
 
}

static char doc_expose[]="expose the whole   window";

static PyObject * pl_expose(PyObject *self, PyObject *args)
{
    TRY (PyArg_ParseTuple(args, ":expose"));

    pl_cmd(PLESC_EXPOSE, (void *) NULL);     

    Py_INCREF(Py_None);
    return Py_None;
 
}

#ifdef ENABLE_tk
static char doc_Pltk_init[]="Initialize the Pltk Tcl extension.";

/*--------------------------------------------------------------------------*\
 * A python module method for initializing the PLtk extension.  This method
 * must be called from python with a single argument, which is the address of
 * the Tcl interpreter into which the Pltk extension is to be injected.
\*--------------------------------------------------------------------------*/

static PyObject *pl_Pltk_init(PyObject *self, PyObject *args)
{
    printf( "in pl_Pltk_init()\n" );
    long x=0;

    TRY( PyArg_ParseTuple(args, "l", &x) );

    if (!x)
    {
        printf( "Something went wrong...\n" );
        Py_INCREF(Py_None);
        return Py_None;
    }

    Tcl_Interp *interp = (Tcl_Interp *) x;

    printf( "Tcl_Interp * = %ld \n", x );

    if (Pltk_Init(interp) == TCL_ERROR)
    {
        printf( "Initizlization of Pltk Tcl extension failed!\n" );
        return NULL;
    }

    printf( "plframe has been installed into the Tcl interpreter.\n" );

    Py_INCREF(Py_None);
    return Py_None;
 
}
#endif

/* ##############################################################################*/

static PyMethodDef plplot_widget_methods[] = {
    /* These are a few functions  that I found I needed
       to call in order to initialise the pyqt widget  */

#ifndef WIN32
    {"plpartialInitXw",	pl_partialInitXw, 1, doc_partialInitXw},
#endif

    /* These are a few functions  that I found I needed
       to filtering events in  pyqt widget  */

    {"plresize",	pl_resize, METH_VARARGS, doc_resize},
    {"plexpose",	pl_expose, METH_VARARGS, doc_expose},

#ifdef ENABLE_tk
    {"Pltk_init",	pl_Pltk_init, METH_VARARGS, doc_Pltk_init},
#endif

    {NULL, NULL, 0, NULL}
};

void initplplot_widget(void)
{
    PyObject *m;
    PyObject *d;

    import_array();

    /* Create the module and add the functions */
    m = Py_InitModule("plplot_widget", plplot_widget_methods);
    d = PyModule_GetDict(m);

    /* Check for errors */
    if (PyErr_Occurred())
	Py_FatalError("plplot_widget module initialization failed");
}
