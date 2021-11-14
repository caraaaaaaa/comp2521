#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "textbuffer.h"

#define TRUE 1
#define FALSE 0

typedef struct line {
	char *value;
	struct line *next;
	struct line *prev;
}Line;

struct textbuffer{
	Line* first;
	Line* last;
	int nlines;
};
 
/* Allocate a new textbuffer whose contents is initialised with the text given
 * in the array.
 */
TB newTB (char text[]) {
	
	TB L;
	Line *new;	
	char *token;	
	char *string = text;
	int i = 0, num = 0;	
	
	L = malloc(sizeof (struct textbuffer));
	L->nlines = 0;
	L->first = NULL;
	L->last = NULL;	
	if (strlen(text) == 0){			
		return L;	
	}
	for (i = 0; i < strlen(text); i++) {
		if (text[i] == '\n') {
			num++;
		}
	}
	for (token = strsep(&string, "\n"); token != NULL && L->nlines < num; token = strsep(&string, "\n")) {				
		new = malloc(sizeof(Line));
		new->value = strdup(token);
		new->prev = new->next = NULL;		
		if (L->last == NULL) {
			L->first = new;
			L->last = new;
		} else {
			L->last->next = new;
			new->prev = L->last;
			L->last = new;
		}					
		L->nlines++;
	}
	return L;
}


/* Free the memory occupied by the given textbuffer.  It is an error to access
 * the buffer afterwards.
 */
void releaseTB (TB tb) {

	assert(tb != NULL);
	Line *curr, *prev;
	curr = tb->first;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
		free(prev->value);
		free(prev);
	}
	free(tb);
}

/* Allocate and return an array containing the text in the given textbuffer.
 * add a prefix corrosponding to line number iff showLineNumbers == TRUE
 */
char *dumpTB (TB tb, int showLineNumbers){
	
	assert(tb != NULL);
	char *array = calloc(1, sizeof (char));
	char *str;
	int n = 1;	
	if (tb->nlines == 0) {
		return array;
	}
	Line *p = tb->first;	
	if (showLineNumbers == TRUE) {
		while (p != NULL) {
			str = calloc(strlen(p->value) + 10, sizeof (char));			
			sprintf(str, "%d. %s\n", n, p->value);
			array = realloc(array, strlen(array) + strlen(str) + 1);			
			strcat(array, str);
			free(str);			
			n++;			
			p = p->next;
		}
	} else {
		while (p != NULL) {
			str = calloc(strlen(p->value) + 10, sizeof (char));		
			sprintf(str, "%s\n", p->value);
			array = realloc(array, strlen(array) + strlen(str) + 1);			
			strcat(array, str);
			free(str);						
			p = p->next;
		}
	}	
	return array;
}

/* Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb){
	assert(tb != NULL);
	return tb->nlines;
}

/* Add a given prefix to all lines between pos1 and pos2
 *
 * - The program is to abort() with an error message if line 'pos1' or line
 *   'pos2' is out of range.  The first line of a textbuffer is at position 0.
 */
void addPrefixTB (TB tb, int pos1, int pos2, char* prefix){
	assert(tb != NULL);
	int i = 0;
	char *str;
	if (pos1 >= linesTB(tb) || pos2 >= linesTB(tb)) {
		printf("postion out of bound\n");
		abort();
	}
	if (strlen(prefix) != 0) {
		Line *p = tb->first;
		while (p != NULL) {
			if (i >= pos1 && i<= pos2) {
				str = malloc(strlen(p->value) + strlen(prefix) + 10);
				sprintf(str, "%s%s", prefix, p->value);
				free(p->value);			
				p->value = strdup(str);
				free(str);
			}			
			i++;
			p = p->next;
		}
	}
}

/* Merge 'tb2' into 'tb1' at line 'pos'.
 *
 * - Afterwards line 0 of 'tb2' will be line 'pos' of 'tb1'.
 * - The old line 'pos' of 'tb1' will follow after the last line of 'tb2'.
 * - After this operation 'tb2' can not be used anymore (as if we had used
 *   releaseTB() on it).
 * - The program is to abort() with an error message if 'pos' is out of range.
 */
