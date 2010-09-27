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


#include "graphics.h"
#include "layout_heap.h"
#include <assert.h>


/** These were taken from SGLIB which is a fantastic ADT library. */
#define ADT_ARRAY_SINGLE_HEAP_SORT(type, a, max, comparator) {\
ADT_ARRAY_HEAP_SORT(type, a, max, comparator, ADT_ARRAY_ELEMENTS_EXCHANGER);\
}

#define ADT_ARRAY_HEAP_SORT(type, a, max, comparator, elem_exchanger) {\
int   _k_;\
for(_k_=(max)/2; _k_>=0; _k_--) {\
    ADT___ARRAY_HEAP_DOWN(type, a, _k_, max, comparator, elem_exchanger);\
}\
for(_k_=(max)-1; _k_>=0; _k_--) {\
    elem_exchanger(type, a, 0, _k_);\
    ADT___ARRAY_HEAP_DOWN(type, a, 0, _k_, comparator, elem_exchanger);\
}\
}

#define ADT___ARRAY_HEAP_DOWN(type, a, ind, max, comparator, elem_exchanger) {\
int   _m_, _l_, _r_, _i_;\
_i_ = (ind);\
_m_ = _i_;\
do {\
    _i_ = _m_;          \
    _l_ = 2*_i_+1;\
    _r_ = _l_+1;\
    if (_l_ < (max)){\
        if (comparator(((a)[_m_]), ((a)[_l_])) < 0) _m_ = _l_;\
        if (_r_ < (max)) {\
            if (comparator(((a)[_m_]), ((a)[_r_])) < 0) _m_ = _r_;\
        }\
    }\
    if (_m_ != _i_) {\
        elem_exchanger(type, a, _i_, _m_);\
    }\
} while (_m_ != _i_);\
}



#define ADT_ARRAY_BINARY_SEARCH(type, a, start_index, end_index, key, comparator, found, result_index) {\
int _kk_, _cc_, _ii_, _jj_, _ff_;\
_ii_ = (start_index); \
_jj_ = (end_index);\
_ff_ = 0;\
while (_ii_ <= _jj_ && _ff_==0) {\
    _kk_ = (_jj_+_ii_)/2;\
    _cc_ = comparator(((a)[_kk_]), (key));\
    if (_cc_ == 0) {\
        (result_index) = _kk_;    \
        _ff_ = 1;\
    } else if (_cc_ < 0) {\
        _ii_ = _kk_+1;\
    } else {\
        _jj_ = _kk_-1;\
    }\
}\
if (_ff_ == 0) {\
    /* not found, but set its resulting place in the array */\
    (result_index) = _jj_+1;\
}\
(found) = _ff_;\
}

#define ADT_ARRAY_ELEMENTS_EXCHANGER(type, a, i, j) {type _sgl_aee_tmp_; _sgl_aee_tmp_=(a)[(i)]; (a)[(i)]=(a)[(j)]; (a)[(j)]= _sgl_aee_tmp_;}

#define CELL_NAME_COMPARE(X,Y) strcmp((X)->name, (Y)->name)
#define CELL_NAME_ONLY_COMPARE(X,Y) strcmp((X)->name, Y)

LayoutHeap *layout_heap_create(size_t dim) 
{
    LayoutHeap *heap = calloc(1, sizeof(LayoutHeap));
    assert(heap != NULL);

    heap->index = calloc(dim, sizeof(cell_t *));
    assert(heap->index != NULL);

    heap->ordered = calloc(dim, sizeof(cell_t *));
    assert(heap->ordered != NULL);
    
    heap->dim = dim;

    return heap;
}


void layout_heap_destroy(LayoutHeap *heap) 
{
    int i = 0;
    for(i = 0; i < heap->top; i++) {
        free(heap->ordered[i]);
    }
    
    free(heap->index);
    free(heap->ordered);
    free(heap);
}


void layout_heap_sort(LayoutHeap *heap) 
{
    heap->sorted = 1;
    ADT_ARRAY_SINGLE_HEAP_SORT(cell_t *, heap->index, heap->top, CELL_NAME_COMPARE);
}


cell_t *layout_heap_find(LayoutHeap *heap, const char *name, size_t *index) 
{
    int i = 0;
    int found = 0;
    
    assert(heap->sorted && "You have not sorted this heap yet");
    
    ADT_ARRAY_BINARY_SEARCH(cell_t *, heap->index, 0, heap->top-1, name, CELL_NAME_ONLY_COMPARE, found, i);
    if(found && index) {
        *index = i;
    }

    return (found ? heap->index[i] : NULL);
}



void layout_heap_push(LayoutHeap *heap, cell_t *cell)
{
    assert(heap->top < heap->dim && "The heap is full.");
    assert(!heap->sorted && "The heap has already been sorted.");
    
    heap->index[heap->top] = cell;
    heap->ordered[heap->top] = cell;
    heap->top++;
}

cell_t *layout_heap_pop(LayoutHeap *heap)
{
    assert(!heap->sorted && "The heap has already been sorted.");
    
    if(heap->top == 0) return NULL;
    
    heap->top--;
    return heap->ordered[heap->top];
}

cell_t *layout_heap_top(LayoutHeap *heap)
{
    if(heap->top == 0) return NULL;
    return heap->ordered[heap->top-1];
}


