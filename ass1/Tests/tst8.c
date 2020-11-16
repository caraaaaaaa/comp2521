
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

int main(void) {
    
    char *input1 = "*some string\n#\n###\n";
    char *cmp1 = "*some string\n#\n<h1>##</h1>\n";

    printf("Test 1: \n\n");
	TB tb1 = newTB(input1);
    char *str0 = dumpTB(tb1, true);
    printf("Input: \n%s\n", str0);
	formRichText(tb1);
	char *str1 = dumpTB(tb1, false);
	printf("Output: \n%s\n", str1);
    printf("Strlen: \n%d\n\n", strlen(str1));

    printf("Compare: \n%s\n",cmp1);
    printf("Strlen: \n%d\n\n", strlen(cmp1));

    if (strcmp(cmp1, str1) != 0)
        printf("NULL\n");

    return 0;
}