
#include "list.h"

void reverseDLList(List l) {
	if (l->size == 0 || l->size == 1) {
		return;
	} 
	Node curr = l->first;
	Node tmp = NULL;
	while (curr != NULL) {
		tmp = curr->prev;
		curr->prev = curr->next;
		curr->next = tmp;
		curr = curr->prev;
	}
	l->last = l->first;
	l->first = tmp->prev;
}