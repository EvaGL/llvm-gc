#include <stdio.h>
#include "gc.h"

typedef struct _tree {
    int meta[3];
    int value;
    struct _tree *left;
    struct _tree *right; 
} tree;

void show(tree *this) {
    if (!this) {
        printf("_");
        return;
    }
    printf(" (");
    show(this->left);
    printf(") ");
    printf("%d", this->value);
    printf(" (");
    show(this->right);
    printf(") ");    
}
 
tree* tree_init(int val, tree *left_new, tree *right_new) {
    tree* new_tree = (tree*) malloc(sizeof(tree));
    new_tree->meta[0] = 2; //pointers count
    new_tree->meta[1] = sizeof(int) * 4; // offset to left
    new_tree->meta[2] = new_tree->meta[1] + sizeof(tree*); // offset to right
    new_tree->value = val;
    new_tree->left = left_new;
    new_tree->right = right_new;
    return new_tree;
}


tree* generate_tree_rec(int height, int number) {
    if (height == 0) {
        return NULL;
    }
    tree* left = generate_tree_rec(height - 1, number * 2);
    tree* right = generate_tree_rec(height - 1, number * 2 + 1);
    tree* node = tree_init(number, left, right);
    node->left = left;
    node->right = right;
    return node;
}



int main() {
    tree* tr1 = generate_tree_rec(3, 1);    
    printf("\n");
    tr1 = generate_tree_rec(2, 1);    
    gc();
    //visit_object(tr);
    return 0;
}
