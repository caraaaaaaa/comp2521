
#include "list.h"

int check(List lt, int value);

List listSetUnion(List s1, List s2) {
	List new = newList();
	Node c1 = s1->head;
	Node c2 = s2->head;
	Node c3 = new->head;
	
	while (c1 != NULL) {
		if (check(new, c1->value) == 1) {
			Node n = newNode(c1->value);
			if (c3 == NULL) {
				new->head = n;
				c3 = n;
			} else {
				c3->next = n;
				c3 = n;
			}
		}
		c1 = c1->next;
	}
	
	while (c2 != NULL) {
		if (check(new, c2->value) == 1) {
			Node a = newNode(c2->value);
				c3->next = a;
				c3 = a;	
		}
		c2 = c2->next;
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

