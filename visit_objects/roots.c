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
			FILE* file_out = graph_init("graph.gv");
			visit_object_graph(*((void **)(rbp + *(pmetadata + i))), file_out);
		}
		rbp = *(void **)(rbp + sizeof(void *));
	}
	graph_delete(file_out);
}
