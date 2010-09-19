/**
 * A sample test layout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cut.h"
#include <claro/base.h>
#include <claro/graphics.h>



void __CUT_BRINGUP__LayoutTest( void ) 
{
    
}

void __CUT__Layout_base_test()
{
    cell_t **cl = NULL;
    size_t size = 0;
    size_t i = 0;
    const char *spec = "[_test|this|<out|>for|me][ok][<onlsmall][>onlysmall2][>leftit|imsmall(2)|flexme]";
    bounds_t b = {0,0,1024,768};
    layout_t *lt = layout_create(NULL,spec, b, 50,50);
    
    for(i = 0, cl = layout_in_order(lt, &size); i < size; i++) {
        ASSERT(cl[i]->bounds.x < 1024, "X bounds outside bounding box");
        ASSERT(cl[i]->bounds.y < 768, "Y outside bounding box");
        ASSERT(cl[i]->bounds.h < 768, "Height bigger than bounding box");
        ASSERT(cl[i]->bounds.h >= 50, "Height smaller than minimum");
        
        ASSERT(lt_bounds(lt, cl[i]->name) != NULL, "could not find named cell");
    }
    
    
    layout_destroy(lt);
}

void __CUT__Layout_perf_test()
{
    int i,j = 0;
    layout_t *lt = NULL;
    time_t start, end;
    int nlayouts = 500;
    bounds_t b = {2,2,1024,764};
    char *spec = calloc(1024 * 1024, sizeof(char));
    double reparse_time, parse_time;
    
    // construct a large layout that's 100 columns by 100 rows
    for(i = 0; i < 100; i++) {
        // start row
        strncat(spec, "[", 1024 * 1024);
        for(j = 0; j < 100; j++) {
            strncat(spec, "|", 1024 * 1024);
        }
        strncat(spec, "]", 1024 * 1024);
    }
    
    clog(CL_INFO, "running performance test");
    
    start = time(NULL);
    for(i = 0; i < nlayouts; i++) {
        lt = layout_create(NULL,spec, b, 40, 50);
        layout_destroy(lt);
    }
    end = time(NULL);
    
    parse_time = difftime(end, start);
    printf("TIME TO CREATE/DESTROY %d LAYOUTS: %f\n", nlayouts, parse_time);
    
    // test reparse performance comparison

    start = time(NULL);
    lt = layout_create(NULL,spec, b, 40, 50);
    for(i = 0; i < nlayouts; i++) {
        layout_reparse(lt, b, 40, 50);
    }
    layout_destroy(lt);
    end = time(NULL);
    
    reparse_time = difftime(end, start);
    printf("TIME TO CREATE, REPARSE %d TIMES, THEN DESTROY: %f\n", nlayouts, reparse_time);
    printf("DIFFERENCE:  %f (positive means reparse faster than parse)\n", parse_time - reparse_time);
    ASSERT(parse_time >= reparse_time, "Looks like reparsing has become slower than parsing.");
    free(spec);
}


void __CUT__Layout_reparse_test() {
    size_t i = 0;
    size_t size = 0;
    cell_t **cl = NULL;
    cell_t **cl_compare = NULL;

    layout_t *lt_compare = NULL;
    layout_t *lt_rep = NULL;
    const char *spec = "[_A|B][C|D][E<|<F|>G][|||][T<|J|K][P||>][set.w(10)][{10}set.h]";
    bounds_t b1 = {0, 0, 1024, 768};
    bounds_t b2 = {0, 0, 100, 100};

    lt_compare = layout_create(NULL,spec, b2, 10,10);  // compare for reparse test
    ASSERT(lt_compare != NULL, "failed to create compare layout");
    
    lt_rep = layout_create(NULL,spec, b1, 50,50);
    ASSERT(lt_rep != NULL, "failed to create initial layout");
    
    printf("REPARSING\n");
    layout_reparse(lt_rep, b2, 10,10);
    
    printf("--- REPARSED ");
    layout_dump(lt_rep);
    printf("\n\n--- COMPARED ");
    layout_dump(lt_compare);
    
    cl_compare = layout_in_order(lt_compare, &size);  // size ignored
    for(i = 0, cl = layout_in_order(lt_rep, &size); i < size; i++) {
        ASSERT_EQUALS(cl[i]->bounds.x, cl_compare[i]->bounds.x, "X reparse failed");
        ASSERT_EQUALS(cl[i]->bounds.y, cl_compare[i]->bounds.y, "Y reparse failed");
        ASSERT_EQUALS(cl[i]->bounds.h, cl_compare[i]->bounds.h, "H reparse failed");
        ASSERT_EQUALS(cl[i]->bounds.w, cl_compare[i]->bounds.w, "W reparse failed");
    }
    
    layout_destroy(lt_rep);
    layout_destroy(lt_compare);
}


void __CUT__Layout_roundtrip_test() 
{
    layout_t *lt = NULL;
    bounds_t b = {0,0,100,100};
    char buffer[1024];
    int res = 0;
    int i = 0;
    
    const char *tests[] = { 
        // tests all the various cell/row options
        "[_test|this][test<|that][{10}those(20)]", 
        // tests doing empty and spacer cells
        "[{10}][(20)|(15)|(10)|||||][][test<|test<]",
        // tests checking that snprintf will abort on overflow
        "[{999999}(999999)|(9999999)|(999999)]"
    };
    
    for(i = 0; i < 3; i++) {
        lt = layout_create(NULL,tests[i], b, 30, 30);
        ASSERT(lt != NULL, "failed to create layout");
        
        res = layout_serialize(lt, buffer, 1023);
        ASSERT(res > 0, "result was not correct size");
        ASSERT_EQUALS(buffer[res], '\0', "buffer not terminated properly");

        printf("\nLAYOUT: %s\n", buffer);
        ASSERT_EQUALS(strcmp(buffer, tests[i]), 0, "results do not match original");
        ASSERT_EQUALS(strlen(buffer), res, "result string is not same length according to strlen");
        
        // make sure it's overflow protected
        res = layout_serialize(lt, buffer, strlen(tests[i])/2);
        ASSERT_EQUALS(res, -1, "serialize didn't detect possible overflow");
        
        layout_destroy(lt);
    }
}

void __CUT__Layout_reparse_cellchange_test() 
{
    layout_t *lt = NULL;
    cell_t *cl = NULL;
    bounds_t b = {0, 0, 100, 100};
    
    lt = layout_create(NULL,"[test]", b, 10, 10);
    ASSERT(lt != NULL, "failed to parse layout");
    
    cl = layout_cell_get(lt, "test");
    ASSERT(cl != NULL, "failed to get the test cell");
    ASSERT_EQUALS(cl->bounds.w, 100, "didn't calc initial width right");
    ASSERT_EQUALS(cl->bounds.h, 10, "didn't calc initial height right");
    
    layout_cell_set_w(lt, "test", 5);
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 5, "didn't calc initial width right after set_w");
    ASSERT_EQUALS(cl->bounds.h, 10, "didn't calc initial height right after set_w");
    
    layout_cell_fix_w(lt, "test");
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 10, "didn't calc initial width right after fix_w");
    ASSERT_EQUALS(cl->bounds.h, 10, "didn't calc initial height right after fix_w");
    
    layout_cell_flex_w(lt, "test");
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 100, "didn't calc initial width right after flex_w");
    ASSERT_EQUALS(cl->bounds.h, 10, "didn't calc initial height right after flex_w");
    
    layout_cell_flex_h(lt, "test"); 
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 100, "didn't calc initial width right after flex_h");
    ASSERT_EQUALS(cl->bounds.h, 100, "didn't calc initial height right after flex_h");
    
    layout_cell_fix_h(lt, "test");
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 100, "didn't calc initial width right after fix_h");
    ASSERT_EQUALS(cl->bounds.h, 10, "didn't calc initial height right after fix_h");
    
    layout_cell_set_h(lt, "test", 5);
    layout_reparse(lt, b, 10, 10);
    ASSERT_EQUALS(cl->bounds.w, 100, "didn't calc initial width right after set_h");
    ASSERT_EQUALS(cl->bounds.h, 5, "didn't calc initial height right after set_h");
    
}


void __CUT_TAKEDOWN__LayoutTest( void ) 
{
    
}
