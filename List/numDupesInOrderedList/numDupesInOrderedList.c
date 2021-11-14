
#include "list.h"

int numDupesInOrderedList(List l) {
	// TODO
	int max = -1000;
	int min = 1000;
	int count = 0;
	Node tmp = l->head;
	if (tmp == NULL || tmp->next == NULL) {
		return 0;
	} 

	while (tmp != NULL) {
		if (tmp->value > max) {
			max = tmp->value;
		} 
		if (tmp->value < min) {
			min = tmp->value;
		}
		tmp = tmp->next;
	}
	int i = min;
	int j = -1;
	tmp = l->head;
	while (i < max + 1) {
		while (tmp != NULL) {
			if (tmp->value == i) {
				j++;
			}
			tmp = tmp->next;
		}
		count = count + j;
		j = -1;
		tmp = l->head;
		i++;
	}
	return count;
}

