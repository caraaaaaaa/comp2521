#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "IntList.h"

// data structures representing IntList
struct IntListNode {
	int data;           /**< value of this list item */
	struct IntListNode *next;
	                    /**< pointer to node containing next element */
};


void IntListInsertInOrder (IntList L, int v)
{
	// This is INCORRECT
	// IntListInsert (L, v);
	struct IntListNode *curr = L->first;
	struct IntListNode *tmp = curr;
	
	if (curr == NULL) {
	    struct IntListNode *n = newIntListNode (v);
	    L->first = n;
	} else if (curr->next == NULL) {
	    struct IntListNode *n = newIntListNode (v);
	    if (curr->data >= v) {
	        n->next = curr;
	    } else {
	        curr->next = n;
	        n->next = NULL;
	    }
	} else {
	    while (curr->next != NULL) {
	        if (curr->data <= v) {
	            tmp = curr->next;
	            curr->next->data = v;
	            tmp = curr->next;
	        } 
	        curr = curr->next;
	    }
	    
	}
	
	L->size++;
}
