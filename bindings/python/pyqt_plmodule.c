/* C code to create dynamically loaded library to implement pyqt_plplot module*/

#include "plplot/plmodule.h"
#include "plplot/plplotP.h"

/* ##############################################################################*/
static char doc_partialInitXw[]="Partially init a new device (X Window) ";

static PyObject * pl_partialInitXw(PyObject *self, PyObject *args)
{
    PLINT ipls;
    PLStream *pls;  
    TRY (PyArg_ParseTuple(args, ":pl_partialInitXw"));
    ipls=0;

    plmkstrm(&ipls);
    printf(" ipls=%d\n",ipls); 
    plgpls(&pls);   // pls points directly to a structure PLStream 
    printf(" pls OK \n"); 

    /* Partially initialize X driver. */

    plD_open_xw(pls);
    printf("plD_open_xw  OK \n"); 
 
    return Py_BuildValue("i", ipls);
}


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

/* ##############################################################################*/

static PyMethodDef pyqt_pl_methods[] = {
    /* These are a few functions  that I found I needed
       to call in order to initialise the pyqt widget  */

    {"plpartialInitXw",	pl_partialInitXw, METH_VARARGS, doc_partialInitXw},

    /* These are a few functions  that I found I needed
       to filtering events in  pyqt widget  */

    {"plresize",	pl_resize, METH_VARARGS, doc_resize},
    {"plexpose",	pl_expose, METH_VARARGS, doc_expose},
    {NULL, NULL, 0, NULL}
};

void initpyqt_pl()
{
    PyObject *m;
    PyObject *d;

    import_array();

    /* Create the module and add the functions */
    m = Py_InitModule("pyqt_pl", pyqt_pl_methods);
    d = PyModule_GetDict(m);

    /* Check for errors */
    if (PyErr_Occurred())
	Py_FatalError("pl module initialization failed");
}
