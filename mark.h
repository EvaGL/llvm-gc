#ifndef _MARK_H_
#define _MARK_H_
#include <stdlib.h>

void mark(void*);
void unmark(void*);
size_t get_mark(void*);

#endif