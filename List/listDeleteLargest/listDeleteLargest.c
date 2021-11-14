
#include "list.h"

int listDeleteLargest(List l) {
	Node prev = l->head;
	Node curr = l->head;
	Node larg = l->head;
	int largV = larg->value;
	
	if (l->head->next == NULL) {
		free(curr);
		l->head = NULL;
	} else {
		while (curr->next != NULL) {
			if (curr->next->value > larg->value) {
				if (curr == l->head) {
					larg = curr->next;
				} else {
					larg = curr->next;
					prev = curr;
				}
			}
			curr = curr->next;
		}
		largV = larg->value;
		if (prev == larg) {
			l->head = larg->next;
		} else {
			prev->next = larg->next;
		}
		free(larg);
	}
	return largV;
}