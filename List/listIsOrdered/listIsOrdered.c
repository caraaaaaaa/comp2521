
#include "list.h"

bool listIsOrdered(List l) {
	// TODO
	if (l->head != NULL) {
		Node prev = l->head;
		Node curr = l->head->next;
		while (curr->next != NULL) {
			if (prev->value >= curr->value) {
				if (curr->value >= curr->next->value) {
					prev = prev->next;
					curr = curr->next;
				} else {
					return false;
				}
			} else {
				if (prev->value <= curr->value) {
					if (curr->value <= curr->next->value) {
						prev = prev->next;
						curr = curr->next;
					} else {
						return false;
					}
				}
			}
			curr = curr->next;
		}
	}
	return true;
}