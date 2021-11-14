#include "textbuffer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct Node {
    char * line;
    struct Node * next; 
    int position;
} node;

typedef struct Node * nodePtr;

typedef struct textbuffer
{
    int size;
    nodePtr first;
    nodePtr last;

} tb;


 nodePtr makeNode(char *string);
 nodePtr insert(TB textbuffer, char *string);


TB newTB (char text[]) {      
    char * array = strdup(text);

    TB temp = malloc(sizeof(tb));
    temp->first=NULL;
    temp->last=NULL;
    
    int nodePos = 0;
  
    char * token = strtok(array,"\n");
    
    while(token!=NULL)
    {   
    //fixes up temp->first within insert function
    nodePtr node = insert(temp,token);
    node->position = nodePos;
    nodePos++;
    temp->last = node;
    token = strtok(NULL,"\n");  
    }
    
    temp->size = nodePos;
    return temp;
 } 



nodePtr makeNode(char *string) {
    nodePtr newNode = malloc(sizeof(node));
    newNode->line = strdup(string);
    newNode->next=NULL;
    return newNode;    
}


nodePtr insert(TB textbuffer, char *string) { 
    //fucntion to insert a node
    TB temp = textbuffer;
    if(textbuffer->first==NULL) {
        nodePtr firstNode = makeNode(string);
        temp->first = firstNode;
        temp->first->position=0;
        return temp->first;
    } else {
      nodePtr temp = textbuffer->last;
      temp->next= makeNode(string);
      return temp->next;
    }
}


void releaseTB (TB tb) {
    assert(tb!=NULL);

    nodePtr release = tb->first;
    while(release!=NULL) { 
        nodePtr prev = release;
        release= release->next;
        free(prev);
        prev = NULL;
    }  

    tb->first=NULL; 
    tb->last=NULL;  
}


int linesTB (TB tb) {
    int count = 0;
    nodePtr temp = tb->first;
    while(temp!=NULL)
    {
        count++;
        temp=temp->next;
    }

    return count;
}


/* Allocate and return an array containing the text in the given textbuffer.
 */
char *dumpTB (TB tb, int showLineNumbers) {   

    if(tb->first==NULL) {  
       printf("empty array");
       return "\0";
    } 

    int length=0;
    nodePtr temp = tb->first;
    
    //get required length needed
    while(temp!=NULL) {   
        length = length + strlen(temp->line)+1;
        temp = temp->next;
    }
    
    int arraySize = length+1; //add one for the null terminator
    char *array = malloc(arraySize*sizeof(char));
    
    nodePtr temp2 = tb->first;
    strcpy(array,temp2->line);
   
    while(temp2!=NULL) {
        if(temp2==tb->first) {
            strcpy(array,temp2->line);
        } else {
            strcat(array,"\n");
            strcat(array,temp2->line);
        }
        temp2=temp2->next;
    }
    strcat(array,"\n"); 
    return array;
}


void swapTB (TB tb, int pos1, int pos2) {
nodePtr switchFirst = tb->first;
 nodePtr switchSecond= tb->first;
 char *tempArray=NULL;
 
if(pos1 >= tb->size) {
    fprintf(stderr,"out of range\n");
    abort();
}

if (pos2 >= tb->size) {
    fprintf(stderr,"out of range\n");
    abort();
}

if (pos1 < 0) {
    fprintf(stderr,"out of range\n");
    abort();
}

if (pos2 < 0)
{
    fprintf(stderr,"out of range\n");
    abort();
}


while(switchFirst->position!=pos1) {
    switchFirst = switchFirst->next;
}

while(switchSecond->position!=pos2) {
    switchSecond = switchSecond->next;
}

tempArray = strdup(switchSecond->line);
switchSecond->line = strdup(switchFirst->line);
switchFirst->line = strdup(tempArray); 
}


