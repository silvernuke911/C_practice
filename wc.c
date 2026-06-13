#include <stdio.h>

// word counter 
#define IN  1 //  inside a word
#define OUT 0 // outside a word
			  
/* This program counts lines, words, and characters
 * as from an input
 * We define words as a sequence of characters that does not contain a blank
 * tab, or new line
 */

int main()
{
	int c, nl, nw, nc, state; // initialize chars, new line, new word, new char, and state

	state = OUT;				// initialize state value to 0
	nl = nw = nc = 0;			// initialize new vars with 0
	while ((c = getchar()) != EOF) {     // while chars is not end of file
		++nc;									// add new character.
		if (c == '\n') {						// if character is new line, add to nl
			nl++;
		} 	
		if (c == ' ' || c == '\n' || c == '\t') { // if character is whitespace, define as out of word
			state = OUT;
		} else if (state == OUT) {				// if we are alrady out and we did not encounter white space
			state = IN;						    // then we are in
			nw++;								// so add a new word to count
		}		
	}
	printf("Line count : %d\nWord count : %d\nChar count : %d\n", nl,nw,nc); // print allat
	return 0;
}
