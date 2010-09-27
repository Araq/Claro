#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graphics.h"
#include "layout_heap.h"

#define CELL_AT(I) (lt->heap->index[(I)])

inline void start_cell(layout_t *lt, int isrow) 
{
    cell_t *cl = NULL;
    
    cl = calloc(1, sizeof(cell_t));
    assert(cl);
    cl->bounds.owner = (object_t *)lt;
    
    if(isrow) {
        cl->flags.row = 1;
        // this will set the row start to the cell we're about to add
        lt->row.start = layout_heap_size(lt->heap);
        lt->row.total++;
    } 
    
    layout_heap_push(lt->heap, cl);
}


/**
 * During cell processing each cell has modifies applied to it that change layout_t.col 
 * parameters.  This then goes back through and sets the final widths for each cell.
 *
 * The parameters that are maintained during the row building are:
 * 
 * col.fixed_w -- the total width of all fixed elements.
 * col.fixed_count -- the NUMBER of flixed columns.  These are set during this function.
 * col.min -- the min is the default minimum for the fixed width cells.
 */

inline void end_row(layout_t *lt)
{
    cell_t *start = NULL;
    cell_t *cl = NULL;
    cell_t *prev = NULL;
    int i = 0;
    int flex_size = 0;
    int flex_count = 0;
    
    // we must calculate the difference in fixed vs. variable width, then divide the remaining
    // width amongst the number of variable width elements.  By default cells in a row are flex (variable).
    start = CELL_AT(lt->row.start);
    
    // this gets the number of columns
    start->ncols = layout_heap_size(lt->heap) - lt->row.start;
    assert(start->ncols > 0 && "number of columns not calculated right");

    // we now have the number of cells, the number that are flexed, the min, and finally the amount of fixed_w
    flex_count = start->ncols - lt->col.fixed_count;
    assert(flex_count >= 0 && "number of flex counts below 0.  not possible");
    
    // we only need to set the flex columns if there are some
    if(flex_count > 0) {
        flex_size = (lt->bounds.w - lt->col.fixed_w) / flex_count;
    }
    
    for(i = lt->row.start; i < layout_heap_size(lt->heap); i++) {
        cl = lt->heap->ordered[i];
        if(!cl->flags.set_w && !cl->flags.fixed_w) {
            cl->bounds.w = flex_size;
        } 
        
        // all cells get their x values adjusted
        if(prev) {
            cl->bounds.x = prev->bounds.x + prev->bounds.w;
        }
        
        prev = cl;
    }

    
    // clear row start for the next row
    lt->col.fixed_w=0;
    lt->col.fixed_count=0;
    lt->row.start=0;
}

inline void update_name(layout_t *lt, char fc)
{
    cell_t *cl = layout_heap_top(lt->heap);

    int len = strlen(cl->name);
    assert(len < 31 && "Layout name is too long");
    cl->name[len] = fc;
    cl->name[len+1] = '\0';
}

inline void flex_row(layout_t *lt) 
{
    cell_t *cl = layout_heap_top(lt->heap);
    cl->flags.flex_h = 1;
    lt->row.flex_count++;
}


/**
 * Sets this cell to have the min width, sets it to be flags.fixed_w, and adds to the layout_t.col.fixed_w total
 * for the end_row function.
 */
inline void fixed_width(layout_t *lt) 
{
    cell_t *cl = layout_heap_top(lt->heap);
    assert(!cl->flags.set_w && "You already set this cell with a specific width.  Don't do col(20)<");
    cl->bounds.w = lt->col.min;
    cl->flags.fixed_w = 1;

    lt->col.fixed_w += lt->col.min;
    lt->col.fixed_count++;
}


/**
 * Sets this cell to have a specific width [specified as mystuff(40)] and adjusts the
 * total layout_t.col.fixed_w to include this width.
 */
inline void set_width(layout_t *lt) 
{
    cell_t *cl = layout_heap_top(lt->heap);
    assert(!cl->flags.fixed_w && "You already set this cell to fixed width.  Don't do >col(10)");
    cl->bounds.w = strtol(lt->temp+1, NULL, 10);
    cl->flags.set_w = 1;

    lt->col.fixed_w += cl->bounds.w;
    lt->col.fixed_count++;
}


/**
 * Sets this cell to have a specific width [specified as mystuff(40)] and adjusts the
 * total layout_t.col.fixed_w to include this width.
 */
