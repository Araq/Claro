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
#include <claro/graphics.h>

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

/*
static PyObject *graphics_example( PyObject *self, PyObject *args )
{
	const char *command;
	int sts;

	if ( !PyArg_ParseTuple( args, "s", &command ) )
		return NULL;
	
	sts = system(command);
	
	return Py_BuildValue("i", sts);
}
*/

static PyObject *graphics_init( PyObject *self, PyObject *args )
{
	claro_graphics_init( );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_window_widget_create( PyObject *self, PyObject *args )
{
	int p, f;
	int x, y, w, h;
	int ptr;
	
	if ( !PyArg_ParseTuple( args, "iiiiii", &p, &x, &y, &w, &h, &f ) )
		return NULL;
	
	ptr = (int)window_widget_create( (object_t *)p, new_bounds( x, y, w, h ), f );
	
	return Py_BuildValue( "i", ptr );
}

static PyObject *graphics_button_widget_create( PyObject *self, PyObject *args )
{
	int p, f;
	int x, y, w, h;
	int ptr;
	
	if ( !PyArg_ParseTuple( args, "iiiiii", &p, &x, &y, &w, &h, &f ) )
		return NULL;
	
	ptr = (int)button_widget_create( (object_t *)p, new_bounds( x, y, w, h ), f );
	
	return Py_BuildValue( "i", ptr );
}

static PyObject *graphics_textbox_widget_create( PyObject *self, PyObject *args )
{
	int p, f;
	int x, y, w, h;
	int ptr;
	
	if ( !PyArg_ParseTuple( args, "iiiiii", &p, &x, &y, &w, &h, &f ) )
		return NULL;
	
	ptr = (int)textbox_widget_create( (object_t *)p, new_bounds( x, y, w, h ), f );
	
	return Py_BuildValue( "i", ptr );
}

static PyObject *graphics_textbox_set_text( PyObject *self, PyObject *args )
{
	int p;
	const char *s;
	
	if ( !PyArg_ParseTuple( args, "is", &p, &s ) )
		return NULL;
	
	textbox_set_text( (object_t *)p, s );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_button_set_text( PyObject *self, PyObject *args )
{
	int p;
	const char *s;
	
	if ( !PyArg_ParseTuple( args, "is", &p, &s ) )
		return NULL;
	
	button_set_text( (object_t *)p, s );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_widget_show( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	widget_show( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_widget_hide( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	widget_hide( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_widget_focus( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	widget_focus( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_window_show( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	window_show( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_window_hide( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	window_hide( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *graphics_window_focus( PyObject *self, PyObject *args )
{
	int p;
	
	if ( !PyArg_ParseTuple( args, "i", &p ) )
		return NULL;
	
	window_focus( (object_t *)p );
	
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef graphics_methods[] = {
	{ "init",  graphics_init, METH_VARARGS,
		"claro_graphics_init( )" },
	
	{ "window_widget_create",  graphics_window_widget_create, METH_VARARGS,
		"window_widget_create()" },
	
	{ "textbox_widget_create",  graphics_textbox_widget_create, METH_VARARGS,
		"textbox_widget_create()" },
	
	{ "button_widget_create",  graphics_button_widget_create, METH_VARARGS,
		"button_widget_create()" },
	
	{ "textbox_set_text",  graphics_textbox_set_text, METH_VARARGS,
		"textbox_set_text()" },
	
	{ "button_set_text",  graphics_button_set_text, METH_VARARGS,
		"button_set_text()" },
	
	{ "widget_show",  graphics_widget_show, METH_VARARGS,
		"widget_show()" },
	
	{ "widget_hide",  graphics_widget_hide, METH_VARARGS,
		"widget_hide()" },
	
	{ "widget_focus",  graphics_widget_focus, METH_VARARGS,
		"widget_focus()" },
	
	{ "window_show",  graphics_window_show, METH_VARARGS,
		"window_show()" },
	
	{ "window_hide",  graphics_window_hide, METH_VARARGS,
		"window_hide()" },
	
	{ "window_focus",  graphics_window_focus, METH_VARARGS,
		"window_focus()" },
	
	{ NULL, NULL, 0, NULL }		/* Sentinel */
};

PyMODINIT_FUNC
initgraphics(void) 
{
	PyObject* m;
	
	m = Py_InitModule3( "graphics", graphics_methods, "graphics module" );
}
