
#include <stdlib.h>

#include "BSTree.h"

int BSTreeNodeDepth(BSTree t, int key) {
	int count = 1;
	if (t == NULL) return -1;

	if (t->value == key) {
		return 0;
	} else if (t->value < key) {
		count = BSTreeNodeDepth(t->right, key);
	} else if (t->value > key) {
		count = BSTreeNodeDepth(t->left, key);
	} 

	if (count != -1) {
		count++;
	}
	return count;
}

