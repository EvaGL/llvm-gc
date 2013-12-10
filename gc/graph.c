#include "graph.h"
#include <stdio.h>

FILE* graph_init(char* name) {
    FILE* file_out = fopen(name, "w");
    fprintf(file_out, "digraph G {\n");
    return file_out;
}
    
void graph_write(void* parent, void* son, FILE* file_out) {
    fprintf(file_out, "    obj_%p -> ", parent);
    if (son) {
        fprintf(file_out, "obj_%p;\n", son);
    } else {
        fprintf(file_out, "objnull_%p;\n", parent);
        fprintf(file_out, "    objnull_%p[label = \"0\"];\n", parent);
    }
    fprintf(file_out, "    obj_%p[label = \"%p\"];\n", parent, parent);
}

void graph_delete(FILE* file_out) {
    fprintf(file_out, "}\n");
    fclose(file_out);
}

