
#include <caml/mlvalues.h>
#include "mark.h"
#include <stdio.h>

//---- remove it! 
#define N 2 
static void* my_stack[N];
static int end = 0;
static FILE* file_out;
static int STACK_IS_FULL = 0;
//---- remove it!

static void visit_object_ocaml(value start_obj) {
    if (!start_obj || get_mark(Op_val(start_obj))) {
        return;
    }
    push(Op_val(start_obj));
    while (end != 0) {
        value ptr = Val_op(pop());
        mark(Op_val(ptr));// we have type value
        header_t header = Hd_val(start_obj);
        tag_t tag = Tag_hd(header);
        if (tag >= No_scan_tag) {
            return;        
        }
        mlsize_t size = Wosize_hd(header);
        value* num_ptr = Op_val(start_obj);
        int i;
        for (i = 0; i < size; ++i) {
            value res = (value) Field(num_ptr, i);
            if (!Is_block(res)) {
                continue;            
            }
            graph_write((void*)Op_val(ptr), (void*)Op_val(res), file_out);
            if (Op_val(res) && !get_mark((void*)Op_val(res))) {
                mark(Op_val(res));
                push(Op_val(res));
            }
        }
    }
}

