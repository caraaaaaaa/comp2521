
#include <stdlib.h>

#include "BSTree.h"

int BSTreeCount(BSTree t);

int BSTreeGetKth(BSTree t, int k) {
	int countLeft = BSTreeCount(t->left);
	if (k == countLeft) return t->value;
	else if (k < countLeft) {
		return BSTreeGetKth(t->left, k);
	} else {
		return BSTreeGetKth(t->right, k - countLeft -1);
	}
}

int BSTreeCount(BSTree t) {
	if (t == NULL) {
		return 0;
	} else {
		return 1 + BSTreeCount(t->left) + BSTreeCount(t->right);
	}
}