void mergeTB (TB tb1, int pos, TB tb2){
	
	assert(tb1 != NULL);
	assert(tb2 != NULL);
	int i = 0;	
	Line *p, *q;	
	
	if (pos > linesTB(tb1) || pos < 0) {
		printf("postion out of bound\n");
		abort();
	}
	if (tb1->nlines == 0 && pos == 0) {
		tb1->first = tb2->first;
		tb1->last = tb2->last;
		tb1->nlines = tb2->nlines;		
		p = tb2->first;
		q = tb1->first;
		while (p->next != NULL) {		
			q->next = p->next;
			p = p->next;
			q = q->next;					
		}
		free(tb2);
		return;				
	}	
	if (tb2->nlines != 0) {
		if (pos == 0) {
			p = tb1->first;
			q = tb2->last;
			p->prev = q;
			q->next = p;
			tb1->first = tb2->first;
		} else if (pos == tb1->nlines) {
			p = tb1->last;
			q = tb2->first;
			p->next = q;
			q->prev = p;
			tb1->last = tb2->last;
		} else {
			p = tb1->first;
			q = tb2->first;
			Line *t = tb2->last;			
			while (p != NULL) {
				if (i == pos - 1) {
					p->next->prev = t;
					t->next = p->next;
					p->next = q;
					q->prev = p;
				}
				i++;
				p = p->next;			
			}
		}
		tb1->nlines += tb2->nlines;
		free(tb2);	
	}
}

/* Copy 'tb2' into 'tb1' at line 'pos'.
 *
 * - Afterwards line 0 of 'tb2' will be line 'pos' of 'tb1'.
 * - The old line 'pos' of 'tb1' will follow after the last line of 'tb2'.
 * - After this operation 'tb2' is unmodified and remains usable independent
 *   of 'tb1'.
 * - The program is to abort() with an error message if 'pos' is out of range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {
	
	assert(tb1 != NULL);
	assert(tb2 != NULL);
	int i = 0;	
	Line *p, *q;

	if (pos > tb1->nlines || pos < 0) {
		printf("postion out of bound");
		abort();
	}
	char *s = dumpTB(tb2,FALSE);	
	TB new = newTB(s);
	if (tb1->nlines == 0 && pos == 0) {
		tb1->first = new->first;
		tb1->last = new->last;
		tb1->nlines = new->nlines;		
		p = new->first;
		q = tb1->first;
		while (p->next != NULL) {		
			q->next = p->next;
			p = p->next;
			q = q->next;					
		}
		free(new);
		free(s);		
		return;				
	}	
	if (tb2->nlines != 0) {
		if (pos == 0) {
			p = tb1->first;
			q = new->last;
			p->prev = q;
			q->next = p;
			tb1->first = new->first;
		} else if (pos == tb1->nlines) {
			p = tb1->last;
			q = new->first;
			p->next = q;
			q->prev = p;
			tb1->last = new->last;
		} else {
			p = tb1->first;
			q = new->first;
			Line *t = new->last;			
			while (p != NULL) {
				if (i == pos - 1) {
					p->next->prev = t;
					t->next = p->next;
					p->next = q;
					q->prev = p;
				}
				i++;
				p = p->next;			
			}
		}
		tb1->nlines += new->nlines;		
		free(new);
		free(s);
	}
}

/* Cut the lines between and including 'from' and 'to' out of the textbuffer
 * 'tb'.
 *
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
TB cutTB (TB tb, int from, int to){
	
	assert(tb != NULL);
	int i = 0;
	Line *p = tb->first;
	Line *q;	
	char *array = calloc(1, sizeof (char));
	char *str;
	//TB new;
	
	if (from > to || from < 0 || to >= linesTB(tb)) {
		printf("try to delete beyond range\n");
		abort();
	}
	if (from == 0) {	
		for (i = 0; i <= to; i++) {
			if (p == tb->last) {
				tb->first = tb->last = NULL;
				str = calloc(strlen(p->value) + 10, sizeof (char));			
				sprintf(str, "%s\n", p->value);
				array = realloc(array, strlen(array) + strlen(str) + 1);			
				strcat(array, str);
				free(str);				
				free(p->value);
				free(p);			
			} else{				
				q = p;
				p = p->next;
				tb->first = p;
				p->prev = NULL;			
				str = calloc(strlen(q->value) + 10, sizeof (char));			
				sprintf(str, "%s\n", q->value);
				array = realloc(array, strlen(array) + strlen(str) + 1);			
				strcat(array, str);
				free(str);				
				free(q->value);
				free(q);
			}
			tb->nlines--;
		}
	} else {
		for (i = 0; i < from; i++) {
			p = p->next;
		}
		for (i = from; i <= to; i++) {
			if (p == tb->last) {
				p->prev->next = NULL;
				tb->last = p->prev;				
				str = calloc(strlen(p->value) + 10, sizeof (char));			
				sprintf(str, "%s\n", p->value);
				array = realloc(array, strlen(array) + strlen(str) + 1);			
				strcat(array, str);
				free(str);							
				free(p->value);
				free(p);	
			} else{			
				q = p;
				p = p->prev;
				p->next = q->next;
				q->next->prev = p;
				str = calloc(strlen(q->value) + 10, sizeof (char));			
				sprintf(str, "%s\n", q->value);
				array = realloc(array, strlen(array) + strlen(str) + 1);			
				strcat(array, str);
				free(str);				
				free(q->value);
				free(q);
				p = p->next;
			}
			tb->nlines--;
		}
	}
	TB new = newTB(array);
	free(array);
	return new;
}


// find whether search is in the text
// if in the text, return its first position in the line
// if not, return -1
static int findString(char *source, char *search) {
	
	int i, j;
	for (i=0; source[i]!='\0'; i++)
	{
		if(source[i]!=search[0]) continue;		
		j = 0;
		while(search[j]!='\0' && source[i+j]!='\0')
		{
			j++;
			if(search[j] != source[i+j]) break;
		}
		if(search[j]=='\0') return i;
	}
	return -1;
}
/*  Return a linked list of Match nodes of all the matches of string search
 *  in tb
 *
 * - The textbuffer 'tb' will remain unmodified.
 * - The user is responsible of freeing the returned list
 */