inline void set_height(layout_t *lt) 
{
    cell_t *cl = layout_heap_top(lt->heap);
    assert(!cl->flags.fixed_w && "You already set this cell to fixed width.  Don't do >col(10)");
    cl->bounds.h = strtol(lt->temp+1, NULL, 10);
    cl->flags.set_h = 1;

    lt->row.set_h += cl->bounds.h;
    lt->row.set_count++;
}


// goes back through the whole list of cells and sets the vertical coords based on flex settings
inline void finish_layout(layout_t *lt) 
{
    cell_t **cl = NULL;
    size_t i = 0;
    size_t size = 0;
    
    int fixed_rows = 0;
    int flex_size = 0;
    int flex_avail = 0;
    int cur_y = 0;
    int cur_h = 0;
    

    fixed_rows = lt->row.total - lt->row.flex_count - lt->row.set_count;
    flex_avail = lt->bounds.h - (fixed_rows * lt->row.min + lt->row.set_h);
    if(lt->row.flex_count) {
        flex_size = flex_avail / lt->row.flex_count;
    }
    
    cur_y = 0;
    cur_h = 0;
        
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        if(cl[i]->flags.row){
            cur_y += cur_h;  // move down from last y

            if(cl[i]->flags.flex_h) {
                cur_h = flex_size;  // adopt a flex height
            } else if(cl[i]->flags.set_h) {
                cur_h = cl[i]->bounds.h;  // this has a set height, leave it alone
            } else {
                // adopt a fixed minimum height
                cur_h = lt->row.min;
            }
        }
        
        cl[i]->bounds.h = cur_h;
        cl[i]->bounds.y = cur_y;
    }

#ifdef LOG_DEBUG
    layout_dump(lt);
#endif
    
    if(!lt->heap->sorted) {
        layout_heap_sort(lt->heap);  // this sorts the heap so cells can be found by name
    }
}


void layout_reparse(layout_t *lt, bounds_t bounds, int min_w, int min_h)
{
    size_t i = 0;
    size_t size = 0;
    cell_t **cl = NULL;
    int top = 0;
    int orig_top = 0;
    
    assert(lt != NULL && "layout was NULL");
    assert(min_w >= 0 && "min_w must be >= 0");
    assert(min_h >= 0 && "min_h must be >= 0");
    
    
    lt->row.min = min_h;
    lt->col.min = min_w;
    lt->bounds = bounds;
    lt->col.fixed_w=0;
    lt->col.fixed_count=0;
    lt->row.start=0;
    
    
    // we skip the very first cell since it's always a row
    lt->row.start = 0;  // the first row
    orig_top = lt->heap->top;
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        top = lt->heap->top;

        if(cl[i]->flags.row && i != 0) {
            lt->heap->top = i;  // go back real quick to simulate end of row ]
            end_row(lt);
            lt->row.start = i;
        }

        // fixed width setting must be after processing end row otherwise end_row will reset
        // the fixed with column count
        if(cl[i]->flags.fixed_w) {
            cl[i]->bounds.w = lt->col.min;
            lt->col.fixed_w += lt->col.min;
            lt->col.fixed_count++;
        }
        
        if(cl[i]->flags.set_w) {
            lt->col.fixed_w += cl[i]->bounds.w;
            lt->col.fixed_count++;
        }
        
        lt->heap->top = top;
    }
    
    end_row(lt);
    finish_layout(lt);
    // done now finish the layout
    assert(lt->heap->top == orig_top && "after reparse the heap is not the right size");
}



%% layout_
{
    alphtype char;
    
    action col { start_cell(fsm, 0); }
    action ltab { start_cell(fsm, 1);  }
    action flexrow { flex_row(fsm); }
    action id { update_name(fsm, fc); }
    action row { end_row(fsm); }
    action done { finish_layout(fsm); }
    action fixwidth { fixed_width(fsm); }
    action setwidth { set_width(fsm); }
    action setheight { set_height(fsm); }
    
    col = "|" %col;
    ltab = "[" $ltab;
    rtab = "]" $row;
    under = "_" $flexrow;
    lt = ("<" | ">") $fixwidth;
    id = (alnum+ | '.') $id;
    setw = ("(" ${ fsm->temp = fpc; } digit+ ")") %setwidth;
    seth = ("{" ${ fsm->temp = fpc; } digit+ "}") %setheight;
    row = ltab (under | seth)? (lt | col | id | space+ | setw)* rtab space*;
    
    main := row+ '\0' %/done;
}
