typedef struct data {
	int meta;
	int value[100];
} data;


data* create_data(int iteration) {
	data* instance = (data*) malloc(sizeof(data));
	instance->meta = 0;
	instance->value[0] = iteration;
	return instance;
}

int main() {
	data* root = 0;
	for (int i = 0; i < 10000; i++)
		root = create_data(i);
	return 0;
}