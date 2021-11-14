
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

int size(Tree t) {
	if (t == NULL) return 0;
	else {
		return 1 + size(t->left) + size(t->right);
	}
}

bool TreeIsPerfectlyBalanced(Tree t) {
	if (t == NULL) return true;
	
	int left = size(t->left);
	int right = size(t->right);
	if (left - right > 1 || left - right < -1) {
		return false;
	} else {
		return TreeIsPerfectlyBalanced(t->left) 
			&& TreeIsPerfectlyBalanced(t->right);
	}
}

