
#include <iostream>
#include "visit.h"

struct tree {
    int meta[3];
    int value;
    tree *left;
    tree *right;
 
    tree(int val, tree *left_new, tree *right_new) {
        meta[0] = 2; //pointers count
        meta[1] = sizeof(int) * 4; // offset to left
        meta[2] = meta[1] + sizeof(tree*); // offset to right
		value = val;
        left = left_new;
		right = right_new;
    }

	void show() {
		if (!this) {
			std::cout << "_";
			return;
		}
		std::cout << " (";
		left->show();
		std::cout << ") ";
		std::cout << value;
		std::cout << " (";
		right->show();
		std::cout << ") ";	
	} 
};

tree* generate_tree_rec(int height, int number) {
	if (height == 0) {
		return NULL;
	}
	tree* left = generate_tree_rec(height - 1, number * 2);
	tree* right = generate_tree_rec(height - 1, number * 2 + 1);
	tree* node = new tree(number, left, right);
	node->left = left;
	node->right = right;
	return node;
}



int main() {
	tree* tr = generate_tree_rec(3, 1);	
	tr->show();
	std::cout << std::endl;
	
	visit_object(tr);
	return 0;
}
