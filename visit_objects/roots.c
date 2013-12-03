#include <stdio.h>
#include "visit.h"

void findRoots() {
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
}