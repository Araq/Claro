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


#include <claro/graphics.h>
#include "layout_heap.h"

#define MAX_LAYOUT_HEAP 1024

void layout__init(layout_t *lt);
int layout__execute(layout_t *lt, const char *spec, int dlen);
int layout__finish(layout_t *lt);

cell_t *layout_cell_get(layout_t *lt, const char *name)
{
    cell_t *cl = NULL;
    assert(lt != NULL && "layout can't be null");
    assert(name != NULL && "cell name can't be null");
    
    cl = layout_heap_find(lt->heap, name, NULL);
    assert(cl != NULL && "cell not found by that name");
    
    return cl;
}

bounds_t *lt_bounds(layout_t *lt, const char *name) 
{
    cell_t *cl = NULL;
    
    cl = layout_cell_get(lt, name);
    if(cl) {
        return &cl->bounds;
    } else {
        // normally asserts inside layout_cell_get will catch this but we return NULL 
        // for when asserts are off and they still pass in the wrong name
        return NULL;
    }
}


void layout_parentsize_handle( object_t *obj, event_t *event )
{
	widget_t *parent;
	layout_t *lt = (layout_t *)obj;
	
	parent = (widget_t *)obj->parent;
	
	layout_reparse( (layout_t *)obj, parent->size_ct, lt->col.min, lt->row.min );
}

layout_t *layout_create(object_t *parent, const char *layout_spec, bounds_t bounds, int min_w, int min_h)
{
    layout_t *lt = (layout_t *)object_create( 0, sizeof(layout_t), "claro.graphics.layout" );
    int dlen = 0;
    
    assert(lt != NULL && "object create failed");
    
    /* recalculate on parent's resize */
    object_addhandler( OBJECT(lt), "update", layout_parentsize_handle );
    object_addhandler( OBJECT(lt), "parent_attach", layout_parentsize_handle );
    
    layout__init(lt);
    dlen = strlen(layout_spec);
    
    lt->row.min = min_h;
    lt->col.min = min_w;
    lt->bounds = bounds;
    lt->heap = layout_heap_create(dlen);  // should be good for now
    
    lt->widget.size_req = &bounds;
    
    switch(layout__execute(lt, layout_spec, dlen)) {
        case 0:
            // just continue, not quite finished
            clog(CL_ERROR, "Layout parsed but wasn't complete: '%s'", layout_spec);
            break;
            
        case 1:
            layout__finish(lt);
            break;
            
        default:
            clog(CL_ERROR, "Layout failed to parse: '%s'", layout_spec);
            return NULL;
    }
    
    if(parent) {
	object_set_parent( OBJECT(lt), parent );
    }
    
    return lt;
}


cell_t **layout_in_order(layout_t *lt, size_t *size) 
{
    assert(lt != NULL);
    *size = layout_heap_size(lt->heap);
    return lt->heap->ordered;
}

void layout_destroy(layout_t *lt) 
{
    layout_heap_destroy(lt->heap);
    free(lt);
}



void layout_cell_set_w(layout_t *lt, const char *name, int w) 
{
    cell_t *cl = layout_cell_get(lt, name);
    assert(w >= 0 && "width must be >= 0");
    
    cl->flags.fixed_w = 0;
    cl->flags.set_w = 1;
    cl->bounds.w = w;
}

void layout_cell_fix_w(layout_t *lt, const char *name) 
{
    cell_t *cl = layout_cell_get(lt, name);

    cl->flags.fixed_w = 1;
    cl->flags.set_w = 0;
    cl->bounds.w = lt->col.min;
}

void layout_cell_flex_w(layout_t *lt, const char *name) 
{
    cell_t *cl = layout_cell_get(lt, name);

    cl->flags.fixed_w = 0;
    cl->flags.set_w = 0;
    cl->bounds.w = 0;
}


void layout_cell_fix_h(layout_t *lt, const char *name) 
{
    cell_t *cl = layout_cell_get(lt, name);
    if(cl->flags.set_h) {
        // it was set height so we have to adjust
        cl->flags.set_h = 0;
        lt->row.set_h -= cl->bounds.h;
        lt->row.set_count--;
        
        assert(lt->row.set_count >= 0 && "set count went below 0, algorithm error");
        assert(lt->row.set_h >= 0 && "set height amount went below 0, algorithm error");
    } else if(cl->flags.flex_h) {
        // it is flex height, change it back
        cl->flags.flex_h = 0;
        lt->row.flex_count--;
        assert(lt->row.flex_count >= 0 && "number of flex rows below 0.  did you call too often?");
    }
        
    cl->bounds.h = lt->row.min;
}



