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

#define NONENUM  0
#define NONESTR "\0"

#define TBLINES	'l'
#define TBTEXT	't'
#define TBMERGE 'm'
#define TBPASTE 'p'
#define MAX     100

static void test_engine(void);				//
static void testNewTB_ReleaseTB(void);		//
static void testDumpTB_LinesTB(void);		//
static void testMergeTB(void);				//
static void testPasteTB(void);				//
static void testPerfixTB(void);				//
static void testCutTB(void);				//	
static void testDeleteTB(void);				//	
static void testSearchTB(void);				//
static void testFormRichText(void);			

static void Test_NEW(TB tb1, char *input, int NO, int lines);
static void Test_DUMP(TB tb, char *input, char *cmp, int NO, bool COMMAND, int line);
static void Test_MERGE(TB tb1, TB tb2, int pos, char *cmp, int NO, int line, char COMMAND);
static void Test_PERFIX(TB tb, char *input, char *cmp, int NO, int from, int to);
static void Test_SEARCH(TB tb, char *input, int cmp1[MAX], int cmp2[MAX], int NO);
static void Test_DELETE(TB tb, char *input, char *cmp, int from, int to, int NO);

static void Test_Begin(char *input, int NO);
static void PrintMap(char *text);
static void PrintFails(char COMMAND, int num, int cmp_n, char *text, char *cmp_s, TB tb);


int main(void) {
	
	test_engine();
	printf("All tests passed! You are awesome!\n");
	return 0;
}

static void test_engine(void) {
	char COMMAND;
	printf("Please enter the test you want to play: \n\n");
	scanf("%c", &COMMAND);

	switch (COMMAND) {
		case 'h' :
			printf("-----------HELP-----------\n"
				   "Input: 'h'    ENQUIRE HELP\n"
				   "Input: 'n'      test newTB\n"
				   "Input: 'r'  test releaseTB\n"
				   "Input: 'm'    test mergeTB\n"
				   "Input: 'p'    test pasteTB\n"
				   "Input: 'd'     test dumpTB\n"
				   "Input: 'l'    test linesTB\n"
				   "Input: 'a'   test perfixTB\n"
				   "Input: 's'   test searchTB\n"
				   "Input: 'f'   test formRich\n"
				   "Input: 'e'   test deleteTB\n"
				   "Input: 'c'      test cutTB\n"
				   "Input: 'q'      quit tests\n"
				   );
			break;
		case 'n' :
		case 'r' :
			testNewTB_ReleaseTB();
			break;
		case 'm' :
			testMergeTB();
			break;
		case 'p' :
			testPasteTB();
			break;
		case 'a' :
			testPerfixTB();
			break;
		case 'd' :
		case 'l' :
			testDumpTB_LinesTB();
			break;
		case 'e' :
			testDeleteTB();
			break;
		case 'c' :
			testCutTB();
			break;
		case 's' :
			testSearchTB();
			break;
		case 'f' :
			testFormRichText();
			break;
		case 'q' :
			return;
			break;
	}

	test_engine();
	return;
}

//	Tests Functions
//////////////////////////////////////////////////////////////////////////////////////
static void testNewTB_ReleaseTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");
	
	char *input1 = "hello there,\nhow\nare\nthings\ngoing\n";
	char *input2 = "This is\n a simple\n";
	char *input3 = "\n\n\n";
	// char *input4 = "\0";

	TB tb1 = newTB(input1);
	TB tb2 = newTB(input2);
	TB tb3 = newTB(input3);
	// TB tb4 = newTB(input4);

	Test_NEW(tb1, input1, 1, 5);
	Test_NEW(tb2, input2, 2, 2);
	Test_NEW(tb3, input3, 3, 3);
	// Test_NEW(tb4, input4, 4, 0);

	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	// releaseTB(tb4);
	
	printf("All newTB tests passed!\n");
	return;
}

