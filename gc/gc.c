#include <stdio.h>
#include "graph.h"
#include "mark.h"

#define N 2
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

static void visit_object(void* start_obj) {
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

static void gc_mark() {
	file_out = graph_init("graph.gv");
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
  /*  if (STACK_IS_FULL) {
        STACK_IS_FULL = 0;
        void* obj = stack_is_full();
        printf("%p\n", obj);
        while (obj) {
            visit_object(obj);
            obj = stack_is_full();            
        }    
    }
    */
	graph_delete(file_out);
}


void gc() {
    gc_mark();
    sweep();
}
