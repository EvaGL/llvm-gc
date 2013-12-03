
#include <iostream>
#include "graph.h"



void visit_object_graph(void* obj, graph& graph_obj) {
    if (!obj) {
        return;
    }
    //std::cout << obj << std::endl;
    void* ptr = obj;
    int* num_ptr =  (int*) ptr;
    int num = *num_ptr;
    for (int i = 1; i <= num; ++i) {
        num_ptr++;
        int* res = (int *) (obj + *num_ptr);
        graph_obj.write(obj, (void*)*res);
        visit_object_graph((void*)*res, graph_obj);
    }
}

void visit_object(void* obj) {
	graph graph_obj("graph.gv");
	visit_object_graph(obj, graph_obj);
}