static void testDumpTB_LinesTB(void) {
	printf("-----------------------------------------\n"
	       "         Lines and dumpTB tests          \n"
	       "-----------------------------------------\n");

	char *input1 = "111\n222\n333\n444\n";
	//	char *input2 = "\0";
	char *input3 = "\n\n\n\n";

	char *cmp1 = "111\n222\n333\n444\n";
	char *cmp2 = "1. 111\n2. 222\n3. 333\n4. 444\n";
	//	char *cmp3 = "\0";
	char *cmp4 = "1. \n2. \n3. \n4. \n";

	TB tb1 = newTB(input1);
	//	TB tb2 = newTB(input2);
	TB tb3 = newTB(input3);

	Test_DUMP(tb1, input1, cmp1, 1, false, 4);
	Test_DUMP(tb1, input1, cmp2, 2, true, 4);
	//	Test_DUMP(tb2, input2, cmp3, 3, false, 0);
	Test_DUMP(tb3, input3, cmp4, 4, true, 4);

	releaseTB(tb1);
	//	releaseTB(tb2);
	releaseTB(tb3);

	printf("All dumpTB tests passed!\n");
	return;
}

static void testMergeTB(void) {
	printf("-----------------------------------------\n"
	       "              mergeTB tests              \n"
	       "-----------------------------------------\n");
	
	char *input1 = "111\n222\n333\n444\n";
	char *input2 = "aaa\nbbb\nccc\nddd\n";
	char *input3 = "baby hippo\npiglet";
	char *input4 = "piglet";

	char *cmp1 = "aaa\nbbb\nccc\nddd\n111\n222\n333\n444\n";
	char *cmp2 = "111\naaa\nbbb\nccc\nddd\n222\n333\n444\n";
	char *cmp3 = "111\n222\naaa\nbbb\nccc\nddd\n333\n444\n";
	char *cmp4 = "111\n222\n333\n444\naaa\nbbb\nccc\nddd\n";
	char *cmp5 = "baby hippo\npigletaaa\nbbb\nccc\nddd\n";
	char *cmp6 = "aaa\nbbb\nbaby hippo\npigletccc\nddd\n";
	char *cmp7 = "aaa\nbbb\nccc\nbaby hippo\npigletddd\n";
	char *cmp8 = "aaa\nbbb\npigletccc\nddd\n";
	

	TB tb11 = newTB(input1);
	TB tb12 = newTB(input1);
	TB tb13 = newTB(input1);
	TB tb14 = newTB(input1);
	TB tb15 = newTB(input2);
	TB tb16 = newTB(input2);
	TB tb17 = newTB(input2);
	TB tb18 = newTB(input2);

	TB tb21 = newTB(input2);
	TB tb22 = newTB(input2);
	TB tb23 = newTB(input2);
	TB tb24 = newTB(input2);
	TB tb25 = newTB(input3);
	TB tb26 = newTB(input3);
	TB tb27 = newTB(input3);
	TB tb28 = newTB(input4);


	Test_MERGE(tb11, tb21, 1, cmp1, 1, 8, TBMERGE);
	Test_MERGE(tb12, tb22, 2, cmp2, 2, 8, TBMERGE);
	Test_MERGE(tb13, tb23, 3, cmp3, 3, 8, TBMERGE);
	Test_MERGE(tb14, tb24, 5, cmp4, 4, 8, TBMERGE);
	Test_MERGE(tb15, tb25, 1, cmp5, 5, 5, TBMERGE);
	Test_MERGE(tb16, tb26, 3, cmp6, 6, 5, TBMERGE);
	Test_MERGE(tb17, tb27, 4, cmp7, 7, 5, TBMERGE);
	Test_MERGE(tb18, tb28, 3, cmp8, 8, 4, TBMERGE);

	releaseTB(tb11);
	releaseTB(tb12);
	releaseTB(tb13);
	releaseTB(tb14);
	releaseTB(tb15);
	releaseTB(tb16);
	releaseTB(tb17);
	releaseTB(tb18);
	
	printf("All mergeTB tests passed!\n");
	return;
}

static void testPasteTB(void) {
	printf("-----------------------------------------\n"
	       "              pasteTB tests              \n"
	       "-----------------------------------------\n");
	char *input1 = "111\n222\n333\n444\n";
	char *input2 = "aaa\nbbb\nccc\nddd\n";
	char *cmp2 = "111\naaa\nbbb\nccc\nddd\n222\n333\n444\n";
	char *cmp9 = "aaa\nbbb\nccc\nddd\naaa\nbbb\nccc\nddd\n";

	TB tb31 = newTB(input1);
	TB tb32 = newTB(input2);
	TB tb33 = newTB(input2);


	Test_MERGE(tb31, tb32, 2, cmp2, 1, 8, TBPASTE);
	Test_MERGE(tb33, tb33, 1, cmp9, 2, 8, TBPASTE);

	releaseTB(tb31);
	releaseTB(tb32);
	releaseTB(tb33);

	printf("All pasteTB tests passed!\n");

	return;
}

