#include <stdio.h>
#include "graph.h"
#include "mark.h"
#include <caml/mlvalues.h>


#define N 30000
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
        //printf("%p ", my_stack[i]);        
    }
    //printf("\n");
}

static void push(void* obj) {
    //printf("push %p\n", obj);
    if (end == N) {
        //printf("Panic! Stack is full!\n");
        STACK_IS_FULL = 1;
        return;    
    }
    my_stack[end] = obj;
    end++;
}

static void* pop() { 
    end--;
    void* val = my_stack[end];
    //printf("pop %p\n", val);
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
    if (!addr) {
        //printf("null object\n");
        return 0;
    }
    if (!address_ok(addr) || (addr > (1ull << 62))) {
        //printf("address is not in heap\n");            
        return 0;    
    }        
    if (get_mark(Hp_val(Val_op(addr)))) {
        //printf("already marked\n");
        return 0;
    }
    return 1;
}

extern void* heap_begin;
static char correct_block(value* addr) {
    if (Op_val(addr) < heap_begin) {
        //printf("too small %p\n", Op_val(addr));
        return 0;
    }
    if (!Is_block(Val_op(addr))) {
        //printf("not block %p\n", Op_val(addr));
        return 0;            
    }
    return 1;    
}

static char should_scan(value* ptr) {
    header_t header = Hd_val(Val_op(ptr));
    tag_t tag = Tag_hd(header);
    if (tag >= No_scan_tag) {
        //printf("bad tag for object %p\n", ptr);
        return 0;        
    }    
    return 1;
}

static mlsize_t get_size(value val) {
    header_t header = Hd_val(val);
    mlsize_t size = Wosize_hd(header);
    //printf("object %p size %d\n", Op_val(val), size);
    return size;    
}

static tag_t get_tag(value val) {
    header_t header = Hd_val(val);
    tag_t tag = Tag_hd(header);
    //printf("object %p tag  %d\n", Op_val(val), tag);
    return tag;    
}

static void visit_object(value* start_obj) {
    //printf("-------------\n");
    //printf("start object: %p\n", start_obj);
    //printf("-------------\n");    
    if (!correct_block(start_obj)) {
        return;    
    }
    if (!should_scan(start_obj)) {
        /*we shouldn't scan this object and its fields, but it is alive*/
        if (checked_address(start_obj)) {
            mark(Hp_val(Val_op(start_obj)));
        }
        return;        
    }
    push(start_obj);

    while (end != 0) {
        /*while stack is not empty*/
        value v = Val_op(pop());
        //printf("object: %p\n", Op_val(v));
        if (checked_address(Op_val(v))) {
            /*ptr belongs to heap*/
            mark(Hp_val(v));
        } else {
            continue;
        }
        if (!should_scan(Op_val(v))) {
            continue;        
        }

        mlsize_t size = get_size(v);
        tag_t tag = get_tag(v);
        mlsize_t i = 0;
        if (tag == Closure_tag) {
            /*thin place FIXME*/
            //printf("closure\n");
            i = 1;        
        }  
        while (i < size) {    
            /*visit fields*/
            value res = Field(v, i);
            //printf("Field #%d for object %p is %p\n", i, Op_val(v), Op_val(res));
            if (correct_block(Op_val(res))) {                
                push(Op_val(res));    
            }
            ++i;
            /*graph_write((void*)Op_val(ptr), (void*)Op_val(res), file_out);*/
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
        //printf("Root objects: %d\n", pointerNumbers);
        int i;
        for (i = 1; i <= pointerNumbers; ++i) {
            void* ptr = *((void **)(currentPtr + *(pmetadata + i)));
            if ((ptr > (1ull << 62)) || (ptr > 0x7f0000000)) {
                /* dirty hack FIXME*/
                //printf("Skip root object %p\n", ptr);
            } else {
                visit_object(ptr);            
            }   
        }
        current = current->parent;
    }
    if (STACK_IS_FULL) {
        STACK_IS_FULL = 0;
        void* obj = stack_is_full();
        //printf("%p\n", obj);
        while (obj) {
            visit_object(obj);
            obj = stack_is_full();            
        }    
    }
    //graph_delete(file_out);
}


void gc() {
    //printf("===============GC Call================\n");
    malloc_stats();
    go_along_heap();
    gc_mark();
    sweep();
    malloc_stats();
    //printf("===============GC Call END================\n");
}
