/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#include <Python.h>
#include <claro/base.h>

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

/*
static PyObject *base_example( PyObject *self, PyObject *args )
{
	const char *command;
	int sts;

	if ( !PyArg_ParseTuple( args, "s", &command ) )
		return NULL;
	
	sts = system(command);
	
	return Py_BuildValue("i", sts);
}
*/

static PyObject *base_init( PyObject *self, PyObject *args )
{
	claro_base_init( );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *base_log( PyObject *self, PyObject *args )
{
	int level;
	const char *text;
	
	if ( !PyArg_ParseTuple( args, "is", &level, &text ) )
		return NULL;
	
	clog( level, "%s", text );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *base_loop( PyObject *self, PyObject *args )
{
	claro_loop( );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef base_methods[] = {
	{ "init",  base_init, METH_VARARGS,
		"claro_base_init( )" },
	
	{ "log",  base_log, METH_VARARGS,
		"clog()" },
	
	{ "loop",  base_loop, METH_VARARGS,
		"claro_loop( )" },
	
	{ NULL, NULL, 0, NULL }		/* Sentinel */
};

PyMODINIT_FUNC
initbase(void) 
{
	PyObject* m;
	
	m = Py_InitModule3( "base", base_methods, "base module" );
}