static void testPerfixTB(void) {
	printf("-----------------------------------------\n"
	       "             addPerficTB tests           \n"
	       "-----------------------------------------\n");

	char *input1 = "111\n222\n333\n444\n";
	char *input2 = "abc\ndef\nabc\ndef";
	char *perfix1 = "numbers: ";
	char *perfix2 = "letters: ";

	char *cmp1 = "numbers: 111\nnumbers: 222\nnumbers: 333\nnumbers: 444\n";
	char *cmp2 = "letters: abc\nletters: def\nletters: abc\nletters: def";
	char *cmp3 = "numbers: 111\nnumbers: numbers: 222\nnumbers: 333\nnumbers: 444\n";
	char *cmp4 = "numbers: 111\n222\n333\n444\n";
	char *cmp5 = "111\nnumbers: 222\nnumbers: 333\n444\n";
	char *cmp6 = "abc\ndef\nabc\nletters: def";

	TB tb1 = newTB(input1);
	TB tb2 = newTB(input2);
	TB tb4 = newTB(input1);
	TB tb5 = newTB(input1);
	TB tb6 = newTB(input2);

	Test_PERFIX(tb1, perfix1, cmp1, 1, 1, 4);
	Test_PERFIX(tb2, perfix2, cmp2, 2, 1, 4);
	Test_PERFIX(tb1, perfix1, cmp3, 3, 2, 2);
	Test_PERFIX(tb4, perfix1, cmp4, 4, 1, 1);
	Test_PERFIX(tb5, perfix1, cmp5, 5, 2, 3);
	Test_PERFIX(tb6, perfix2, cmp6, 6, 4, 4);

	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb4);
	releaseTB(tb5);
	releaseTB(tb6);

	printf("All addPerfixTB tests passed!\n");
	return;	
}

static void testDeleteTB(void) {
	printf("-----------------------------------------\n"
	       "               deleteTB tests            \n"
	       "-----------------------------------------\n");

	char *input1 = "bird\nturtle\neagle is not a bird\nB-b-b bird, bird, bird, b-bird's the word\n";
	char *input2 = "abracadabra alacazam\nabracadabracadabracadabracadabra\n";

	char *cmp1 = "bird\nB-b-b bird, bird, bird, b-bird's the word\n";
	//  char *cmp2 = "bird\nturtle\neagle is not a bird\n";
	char *cmp3 = "abracadabra alacazam\n";
	char *cmp4 = "\0";

	TB tb1 = newTB(input1);
	//	TB tb2 = newTB(input1);
	TB tb3 = newTB(input2);

	Test_DELETE(tb1, input1, cmp1, 2, 3, 1);
	//	Test_DELETE(tb2, input1, cmp2, 4, 4, 2);
	Test_DELETE(tb3, input2, cmp3, 2, 2, 3);
	Test_DELETE(tb3, input2, cmp4, 1, 1, 4);

	releaseTB(tb1);
	//	releaseTB(tb2);
	releaseTB(tb3);

	printf("All CUT Tests Passed!!!\n\n");
	return;
}

static void testCutTB(void) {
	printf("-----------------------------------------\n"
	       "                 cutTB tests             \n"
	       "-----------------------------------------\n");

	char *answer = "bird\nturtle\neagle is not a bird\nB-b-b bird, bird, bird, b-bird's the word\n";

	TB new_tb1 = newTB(answer);
    printf("Part 1:\n\n");
	char *str1 = dumpTB(new_tb1, true);
    printf("%s", str1);

    TB cut_tb1 = cutTB(new_tb1, 2, 3);
	char *str2 = dumpTB(new_tb1, true);
	char *str3 = dumpTB(cut_tb1, true);
    printf("\nnew_TB:\n\n");
    printf("%s", str2);
    printf("\ncut_TB:\n\n");
    printf("%s", str3);
    
	TB new_tb2 = newTB(answer);
    printf("Part 2:\n\n");
	char *str4 = dumpTB(new_tb1, true);
    printf("%s", str4);

    TB cut_tb2 = cutTB(new_tb2, 4, 2);
	char *str5 = dumpTB(new_tb2, true);
	char *str6 = dumpTB(cut_tb2, true);
    printf("\nnew_TB:\n\n");
    printf("%s", str5);
    printf("\ncut_TB:\n\n");
    printf("%s\n", str6);

	printf("All CUT Tests Passed!!!\n\n");
	return;
}

