
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

static void printmap(Match mc);

int main(void) {
    
    char *ans1 = "aaa\nbbb\nccc\nddd\n";
    char *ans2 = "111\n222\n333\n444\n";

	TB new_tb1 = newTB(ans1);
    TB new_tb2 = newTB(ans2);
    printf("\n");

    mergeTB(new_tb1, 1, new_tb2);
    char *result1 = dumpTB(new_tb1, true);
    printf("%s\n", result1);
	addPrefixTB(new_tb1, 2, 5, "ddddd ");
	char *result2 = dumpTB(new_tb1, true);
	printf("%s\n", result2);

    releaseTB(new_tb1);

	return 0;
}

// Print map
static void printmap(Match mc) {
	if (mc == NULL)
        abort();
	Match curr = mc;

	printf("\nMatches: \n");	
	while (curr != NULL) {
		printf("row: %d, col: %d\n", curr->lineNumber, curr->columnNumber);
		curr = curr->next;
	}

	return;
}