Match searchTB (TB tb, char* search){
	
	assert(tb != NULL);
	int num = 0, pos;
	Match new, head = NULL, tail = NULL;

	if (search == NULL) {
		printf("serch text is null");
		abort();
	}
	if (strlen(search) == 0) {
		return head;
	}
	Line *p = tb->first;
	while (p != NULL) {
		if (strlen(search) > strlen(p->value)) {
			p = p->next;
		} else {
			if((pos = findString(p->value, search)) >= 0) {
				new = malloc(sizeof (matchNode));
				if (tail == NULL) {
					head = new;
					tail = new;
				} else {
					tail->next = new;
					tail = new;
				}
				new->lineNumber = num + 1;
				new->charIndex = pos;
				new->next = NULL;
			}
			p = p->next;
		}
		num++;				
	}
	return head;
}



/* Remove the lines between and including 'from' and 'to' from the textbuffer
 * 'tb'.
 *
 * - The program is to abort() with an error message if 'from' or 'to' is out
 *   of range.
 */
void deleteTB (TB tb, int from, int to){
	
	assert(tb != NULL);
	int i = 0;
	Line *p = tb->first;
	Line *q;	
	if (from > to || from < 0 || to >= linesTB(tb)) {
		printf("try to delete beyond range\n");
		abort();
	}
	if (from == 0) {
		for (i = 0; i <= to; i++) {
			if (p == tb->last) {
				tb->first = tb->last = NULL;
				free(p->value);
				free(p);			
			} else{				
				q = p;
				p = p->next;
				tb->first = p;
				p->prev = NULL;			
				free(q->value);
				free(q);
			}
			tb->nlines--;
		}
	} else {
		for (i = 0; i < from; i++) {
			p = p->next;
		}
		for (i = from; i <= to; i++) {
			if (p == tb->last) {
				p->prev->next = NULL;
				tb->last = p->prev;				
				free(p->value);
				free(p);	
			} else{			
				q = p;
				p = p->prev;
				p->next = q->next;
				q->next->prev = p;
				free(q->value);
				free(q);
				p = p->next;
			}
			tb->nlines--;
		}
	}
}


/* Search every line of tb for each occurrence of a set of specified subsitituions
 * and alter them accordingly
 *
 * refer to spec for table.
 */
