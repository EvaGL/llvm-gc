#include "mark.h"

void mal (void) {
	int i = 0;
	for (i = 0; i < 10000; i++) {
		void * temp = malloc (i);
	    printf("%p\n", temp);
    }
}

int main (void) {
	int i = 0;
	for (i = 0 ; i < 1; i++) {
		mal();
		sweep();
	}
	return 0;
}
