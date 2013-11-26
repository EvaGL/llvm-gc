
#include <iostream>

void visit_object(void* obj)
{
	if (!obj)
	{
		return;
	}
	std::cout << obj << std::endl;
	void* ptr = obj;
	int* num_ptr =  (int*) ptr;
	int num = *num_ptr;
	//std::cout << "num of metadata: " << num << std::endl;
	for (int i = 1; i <= num; ++i)
	{
		num_ptr++;
		//std::cout << "meta[" << i << "] " << *num_ptr << std::endl;
		int* res = (int *) (obj + *num_ptr);
		//std::cout << "obj_ptr "<< (void*)*res << std::endl;
		visit_object((void*)*res);
	}

}
