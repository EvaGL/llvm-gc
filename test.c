#include "mark.h"
int main() {
	int * a = (int*) malloc(2045*sizeof(int));
	mark(a);
	printf("mark is %d\n", get_mark(a));
	unmark(a);
	printf("mark is %d\n", get_mark(a));
	a[5] = 47;
	free(a);
	return 0;
}