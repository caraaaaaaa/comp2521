
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <assert.h>

#include "textbuffer.h"

#define NONE   -1
#define BEFORE 1
#define AFTER  2

#define TRUE 1
#define FALSE 0

typedef struct TN {
	int line;
	char *value;				// The string that node represents for
	struct TN *prev;
	struct TN *next;
} TN;

typedef struct textbuffer {
	int size;
	char *string;				// The string returns
	TN *first;
	TN *last;
	TN *curr;
} textbuffer;


static TB emptyTB(char *text);
static void insertTN(TB tb, TN *tn, int COMMAND);
static TN *newTN(TB tb, char *value);
static void re_line (TB tb);
static void addTN (TB tb, char *text);
static char *cut_string(char *text, int start, int end);



// create a new empty textbuffer 
static TB emptyTB(char *text) {
	TB new_tb = malloc(sizeof *new_tb);
	assert(new_tb != NULL);

	new_tb->first = new_tb->last = new_tb->curr = NULL;
	new_tb->string = text;
	new_tb->size = 0;

	return new_tb;
}


// insert textnode into textbuffer 
// for the COMMAND, BEFORE and AFTER has defined 
static void insertTN(TB tb, TN *tn, int COMMAND) {
	assert(tb != NULL && tn != NULL);

		//	tn is the only item
	if (tb->first == NULL || tb->curr == NULL) {
		tb->first = tb->curr = tb->last = tn;
		// add node before current node
	} else if (COMMAND == BEFORE) {
		tn->next = tb->curr;
		tn->prev = tb->curr->prev;

		if (tb->curr->prev != NULL) 
			tb->curr->prev->next = tn;
		tb->curr->prev = tn;
		// add node after current node
	} else if (COMMAND == AFTER) {
		tn->prev = tb->curr;
		tn->next = tb->curr->next;
		
		if (tb->curr->next != NULL)
			tb->curr->next->prev = tn;
		tb->curr->next = tn;
	}

	if (tn->next == NULL)
		tb->last = tn;
	if (tn->prev == NULL)
		tb->first = tn;
	tb->curr = tn;
	tb->size++;
	re_line(tb);

	return;
}


// create new textnode with the 'value'
// value is the string in the textnode
static TN *newTN(TB tb, char *value) {
	TN *new_tn = malloc(sizeof *new_tn);
	assert(new_tn != NULL && value != NULL);

	new_tn->prev = new_tn->next = NULL;
	new_tn->value = value;
	new_tn->line = NONE;

	insertTN(tb, new_tn, AFTER);

	return new_tn;
}


// after I make any change in the textbuffer
// the line number contained in each textnode will change
// This function is to re-allocate each textnode from first to last
static void re_line (TB tb) {
	TN *tmp = tb->first;
	tmp->line = 0;

	while (tmp != NULL) {
		tmp->line++;
		tmp = tmp->next;
	}
	return;
}


// addn textnode into textbuffer one by one
static void addTN (TB tb, char *text) {
	assert(tb != NULL && text != NULL);
	int len = strlen(text);  
	int i = 0;
	int j = 0;

	while (i < len) {
		if (text[i] == '\n' || (text[i] == '\0' && text[i - 1] != '\0')) {
			char *string = cut_string(text, j, i);
			newTN(tb, string);
			j = i + 1;
		}
		i++;
	}
	return;
}


// separate the whole text into different string 
static char *cut_string(char *text, int start, int end) {
	int i = 0, j = start;
	char *string = malloc((end - start + 2) * sizeof(char));

	while (j < end + 1) {
		string[i] = text[j];
		j++;
		i++;
	}
	string[i] = '\0';

	return string;
}


/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB(char *text) {
	TB new_tb = emptyTB(text);
	assert(new_tb != NULL);

	addTN(new_tb, text);
	
    /*	
	TN *tmp = new_tb->first;
	while (tmp != NULL) {
		printf("%s", tmp->value);
		tmp = tmp->next;
	}
    */	

	return new_tb;
	// return NULL;
}