void deleteTB (TB tb, int from, int to) {
     nodePtr freePtr = tb->first;

if(from<0 || from > tb->size-1) {
    fprintf(stderr, "out of range\n");
    abort();
}

if(to<0 || to > tb->size-1) {
    fprintf(stderr, "out of range\n");
    abort();
}

if(from==to) {
    nodePtr prev=tb->first;
    while(prev->position!=from-1) {
        prev=prev->next;
    }

    while(freePtr->position!=from) {
        freePtr=freePtr->next;
    }

    nodePtr temp2=freePtr->next;
    free(freePtr);
    prev->next= temp2;
} else if(from==0 && to < tb->size-1) {
    while(freePtr->position!=to+1) {
        nodePtr prev = freePtr;
        freePtr = freePtr->next;
        free(prev);
    }
    tb->first=freePtr;
} else if(from==0 && to == tb->size-1) {  //free first to last;
    while( freePtr != NULL ) {
        nodePtr prev = freePtr;
        freePtr = freePtr->next;
        free( prev );
    }
    tb->first=NULL;
    tb->last=NULL;
} else if(from!=0 && to==tb->size-1) { //goes to last one
    nodePtr newLast = tb->first;
    while(newLast->position!=from-1) {
        newLast=newLast->next;            
    }

    freePtr = newLast->next;
    while(freePtr!=NULL){
        nodePtr prev = freePtr;
        freePtr=freePtr->next;
        free(prev);
        prev =NULL; 
    }

    newLast->next =NULL;
    tb->last= newLast;
} else { //everything else
    nodePtr beforeDeletion = tb->first;
    while(beforeDeletion->position!=from-1) {
        beforeDeletion=beforeDeletion->next;
    }

    nodePtr freePtr= tb->first;

    while(freePtr->position!=from) {   
        freePtr=freePtr->next;
    }
 
    nodePtr temp = freePtr;

    while(temp->position!=to+1) {
        nodePtr prev= temp;
        temp= temp->next;
        free(prev);
    }
    beforeDeletion->next = temp;    
}
    
   
   //fixing up the positions
   int  fix =0;
   nodePtr temp = tb->first;

   while(temp!=NULL) {
    temp->position=fix;
    fix++;
    temp=temp->next;
   }
}

void mergeTB (TB tb1, int pos, TB tb2) {
    if(pos<0 || pos> tb1->size-1) {
        fprintf(stderr,"pos it out of range\n");
        abort();
    }

    if(pos==0) {
        char*array=dumpTB(tb2, 0);
        TB anotherOne = newTB(array);
        nodePtr link= anotherOne->last;
        link->next= tb1->first;
        tb1->first=anotherOne->first;        
     } else {
         nodePtr prev = tb1->first;
         nodePtr curr = tb1->first;

         while(prev->position!=pos-1) {
            prev = prev->next;
        }
         
         while(curr->position!=pos) {
            curr=curr->next;
         }
         
        char * array= dumpTB(tb2, pos);
        TB anotherOne = newTB(array);

        prev->next = anotherOne->first;
        nodePtr link = anotherOne->last;

        link->next= curr; 
    }
     //release tb2
     nodePtr freePtr = tb2->first;
     while(freePtr!=NULL) {   
        nodePtr prevPtr=freePtr;
        freePtr = freePtr->next;
        free(prevPtr);
        prevPtr=NULL;
    } 
 }


void pasteTB (TB tb1, int pos, TB tb2) {
     if(pos<0 || pos> tb1->size-1) {
        fprintf(stderr,"pos it out of range\n");
        abort();
     }

     nodePtr prev = tb1->first;
     nodePtr curr = tb1->first;

     while(prev->position!=pos-1) {
        prev = prev->next;
     }
     
     while(curr->position!=pos) {
        curr=curr->next;
     }
     
    char * array= dumpTB(tb2, pos);
    TB anotherOne = newTB(array);
     prev->next = anotherOne->first;
     nodePtr link = anotherOne->last;
     link->next= curr; 
 }



