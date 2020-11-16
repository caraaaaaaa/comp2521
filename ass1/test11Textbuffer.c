// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

static void testNewTB(void);

// TODO: Add more function prototypes


int main(void) {
	
	testNewTB();
	
	// TODO: Call more test functions
	
	printf("All tests passed! You are awesome!\n");
}

static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");

	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");
	TB tb2 = newTB("hello there,\napp\nare\nthing\n");

	char *str = diffTB(tb1, tb2);
	printf("%s", str);

	free(str);
	releaseTB(tb1);
	releaseTB(tb2);
}

// TODO: Add more test functions

