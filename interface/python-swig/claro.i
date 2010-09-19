%module claro
%{
#include <claro/base.h>
static void PythonObjectEventCallBack( object_t *obj, event_t *event, void *clientdata );
#include <claro/graphics.h>
#undef NO_BOUNDS
bounds_t *NO_BOUNDS = &no_bounds;
%}

extern bounds_t *NO_BOUNDS;

%rename claro_base_init init;
%rename claro_loop loop;
%rename claro_graphics_init graphics_init;

extern void claro_graphics_init( );
extern void claro_base_init( );
extern void claro_loop( );
extern void claro_shutdown( );

#define CLARO_SCRIPT_INTERFACE
#define CLFEXP extern
#define CLVEXP extern

%include "../../src/claro/base/list.h"
%include "../../src/claro/base/object.h"
%include "../../src/claro/graphics/font.h"
%include "../../src/claro/graphics/widget.h"
%include "../../src/claro/graphics/layout/layout.h"
%include "../../src/claro/graphics/image.h"
%include "../../src/claro/graphics/widgets/list.h"
%include "../../src/claro/graphics/widgets/stock.h"
%include "../../src/claro/graphics/widgets/window.h"
%include "../../src/claro/graphics/widgets/menubar.h"
%include "../../src/claro/graphics/widgets/toolbar.h"
%include "../../src/claro/graphics/widgets/textbox.h"
%include "../../src/claro/graphics/widgets/button.h"
%include "../../src/claro/graphics/widgets/canvas.h"
%include "../../src/claro/graphics/widgets/splitter.h"
%include "../../src/claro/graphics/widgets/container.h"
%include "../../src/claro/graphics/widgets/textarea.h"
%include "../../src/claro/graphics/widgets/checkbox.h"
%include "../../src/claro/graphics/widgets/dialog.h"
%include "../../src/claro/graphics/widgets/label.h"
%include "../../src/claro/graphics/widgets/progress.h"
%include "../../src/claro/graphics/widgets/radio.h"
%include "../../src/claro/graphics/widgets/splitter.h"
%include "../../src/claro/graphics/widgets/statusbar.h"
%include "../../src/claro/graphics/widgets/frame.h"
%include "../../src/claro/graphics/widgets/image.h"
%include "../../src/claro/graphics/widgets/workspace.h"
%include "../../src/claro/graphics/widgets/scrollbar.h"
%include "../../src/claro/graphics/widgets/menu.h"
%include "../../src/claro/graphics/widgets/listview.h"
%include "../../src/claro/graphics/widgets/treeview.h"
%include "../../src/claro/graphics/widgets/listbox.h"
%include "../../src/claro/graphics/widgets/combo.h"
%include "../../src/claro/graphics/widgets/opengl.h"
%include "../../src/claro/graphics/system.h"
%include "../../src/claro/graphics/notify.h"

%extend(python) bounds_t {
     char *__str__() {
         static char a[1024];
         sprintf( a,"%d,%d %dx%d", self->x, self->y, self->w, self->h );
         return a;
     }
}

object_t *claro;

%typemap(python,in) PyObject *pyfunc {
  if (!PyCallable_Check($1)) {
      PyErr_SetString(PyExc_TypeError, "Need a callable object!");
      return NULL;
  }
  $result = $input;
}

%{
#ifndef SWIGTYPE_p_object_t
#define SWIGTYPE_p_object_t SWIGTYPE_p_object_
#define SWIGTYPE_p_event_t SWIGTYPE_p_event_
#endif

static void PythonObjectEventCallBack( object_t *obj, event_t *event, void *clientdata )
{
   PyObject *obj_swigptr=0;
   PyObject *evt_swigptr=0;
   PyObject *func, *arglist, *result;
   double    dres = 0;
   
   func = (PyObject *) clientdata;               // Get Python function

   obj_swigptr = SWIG_NewPointerObj( (void *)obj, SWIGTYPE_p_object_t, 0 );
   
   evt_swigptr = SWIG_NewPointerObj( (void *)event, SWIGTYPE_p_event_t, 0 );

   arglist = Py_BuildValue("(OO)", obj_swigptr, evt_swigptr);
   result = PyEval_CallObject(func,arglist);     // Call Python
   Py_DECREF(arglist);                           // Trash arglist
   
   Py_DECREF(obj_swigptr);
   Py_DECREF(evt_swigptr);
   
   Py_XDECREF(result);
}
%}

%extend(python) list_ 
{
	int get_length() 
	{
		return LIST_LENGTH(self);
	}
	
	PyObject * get_items() 
	{
		PyObject * items; 
		int i, len;
		node_t * n;
		
		i = 0;
		len = LIST_LENGTH(self);
		items = PyTuple_New(len);
		LIST_FOREACH( n, self->head )		
		{
			PyObject * item_swigptr = SWIG_NewPointerObj( (void *)n->data, SWIGTYPE_p_object_, 0 );
			PyTuple_SetItem(items, i, item_swigptr);
			i ++;
			//Py_DECREF(item_swigptr);
		}	
		return items;
	}
}

%extend(python) object_t {
   // Set a Python function object as a callback function
   // Note : PyObject *pyfunc is remapped with a typempap
   void addhandler( char *name, PyObject *pyfunc ) {
     object_addhandler_interface( self, name, PythonObjectEventCallBack, (void *) pyfunc );
     Py_INCREF(pyfunc);
   }
}

%extend(python) list_item_t {
   // Set a Python function object as a callback function
   // Note : PyObject *pyfunc is remapped with a typempap
   void addhandler( char *name, PyObject *pyfunc ) {
     object_addhandler_interface( self, name, PythonObjectEventCallBack, (void *) pyfunc );
     Py_INCREF(pyfunc);
   }
}

%extend(python) list_item_t {
   char *get_string( int item ) {
     char *tmp = (char *)self->data[item];
     return tmp;
   }
}

