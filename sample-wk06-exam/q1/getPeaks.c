// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

DLList getPeaks(DLList L){
	DLList peaksL = newDLList();
	if (DLListLength(L) < 3) {
	    return peaksL;
	} else {
	    DLListNodeP tmp = L->first->next;
	    DLListNodeP new = NULL;

        while (tmp->next != NULL) {
            if (tmp->value > tmp->prev->value && tmp->value > tmp->next->value) {
                new = newDLListNode(tmp->value);
                if (peaksL->first == NULL) {
                    peaksL->first = new;
                    peaksL->curr = new;
                    peaksL->last = new;
                } else {
                    peaksL->last->next = new;
                    peaksL->last = new;
                }
                peaksL->nitems++;
            }
            tmp = tmp->next;
        }
	    return peaksL;
    }
}
