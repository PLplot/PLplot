/* gcwmodule - python wrapper for the gcw plplot driver

  Copyright (C) 2004, 2005 Thomas J. Duck
  All rights reserved.

  Thomas J. Duck <tom.duck@dal.ca>
  Department of Physics and Atmospheric Science,
  Dalhousie University, Halifax, Nova Scotia, Canada, B3H 3J5


NOTICE

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
  USA 
*/

#include "gcw.h"

#include <pygobject.h>           
#include <pygtk/pygtk.h>

/* global variable declared at top of file */
static PyTypeObject *PyGObject_Type=NULL;    

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_install_canvas(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  GnomeCanvas* canvas;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",1,1,&canvas_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_background_group: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  /* Make the call */
  gcw_install_canvas(canvas);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_set_canvas_zoom(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  PyObject *zoom_;
  GnomeCanvas* canvas;
  PLFLT zoom;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",2,2,&canvas_,&zoom_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_set_canvas_zoom: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  if(!PyFloat_Check(zoom_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_set_canvas_zoom: PyFloat expected as arg 2.");
    return NULL;
  }
  zoom=PyFloat_AsDouble(zoom_);

  /* Make the call */
  gcw_set_canvas_zoom(canvas,zoom);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_set_canvas_size(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  PyObject *width_, *height_;
  GnomeCanvas* canvas;
  PLFLT width,height;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",3,3,&canvas_,&width_,&height_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_set_canvas_size: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  if(!PyFloat_Check(width_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_set_canvas_width: PyFloat expected as arg 2.");
    return NULL;
  }
  width=PyFloat_AsDouble(width_);

  if(!PyFloat_Check(height_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_set_canvas_height: PyFloat expected as arg 3.");
    return NULL;
  }
  height=PyFloat_AsDouble(height_);

  /* Make the call */
  gcw_set_canvas_size(canvas,width,height);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_text(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  PyObject *use_text_;
  GnomeCanvas* canvas;
  gboolean use_text;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",2,2,&canvas_,&use_text_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_text: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  if(!PyInt_Check(use_text_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_text: PyInt expected as arg 2.");
    return NULL;
  }
  use_text=(gboolean)PyInt_AsLong(use_text_);

  /* Make the call */
  gcw_use_text(canvas,use_text);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_fast_rendering(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  PyObject *use_fast_rendering_;
  GnomeCanvas* canvas;
  gboolean use_fast_rendering;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",2,2,&canvas_,&use_fast_rendering_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_fast_rendering: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  if(!PyInt_Check(use_fast_rendering_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_fast_rendering: PyInt expected as arg 2.");
    return NULL;
  }
  use_fast_rendering=(gboolean)PyInt_AsLong(use_fast_rendering_);

  /* Make the call */
  gcw_use_fast_rendering(canvas,use_fast_rendering);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_pixmap(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  PyObject *use_pixmap_;
  GnomeCanvas* canvas;
  gboolean use_pixmap;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",2,2,&canvas_,&use_pixmap_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_fast_rendering: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  if(!PyInt_Check(use_pixmap_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_fast_rendering: PyInt expected as arg 2.");
    return NULL;
  }
  use_pixmap=(gboolean)PyInt_AsLong(use_pixmap_);

  /* Make the call */
  gcw_use_pixmap(canvas,use_pixmap);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_foreground_group(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  GnomeCanvas* canvas;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",1,1,&canvas_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_foreground_group: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  /* Make the call */
  gcw_use_foreground_group(canvas);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_background_group(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  GnomeCanvas* canvas;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",1,1,&canvas_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_background_group: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  /* Make the call */
  gcw_use_background_group(canvas);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

static PyObject * _wrap_gcw_use_default_group(PyObject *self, PyObject *args)
{
  /* Variables for parsing the args and converting the PyObjects */
  PyGObject *canvas_;
  GnomeCanvas* canvas;

  /* Parse the args */
  if(!PyArg_UnpackTuple(args,"ref",1,1,&canvas_))
  {
    PyErr_SetString(PyExc_TypeError,
                    "_wrap_gcw_use_default_group: Cannot parse arguments.");
    return NULL;
  }

  /* Convert the PyObjects */
  canvas=GNOME_CANVAS(canvas_->obj);

  /* Make the call */
  gcw_use_default_group(canvas);

  return Py_BuildValue("");
}

/*-------------------------------------------------------------------------*/

/* Method table mapping names to wrappers */
static PyMethodDef gcwmoduleMethods[]=
{
  { "install_canvas", _wrap_gcw_install_canvas, METH_VARARGS },
  { "set_canvas_zoom", _wrap_gcw_set_canvas_zoom, METH_VARARGS },
  { "set_canvas_size", _wrap_gcw_set_canvas_size, METH_VARARGS },
  { "use_text", _wrap_gcw_use_text, METH_VARARGS },
  { "use_fast_rendering", _wrap_gcw_use_fast_rendering, METH_VARARGS },
  { "use_pixmap", _wrap_gcw_use_pixmap, METH_VARARGS },
  { "use_foreground_group", _wrap_gcw_use_foreground_group, METH_VARARGS },
  { "use_background_group", _wrap_gcw_use_background_group, METH_VARARGS },
  { "use_default_group", _wrap_gcw_use_default_group, METH_VARARGS },
  { NULL, NULL }
};

void initgcw(void)
{                 
  PyObject *module;

  Py_InitModule("gcw",gcwmoduleMethods);

  init_pygobject();
  init_pygtk();
  module = PyImport_ImportModule("gobject");
  if (module) {
    PyGObject_Type =
      (PyTypeObject*)PyObject_GetAttrString(module, "GObject");
    Py_DECREF(module);
  }
}    
