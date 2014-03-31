#ifndef _MARK_H_
#define _MARK_H_
#include <stdlib.h>

void mark(void*);
void unmark(void*);
size_t get_mark(void*);
void sweep();
void* stack_is_full();
char address_ok(void* addr);
size_t go_along_heap();
#endif