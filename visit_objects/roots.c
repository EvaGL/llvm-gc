#include <stdio.h>
#include "visit.h"

void findRoots() {
	void* rbp;
    asm("\t movl %%ebp,%0" : "=r"(rbp));
    printf("%p", i);
    while (rbp) {
    	int* pmetadata = (int*) rbp;
    	int pointerNumbers = *(*pmetadata));
		for (int i = 0; i < pointerNumbers; ++i) {
			visit_object(*((void **)(rbp + *(pmetadata + i));
		}
		rbp = *(rbp + sizeof(void *));
	}
}