/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB(TB tb) {
	if (tb == NULL) return;
	TN *curr = tb->first;
	while (curr != NULL) {
		TN *tmp = curr;
		curr = curr->next;
		free(tmp->value);
		free(tmp);
	}
	free(tb);
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB(TB tb, bool showLineNumbers) {
	if (tb->first == NULL) return NULL;
	assert(tb->first != NULL);

	re_line(tb);
	char *array;

	// when showLineNUmbers is false, only return text
    if (showLineNumbers == FALSE) {
		TN *tmp = tb->first;
		int length;

		for (length = 0; tmp != NULL; tmp = tmp->next) {
			length += strlen(tmp->value);
		}

		array = malloc(sizeof(char) * (length + 1));

		for (tmp = tb->first; tmp != NULL; tmp = tmp->next) {
			char *curr_value = tmp->value;
			strcat(array, curr_value);
		}
	 
	// when showLineNumbers is true, return text with linenumber at first of the line
	} else if (showLineNumbers == TRUE) {
		int length = 0, num = 1;
		array = malloc(sizeof(char) * (length + 1));

		for (TN *tmp = tb->first; tmp != NULL; tmp = tmp->next) {
			
			// Generate Num_String
			int len = snprintf(NULL, 0, "%d", tmp->line);
			char *str = malloc(sizeof(char) * strlen(tmp->value) + len + 3), *dot = ". ";;
			snprintf(str, len + 1, "%d", num);								
			strcat(str, dot);

			//	Create New_String
			char *tmp_value = malloc((strlen(tmp->value) + strlen(str) + 1) * sizeof(char));
			strcat(str, tmp->value);
			strcpy(tmp_value, str);
			strcat(array, tmp_value);

			num++;
		}

	}
	
    return array;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB(TB tb) {
	assert(tb != NULL);
	return tb->size;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB(TB tb, int from, int to, char *prefix) {
	if (to < from || from > tb->size || to > tb->size) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB(TB tb1, int pos, TB tb2) {
	assert(tb1 != NULL);
	assert(tb2 != NULL);

	if (pos > tb1->size + 1 || pos <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 
	
	TN *tmp1;
	TN *tmp2;

	// if (tb2->size > 0) {
		if (tb1->size == 0 && pos == 1) {
			tb1->first = tb2->first;
			tb1->last = tb2->last;
			tb1->size = tb2->size;
			tmp1 = tb1->first;
			tmp2 = tb2->first;
			while (tmp2->next != NULL) {	
				tmp1 = tmp2;	
				tmp1->next = tmp2->next;
				tmp1 = tmp1->next;
				tmp2 = tmp2->next;					
			}
			
		} else {
			if (pos == 1) {
				tb1->first = tb2->first;
				tb2->last->next = tb1->first;
				tb1->first->prev = tb2->last->next;
				
			} else if (pos == tb1->size+1) {
				tb1->last->next = tb2->first;
				tb2->first->prev = tb1->last;
				tb1->last = tb2->last;
			} else {
				TN *tmp = tb1->first;
				int i = 0;		
				while (tmp != NULL) {
					if (i == pos - 1) {
						tmp->next->prev = tb2->last;
						tb2->last->next = tmp->next;
						tmp->next = tb2->first;
						tb2->first->prev = tmp;
					}
					i++;
					tmp = tmp->next;			
				}
			}
			tb1->size += tb2->size;
		}
	//}
	releaseTB(tb2);
	re_line(tb1);

/*
	TN *test = tb1->first;
	while (test != NULL) {
		printf("%s", test->value);
		test = test->next;
	}
	*/
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB(TB tb1, int pos, TB tb2) {
	assert(tb1 != NULL);
	assert(tb2 != NULL);

	if (pos > tb1->size || pos < 0 || (tb1->size > 0 && pos == 0)) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 
	char *s = dumpTB(tb2,FALSE);	
	TB tb3 = newTB(s);
	if (tb2->size > 0) {
		tb3->first = tb2->first;
		tb3->last = tb2->last;
		if (tb1->first == NULL && pos == 0) {
			tb1->first = tb3->first;
			tb3->last = tb3->last;
			tb1->size = tb3->size;
		} else {
			if (pos == 1) {
				tb3->last->next = tb1->first;
				tb1->first->prev = tb3->last->next;
				tb1->first = tb3->first;
			} else if (pos == tb1->size) {
				tb1->last->next = tb3->first;
				tb3->first->prev = tb1->last;
				tb1->last = tb3->last;
			} else {
				TN *tmp = tb1->first;
				int i = 0;		
				while (tmp != NULL) {
					if (i == pos - 1) {
						tmp->next->prev = tb3->last;
						tb3->last->next = tmp->next;
						tmp->next = tb3->first;
						tb3->first->prev = tmp;
					}
					i++;
					tmp = tmp->next;			
				}
			}
			tb1->size += tb2->size;
			releaseTB(tb3);
		}

	}
	re_line(tb1);
/*
	TN *tmp = tb1->first;
	while (tmp != NULL) {
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
*/
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB(TB tb, int from, int to) {
	return NULL;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	return NULL;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {
	// assert(tb->first != NULL);

	// Out of range
	if (to < from || from > tb->size || to > tb->size) {
		printf("Error: The line number is out of range.\n");
		abort();
	} else {
		// find the node of from and to
		TN *fom = tb->first;
		TN *too = tb->first;
		for (int i = 1; i < from; i++) {
			fom = fom->next;
		}
		for (int j = 1; j < to; j++) {
			too = too->next;
		}

		// both from and to in the middle 
		if (fom->prev != NULL && too->next != NULL) {
			for (int i = 0; i <= (to - from); i++) {
				TN *del = fom;
				fom = fom->next;
				fom->prev = del->prev;
				del->prev->next = fom;
				free(del->value);
				free(del);
				tb->size--;
			}
		// delete from first , to is no last
		} else if (fom == tb->first && too->next != NULL) {
			tb->first = too->next;
			TN *del = fom;
			fom = fom->next;
			fom->prev = NULL;
			free(del->value);
			free(del);
			tb->size--;
			for (int i = 0; i <= (to - from - 1); i++) {
				TN *del = fom;
				fom = fom->next;
				fom->prev = NULL;
				free(del->value);
				free(del);
				tb->size--;
			}
		// delete from first to last
		} else if (fom == tb->first && to == tb->size) {
			tb->size = 0;
			while (fom->next != NULL) {
				TN *del = fom;
				fom = fom->next;
				free(del->value);
				free(del);
			}
			tb->first = tb->last = NULL;
		// delete from middle to last
		} else if (fom->prev != NULL && too->next == NULL) {
			tb->last = fom->prev;
			for (int i = 0; i <= (to - from - 1); i++) {
				TN *del = fom;
				fom = fom->next;
				fom->prev = del->prev;
				del->prev->next = fom;
				free(del->value);
				free(del);
				tb->size--;
			}
			fom->next = NULL;
			TN *del = too;
			free(del->value);
			free(del);
			tb->size--;
		}
		
	}

/*	
	TN *tmp = tb->first;
	while (tmp != NULL) {
		printf("%s", tmp->value);
		tmp = tmp->next;
	}
*/
	re_line (tb);
}


/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb) {

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2) {
	return NULL;
}

void undoTB(TB tb) {

}

void redoTB(TB tb) {

}