static void testSearchTB(void) {
	printf("-----------------------------------------\n"
	       "               searchTB tests            \n"
	       "-----------------------------------------\n");

	char *input1 = "bird\nturtle\neagle is not a bird\nB-b-b bird, bird, bird, b-bird's the word\n";
	char *input2 = "abracadabra alacazam\nabracadabracadabracadabracadabra\n";

	char *search1 = "textbuffer";
	char *search2 = "bird";
	char *search3 = "\0";
	char *search4 = "abracadabra";

	int cmp11[MAX] = {0}, cmp12[MAX] = {0};
	int cmp21[MAX] = {1, 3, 4, 4, 4, 4}, cmp22[MAX] = {1, 16, 7, 13, 19, 27};
	int cmp31[MAX] = {0}, cmp32[MAX] = {0};
	int cmp41[MAX] = {1, 2, 2}, cmp42[MAX] = {1, 1, 15};

	TB tb1 = newTB(input1);
	TB tb2 = newTB(input1);
	TB tb3 = newTB(input1);
	TB tb4 = newTB(input2);

	Test_SEARCH(tb1, search1, cmp11, cmp12, 1);
	Test_SEARCH(tb2, search2, cmp21, cmp22, 2);
	Test_SEARCH(tb3, search3, cmp31, cmp32, 3);
	Test_SEARCH(tb4, search4, cmp41, cmp42, 4);

	releaseTB(tb1);
	releaseTB(tb2);
	releaseTB(tb3);
	releaseTB(tb4);

	printf("All Searching Tests Passed!!!\n\n");
	return;
}

static void testFormRichText(void) {
	printf("-----------------------------------------\n"
	       "            FormRichText tests           \n"
	       "-----------------------------------------\n");

	char *cmp1 = "*<b>some string</b>lol*\n<b> </b>\n<b>some_string</b>again_\n<b>some</b> <i>string</i>\nsome *string<i>again</i>\nsome#string<b>once_again</b>\n<h1>string_stuff_</h1>\n#\n<h1>##</h1>\n";
	char *input1 = "*some string*lol*\n* *\n*some_string*again_\n*some* _string_\nsome *string_again_\nsome#string*once_again*\n#string_stuff_\n#\n###\n";

    printf("Test: \n\n");
	TB tb1 = newTB(input1);
    char *str0 = dumpTB(tb1, false);
    printf("Input: \n%s\n", str0);
	formRichText(tb1);
	char *str1 = dumpTB(tb1, false);
	printf("Output: \n%s\n", str1);

	assert(cmp1 != str1);

	printf("Pass!!!\n");

	return;
}

//	Process Functions
//////////////////////////////////////////////////////////////////////////////////////
static void Test_NEW(TB tb1, char *input, int NO, int line) {
	Test_Begin(input, NO);
	char *text1 = dumpTB(tb1 ,false);
	int lines = linesTB(tb1);

	PrintFails(TBLINES, lines, line, NONESTR, NONESTR, tb1);
	PrintFails(TBTEXT, NONENUM, NONENUM, text1, input, tb1);
	
	printf("------------------------\nThe Answer is: \n\n");
	
	PrintMap(text1);
	
	if (strlen(text1) != 0)
		free(text1);		//Only strings who contained things have memory.

	printf("\n\nNewTB test %d passed\n\n", NO);
	return;
}

static void Test_DUMP(TB tb, char *input, char *cmp, int NO, bool COMMAND, int line) {
	Test_Begin(input, NO);
	char *str = dumpTB(tb, COMMAND);
	int lines = linesTB(tb);
	
	PrintFails(TBLINES, lines, line, NONESTR, NONESTR, tb);
	PrintFails(TBTEXT, NONENUM, NONENUM, str, cmp, tb);
	printf("Lines : %d\n", lines);
	printf("%s", str);
	printf("\ntest %d is passed.\n\n", NO);

	if (strlen(str) != 0)
		free(str);
	return;
}

