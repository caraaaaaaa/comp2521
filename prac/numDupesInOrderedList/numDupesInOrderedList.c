
#include "list.h"

int numDupesInOrderedList(List l) {
    Node a = l->head;
    int count = 0;
    while (a->next != NULL) {
        if (a->next->value == a->value) {
            count++;
        }
        a = a->next;
    }
    
    return count;
}

