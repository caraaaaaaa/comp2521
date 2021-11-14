
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

BSTree BSTreeInsert(BSTree t, int val) {
	// BSTree n = newBSTNode(val);
	if (t == NULL) {
		t = malloc(sizeof(*t));
		t->left = NULL;
		t->right = NULL;
		t->value = val;
	}
	
	if (val < t->value) 
        t->left  = BSTreeInsert(t->left, val); 
    else if (val > t->value) 
        t->right = BSTreeInsert(t->right, val);
	return t;
}