static void Test_MERGE(TB tb1, TB tb2, int pos, char *cmp, int NO, int line, char COMMAND) {
	Test_Begin(NULL, NO);
	if (COMMAND == TBMERGE)
		mergeTB(tb1, pos, tb2);
	if (COMMAND == TBPASTE)
		pasteTB(tb1, pos, tb2);
	char *str = dumpTB(tb1, false);
	char *show = dumpTB(tb1, true);
	int lines = linesTB(tb1);

	PrintMap(show);

	PrintFails(TBLINES, lines, line, NONESTR, NONESTR, tb1);	
	PrintFails(TBTEXT, NONENUM, NONENUM, str, cmp, tb1);
	PrintFails(COMMAND, NONENUM, NONENUM, NONESTR, NONESTR, tb2);

	printf("\ntest %d is passed.\n", NO);

	if (strlen(str) != 0)
		free(str);
	if (strlen(show) != 0)
		free(show);
	return;
}

static void Test_PERFIX(TB tb, char *input, char *cmp, int NO, int from, int to) {
	Test_Begin(input, NO);
	addPrefixTB(tb, from, to, input);
	char *str = dumpTB(tb, false);
	int lines = linesTB(tb);
	if ((from < 1 || from > lines) || (to < 1 || to > lines) || (from > to)) {
		printf("\ntest %d is passed.\n\n", NO);
		return;
	}

	PrintFails(TBTEXT, NONENUM, NONENUM, str, cmp, tb);
	printf("%s", str);
	printf("\ntest %d is passed.\n\n", NO);

	if (strlen(str) != 0)
		free(str);
	return;
}

static void Test_SEARCH(TB tb, char *input, int cmp1[MAX], int cmp2[MAX], int NO) {
	Test_Begin(input, NO);
	Match M = searchTB(tb, input);
	int counter = 0;

	char *text = dumpTB(tb, true);
	PrintMap(text);
	while (M != NULL) {
		printf("Order： %d | line: %d, col: %d.\n", counter, M->lineNumber, M->columnNumber);

		PrintFails(TBLINES, M->lineNumber, cmp1[counter], NONESTR, NONESTR, tb);
		PrintFails(TBLINES, M->columnNumber, cmp2[counter], NONESTR, NONESTR, tb);

		counter++;
		M = M->next;
	}

	printf("\nSearching Test %d Passed!\n\n", NO);
	free(text);
	return;
}

static void Test_DELETE(TB tb, char *input, char *cmp, int from, int to, int NO) {
	Test_Begin(input, NO);
	deleteTB(tb, from, to);
	char *text = dumpTB(tb, false);

	printf("Answer:  \n\n");
	PrintMap(text);
	PrintFails(TBTEXT, NONENUM, NONENUM, text, cmp, tb);

	printf("\nDelete Test %d Passed!\n\n", NO);
	
	if (strlen(text) != 0)
		free(text);
	return;
}

//	Help Functions
//////////////////////////////////////////////////////////////////////////////////////
static void Test_Begin(char *input, int NO) {
	printf("\nTEST %d:\n", NO);
	if (input != NULL)
		printf("The Input is :\n\n%s\n", input);
	else
		printf("The Input is NULL\n");
	
	return;
}

static void PrintMap(char *text) {
	printf("%s", text);
	return;
}

static void PrintFails(char COMMAND, int num, int cmp_n, char *text, char *cmp_s, TB tb) {

	switch(COMMAND) {
		case TBLINES : 
			if (num == cmp_n)
				return;
			printf("Test failed: The lines of textbuffer is %d, which is wrong.\n", num);
			break;
		
		case TBTEXT	:
			if (strcmp(text, cmp_s) == 0)
				return;
			printf("Test failed: The text is different from the sample text.\n");
			break;
		
		case TBMERGE:
			if (COMMAND == TBMERGE)	// (tb == NULL)
				return;
			printf("Test failed: TB2 could be accessed.\n");
			break;

		case TBPASTE :
			if (tb != NULL)
				return;
			printf("Test failed: TB2 was modified.\n");
			break;
	}

	abort();
	return;		
}