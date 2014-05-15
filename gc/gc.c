#include <stdio.h>
#include "graph.h"
#include "mark.h"
#include <caml/mlvalues.h>

#define N 20
#define DFS 1
/*Select algorithm for visiting objects*/
/*in DFS we use array as stack*/
/*in BFS we use array as queue*/

static void* array[N];
static int end = 0;
/*in stack and in queue ptr to the next _empty_ field
  in stack and in queue = N in case of overflow*/
static int beg = 0;
/*in stack always 0
  in queue ptr to the first element*/
static FILE* file_out;
static int stack_overflow = 0;
static int callnum = 1;
static int is_double_marked = 0;

typedef struct _chain {
    int* meta;
    struct _chain *parent;
} chain;

chain* chainBottom = NULL;
static show_stack() {
    int i = 0;
    for (i = 0; i < N; ++i) {
        MDEBUG("%p ", array[i]);        
    }
    MDEBUG("\n");
}

static int empty() {
    if (end == beg) {
        return 1;
    }
    return 0;
}

static void push(void* obj) {
    MDEBUG("push %p\n", obj);
    if (end == N) {
        MDEBUG("Panic! Stack is full!\n");
        stack_overflow = 1;
        return;            
    }
    if (DFS) {
        array[end] = obj;
        end++;
    } else {
        if (end == (N + beg - 1) % N) {
            array[end] = obj;
            end = N; /*special sign for queue overflow*/
        } else {
            array[end] = obj;
            end = (end + 1) % N;
        }
    }
}

static void* pop() { 
    if (DFS) {
        end--;
        void* val = array[end];
        MDEBUG("pop %p\n", val);
        array[end] = NULL;
        return val;
    } else {
        if (end == N) {
            end = beg;    
        }
        void* val = array[beg];        
        beg = (beg + 1) % N;
        MDEBUG("pop %p\n", val);
        array[end] = NULL;
        return val;
    }
}

static char checked_address(value* addr) {
    if (!addr) {
        MDEBUG("null object\n");
        return 0;
    }
    if (!address_ok(addr) || (addr > (1ull << 62))) {
        MDEBUG("address is not in heap\n");            
        return 0;    
    }        
    if (get_mark(Hp_val(Val_op(addr)))) {
        MDEBUG("already marked\n");
        if (is_double_marked) {
            is_double_marked = 0;
            return 1;
        } else {
            return 0;
        }
    }
    return 1;
}

extern void* heap_begin;
static char correct_block(value* addr) {
    if (Op_val(addr) < heap_begin) {
        MDEBUG("too small %p\n", Op_val(addr));
        return 0;
    }
    if (!Is_block(Val_op(addr))) {
        MDEBUG("not block %p\n", Op_val(addr));
        return 0;            
    }
    return 1;    
}

static char should_scan(value* ptr) {
    header_t header = Hd_val(Val_op(ptr));
    tag_t tag = Tag_hd(header);
    if (tag >= No_scan_tag) {
        MDEBUG("bad tag for object %p\n", ptr);
        return 0;        
    }    
    return 1;
}

static mlsize_t get_size(value val) {
    header_t header = Hd_val(val);
    mlsize_t size = Wosize_hd(header);
    MDEBUG("object %p size %d\n", Op_val(val), size);
    return size;    
}

static tag_t get_tag(value val) {
    header_t header = Hd_val(val);
    tag_t tag = Tag_hd(header);
    MDEBUG("object %p tag  %d\n", Op_val(val), tag);
    return tag;    
}

static void visit_object(value* start_obj) {
    MDEBUG("-------------\n");
    MDEBUG("start object: %p\n", start_obj);
    MDEBUG("-------------\n");    
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


    int max = 1;
    while (!empty()) {
        /*while stack is not empty*/
        value v = Val_op(pop());
        MDEBUG("object: %p\n", Op_val(v));
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
            MDEBUG("closure\n");
            i = 1;        
        }  
        while (i < size) {    
            /*visit fields*/
            value res = Field(v, i);
            MDEBUG("Field #%d for object %p is %p\n", i, Op_val(v), Op_val(res));
            if (correct_block(Op_val(res))) {                
                push(Op_val(res));    
                /*if (max < end - 1) {
                  max = end - 1;
                }*/
            }
            ++i;
            /*graph_write((void*)Op_val(ptr), (void*)Op_val(res), file_out);*/
        }
        /*printf("%d\n", max);*/
    }
}


static void gc_mark() {
    //file_out = graph_init("graph.gv");
    chain* current = chainBottom;
    while (current) {
        int* pmetadata = current->meta;
        void* currentPtr = current;
        int pointerNumbers = *pmetadata;
        MDEBUG("Root objects: %d\n", pointerNumbers);
        int i;
        for (i = 1; i <= pointerNumbers; ++i) {
            void* ptr = *((void **)(currentPtr + *(pmetadata + i)));
            if ((ptr > (1ull << 62)) || (ptr > 0x7f0000000)) {
                /* dirty hack FIXME*/
                MDEBUG("Skip root object %p\n", ptr);
            } else {
                visit_object(ptr);            
            }   
        }
        current = current->parent;
    }
    size_t k = 0;
    size_t bound_stack_overflow = 0;
    if (stack_overflow) {
      MDEBUG("Stack is full\n");
      //printf("Stack is full\n");
      bound_stack_overflow = count_used_chunks();
      MDEBUG("Num of objects: %d\n", bound_stack_overflow);
      bound_stack_overflow = bound_stack_overflow;
      MDEBUG("bound = %d\n", bound_stack_overflow);
    }
    while (stack_overflow && (k < bound_stack_overflow)) {
        MDEBUG("%d %d \n", bound_stack_overflow, k);
        //printf("Stack is full\n");
        stack_overflow = 0;
        ++k;
        void* obj = stack_is_full();
        MDEBUG("Stack is full object: %p\n", obj);
        while (obj) {
            visit_object(Val_hp(obj));
            obj = stack_is_full();
            is_double_marked = 1;
            MDEBUG("Stack is full object: %p\n", obj);            
        }    
    }
    /*if (k == bound_stack_overflow) {
        printf("Because of k!\n");    
    }*/
    //go_along_heap();
    MDEBUG("Num of objects: %d\n", count_used_chunks());
    MDEBUG("bound = %d\n", bound_stack_overflow);
}


void gc() {
    MDEBUG("===============GC Call %d================\n", callnum);
    ++callnum;
    //malloc_stats();
    //go_along_heap();
    gc_mark();
    sweep();
    //malloc_stats();
    MDEBUG("===============GC Call END================\n");
}
