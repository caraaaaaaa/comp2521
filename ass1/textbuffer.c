// COMP2521 assignment 1 textbuffer
// Zixuan GUO (z5173593)
// 2019.10

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

// textnode
typedef struct TN {
	int line;
	char *value;				// The string that node represents for
	struct TN *prev;
	struct TN *next;
} TN;

// textbuffer
typedef struct textbuffer {
	int size;
	char *string;				// The string returns
	TN *first;
	TN *last;
	TN *curr;
} textbuffer;


static TB emptyTB(char *text);
static void insertTN(TB tb, TN *tn, int COMMAND);
static TN *newTN(char *value);
static void re_line (TB tb);
static void addTN (TB tb, char *text);
static char *cut_string(char *text, int start, int end);

static char *suggestion(int num, int length, char *text, char *command);
static int number_digit(int num);
static int max(int a, int b);


// create a new empty textbuffer 
static TB emptyTB(char *text) {
	TB new_tb = malloc(sizeof *new_tb);
	assert(new_tb != NULL);

	// initialise textbuffer
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
static TN *newTN(char *valu) {
	TN *new_tn = malloc(sizeof *new_tn);
	assert(new_tn != NULL && valu != NULL);

	// initialise textnode
	new_tn->prev = new_tn->next = NULL;
	new_tn->value = valu;
	new_tn->line = NONE;

	return new_tn;
}


// after I make any change in the textbuffer
// the line number contained in each textnode will change
// This function is to re-allocate each textnode from first to last
static void re_line (TB tb) {
	TN *tmp = tb->first;
	tmp->line = 0;

	while (tmp != NULL) {
		TN *curr = tb->first;
		int line = 1;

		while (curr != tmp) {
			curr = curr->next;
			line++;
		}
		tmp->line = line;
		tmp = tmp->next;
	}
	return;
}


// add textnode into textbuffer one by one
static void addTN (TB tb, char *text) {
	assert(tb != NULL && text != NULL);
	int len = strlen(text);  
	int i = 0;
	int j = 0;

	while (i < len) {
		char *string = NULL;
		if (text[i] == '\n' || (text[i] == '\0' && text[i - 1] != '\0')) {
			string = cut_string(text, j, i);
			TN *new_tn = newTN(string);
			insertTN(tb, new_tn, AFTER);
			j = i + 1;
		}
		i++;
	}
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

	return new_tb;
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
	assert(tb != NULL);
	if (tb->first == NULL) return NULL;
	assert(tb->first != NULL);

	// create a string nemaed 'array' to store the result of the fuction 
	// and make it be the return value of this function
	re_line(tb);
	char *array = NULL;

	// when showLineNUmbers is false, only return text
    if (showLineNumbers == FALSE) {
		TN *tmp2 = tb->first;

		int length = 0;

		while (tmp2 != NULL) {
			length += strlen(tmp2->value);
			tmp2 = tmp2->next;
		}

		// malloc memory for the array 
		// '\n' will at the end of string  so length + 1
		array = malloc(sizeof(char) * (length + 1));
		tmp2 = tb->first; 
		while (tmp2 != NULL) {
			if (tmp2 == tb->first) strcpy(array, tb->first->value);
			else strcat(array, tmp2->value);
			tmp2 = tmp2->next;
		}
	// when showLineNumbers is true, return text with linenumber at first of the line
	} else {
		int num = 1, length = 0;
		TN *tmp1 = tb->first;
		for (tmp1 = tb->first; tmp1 != NULL; tmp1 = tmp1->next) {
			length += strlen(tmp1->value);
		}
		// to malloc memory for array, need to get how many digits
		int m_len = snprintf(NULL, 0, "%d", tb->size), x = tb->size;
		array = malloc(sizeof(char) * (length + (m_len + 2) * x + 1));

		tmp1 = tb->first;
		while (tmp1 != NULL) {
			// Generate Num_String
			char num_str[m_len + 3];
			sprintf(num_str, "%d. ", num);

			if (tmp1 == tb->first) strcpy(array, num_str);
			else strcat(array, num_str);
			strcat(array, tmp1->value);

			tmp1 = tmp1->next;
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
	// print error message when out of range and abort()
	if (to < from || from > tb->size || to > tb->size || from <= 0 || to <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 

	// to get the right 'from' node
	TN *fo = tb->first;
	for (int i = 1; i < from; i++) {
		fo = fo->next;
	}

	for (int f = 1; f < (to - from + 2); f++) {
		char *add = malloc(sizeof(char) * (strlen(fo->value) + strlen(prefix) + 3));
		char *ini = fo->value;

		// store prefix and initial string in 'add'
		strcpy(add, prefix);
		strcat(add, ini);

		fo->value = add;
		fo = fo->next;
		free(ini);
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

	// when the position is out of range, 
	// abort() with an error message
	if (pos > tb1->size + 1 || pos <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 

	// when tb2 is empty, the function do nothing
	// so only when tb2 is not empty it will go further the function
	if (tb2->first != NULL) {
		tb2->curr = tb2->first;
		// when tb1 is empty, merge tb2 to the start of tb1
		if (tb1->size == 0) {
			TN *tmp = tb2->first;
			while (tmp != NULL) {
				char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
				strcpy(new_value, tmp->value);
				TN *new = newTN(new_value);
				
				if (tb1->first == NULL) {
					tb1->first =  tb1->curr = tb1->last = new;
				} else {
					tb1->last->next = new;
					new->prev = tb1->last;
					tb1->last = new;
					tb1->curr = new;
				}
				tmp = tmp->next;
				tb1->size++;
			}
		} else {
			// when tb1 is not empty, and merge tb2 before the original tb1
			if (pos == 1) {
				tb2->curr = tb2->first;
				TN *tmp = tb2->first;
				TN *head = tb1->first;
				tb1->first = NULL;
				TN *new = NULL;
				while (tmp != NULL) {
					// malloc memory to store the string of tb2 node
					// then copy the string of tb2 to the new string
					// create a new node with value = string of old tb2 node
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);
					new = newTN(new_value);
					
					if (tb1->first == NULL) {
						tb1->first =  tb1->curr = new;
					} else {
						tb1->curr->next = new;
						new->prev = tb1->curr;
						tb1->curr = new;
					}
					tmp = tmp->next;
					tb1->size++;				
				}
				new->next = head;
				head->prev = new;
			// when merge tb2 after the end of tb1
			} else if (pos == tb1->size + 1) {
				TN *new = NULL;
				TN *tmp = tb2->first;
				while (tmp != NULL) {
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);
					new = newTN(new_value);
						tb1->last->next = new;
						new->prev = tb1->last;
						tb1->last = new;
					tmp = tmp->next;
					tb1->size++;
				}
			// merge tb2 in the middle of tb1
			} else {
				// let fom be the previous node of the merge position
				// too be the node after merge 
				TN *fomm = tb1->first;
				TN *tooo = tb1->first;
				for (int i = 1; i < pos - 1; i++) {
					fomm = fomm->next;
				}
				for (int j = 1; j < pos; j++) {
					tooo = tooo->next;
				}

				TN *new = NULL;
				TN *tmp = tb2->first;
				tb1->curr = fomm;
				while (tmp != NULL) {
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);
					new = newTN(new_value);

					tb1->curr->next = new;
					new->prev = tb1->curr;
					tb1->curr = new;

					tmp = tmp->next;
					tb1->size++;
				}
				// connect the new nodes with the origional tb1
				new->next = tooo;
				tooo->prev = new;
			}
		}
		
		re_line(tb1);
		releaseTB(tb2);
	}
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

	// when the position is out of range, 
	// abort() with an error message
	if (pos > tb1->size + 1 || pos <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 

	// when tb2 is empty, the function do nothing
	// so only when tb2 is not empty it will go further the function
	if (tb2->first != NULL) {
		tb2->curr = tb2->first;
		// when tb1 is empty, paste tb2 to the start of tb1
		if (tb1->size == 0) {
			TN *tmp = tb2->first;
			while (tmp != NULL) {
				char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
				strcpy(new_value, tmp->value);
				TN *new = newTN(new_value);
				
				if (tb1->first == NULL) {
					tb1->first =  tb1->curr = tb1->last = new;
				} else {
					tb1->last->next = new;
					new->prev = tb1->last;
					tb1->last = new;
					tb1->curr = new;
				}
				tmp = tmp->next;
				tb1->size++;
			}
		} else {
			// when tb1 is not empty, and paste tb2 before the original tb1
			if (pos == 1) {
				tb2->curr = tb2->first;
				TN *tmp = tb2->first;
				TN *head = tb1->first;
				tb1->first = NULL;
				TN *new = NULL;
				while (tmp != NULL) {
					// malloc memory to store the string of tb2 node
					// then copy the string of tb2 to the new string
					// create a new node with value = string of old tb2 node
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);
					new = newTN(new_value);
					
					if (tb1->first == NULL) {
						tb1->first =  tb1->curr = new;
					} else {
						tb1->curr->next = new;
						new->prev = tb1->curr;
						tb1->curr = new;
					}
					tmp = tmp->next;
					tb1->size++;				
				}
				new->next = head;
				head->prev = new;
			// when paste tb2 after the end of tb1
			} else if (pos == tb1->size + 1) {
				TN *new = NULL;
				TN *tmp = tb2->first;
				while (tmp != NULL) {
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);

					new = newTN(new_value);
						tb1->last->next = new;
						new->prev = tb1->last;
						tb1->last = new;
					tmp = tmp->next;
					tb1->size++;
				}
			// paste tb2 in the middle of tb1
			} else {
				// let fom be the previous node of the merge position
				// too be the node after merge 
				TN *fomm = tb1->first;
				TN *tooo = tb1->first;
				for (int i = 1; i < pos - 1; i++) {
					fomm = fomm->next;
				}
				for (int j = 1; j < pos; j++) {
					tooo = tooo->next;
				}

				TN *new = NULL;
				TN *tmp = tb2->first;
				tb1->curr = fomm;
				while (tmp != NULL) {
					char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
					strcpy(new_value, tmp->value);
					new = newTN(new_value);

					tb1->curr->next = new;
					new->prev = tb1->curr;
					tb1->curr = new;

					tmp = tmp->next;
					tb1->size++;
				}
				// connect the new nodes with the origional tb1
				new->next = tooo;
				tooo->prev = new;
			}
		}
		re_line(tb1);
	}
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
	assert(tb != NULL);
	if (to < from || from > tb->size || to > tb->size || from <= 0 || to <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 

	// find the right position for 'from' node and 'to' node
	TN *fro = tb->first;
	TN *t = tb->first;
	for (int i = 1; i < from; i++) {
		fro = fro->next;
	}
	for (int j = 1; j < to + 1; j++) {
		t = t->next;
	}

	// create a new textbuffer be the return textbuffer
	TB tb2 = newTB("");

	TN *tmp = fro;
	while (tmp != t) {
		char *new_value = malloc(sizeof(char) * (strlen(tmp->value) + 1));
		strcpy(new_value, tmp->value);
		TN *new = newTN(new_value);
		
		if (tb2->first == NULL) {
			tb2->first =  tb2->curr = tb2->last = new;
		} else {
			tb2->last->next = new;
			new->prev = tb2->last;
			tb2->last = new;
			tb2->curr = new;
		}
		tmp = tmp->next;
		tb2->size++;
	}

	deleteTB(tb, from, to);
	return tb2;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	assert(tb != NULL);

	if (search == NULL) {
		printf("The string 'search' is NULL.\n");
		abort();
	}

	Match head = NULL;
	Match tail = NULL;

	if (strlen(search) == 0) return head;
	TN *s = tb->first;
	
	int sea_line = 1;
	while (s != NULL) {
		if (strlen(s->value) >= strlen(search)) {

			for (int c = 0; c <= strlen(s->value) - strlen(search); c++) {
				char *cmp = cut_string(s->value, c, c + strlen(search) - 1);

				if (strcmp(cmp, search) == 0) {
					
					// malloc match and setup
					Match sear_node = malloc(sizeof *sear_node);
					sear_node->next = NULL;
					sear_node->lineNumber = sea_line;
					sear_node->columnNumber = c + 1;

					if (tail == NULL) {
						head = tail = sear_node;
					} else {
						tail->next = sear_node;
						tail = sear_node;
					}
					// keep loop
					c += strlen(search) - 1;
				}
				free(cmp);
			}
		}
		sea_line++;
		s = s->next;
	}
	return head;
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
	if (to < from || from > tb->size || to > tb->size || from <= 0 || to <= 0) {
		printf("Error: The line number is out of range.\n");
		abort();
	} 

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
			TN *dele = fom;
			fom = fom->next;
			fom->prev = NULL;
			free(dele->value);
			free(dele);
			tb->size--;
		}
	// delete from first to last
	} else if (from == 1 && to == tb->size) {
		while (fom != NULL) {
			TN *del = fom;
			fom = fom->next;
			free(del->value);
			free(del);
			tb->size--;
		}
		tb->first = tb->last = tb->curr = NULL;
		
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

		tb->last->next = NULL;
		tb->size--;

		free(del->value);
		free(del);
	}

	re_line (tb);
}


/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
// *some string*       <b>some string</b>
// _some string_       <i>some string</i>
// #some string ...    <h1>some string ...</h1>
void formRichText(TB tb) {
}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2) {

	// set-up and allocate memory
	TN *tmp = tb1->first, *tmp1 = tb1->first, *tmp2 = tb2->first;
	int length = 0, size = max(tb1->size, tb2->size), digit = number_digit(size), c = 0, i = 1;
	for (tmp = tb1->first; tmp != NULL; tmp = tmp->next) {
		length += strlen(tmp->value);
	}
	char *answer = malloc((length + (digit * 2 + 8) * size + 1) * sizeof(char));

	// Compare and indicate solutions
	while (tmp1 != NULL && tmp2 != NULL) {
		if (strcmp(tmp1->value, tmp2->value) != 0) {
			
			char *add = suggestion(i, 4 + number_digit(i) + strlen(tmp2->value), tmp2->value, "+,");
			char *delete = suggestion(i + 1, 3 + number_digit(i + 1), tmp2->value, "-,");

			if (c == 0) {
				strcpy(answer, add);
			} else if (c == 1) {
				strcat(answer, add);
			}
			strcat(answer, delete);
			c = 1;

			free(add);
			free(delete);
		}
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
		i++;
	}

	// when tb1 reach the end while tb2 still not reach the end
	if (tmp2 != NULL && tmp1 == NULL) {
		while (tmp2 != NULL) {
			char *add = suggestion(i, 4 + number_digit(i), tmp2->value, "+,");
			strcat(answer, add);
			free(add);
			tmp2 = tmp2->next;
			i++;
		}
	}

	// when tb2 reach the end while tb1 still not reach the end
	if (tmp2 == NULL && tmp1 != NULL) {
		while (tmp1 != NULL) {
			char *delete = suggestion(i, 3 + number_digit(i), tmp2->value, "-,");
			strcat(answer, delete);
			free(delete);
			tmp1 = tmp1->next;
		}
	}
	return answer;
}

static char *suggestion(int num, int length, char *text, char *command) {
	char *ans = malloc((length + 1) * sizeof(char));

	// assign command
	strcpy(ans, command);

	// generate number length
	int len = snprintf(NULL, 0, "%d", num);

	if (strcmp(command, "+,") == 0) {
		char num_str[len + 2];
		sprintf(num_str, "%d,", num);
		strcat(ans, num_str);
		strcat(ans, text);
	} else if (strcmp(command, "-,") == 0) {
		char num_str[len + 1];
		sprintf(num_str, "%d", num);
		strcat(ans, num_str);
		strcat(ans, "\n");
	}
	return ans;
}

// to find the number of digit it take
static int number_digit(int num) {
	int x = num, i = 0;

	while (x >= 1) {
		x = x / 10;
		i++;
	}
	return i;
}

// find max
static int max(int a, int b) {
	if (a < b) return b;
	return a;
}











void undoTB(TB tb) {

}

void redoTB(TB tb) {

}