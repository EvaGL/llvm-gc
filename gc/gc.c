#include <stdio.h>
#include "graph.h"
#include "mark.h"

#define N 100
static void* my_stack[N];
static int end = 0;
static FILE* file_out;

static show_stack() {
    int i = 0;
    for (i = 0; i < N; ++i) {
        printf("%p ", my_stack[i]);        
    }
    printf("\n");
}

static void push(void* obj) {
    if (end == N + 1) {
        printf("Panic! Stack is full!\n");
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
            if (*res && !get_mark(res)) {
                mark(res);
                push((void*)*res);
            }
        }
    }
}

static void gc_mark() {
	file_out = graph_init("graph.gv");
	void* rbp;
    asm("\t movq %%rbp,%0" : "=r"(rbp));
    while (rbp) {
    	int* pmetadata = *((int**) rbp);
    	int pointerNumbers = *pmetadata;
		int i;
        for (i = 1; i <= pointerNumbers; ++i) {
			visit_object(*((void **)(rbp + *(pmetadata + i))));
		}
		rbp = *(void **)(rbp + sizeof(void *));
	}
	graph_delete(file_out);
}


void gc() {
    gc_mark();
    sweep();
}
