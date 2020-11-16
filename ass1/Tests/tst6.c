
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

static void printmap(Match mc);

int main(void) {
    
    char *ans1 = "bird\nturtle\neagle is not a bird\nB-b-b bird, bird, bird, b-bird's the word\n";

	TB new_tb1 = newTB(ans1);
    printf("\n");

    Match mc = searchTB(new_tb1, "bird");
    printmap(mc);

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