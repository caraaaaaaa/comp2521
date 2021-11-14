
#include "list.h"

void listReverse(List l) {
	if (l->head == NULL || l->head->next == NULL) {
		return;
	}
	if (l->head != NULL || l->head->next != NULL) {
		Node prev = NULL;
		Node curr = l->head;
		Node next = NULL;
		while (curr != NULL) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		l->head = prev;
	}
}

