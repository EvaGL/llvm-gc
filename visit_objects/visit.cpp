
#include <iostream>

void visit_object(void* obj) {
	if (!obj) {
		return;
	}
	std::cout << obj << std::endl;
	void* ptr = obj;
	int* num_ptr =  (int*) ptr;
	int num = *num_ptr;
	for (int i = 1; i <= num; ++i) {
		num_ptr++;
		int* res = (int *) (obj + *num_ptr);
		visit_object((void*)*res);
	}

}
