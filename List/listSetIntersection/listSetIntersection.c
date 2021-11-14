
#include "list.h"

int check(List lt, int value);

List listSetIntersection(List s1, List s2) {
	List new = newList();
	Node c1 = s1->head;
	Node c2 = s2->head;
	Node c3 = new->head;

	if (s1->head == NULL || s2->head == NULL) {
		return new;
	}

	while (c1 != NULL) {
		while (c2 != NULL) {
			if (check(new, c2->value) == 1) {
				if (c2->value == c1->value) {
					Node a = newNode(c2->value);
					if (new->head == NULL) {
						new->head = a;
						c3 = a;
					} else {
						c3->next = a;
						c3 = a;
					}
				}
			}
			
			c2 = c2->next;
		}
		c1 = c1->next;
		c2 = s2->head;
	}
	return new;
}

int check(List lt, int value) {
	Node c = lt->head;
	while (c != NULL) {
		if (c->value == value) {
			return 0;
		}
		c = c->next;
	}
	return 1;
}