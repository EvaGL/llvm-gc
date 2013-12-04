
#include <stdio.h>
#include "graph.h"

void visit_object_graph(void* obj, FILE* file_out) {
    if (!obj) {
        return;
    }
    void* ptr = obj;
    int* num_ptr =  (int*) ptr;
    int num = *num_ptr;
    int i;
    for (i = 1; i <= num; ++i) {
        num_ptr++;
        int* res = (int *) (obj + *num_ptr);
        graph_write(obj, (void*)*res, file_out);
        visit_object_graph((void*)*res, file_out);
    }
}

void visit_object(void* obj) {
    FILE* file_out = graph_init("graph.gv");
    visit_object_graph(obj, file_out);
    graph_delete(file_out);
}
