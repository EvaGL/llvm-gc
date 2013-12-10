
#include <stdio.h>
#include "graph.h"

#define N 100
static void* my_stack[N];
static int end = 0;

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
    //show_stack();
    end++;
}

static void* pop() { 
    end--;
    void* val = my_stack[end];
    my_stack[end] = NULL;
    //show_stack();
    return val;
}


void visit_object_graph(void* start_obj, FILE* file_out) {
    if (!start_obj) {
        return;
    }
    push(start_obj);
    while (end != 0) {
        void* ptr = pop();
        int* num_ptr =  (int*) ptr;
        int num = *num_ptr;
        int i;
        for (i = 1; i <= num; ++i) {
            num_ptr++;
            int* res = (int *) (ptr + *num_ptr);
            graph_write(ptr, (void*)*res, file_out);
            if (*res) {            
                push((void*)*res);
            }
        }
    }
}

/*void visit_object(void* obj) {
    FILE* file_out = graph_init("graph.gv");
    visit_object_graph(obj, file_out);
    graph_delete(file_out);
}*/
