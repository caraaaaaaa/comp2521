
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

int main(void) {
    
    char *answer = "abc\ndef\nghz\ncxk\n";//string;
	TB new_tb = newTB(answer);
    printf("\n");
    addPrefixTB(new_tb, 1, 3, "bbb ");
    printf("\n\n\n%s", dumpTB(new_tb, true));
    printf("\n\n\n");
    deleteTB(new_tb, 2, 3);
    printf("Lines: %d\n", linesTB(new_tb));
    releaseTB(new_tb);

	return 0;
}