
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

int main(void) {
    
    char *answer = "bird\nturtle\neagle is not a bird\nB-b-b bird, bird, bird, b-bird's the word\n";//string;
	TB new_tb = newTB(answer);
    printf("Part 1:\n\n");
    printf("%s", dumpTB(new_tb, true));

    TB cut_tb = cutTB(new_tb, 4, 1);
    printf("\nPart 2:\n\n");
    printf("%s", dumpTB(new_tb, true));
    printf("\nPart 3:\n\n");
    printf("%s", dumpTB(cut_tb, true));
    printf("\nFINISH!!!");
    
    releaseTB(new_tb);

	return 0;
}