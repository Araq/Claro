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


#ifndef layout_heap_h
#define layout_heap_h


/** Implements a simple heap/stack hybrid used in the layout.c code and 
 * specifically designed for how the layout parser works.  It's unusual
 * in that it gives heap array speed access by the cell names, ordered
 * listing, and stack operations when building the heap.
 *
 * It works by using layout_heap_top, layout_heap_push, and layout_heap_pop
 * to quickly build the heap during parsing.  These stack operations are
 * great for parsers.  During this time the heap is not sorted yet so 
 * you aren't paying the usual heap insert/delete costs.
 *
 * When the heap is built the layout.c code calls layout_heap_sort to
 * get the LayoutHeap->index list constructed as a sorted array 
 * of cell_t pointers.  This sorted array allows for fast binary 
 * searching by cell name and is only done once for the life of the 
 * layout.
 */
typedef struct LayoutHeap {
    size_t dim;
    int top;
    int sorted;
    
    cell_t **ordered;
    cell_t **index;
} LayoutHeap;



/** Creates a layout with the given dimension. */
LayoutHeap *layout_heap_create(size_t dim);

/** Destroys the heap.  Yep, pretty clever. */
void layout_heap_destroy(LayoutHeap *heap);

/** Sorts the heap with it's contents.  You can actually push some stuff,
 * sort, push more stuff, sort again.
 */
void layout_heap_sort(LayoutHeap *heap);

/** Finds a cell by it's name.  Asserts that the heap has been sorted.
 */
cell_t *layout_heap_find(LayoutHeap *heap, const char *name, size_t *index);

/** Pushes the cell onto the heap like it's a stack.  It puts the cell at the
 * end, and a pop takes off the end.  This lets you "append" to the heap, keeps
 * the right order, but still gives you a nice stack to use.
 */
void layout_heap_push(LayoutHeap *heap, cell_t *cell);

/** Pops the "top" cell off (the one at the end if you went in order). */
cell_t *layout_heap_pop(LayoutHeap *heap);

/** Returns the element at the top without poping it off. */
cell_t *layout_heap_top(LayoutHeap *heap);

/** Returns the number of elements in the heap/stack. */
#define layout_heap_size(H) (H)->top

#endif