void formRichText (TB tb){
	
	int flag, i, j;
	int len;

	Line *p = tb->first;
	while (p != NULL) {
		if (p->value[0] == '#' && strlen(p->value) > 1) {
			p->value[0] = '>';			
			char *s = calloc(strlen(p->value) + 10, sizeof (char));			
			sprintf(s, "<h1%s</h1>", p->value);
			p->value = realloc(p->value, strlen(s) + 10);
			sprintf(p->value, "%s", s);
			free(s);
		} else {
			for (i = 0; p->value[i] != '\0'; i++) {
				flag = -1;				
				if (p->value[i] == '*') {
					flag = -1;
					for (j = i + 1; p->value[j] != '\0'; j++) {
						if (p->value[j] == '*' && j == i + 1) {
							break;
						}
						if (p->value[j] == '*' && j > i + 1) {
							flag = j;
							break;
						}
					}
					if (flag != -1) {
						p->value = realloc(p->value, strlen(p->value) + 10);
						len = strlen(p->value) + 2;
						p->value[len] = '\0';
						for (j = len - 1; j > i + 2; j--) {
							p->value[j] = p->value[j - 2];
						}
						p->value[i] = '<';
						p->value[i + 1] = 'b';
						p->value[i + 2] = '>';
						flag += 2;						
						len = strlen(p->value) + 3;
						p->value[len] = '\0';
						for (j = len - 1; j > flag + 3; j--) {
							p->value[j] = p->value[j - 3];
						}
						p->value[flag] = '<';
						p->value[flag + 1] = '/';
						p->value[flag + 2] = 'b';
						p->value[flag + 3] = '>';
						i = flag + 3;
					}
				} else if (p->value[i] == '_') {
					flag = -1;
					for (j = i + 1; p->value[j] != '\0'; j++) {
						if (p->value[j] == '_' && j == i + 1) {
							break;
						}
						if (p->value[j] == '_' && j > i + 1) {
							flag = j;
							break;
						}
					}
					if (flag != -1) {
						p->value = realloc(p->value, strlen(p->value) + 10);
						len = strlen(p->value) + 2;
						p->value[len] = '\0';
						for (j = len - 1; j > i + 2; j--) {
							p->value[j] = p->value[j - 2];
						}
						p->value[i] = '<';
						p->value[i + 1] = 'i';
						p->value[i + 2] = '>';
						flag += 2;						
						len = strlen(p->value) + 3;
						p->value[len] = '\0';
						for (j = len - 1; j > flag + 3; j--) {
							p->value[j] = p->value[j - 3];
						}
						p->value[flag] = '<';
						p->value[flag + 1] = '/';
						p->value[flag + 2] = 'i';
						p->value[flag + 3] = '>';
						i = flag + 3;
					}
				}
			}
		}		
		p = p->next;
	}
}

/* Your whitebox tests
 */
void validTests(TB tb1) {
	if (tb1 == NULL) {
		fprintf(stderr,"textbuffer is null\n");
	}
	if (tb1->first == NULL) {
		if (tb1->last != NULL) {
			fprintf(stderr,"Non-null pointers in empty list\n");
		}
	} else {
		if (tb1->last == NULL) {
			fprintf(stderr,"Null pointers in non-empty list\n");
		}
	}
	int count = 0;
	Line *curr;
	//scan from first to last and check the link.	
	for (curr = tb1->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf(stderr, "Invalid forward link into node (%s)\n",curr->value);
		}
		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf(stderr, "Invalid backward link into node (%s)\n",curr->value);
		}
		count++;
	}
	if (count != tb1->nlines) {
		fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n", count, tb1->nlines);
	}
	count = 0;
	//scan from last to first.	
	for (curr = tb1->last; curr != NULL; curr = curr->prev) {
		count++;
	}
	if (count != tb1->nlines) {
		fprintf(stderr, "Backward count mismatch; counted=%d, nitems=%d\n", count, tb1->nlines);
	}	
}

void whiteBoxTests() {

	char str1[] ="line 01\n"
                 "line 02\n"
                 "line 03\n"
                 "line 04\n"
                 "line 05\n"
                 "line 06\n"
                 "line 07\n"
                 "line 08\n"
                 "line 09\n"
                 "*line* 10\n";
	
	char str2[] ="merge line01\n"
				 "merge line02\n";

	char *prefix = "GOOD NIGHT";	
	char *s;	
	// test newTB	
	TB tb1 = newTB(str1);
	TB tb2 = newTB(str2);
	validTests(tb1);
	validTests(tb2);
	// test dumpTB	
	s = dumpTB(tb1, FALSE);
	validTests(tb1);
	free(s);
	s = dumpTB(tb1, TRUE);
	validTests(tb1);
	free(s);
	// test add prefix
	addPrefixTB(tb1, 0, 5, prefix);
	validTests(tb1);
	// test merge	
	mergeTB(tb1, 0, tb2);
	validTests(tb1);
	// test paste
	tb2 = newTB(str2);
	pasteTB(tb1, 0, tb2);
	validTests(tb1);
	validTests(tb2);
	// test cut
	TB tb3 = cutTB(tb1, 0, 1);
	validTests(tb1);
	validTests(tb3);
	// test delete
	deleteTB(tb1, 0, 1);
	validTests(tb1);
	// test rich
	formRichText(tb1);
	validTests(tb1);
	// release
	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
}
