#include <stdio.h>
#include "graph.h"
#include "mark.h"
#include <caml/mlvalues.h>


#define N 100
static void* my_stack[N];
static int end = 0;
static FILE* file_out;
static int STACK_IS_FULL = 0;

typedef struct _chain {
    int* meta;
    struct _chain *parent;
} chain;

chain* chainBottom = NULL;
static show_stack() {
    int i = 0;
    for (i = 0; i < N; ++i) {
        printf("%p ", my_stack[i]);        
    }
    printf("\n");
}

static void push(void* obj) {
    printf("end = %d\n", end);
    if (end == N) {
        printf("Panic! Stack is full!\n");
        STACK_IS_FULL = 1;
        return;    
    }
    my_stack[end] = obj;
    end++;
}

static void* pop() { 
    end--;
    void* val = my_stack[end];
    my_stack[end] = NULL;
    return val;
}

/*static void visit_object(void* start_obj) {
    if (!start_obj || get_mark(start_obj)) {
        return;
    }
    push(start_obj);
    while (end != 0) {
        void* ptr = pop();
        mark(ptr);
        int* num_ptr =  (int*) ptr;
        int num = *num_ptr;
        int i;
        for (i = 1; i <= num; ++i) {
            num_ptr++;
            int* res = (int *) (ptr + *num_ptr);
            graph_write(ptr, (void*)*res, file_out);
            if (*res && !get_mark(*res)) {
                mark(*res);
                push((void*)*res);
            }
        }
    }
}
*/


static char checked_address(value* addr) {
    if (!Is_block(Val_op(addr))) {
        printf("bad object %p\n", Op_val(addr));
        return 0;            
    }
    if (!addr) {
        printf("null object\n");
        return 0;
    }
    /*Terrible code!!!!*/
    if (addr > 0xfffffffff) {
        printf("big number\n");
        return 0;
    }
    if (!address_ok(addr)) {
        printf("address is not in heap\n");            
        return 0;    
    }
    if (get_mark(Hp_val(Val_op(addr)))) {
        printf("already marked\n");
        return 0;
    }
    header_t header = Hd_val(Val_op(addr));
    tag_t tag = Tag_hd(header);
	printf("Tag: %d\n", tag);
    if (tag >= No_scan_tag) {
        printf("bad tag for object %p\n", addr);            
        return 0;        
    }
    return 1;
}


static void visit_object(value* start_obj) {
    printf("start object: %p\n", start_obj);  
    if (!checked_address(Op_val(start_obj))) {
        return;    
    }
    value start_obj_val = Val_op(start_obj);
    push(start_obj);

    while (end != 0) {
        value ptr = Val_op(pop());
        printf("object: %p\n", Op_val(ptr));
        
        mark(Hp_val(ptr));
        header_t header = Hd_val(ptr);
        
        mlsize_t size = Wosize_hd(header);
        printf("size is %d\n", size);
        mlsize_t i;
        for (i = 0; i < size; ++i) {
            
            value* res = &Field(ptr, i);
            printf("Field #%d for object %p is %p\n", i, Op_val(ptr), res);
            
            header_t header = Hd_val(Val_op(res));
            tag_t tag = Tag_hd(header);
	        printf("tag for obj %p: %d\n", res, tag);

            if (!checked_address(res)) {
                continue;
            }
            /*graph_write((void*)Op_val(ptr), (void*)Op_val(res), file_out);*/
            push(res);
        }
    }
}



static void gc_mark() {
	//file_out = graph_init("graph.gv");
    chain* current = chainBottom;
    while (current) {
    	int* pmetadata = current->meta;
        void* currentPtr = current;
    	int pointerNumbers = *pmetadata;
        printf("%d\n", pointerNumbers);
		int i;
        for (i = 1; i <= pointerNumbers; ++i) {
			visit_object(*((void **)(currentPtr + *(pmetadata + i))));
		}
        current = current->parent;
	}
    if (STACK_IS_FULL) {
        STACK_IS_FULL = 0;
        void* obj = stack_is_full();
        printf("%p\n", obj);
        while (obj) {
            visit_object(obj);
            obj = stack_is_full();            
        }    
    }
	//graph_delete(file_out);
}


void gc() {
    go_along_heap();
    gc_mark();
    sweep();
}
