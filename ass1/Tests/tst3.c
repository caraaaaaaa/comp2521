
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "textbuffer.h"

static void printmap(Match mc);

int main(void) {
    char *ans1 = "###\n#\n###\n";

	TB new_tb1 = newTB(ans1);

    char *result1 = dumpTB(new_tb1, true);
    char *result2 = dumpTB(new_tb1, false);
    char *result3 = dumpTB(new_tb1, true);
    char *result4 = dumpTB(new_tb1, false);
    char *result5 = dumpTB(new_tb1, false);

    printf("%s\n\n", result1);
    printf("%s\n\n", result2);
    printf("%s\n\n", result3);
    printf("%s\n\n", result4);
    printf("%s\n\n", result5);
    printf("%d\n", linesTB(new_tb1));

    releaseTB(new_tb1);

	return 0;
}