void layout_cell_flex_h(layout_t *lt, const char *name) 
{
    cell_t *cl = layout_cell_get(lt, name);
    if(!cl->flags.flex_h) {
        lt->row.flex_count++;
        assert(lt->row.flex_count <= lt->row.total && "number of flex rows greater than number of rows.  did you call too often?");
        
        if(cl->flags.set_h) {
            // it was set height, adjust
            cl->flags.set_h = 0;
            lt->row.set_h -= cl->bounds.h;
            lt->row.set_count--;
            
            assert(lt->row.set_count >= 0 && "set count went below 0, algorithm error");
            assert(lt->row.set_h >= 0 && "set height amount went below 0, algorithm error");
        } 
        
        // if it was fixed height that's the default so we don't need to do anything
        cl->flags.flex_h = 1;
        cl->bounds.h = -1;
    }
}


void layout_cell_set_h(layout_t *lt, const char *name, int h)
{
    assert(h >= 0 && "height must be >= 0");
    cell_t *cl = layout_cell_get(lt, name);

    // don't bother doing anything if this does nothing (gee, clever)
    if(/*!cl->flags.set_h && */cl->bounds.h != h) {
        
        if(cl->flags.flex_h) {
            // it was flex height so adjust
            lt->row.flex_count--;
        } else {
            // it's set height but our test at the top means that the height has changed
            lt->row.set_h -= cl->bounds.h;  // just back it up
            lt->row.set_count--;
        }

        cl->flags.flex_h = 0;
        cl->flags.set_h = 1;
        cl->bounds.h = h;
        lt->row.set_h += cl->bounds.h;
        lt->row.set_count++;
    }
}


void cell_dump(cell_t *cl) {
    printf("%s: (%d,%d,%d,%d) row: %d, ncol: %d, fixed_w: %d, flex_h: %d\n", cl->name, 
           cl->bounds.x, cl->bounds.y, cl->bounds.w, cl->bounds.h,
           cl->flags.row, cl->ncols, cl->flags.fixed_w, cl->flags.flex_h);
}


void layout_dump(layout_t *lt)
{
    size_t i = 0;
    size_t size = 0;
    cell_t **cl = NULL;
    
    printf("LAYOUT:\n [rows] total: %d, flex: %d, min: %d, start: %d\n"
           " [cols] fixed_w: %d, fixed_count: %d, min: %d\n"
           " [bounds] (%d,%d,%d,%d)\n"
           " [heap] %d total cells\n------\n",
           lt->row.total, lt->row.flex_count, lt->row.min, lt->row.start,
           lt->col.fixed_w, lt->col.fixed_count, lt->col.min,
           lt->bounds.x, lt->bounds.y, lt->bounds.w, lt->bounds.h,
           layout_heap_size(lt->heap));
    
    
    
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        cell_dump(cl[i]);
    }
}

/** Just a handy macro to safely append a char to a buffer in a cross platform way.*/
#define write_char(S, I, C, LEN) if(((int)LEN)-(I) <= 0) return -1; (S)[(I)] = (C); (I)++; 

int layout_serialize(layout_t *lt, char *buffer, size_t len)
{
    size_t i, size;
    int buf_i = 0;
    int nwrite = 0;
    cell_t **cl = NULL;
    
    assert(buffer != NULL && "buffer can't be NULL");
    assert(lt != NULL && "layout can't be NULL");
    assert(len > 0 && "buffer length must be greater than 0");
    
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        // deal with the row start
        if(cl[i]->flags.row) {
            write_char(buffer, buf_i, '[', len);
            
            // write whether the row is flex or set width
            if(cl[i]->flags.flex_h) {
                write_char(buffer, buf_i, '_', len);
            } else if(cl[i]->flags.set_h) {
                nwrite = snprintf(buffer+buf_i, len-buf_i, "{%d}", cl[i]->bounds.h);
                if(nwrite < 0) return -1;
                buf_i += nwrite;
            }
        }
        
        // write the name of the cell if there is one
        if(strlen(cl[i]->name) > 0) {
            nwrite = snprintf(buffer+buf_i, len-buf_i, "%s", cl[i]->name);
            if(nwrite < 0) return -1;
            buf_i += nwrite;
        }
        
        // now deal with the cell options
        if(cl[i]->flags.fixed_w) {
            // fixed width
            write_char(buffer, buf_i, '<', len);
        } else if(cl[i]->flags.set_w) {
            // set width
            nwrite = snprintf(buffer+buf_i, len-buf_i, "(%d)", cl[i]->bounds.w);
            if(nwrite < 0) return -1;
            buf_i += nwrite;
        }
        
        // now we determine the line ending based on what the next cell is
        // three conditions, next one is a row start, next one is another cell, we're at the end
        if((i+1 < size && cl[i+1]->flags.row) || i == size-1) {
            // either next is a row start or i is at end
            write_char(buffer, buf_i, ']', len);
        } else {
            // it's just another cell
            write_char(buffer, buf_i, '|', len);
        }
    }
    
    // cap off the string
    buffer[buf_i] = '\0';
    
    return buf_i;
}

