#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

FILE* graph_init(char* name);
void graph_write(void* parent, void* son, FILE* file_out);
void graph_delete(FILE* file_out);

#endif 
