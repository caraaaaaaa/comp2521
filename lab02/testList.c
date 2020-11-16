// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DLList.h"

int main (void) {
    printf("print input file\n\n");
    DLList myList = getDLList (stdin);
    putDLList (stdout, myList);
    assert (validDLList (myList));

    // TODO: more tests needed here
    


    ///////////////testing DLListBefore////////////////////
    
    char *it1 = "test 123 123";
    char *it2 = "test abc abc";
    char *it3 = "test def def";
    char *it4 = "test 456 456";
    
    printf("\n\nTesting DLListBefore............\n\n");
    
    printf("delete the input file and recreate an empty list\n");
   
    //creat an empty list
    freeDLList (myList);
    myList = newDLList ();
    assert (validDLList (myList));
    
    printf("1.insert an item before curr, when the List is empty.\n");
    DLListBefore (myList, it1);
    assert (strcmp(DLListCurrent (myList), it1) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    //t2 t4 t3 t1
    
    printf("2.insert an item before curr, when curr is the first item.\n");
    DLListBefore (myList, it2);
    assert (strcmp(DLListCurrent (myList), it2) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    printf("move curr to the next node\n\n");
    DLListMove (myList, 1);
    //printf("%s", DLListCurrent (myList));
    
    printf("3.insert an item before curr when curr is the last item.\n");
    DLListBefore (myList, it3);
    assert (strcmp(DLListCurrent (myList), it3) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    printf("4.insert an item before curr when curr is the middle item.\n");
    DLListBefore (myList, it4);
    assert (strcmp(DLListCurrent (myList), it4) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    /////////////////Testing DLListDelete////////////////////////
    
    printf("\n\nTesting DLListDelete............\n\n");
    
    printf("1.delete curr when it is the middle item\n");
    DLListDelete (myList);
    assert (strcmp(DLListCurrent (myList), it3) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
 
    printf("move curr to the last node\n\n");
    DLListMove (myList, 1);
    //printf("%s", DLListCurrent (myList));
    printf("2.delete curr when it is the last item\n");
    DLListDelete (myList);
    assert (strcmp(DLListCurrent (myList), it3) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
 
    printf("move curr to the first node\n\n");
    DLListMove (myList, -1);
    
    printf("3.delete curr when it is the first item\n");
    DLListDelete (myList);
    assert (strcmp(DLListCurrent (myList), it3) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
 
    printf("3.delete curr when it is the only item\n");
    DLListDelete (myList);    
    DLListDelete (myList);
    assert (DLListCurrent (myList) == NULL);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
 
 
    ///////////////////Testing DLListAfter /////////////////////
    
    printf("\n\nTesting DLListAfter............\n\n");
 
    printf("1.insert an item after curr, when the List is empty.\n");
    DLListAfter (myList, it1);
    assert (strcmp(DLListCurrent (myList), it1) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    //it1 it2 it3
    
    printf("2.insert an item after curr, when curr is the first item.\n");
    DLListAfter (myList, it2);
    assert (strcmp(DLListCurrent (myList), it2) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    printf("3.insert an item after curr when curr is the last item.\n");
    DLListBefore (myList, it3);
    assert (strcmp(DLListCurrent (myList), it3) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    printf("move curr to the middle node\n\n");
    DLListMove (myList, -1);
    
    printf("4.insert an item after curr when curr is the middle item.\n");
    DLListBefore (myList, it4);
    assert (strcmp(DLListCurrent (myList), it4) == 0);
    assert (validDLList (myList));
    printf("pass!!!\n\n");
    
    printf("\n\nTesting size of List............\n");
    assert(DLListLength (myList) == 4);
    DLListDelete (myList);
    assert(DLListLength (myList) == 3);
    DLListDelete (myList);
    assert(DLListLength (myList) == 2);
    DLListDelete (myList);
    assert(DLListLength (myList) == 1);
    DLListDelete (myList);
    assert(DLListLength (myList) == 0);
    printf("pass!!!\n\n\n");
    
    printf("All the tests have been passed\n");
    freeDLList (myList);
    return EXIT_SUCCESS;
}
