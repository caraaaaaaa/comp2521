
#include "list.h"

bool listIsPalindromic(List l) {
	// TODO
	Node tmp = l->first;
	Node pr = l->last;

	int i = 0;
	while (i < l->size/2) {
		if (tmp->value != pr->value) {
			return false;
		} else {
			tmp = tmp->next;
			pr = pr->prev;
		}
		i++;
	}
	return true;
}