TB cutTB (TB tb, int from, int to) {
     if(from < 0 || from> tb->size-1) {
        fprintf(stderr,"out of range\n");
        abort();
     }

     if(to<0 || to> tb->size-1) {
        fprintf(stderr, "out of range\n");
        abort();
     }

     nodePtr prev = tb->first;
     nodePtr newNext = tb->first;
     nodePtr freePtr = tb->first;
     
     TB tb2=malloc(sizeof(tb));
     tb2->first=NULL;
     tb2->last=NULL;


     if(from==0) {  
        while(freePtr->position!=to+1) { 
            if(tb2->first==NULL) {  
                nodePtr newnode = makeNode(strdup(freePtr->line));
                tb2->last = newnode; 
                tb2->first= newnode;
             } else if(tb2->first!=NULL) {
                nodePtr newnode = insert(tb2,freePtr->line);
                tb2->last= newnode;
             }
             prev=freePtr;
             freePtr = freePtr->next;
             free(prev);
             prev=NULL;
        }
        tb->first=freePtr;
    } else if(from!=0 && to==tb->size-1) {
        nodePtr newLast = tb->first;
        while(newLast->position!=from-1) {
            newLast=newLast->next;            
        }

        freePtr = newLast->next;
        
        while(freePtr!=NULL) {  
            if(tb2->first==NULL) {  
                nodePtr newnode = makeNode(strdup(freePtr->line));
                tb2->last = newnode; 
                tb2->first= newnode;
             } else if(tb2->first!=NULL) {
                nodePtr newnode = insert(tb2,freePtr->line);
                tb2->last= newnode;
             } 

            nodePtr prev = freePtr;
            freePtr=freePtr->next;
            free(prev);
            prev =NULL;
            
        }
        
        newLast->next =NULL;
        tb->last= newLast;
    } else {
         while(newNext->position!=to+1) {
            newNext=newNext->next;
         }
         
         while(prev->position!=from-1) {
            prev=prev->next;
         }

         nodePtr freePtr = prev->next;

         while(freePtr->position!=to+1) {
            if(tb2->first==NULL) {  
                nodePtr newnode = makeNode(strdup(freePtr->line));
                tb2->last = newnode; 
                tb2->first= newnode;
             } else if(tb2->first!=NULL) {
                nodePtr newnode = insert(tb2,freePtr->line);
                tb2->last= newnode;
             }

             nodePtr prev=freePtr;
             freePtr = freePtr->next; 
             free(prev);
         }
         prev->next=newNext;         
    }
    return tb2;
 }



TB copyTB (TB tb, int from, int to) {
     if(from < 0 || from> tb->size-1) {
        fprintf(stderr,"out of range\n");
        abort();
     }

     if(to<0 || to> tb->size-1) {
        fprintf(stderr, "out of range\n");
     }

     nodePtr tempFrom = tb->first;

     while(tempFrom->position!=from) {
        tempFrom = tempFrom->next;
    }

    nodePtr tempTo= tb->first;
     while(tempTo->position!= to) {
        tempTo= tempTo->next;
    } 

 
    int length=0;
    nodePtr copy = tempFrom;
    
    //get required length needed
    while(copy!=tempTo->next) {   
        length = length + strlen(copy->line)+1;
        copy = copy->next;
    }
    
    int arraySize = length+1; //add one for the null terminator
    char *array = malloc(arraySize*sizeof(char));
    
    nodePtr store = tempFrom;
   
    while(store!=tempTo->next) {
        strcat(array,"\n");
        strcat(array,store->line);
        store=store->next;       
    }
    TB copyFinal = newTB(array);
    return copyFinal;
 }



void replaceText (TB tb, char* str1, char* str2) {
    assert(strlen(str1)==strlen(str2));

    nodePtr replace = tb->first;
    while(replace!=NULL) {
        char * ret = strstr(replace->line,str1);
        if(ret!=NULL) {   
            int i=0;
            for(i=0;i<strlen(str1);i++) {
                ret[i]=str2[i];
            }
        }
        replace=replace->next;
    }
}



void whitebox(void) {
    char string[]= "usman\nhaidar\nwhitebox\ntests\n";
    TB tb1 = newTB(string);
    assert(strcmp(tb1->first->line,"usman\n")==0);
    assert(strcmp(tb1->last->line,"tests\n")==0);

    int numberOflines = linesTB(tb1);
    assert(numberOflines==4);

    char * array =dumpTB(tb1, 0);
    assert(strcmp(array,"usman\nhaidar\nwhitebox\ntests\n")==0);
    free(array);
    
    TB tb2 = newTB("this\nis\nto be\nmerged");
    mergeTB(tb1,0,tb2);
    assert(strcmp(tb1->first->line,"this\n")==0);
    
    TB tb3= newTB("this\nis\na\nnew\nstring");
    swapTB(tb3,0,1);
    assert(strcmp(tb1->first->line,"is\n")==0);
    
    printf("all tests passed");
}